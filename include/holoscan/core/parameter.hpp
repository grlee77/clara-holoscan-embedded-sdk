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

#ifndef HOLOSCAN_CORE_PARAMETER_HPP
#define HOLOSCAN_CORE_PARAMETER_HPP

#include "./common.hpp"

#include <any>
#include <functional>
#include <iostream>
#include <optional>
#include <typeinfo>

#include "./arg.hpp"

namespace holoscan {

/**
 * @brief Enum class to define the type of a parameter.
 */
enum class ParameterFlag {
  kNone = 0,      ///< No flag
  kOptional = 1,  ///< The parameter is optional
  kDynamic = 2,   ///< The parameter is dynamic
};

/**
 * @brief Class to wrap a parameter with std::any.
 */
class ParameterWrapper {
 public:
  ParameterWrapper() = default;

  /**
   * @brief Construct a new ParameterWrapper object.
   *
   * @tparam typeT The type of the parameter.
   * @param param The parameter to wrap.
   */
  template <typename typeT>
  explicit ParameterWrapper(Parameter<typeT>& param)
      : type_(&typeid(typeT)), arg_type_(ArgType::create<typeT>()), value_(&param) {}


  /**
   * @brief Construct a new ParameterWrapper object.
   *
   * @param value The parameter to wrap.
   * @param type The type of the parameter.
   * @param arg_type The type of the parameter as an ArgType.
   */
  ParameterWrapper(std::any value, const std::type_info* type, const ArgType& arg_type)
      : type_(type), arg_type_(arg_type), value_(std::move(value)) {}

  /**
   * @brief Get the type of the parameter.
   *
   * @return The type info of the parameter.
   */
  const std::type_info& type() const {
    if (type_) { return *type_; }
    return typeid(void);
  }
  /**
   * @brief Get the type of the parameter as an ArgType.
   *
   * @return The type of the parameter as an ArgType.
   */
  const ArgType& arg_type() const { return arg_type_; }

  /**
   * @brief Get the value of the parameter.
   *
   * @return The reference to the value of the parameter.
   */
  std::any& value() { return value_; }

 private:
  const std::type_info* type_ = nullptr;  /// element type of Parameter
  ArgType arg_type_;                      /// type of argument
  std::any value_;
};

/**
 * @brief Class to define a parameter.
 */
template <typename ValueT>
class MetaParameter {
 public:
  MetaParameter() = default;

  /**
   * @brief Construct a new MetaParameter object.
   *
   * @param value The value of the parameter.
   */
  explicit MetaParameter(ValueT& value) : value_(value) {}
  /**
   * @brief Construct a new MetaParameter object.
   *
   * @param value The value of the parameter.
   */
  explicit MetaParameter(ValueT&& value) : value_(std::move(value)) {}

  /**
   * @brief Define the assignment operator.
   *
   * @param value The value of the parameter.
   * @return The reference to the parameter.
   */
  MetaParameter& operator=(const ValueT& value) {
    value_ = value;
    return *this;
  }
  /**
   * @brief Define the assignment operator.
   *
   * @param value The value of the parameter.
   * @return The reference to the parameter.
   */
  MetaParameter&& operator=(ValueT&& value) {
    value_ = std::move(value);
    return std::move(*this);
  }

  /**
   * @brief Get the key (name) of the parameter.
   *
   * @return The key (name) of the parameter.
   */
  const std::string& key() const { return key_; }

  /**
   * @brief Check whether the parameter contains a value.
   *
   * @return true if the parameter contains a value.
   */
  bool has_value() const { return value_.has_value(); }

  /**
   * @brief Get the value of the parameter.
   *
   * @return The reference to the value of the parameter.
   */
  ValueT& get() {
    if (value_.has_value()) {
      return value_.value();
    } else {
      throw std::runtime_error("MetaParameter: value is not set");
    }
  }

  /**
   * @brief Set the default value object if the parameter does not contain a value.
   */
  void set_default_value() {
    if (!value_.has_value()) { value_ = default_value_; }
  }

  /**
   * @brief Get the value of the argument.
   *
   * @return The reference to the value of the parameter.
   */
  operator ValueT&() { return get(); }

 private:
  friend class ComponentSpec;
  friend class OperatorSpec;
  std::optional<ValueT> value_;
  std::optional<ValueT> default_value_;
  std::string key_;
  std::string headline_;
  std::string description_;
  ParameterFlag flag_ = ParameterFlag::kNone;
};

}  // namespace holoscan

#endif /* HOLOSCAN_CORE_PARAMETER_HPP */
