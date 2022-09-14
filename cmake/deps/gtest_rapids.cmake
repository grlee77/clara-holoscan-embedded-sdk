# SPDX-FileCopyrightText: Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# https://docs.rapids.ai/api/rapids-cmake/stable/packages/rapids_cpm_gtest.html
include(${rapids-cmake-dir}/cpm/gtest.cmake)

# It uses GTest version 1.10.0 (as of rapids-cmake v22.06)
# https://github.com/rapidsai/rapids-cmake/blob/branch-22.06/rapids-cmake/cpm/versions.json
rapids_cpm_gtest()
