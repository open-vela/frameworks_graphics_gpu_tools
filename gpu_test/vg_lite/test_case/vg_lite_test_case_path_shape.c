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
    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_test_path_t* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);

    vg_lite_test_path_set_bounding_box(path, 0, 0, 240, 240);

    vg_lite_test_path_append_rect(path, 0, 0, 100, 100, 0);
    vg_lite_test_path_end(path);

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);

    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw(
            target_buffer,
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFFFF0000));

    vg_lite_translate(120, 0, &matrix);
    vg_lite_test_path_reset(path, VG_LITE_FP32);
    vg_lite_test_path_append_rect(path, 0, 0, 100, 100, 20);
    vg_lite_test_path_end(path);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw(
            target_buffer,
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFF00FF00));

    vg_lite_translate(120, 0, &matrix);
    vg_lite_test_path_reset(path, VG_LITE_FP32);
    vg_lite_test_path_append_circle(path, 50, 50, 50, 50);
    vg_lite_test_path_append_circle(path, 50, 50, 40, 40);
    vg_lite_test_path_end(path);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw(
            target_buffer,
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFF0000FF));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(path_shape, NONE, "Draw round rect and circle");
