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

/*********************
 *      INCLUDES
 *********************/

#ifndef GPU_TEST_CONTEXT_DEFAULT_DISABLE

#include "gpu_assert.h"
#include "gpu_context.h"
#include "gpu_fb.h"
#include "gpu_log.h"
#include <stddef.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void gpu_test_context_setup(struct gpu_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);

    GPU_LOG_INFO("Initializing GPU");
    extern void gpu_init(void);
    gpu_init();

    if (ctx->param.fbdev_path) {
        ctx->fb = gpu_fb_create(ctx->param.fbdev_path);

        if (ctx->fb) {
            gpu_fb_get_buffer(ctx->fb, &ctx->target_buffer);
        }
    }
}

void gpu_test_context_teardown(struct gpu_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);

    if (ctx->fb) {
        gpu_fb_destroy(ctx->fb);
        ctx->fb = NULL;
    }

    extern void gpu_deinit(void);
    GPU_LOG_INFO("Deinitializing GPU");
    gpu_deinit();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /* GPU_TEST_CONTEXT_DEFAULT_DISABLE */
