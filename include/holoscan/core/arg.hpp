/*
 * SPDX-FileCopyrightText: Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HOLOSCAN_CORE_ARG_HPP
#define HOLOSCAN_CORE_ARG_HPP

#include "./common.hpp"

#include <any>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "./type_traits.hpp"

namespace holoscan {

/**
 * @brief Enum class for the element type of an argument.
 */
enum class ArgElementType {
  kCustom,      ///< Custom type
  kBoolean,     ///< Boolean type (bool)
  kInt8,        ///< 8-bit integer type (uint8_t)
  kUnsigned8,   ///< 8-bit unsigned integer type (uint8_t)
  kInt16,       ///< 16-bit integer type (int16_t)
  kUnsigned16,  ///< Unsigned 16-bit integer (uint16_t)
  kInt32,       ///< 32-bit integer type (int32_t)
  kUnsigned32,  ///< Unsigned 32-bit integer (uint32_t)
  kInt64,       ///< 64-bit integer type (int64_t)
  kUnsigned64,  ///< Unsigned 64-bit integer (uint64_t)
  kFloat32,     ///< 32-bit floating point type (float)
  kFloat64,     ///< 64-bit floating point type (double)
  kString,      ///< String type (std::string)
  kHandle,      ///< Handle type (std::any)
  kYAMLNode,    ///< YAML node type (YAML::Node)
  kIOSpec,      ///< I/O spec type (holoscan::IOSpec*)
  kCondition,   ///< Condition type (std::shared_ptr<Condition>)
  kResource,    ///< Resource type (std::shared_ptr<Resource>)
};

/**
 * @brief Enum class for the container type of an argument.
 */
enum class ArgContainerType : uint8_t {
  kNative,  ///< Native type (e.g. a scalar type such as int32_t)
  kVector,  ///< Vector type (std::vector)
  kArray,   ///< Array type (std::array)
};

/**
 * @brief Struct for the type information of an argument.
 */
class ArgType {
 public:
  ArgType() = default;
  /**
   * @brief Construct a new ArgType object.
   *
   * @param element_type The element type of the argument.
   * @param container_type The container type of the argument.
   */
  ArgType(ArgElementType element_type, ArgContainerType container_type)
      : element_type_(element_type), container_type_(container_type) {}

  /**
   * @brief Get the element type of the argument.
   *
   * @param index The type index of the argument.
   * @return The element type of the argument.
   */
  static ArgElementType get_element_type(std::type_index index) {
    auto& elem_type_map = element_type_map_;

    if (elem_type_map.find(index) == elem_type_map.end()) { return ArgElementType::kCustom; }

    const auto& elem_type = elem_type_map[index];
    return elem_type;
  }

  /**
   * @brief Get the argument type from the given type.
   *
   * @tparam typeT The type of the argument.
   * @return The argument type.
   */
  template <typename typeT>
  static ArgType create() {
    if constexpr (holoscan::is_scalar_v<std::decay_t<typeT>>) {
      auto index =
          std::type_index(typeid(typename holoscan::type_info<std::decay_t<typeT>>::element_type));
      return ArgType(get_element_type(index), ArgContainerType::kNative);
    } else if constexpr (holoscan::is_vector_v<std::decay_t<typeT>>) {
      auto elem_index = std::type_index(typeid(typename holoscan::type_info<typeT>::element_type));
      return ArgType(get_element_type(elem_index), ArgContainerType::kVector);
    } else if constexpr (holoscan::is_array_v<std::decay_t<typeT>>) {
      auto elem_index = std::type_index(typeid(typename holoscan::type_info<typeT>::element_type));
      return ArgType(get_element_type(elem_index), ArgContainerType::kArray);
    } else {
      HOLOSCAN_LOG_ERROR("No element type for '{}' exists", typeid(std::decay_t<typeT>).name());
      return ArgType(ArgElementType::kCustom, ArgContainerType::kNative);
    }
  }

  /**
   * @brief Get the element type of the argument.
   *
   * @return The element type of the argument.
   */
  ArgElementType element_type() const { return element_type_; }
  /**
   * @brief Get the container type of the argument.
   *
   * @return The container type of the argument.
   */
  ArgContainerType container_type() const { return container_type_; }

 private:
  template <class typeT>
  inline static std::pair<const std::type_index, ArgElementType> to_element_type_pair(
      ArgElementType element_type) {
    return {std::type_index(typeid(std::decay_t<typeT>)), element_type};
  }
  inline static std::unordered_map<std::type_index, ArgElementType> element_type_map_{
      to_element_type_pair<bool>(ArgElementType::kBoolean),
      to_element_type_pair<int8_t>(ArgElementType::kInt8),
      to_element_type_pair<uint8_t>(ArgElementType::kUnsigned8),
      to_element_type_pair<int16_t>(ArgElementType::kInt16),
      to_element_type_pair<uint16_t>(ArgElementType::kUnsigned16),
      to_element_type_pair<int32_t>(ArgElementType::kInt32),
      to_element_type_pair<uint32_t>(ArgElementType::kUnsigned32),
      to_element_type_pair<int64_t>(ArgElementType::kInt64),
      to_element_type_pair<uint64_t>(ArgElementType::kUnsigned64),
      to_element_type_pair<float>(ArgElementType::kFloat32),
      to_element_type_pair<double>(ArgElementType::kFloat64),
      to_element_type_pair<std::string>(ArgElementType::kString),
      to_element_type_pair<std::any>(ArgElementType::kHandle),
      to_element_type_pair<YAML::Node>(ArgElementType::kYAMLNode),
      to_element_type_pair<holoscan::IOSpec*>(ArgElementType::kIOSpec),
      to_element_type_pair<std::shared_ptr<Condition>>(ArgElementType::kCondition),
      to_element_type_pair<std::shared_ptr<Resource>>(ArgElementType::kResource),
  };
  ArgElementType element_type_ = ArgElementType::kCustom;
  ArgContainerType container_type_ = ArgContainerType::kNative;
};

/**
 * @brief Class for holding the argument information.
 */
class Arg {
 public:
  /**
   * @brief Construct a new Arg object.
   *
   * @param name The name of the argument.
   */
  explicit Arg(const std::string& name) : name_(name) {}
  ~Arg() = default;

  /**
   * @brief Define the assignment operator.
   *
   * @tparam ArgT The type of the argument.
   * @param value The value of the argument.
   * @return The reference to the argument.
   */
  template <typename ArgT>
  Arg& operator=(const ArgT& value) {
    arg_type_ = ArgType::create<ArgT>();
    HOLOSCAN_LOG_TRACE(
        "Arg& operator=(const ArgT& value)({}) parameter: {}, element_type: {}, container_type: {}",
        typeid(ArgT).name(),
        name(),
        (int)arg_type_.element_type(),
        (int)arg_type_.container_type());

    if constexpr (is_one_of_v<typename holoscan::type_info<ArgT>::element_type,
                              std::shared_ptr<Resource>,
                              std::shared_ptr<Condition>>) {
      if constexpr (is_scalar_v<ArgT>) {
        value_ = std::dynamic_pointer_cast<
            base_type_t<typename holoscan::type_info<ArgT>::derived_type>>(value);
      } else if constexpr (is_vector_v<ArgT> && holoscan::type_info<ArgT>::dimension == 1) {
        std::vector<typename holoscan::type_info<ArgT>::element_type> components;
        components.reserve(value.size());
        for (auto& value_item : value) {
          auto component = std::dynamic_pointer_cast<
              base_type_t<typename holoscan::type_info<ArgT>::derived_type>>(value_item);
          components.push_back(component);
        }
        value_ = components;
      }
    } else {
      value_ = value;
    }

    return *this;
  }

  /**
   * @brief Define the assignment operator.
   *
   * @tparam ArgT The type of the argument.
   * @param value The value of the argument.
   * @return The reference to the argument.
   */
  template <typename ArgT>
  Arg&& operator=(ArgT&& value) {
    arg_type_ = ArgType::create<ArgT>();
    HOLOSCAN_LOG_TRACE(
        "Arg&& operator=(ArgT&& value)({}) parameter: {}, element_type: {}, container_type: {}, "
        "ArgT: {}",
        typeid(ArgT).name(),
        name(),
        (int)arg_type_.element_type(),
        (int)arg_type_.container_type(),
        typeid(ArgT).name());

    if constexpr (is_one_of_v<typename holoscan::type_info<ArgT>::element_type,
                              std::shared_ptr<Resource>,
                              std::shared_ptr<Condition>>) {
      if constexpr (is_scalar_v<ArgT>) {
        value_ = std::move(std::dynamic_pointer_cast<
                           base_type_t<typename holoscan::type_info<ArgT>::derived_type>>(value));
      } else if constexpr (is_vector_v<ArgT> && holoscan::type_info<ArgT>::dimension == 1) {
        std::vector<typename holoscan::type_info<ArgT>::element_type> components;
        components.reserve(value.size());
        for (auto& value_item : value) {
          auto component = std::dynamic_pointer_cast<
              base_type_t<typename holoscan::type_info<ArgT>::derived_type>>(value_item);
          components.push_back(std::move(component));
        }
        value_ = std::move(components);
      }
    } else {
      value_ = std::forward<ArgT>(value);
    }
    return std::move(*this);
  }

  /**
   * @brief Get the name of the argument.
   *
   * @return The name of the argument.
   */
  const std::string& name() const { return name_; }

  /**
   * @brief Get the type of the argument.
   *
   * @return The type of the argument.
   */
  const ArgType& arg_type() const { return arg_type_; }

  /**
   * @brief Check whether the argument contains a value.
   *
   * @return true if the argument contains a value.
   */
  bool has_value() const { return value_.has_value(); }
  /**
   * @brief Get the value of the argument.
   *
   * @return The reference to the value of the argument.
   */
  std::any& value() { return value_; }

 private:
  std::string name_;  ///< The name of the argument.
  ArgType arg_type_;  ///< The type of the argument.
  std::any value_;    ///< The value of the argument.
};

/**
 * @brief Vector-like container for holding the arguments.
 */
class ArgList {
 public:
  ArgList() = default;
  /**
   * @brief Construct a new ArgList object.
   *
   * @param args The arguments as an initializer list.
   */
  explicit ArgList(std::initializer_list<Arg> args) {
    for (auto& arg : args) { args_.push_back(arg); }
  }

  ~ArgList() = default;

  /**
   * @brief Get the number of arguments.
   *
   * @return The number of arguments.
   */
  size_t size() const { return args_.size(); }
  /**
   * @brief Return an iterator to the beginning.
   *
   * @return The iterator to the first element.
   */
  std::vector<Arg>::iterator begin() { return args_.begin(); }
  /**
   * @brief Return an iterator to the end.
   *
   * @return The iterator to the element following the last element.
   */
  std::vector<Arg>::iterator end() { return args_.end(); }
  /**
   * @brief Return a const iterator to the beginning.
   *
   * @return The const iterator to the first element.
   */
  std::vector<Arg>::const_iterator begin() const { return args_.begin(); }
  /**
   * @brief Return a const iterator to the end.
   *
   * @return The const iterator to the element following the last element.
   */
  std::vector<Arg>::const_iterator end() const { return args_.end(); }
  /**
   * @brief Erase all elements from the container.
   */
  void clear() { args_.clear(); }

  /**
   * @brief Get the vector of arguments.
   *
   * @return The reference to the vector of arguments.
   */
  std::vector<Arg>& args() { return args_; }

  /**
   * @brief Get the value of the argument as a specific type.
   *
   * This method takes a type as a template parameter and returns the value of the argument as that
   * type. Only the first item in the argument list is taken into account.
   *
   * If the cast fails, an error message is logged and a default value for the type is returned.
   *
   * Example:
   *
   * ```cpp
   * bool is_rdma = from_config("aja.rdma").as<bool>();
   * ```
   *
   * @tparam typeT The type to cast the argument to.
   * @return The value of the argument as the specified type.
   */
  template <typename typeT>
  typeT as() {
    if (args_.empty()) {
      HOLOSCAN_LOG_ERROR("No item available in the argument list.");
      return typeT();
    }

    // Only take the first item in the argument list.
    auto& argument = args_[0];
    if (argument.arg_type().element_type() != ArgElementType::kYAMLNode) {
      HOLOSCAN_LOG_ERROR("The type of the argument '{}' should be kYAMLNode.");
      return typeT();
    }

    auto node = std::any_cast<YAML::Node>(argument.value());
    try {
      return node.as<typeT>();
    } catch (...) {
      std::stringstream ss;
      ss << node;
      HOLOSCAN_LOG_ERROR("Unable to parse YAML node: '{}'", ss.str());
      return typeT();
    }
  }

  /**
   * @brief Add an argument to the list.
   *
   * @param arg The argument to add.
   */
  void add(const Arg& arg) { args_.emplace_back(arg); }

  /**
   * @brief Add an argument to the list.
   *
   * @param arg The argument to add.
   */
  void add(Arg&& arg) { args_.emplace_back(std::move(arg)); }

  /**
   * @brief Add an argument list to the list.
   *
   * @param arg The argument list to add.
   */
  void add(const ArgList& arg) {
    args_.reserve(args_.size() + arg.size());
    args_.insert(args_.end(), arg.begin(), arg.end());
  }
  /**
   * @brief Add an argument list to the list.
   *
   * @param arg The argument list to add.
   */
  void add(ArgList&& arg) {
    args_.reserve(args_.size() + arg.size());
    args_.insert(
        args_.end(), std::make_move_iterator(arg.begin()), std::make_move_iterator(arg.end()));
    arg.clear();
  }

  /**
   * @brief Get the name of the argument list.
   *
   * @return The name of the argument list.
   */
  const std::string& name() const { return name_; }

 private:
  std::string name_{"arglist"};  ///< The name of the argument list.
  std::vector<Arg> args_;        ///< The vector of arguments.
};

}  // namespace holoscan

#endif /* HOLOSCAN_CORE_ARG_HPP */
