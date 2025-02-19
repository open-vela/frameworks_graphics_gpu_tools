/*
 * Copyright (C) 2025 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GPU_CONTEXT_H
#define GPU_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "gpu_buffer.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/

#define GPU_TEST_DESIGN_WIDTH 480
#define GPU_TEST_DESIGN_HEIGHT 480

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_recorder_s;
struct gpu_fb_s;

enum gpu_test_mode_e {
    GPU_TEST_MODE_DEFAULT = 0,
    GPU_TEST_MODE_STRESS,
};

struct gpu_test_param_s {
    int argc;
    char** argv;
    enum gpu_test_mode_e mode;
    const char* output_dir;
    const char* testcase_name;
    const char* fbdev_path;
    int target_width;
    int target_height;
    int run_loop_count;
    int cpu_freq;
    bool screenshot_en;
};

struct gpu_test_context_s {
    struct gpu_recorder_s* recorder;
    struct gpu_fb_s* fb;
    struct gpu_test_param_s param;
    struct gpu_buffer_s target_buffer;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Initialize the GPU test context.
 * @param ctx The GPU test context to initialize.
 */
bool gpu_test_context_setup(struct gpu_test_context_s* ctx);

/**
 * @brief Deinitialize the GPU test context.
 * @param ctx The GPU test context to deinitialize.
 */
void gpu_test_context_teardown(struct gpu_test_context_s* ctx);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_CONTEXT_H*/
