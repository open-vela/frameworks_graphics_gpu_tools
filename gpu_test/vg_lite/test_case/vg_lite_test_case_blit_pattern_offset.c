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

static vg_lite_error_t on_setup(struct vg_lite_test_context_s* ctx)
{
    vg_lite_test_context_alloc_src_buffer(ctx, 256, 50, VG_LITE_BGRA8888, VG_LITE_TEST_STRIDE_AUTO);
    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);
    vg_lite_translate(50, 0, &matrix);
    vg_lite_rotate(45, &matrix);

    struct vg_lite_test_path_s* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);
    vg_lite_test_path_set_bounding_box(path, 0, 0, 256, 50);
    vg_lite_test_path_append_rect(path, 0, 0, 256, 50, 0);
    vg_lite_test_path_end(path);

    vg_lite_buffer_t* image = vg_lite_test_context_get_src_buffer(ctx);
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);

    /* Test vg_lite_draw_pattern */
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(image, NULL, 0xFF0000FF));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw_pattern(
            target_buffer,
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            VG_LITE_PATTERN_COLOR,
            0,
            0,
            VG_LITE_FILTER_BI_LINEAR));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

    /* Test vg_lite_blit */
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(image, NULL, 0xFF00FF00));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            target_buffer,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0,
            VG_LITE_FILTER_BI_LINEAR));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

    /* Test vg_lite_blit_rect */
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(image, NULL, 0xFFFF0000));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

    vg_lite_rectangle_t rect = {
        .x = 0,
        .y = 0,
        .width = image->width,
        .height = image->height,
    };

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit_rect(
            target_buffer,
            image,
            &rect,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0,
            VG_LITE_FILTER_BI_LINEAR));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(blit_pattern_offset, NONE, "Test whether blit and draw_pattern completely overlap");
