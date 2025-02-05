/*
 * Copyright (c) 2022, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SAMPLE_PING_RX_HPP
#define SAMPLE_PING_RX_HPP

#include "gxf/std/codelet.hpp"
#include "gxf/std/receiver.hpp"

namespace nvidia {
namespace holoscan {
namespace sample {
// Sample codelet implementation to receive an entity
class PingRx : public gxf::Codelet {
 public:
  virtual ~PingRx() = default;

  gxf_result_t registerInterface(gxf::Registrar* registrar) override;
  gxf_result_t start() override { return GXF_SUCCESS; }
  gxf_result_t tick() override;
  gxf_result_t stop() override { return GXF_SUCCESS; }

 private:
  gxf::Parameter<gxf::Handle<gxf::Receiver>> signal_;
  int count = 1;
};

}  // namespace sample
}  // namespace holoscan
}  // namespace nvidia

#endif /* SAMPLE_PING_RX_HPP */
