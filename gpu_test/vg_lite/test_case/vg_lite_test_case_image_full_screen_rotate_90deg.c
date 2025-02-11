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
        target_buffer->width,
        target_buffer->height,
        VG_LITE_BGRA8888,
        VG_LITE_TEST_STRIDE_AUTO);

    vg_lite_test_fill_gray_gradient(image);

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_buffer_t* image = vg_lite_test_context_get_src_buffer(ctx);

    vg_lite_matrix_t matrix;
    vg_lite_identity(&matrix);

    /* Rotate the image 90 degrees around the center of the image buffer */
    vg_lite_translate(image->width / 2, image->height / 2, &matrix);
    vg_lite_rotate(90, &matrix);
    vg_lite_translate(-image->width / 2, -image->height / 2, &matrix);

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_blit(target_buffer, image, &matrix, VG_LITE_BLEND_SRC_OVER, 0, VG_LITE_FILTER_BI_LINEAR));
    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(image_full_screen_rotate_90deg, NONE, "Draw BGRA8888 image on full screen with rotated 90 degrees");
