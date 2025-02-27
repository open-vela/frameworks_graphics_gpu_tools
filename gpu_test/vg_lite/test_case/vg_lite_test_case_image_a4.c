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

#include "../vg_lite_test_context.h"
#include "../vg_lite_test_path.h"
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

static vg_lite_error_t draw_image(struct vg_lite_test_context_s* ctx, vg_lite_buffer_t* image, int x_ofs, int y_ofs, vg_lite_color_t bg_color)
{
    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);
    vg_lite_translate(x_ofs, y_ofs, &matrix);

    vg_lite_rectangle_t rect = {
        .x = 0,
        .y = 0,
        .width = image->width * 3,
        .height = image->height,
    };

    vg_lite_test_transform_retangle(&rect, &matrix);

    vg_lite_rectangle_t rect_image = {
        .x = image->width / 4,
        .y = 0,
        .width = image->width / 2,
        .height = image->height,
    };

    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_path_t* vg_path = vg_lite_test_path_get_path(vg_lite_test_context_get_path(ctx));

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, &rect, bg_color));

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            target_buffer,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFF0000FF,
            VG_LITE_FILTER_BI_LINEAR));

    vg_lite_translate(image->width, 0, &matrix);
    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit_rect(
            target_buffer,
            image,
            &rect_image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFF0000FF,
            VG_LITE_FILTER_BI_LINEAR));

    vg_lite_translate(image->width, 0, &matrix);
    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw_pattern(
            target_buffer,
            vg_path,
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            VG_LITE_PATTERN_COLOR,
            0,
            0xFF0000FF,
            VG_LITE_FILTER_BI_LINEAR));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_setup(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* image = vg_lite_test_context_alloc_src_buffer(ctx, 64, 64, VG_LITE_A4, VG_LITE_TEST_STRIDE_AUTO);

    vg_lite_test_fill_gray_gradient(image);

    vg_lite_test_path_t* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);
    vg_lite_test_path_set_bounding_box(path, 0, 0, 64, 64);
    vg_lite_test_path_append_circle(path, 32, 32, 32, 32);
    vg_lite_test_path_end(path);

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* image = vg_lite_test_context_get_src_buffer(ctx);

    VG_LITE_TEST_CHECK_ERROR_RETURN(draw_image(ctx, image, 0, 0, 0xFF000000));
    VG_LITE_TEST_CHECK_ERROR_RETURN(draw_image(ctx, image, 0, 100, 0xFF1F1F1F));
    VG_LITE_TEST_CHECK_ERROR_RETURN(draw_image(ctx, image, 0, 200, 0xFFFFFFFF));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(image_a4, NONE, "Draw A4 image using different drawing methods");
