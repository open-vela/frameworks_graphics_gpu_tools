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

static vg_lite_error_t clear_buffer(vg_lite_buffer_t* image, int x, vg_lite_color_t color)
{
    vg_lite_rectangle_t rect = {
        .x = x,
        .y = 0,
        .width = image->width / 4,
        .height = image->height,
    };

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(image, &rect, color));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_setup(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_buffer_t* image = vg_lite_test_context_alloc_src_buffer(
        ctx,
        target_buffer->width,
        target_buffer->height,
        VG_LITE_BGRA8888,
        VG_LITE_TEST_STRIDE_AUTO);

    /* Draw 4 rectangles on the image */
    VG_LITE_TEST_CHECK_ERROR_RETURN(clear_buffer(image, 0, 0xFFFFFFFF));
    VG_LITE_TEST_CHECK_ERROR_RETURN(clear_buffer(image, image->width * 0.25f, 0xFFF00000));
    VG_LITE_TEST_CHECK_ERROR_RETURN(clear_buffer(image, image->width * 0.50f, 0xFF00FF00));
    VG_LITE_TEST_CHECK_ERROR_RETURN(clear_buffer(image, image->width * 0.75f, 0xFF0000FF));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

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
