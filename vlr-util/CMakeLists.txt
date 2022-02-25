#
# Copyright 2021 The VLR Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

vlr_cc_library(
  NAME
    zstring_view
  HDRS
    "zstring_view.h"
  DEPS
  COPTS
    ${ABSL_DEFAULT_COPTS}
)

vlr_cc_library(
  NAME
    ActionOnDestruction
  HDRS
    "ActionOnDestruction.h"
  DEPS
  COPTS
    ${ABSL_DEFAULT_COPTS}
)

# file(GLOB_RECURSE
#   FILES_INCLUDE
#   "*.h;*.hpp"
# )

# file(GLOB_RECURSE
#   FILES_SOURCE
#   "*.cpp"
# )

# vlr_cc_library(
#   NAME
#     vlr-util
#   HDRS
#     ${FILES_INCLUDE}
#   SRCS
#     ${FILES_SOURCE}
#   COPTS
#     ${ABSL_DEFAULT_COPTS}
# )
