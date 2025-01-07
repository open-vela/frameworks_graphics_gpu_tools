#
# Copyright (C) 2025 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#

if(CONFIG_GPU_TEST)

  include_directories(${NUTTX_APPS_DIR}/external/libpng
                      ${NUTTX_APPS_DIR}/external/libpng/libpng)

  if(CONFIG_GPU_TEST_VG_LITE_INCLUDE)
    include_directories(${NUTTX_APPS_DIR}/../${CONFIG_GPU_TEST_VG_LITE_INCLUDE})
  endif()

  # Basic definitions
  add_definitions(
    -DGPU_TEST_CONTEXT_DEFAULT_DISABLE=1 -DGPU_TEST_CONTEXT_LINUX_DISABLE=1
    -DGPU_TEST_CONTEXT_NUTTX_ENABLE=1 -DGPU_OUTPUT_DIR_DEFAULT="/data/gpu"
    -DGPU_LOG_USE_SYSLOG=1)

  # NuttX cache definitions
  add_definitions(
    -DGPU_CACHE_INCLUDE_H="nuttx/cache.h"
    -DGPU_CACHE_INVALIDATE_FUNC=up_invalidate_dcache
    -DGPU_CACHE_CLEAN_FUNC=up_clean_dcache
    -DGPU_CACHE_FLUSH_FUNC=up_flush_dcache)

  file(GLOB_RECURSE CSRCS "${CMAKE_CURRENT_LIST_DIR}/*.c"
       "${CMAKE_CURRENT_LIST_DIR}/vg_lite/*.c"
       "${CMAKE_CURRENT_LIST_DIR}/vg_lite/*/*.c")

  # Make sure gpu_main.c is the first file in the list
  list(REMOVE_ITEM CSRCS ${CMAKE_CURRENT_LIST_DIR}/gpu_main.c)

  nuttx_add_application(
    NAME
    gpu_test
    SRCS
    gpu_main.c
    ${CSRCS}
    STACKSIZE
    ${CONFIG_GPU_TEST_STACKSIZE}
    PRIORITY
    ${CONFIG_GPU_TEST_PRIORITY})

endif()
