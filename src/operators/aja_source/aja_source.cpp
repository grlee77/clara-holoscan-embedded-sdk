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

#include "holoscan/operators/aja_source/aja_source.hpp"

#include "holoscan/core/gxf/entity.hpp"
#include "holoscan/core/operator_spec.hpp"

namespace holoscan::ops {

void AJASourceOp::setup(OperatorSpec& spec) {
  auto& signal = spec.output<::gxf::Entity>("signal");

  constexpr char kDefaultDevice[] = "0";
  constexpr NTV2Channel kDefaultChannel = NTV2_CHANNEL1;
  constexpr uint32_t kDefaultWidth = 1920;
  constexpr uint32_t kDefaultHeight = 1080;
  constexpr uint32_t kDefaultFramerate = 60;
  constexpr bool kDefaultRDMA = false;

  spec.param(signal_, "signal", "Output", "Output signal.", &signal);
  spec.param(
      device_specifier_, "device", "Device", "Device specifier.", std::string(kDefaultDevice));
  spec.param(channel_, "channel", "Channel", "NTV2Channel to use.", kDefaultChannel);
  spec.param(width_, "width", "Width", "Width of the stream.", kDefaultWidth);
  spec.param(height_, "height", "Height", "Height of the stream.", kDefaultHeight);
  spec.param(framerate_, "framerate", "Framerate", "Framerate of the stream.", kDefaultFramerate);
  spec.param(use_rdma_, "rdma", "RDMA", "Enable RDMA.", kDefaultRDMA);
}

void AJASourceOp::initialize() {
  register_converter<NTV2Channel>();

  holoscan::ops::GXFOperator::initialize();
}

}  // namespace holoscan::ops
