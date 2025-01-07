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

#include "../resource/image_cogwheel_index8.h"
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
    vg_lite_test_context_load_src_image(
        ctx,
        imgae_cogwheel_index8_map,
        IMAGE_COGWHEEL_INDEX8_WIDTH,
        IMAGE_COGWHEEL_INDEX8_HEIGHT,
        IMAGE_COGWHEEL_INDEX8_FORMAT,
        IMAGE_COGWHEEL_INDEX8_STRIDE);

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_gaussian_filter(0.2f, 0.1f, 0.1f));

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_set_CLUT(
        sizeof(imgae_cogwheel_index8_color_table) / sizeof(vg_lite_uint32_t),
        (vg_lite_uint32_t*)imgae_cogwheel_index8_color_table));

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            vg_lite_test_context_get_target_buffer(ctx),
            vg_lite_test_context_get_src_buffer(ctx),
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

VG_LITE_TEST_CASE_ITEM_DEF(blur_gaussian, GAUSSIAN_BLUR, "Test gaussian blur filter");
