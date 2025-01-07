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

#include "../resource/glphy_paths.h"
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
    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_path_t path;
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_init_path(
        &path,
        VG_LITE_S16,
        VG_LITE_HIGH,
        sizeof(glphy_u0030_path_data),
        (void*)glphy_u0030_path_data, -10000, -10000, 10000, 10000));

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);
    vg_lite_translate(0, 50, &matrix);
    vg_lite_scale(0.005, 0.005, &matrix);

    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);

    const vg_lite_quality_t quality_settings[] = {
        VG_LITE_LOW,
        VG_LITE_MEDIUM,
        VG_LITE_HIGH,
    };

    for (int i = 0; i < sizeof(quality_settings) / sizeof(vg_lite_quality_t); i++) {

        /* Set the path quality */
        path.quality = quality_settings[i];

        vg_lite_translate(10000, 0, &matrix);

        VG_LITE_TEST_CHECK_ERROR_RETURN(
            vg_lite_draw(
                target_buffer,
                &path,
                VG_LITE_FILL_NON_ZERO,
                &matrix,
                VG_LITE_BLEND_SRC_OVER,
                0xFFFFFFFF));
    }

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(path_quality, NONE, "Draw '0' glyph with LOW/MEDIUM/HIGH path quality settings");
