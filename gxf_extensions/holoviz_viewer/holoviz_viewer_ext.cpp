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

#include "gxf/std/extension_factory_helper.hpp"

#include "holoviz_viewer.hpp"

GXF_EXT_FACTORY_BEGIN()
GXF_EXT_FACTORY_SET_INFO(0x1c3764640543460f, 0x95c882615dbf23d9, "HolovizViewer",
                         "Holoviz viewer extension", "NVIDIA", "0.1.0", "LICENSE");
GXF_EXT_FACTORY_ADD(0x9c6153f82d0a4fdf, 0xa7df68545c44ccb3, nvidia::holoscan::HolovizViewer,
                    nvidia::gxf::Codelet, "Holoviz viewer.");
GXF_EXT_FACTORY_END()
