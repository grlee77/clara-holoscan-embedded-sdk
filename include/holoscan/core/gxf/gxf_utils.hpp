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

#ifndef HOLOSCAN_CORE_GXF_GXF_UTILS_HPP
#define HOLOSCAN_CORE_GXF_GXF_UTILS_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <gxf/core/gxf.h>

namespace holoscan::gxf {

inline gxf_result_t add_connection(gxf_context_t context, gxf_uid_t source_cid,
                                   gxf_uid_t target_cid) {
  gxf_result_t code;
  gxf_uid_t connect_eid;
  const GxfEntityCreateInfo connect_entity_create_info = {nullptr, GXF_ENTITY_CREATE_PROGRAM_BIT};
  code = GxfCreateEntity(context, &connect_entity_create_info, &connect_eid);

  gxf_tid_t connect_tid;
  code = GxfComponentTypeId(context, "nvidia::gxf::Connection", &connect_tid);
  gxf_uid_t connect_cid;
  code = GxfComponentAdd(context, connect_eid, connect_tid, "", &connect_cid);

  code = GxfParameterSetHandle(context, connect_cid, "source", source_cid);
  code = GxfParameterSetHandle(context, connect_cid, "target", target_cid);
  return code;
}

inline gxf_uid_t get_component_eid(gxf_context_t context, gxf_uid_t cid) {
  gxf_result_t code;
  gxf_uid_t eid;
  code = GxfComponentEntity(context, cid, &eid);
  (void)code;
  return eid;
}

inline std::string get_full_component_name(gxf_context_t context, gxf_uid_t cid) {
  gxf_result_t code;

  const char* cname;
  code = GxfComponentName(context, cid, &cname);
  gxf_uid_t eid;
  code = GxfComponentEntity(context, cid, &eid);
  const char* ename;
  code = GxfComponentName(context, eid, &ename);

  std::stringstream sstream;
  sstream << ename << "/" << cname;
  (void)code;
  return sstream.str();
}

inline std::string create_name(const char* prefix, int index) {
  std::stringstream sstream;
  sstream << prefix << "_" << index;
  return sstream.str();
}

inline std::string create_name(const char* prefix, const std::string& name) {
  std::stringstream sstream;
  sstream << prefix << "_" << name;
  return sstream.str();
}

}  // namespace holoscan::gxf

#endif /* HOLOSCAN_CORE_GXF_GXF_UTILS_HPP */
