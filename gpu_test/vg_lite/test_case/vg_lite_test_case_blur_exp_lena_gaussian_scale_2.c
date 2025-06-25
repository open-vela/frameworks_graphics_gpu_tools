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
    if (!vg_lite_test_context_load_src_image_from_file(ctx, "lena.png")) {
        return VG_LITE_NOT_SUPPORT;
    }

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
    args.radius = 20;

    if (!gpu_buf_blur(&src_gpu_buffer, &src_gpu_buffer, &args)) {
        return VG_LITE_NOT_SUPPORT;
    }

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_gaussian_filter(0.2f, 0.1f, 0.1f));

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);
    vg_lite_scale(2, 2, &matrix);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            target_buffer,
            src_buffer,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0,
            VG_LITE_FILTER_GAUSSIAN));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(blur_exp_lena_gaussian_scale_2, GAUSSIAN_BLUR, "Test exp blur (radius=20) with lena.png image and use gaussian filter and scale 2.0");
