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

#include "holoscan/core/resources/gxf/double_buffer_transmitter.hpp"

#include "holoscan/core/component_spec.hpp"

namespace holoscan {

void DoubleBufferTransmitter::setup(ComponentSpec& spec) {
  spec.param(capacity_, "capacity", "Capacity", "", 1UL);
  spec.param(policy_, "policy", "Policy", "0: pop, 1: reject, 2: fault", 2UL);
}

}  // namespace holoscan
