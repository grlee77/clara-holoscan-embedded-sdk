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

#ifndef HOLOSCAN_CORE_EXECUTION_CONTEXT_HPP
#define HOLOSCAN_CORE_EXECUTION_CONTEXT_HPP

#include "./common.hpp"

namespace holoscan {

/**
 * @brief Class to hold the execution context.
 *
 * TBD
 */
class ExecutionContext {
 public:
  ExecutionContext() = default;

  void* gxf_context() { return gxf_context_; }

 private:
  void* gxf_context_ = nullptr;  /// The GXF context
};

}  // namespace holoscan

#endif /* HOLOSCAN_CORE_EXECUTION_CONTEXT_HPP */
