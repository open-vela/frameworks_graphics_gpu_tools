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

#include "../../gpu_buf_blur.h"
#include "../../gpu_utils.h"
#include "../vg_lite_test_context.h"
#include "../vg_lite_test_utils.h"

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

/**********************
 *   STATIC FUNCTIONS
 **********************/

static vg_lite_error_t on_setup(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_buffer_t* image = vg_lite_test_context_alloc_src_buffer(
        ctx,
        GPU_ALIGN_UP(target_buffer->width, 16),
        GPU_ALIGN_UP(target_buffer->height, 16),
        VG_LITE_BGRA8888,
        VG_LITE_TEST_STRIDE_AUTO);

    vg_lite_rectangle_t rect = { 0, 0, target_buffer->width, target_buffer->height };

    /* Black */
    vg_lite_test_clear(image, NULL, 0xFF000000);

    /* White */
    rect.width /= 2;
    rect.height /= 2;
    vg_lite_test_clear(image, &rect, 0xFFFFFFFF);

    /* Blue */
    rect.width /= 2;
    rect.height /= 2;
    vg_lite_test_clear(image, &rect, 0xFF0000FF);

    /* Green */
    rect.width /= 2;
    rect.height /= 2;
    vg_lite_test_clear(image, &rect, 0xFF00FF00);

    /* Red */
    rect.width /= 2;
    rect.height /= 2;
    vg_lite_test_clear(image, &rect, 0xFFFF0000);

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_buffer_t* src_buffer = vg_lite_test_context_get_src_buffer(ctx);

    struct gpu_buffer_s src_gpu_buffer;
    vg_lite_test_vg_buffer_to_gpu_buffer(&src_gpu_buffer, src_buffer);

    struct gpu_buffer_s target_gpu_buffer;
    vg_lite_test_vg_buffer_to_gpu_buffer(&target_gpu_buffer, target_buffer);

    gpu_buf_blur_args_t args;
    gpu_buf_blur_args_init(&args);
    args.type = GPU_BUF_BLUR_TYPE_EXP;
    args.radius = 10;

    if (!gpu_buf_blur(&target_gpu_buffer, &src_gpu_buffer, &args)) {
        return VG_LITE_NOT_SUPPORT;
    }

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(blur_exp_full_screen, NONE, "Test exp blur (radius=10) full screen");
