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
    vg_lite_test_path_t* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);
    vg_lite_test_path_set_bounding_box(path, -240, -240, 240, 240);
    vg_lite_test_path_append_circle(path, 0, 0, 50, 50);
    vg_lite_test_path_end(path);

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    const vg_lite_blend_t blend_modes[] = {
        VG_LITE_BLEND_NONE,
        VG_LITE_BLEND_SRC_OVER,
        VG_LITE_BLEND_DST_OVER,
        VG_LITE_BLEND_SRC_IN,
        VG_LITE_BLEND_DST_IN,
        VG_LITE_BLEND_MULTIPLY,
        VG_LITE_BLEND_SCREEN,
        VG_LITE_BLEND_ADDITIVE,
        VG_LITE_BLEND_SUBTRACT,
    };

    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, NULL, 0x5A5A5A5A));

    const vg_lite_color_t colors[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFFFFFF00 };
    const int color_len = sizeof(colors) / sizeof(vg_lite_color_t);

    vg_lite_path_t* vg_path = vg_lite_test_path_get_path(vg_lite_test_context_get_path(ctx));

    for (int i = 0; i < sizeof(blend_modes) / sizeof(vg_lite_blend_t); i++) {

        const vg_lite_blend_t blend_mode = blend_modes[i];
        vg_lite_matrix_t matrix;
        vg_lite_test_context_get_transform(ctx, &matrix);
        vg_lite_translate(100 + 80 * (i % color_len), 100 + 80 * (i / color_len), &matrix);

        VG_LITE_TEST_CHECK_ERROR_RETURN(
            vg_lite_draw(
                target_buffer,
                vg_path,
                VG_LITE_FILL_NON_ZERO,
                &matrix,
                blend_mode,
                colors[i % color_len]));
    }

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(blend_mode_base, NONE, "Test blend mode base.");
