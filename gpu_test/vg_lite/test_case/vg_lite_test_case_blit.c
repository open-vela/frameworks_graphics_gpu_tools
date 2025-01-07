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

#include "../resource/image_needle_bgra8888.h"
#include "../vg_lite_test_context.h"
#include "../vg_lite_test_utils.h"
#include <string.h>

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

    vg_lite_test_context_load_src_image(
        ctx,
        image_needle_bgra8888_map,
        IMAGE_NEEDLE_BGRA8888_WIDTH,
        IMAGE_NEEDLE_BGRA8888_HEIGHT,
        IMAGE_NEEDLE_BGRA8888_FORMAT,
        IMAGE_NEEDLE_BGRA8888_STRIDE);

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, NULL, 0xFFFFFFFF));
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_finish());

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_buffer_t* image = vg_lite_test_context_get_src_buffer(ctx);

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);

    /* Normal blit */
    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            target_buffer,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0,
            VG_LITE_FILTER_BI_LINEAR));

    /* 50% transparent blit */
    image->image_mode = VG_LITE_MULTIPLY_IMAGE_MODE;
    vg_lite_translate(50, 0, &matrix);
    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            target_buffer,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0x7F7F7F7F,
            VG_LITE_FILTER_BI_LINEAR));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(blit, NONE, "Draw two needles opaque and 50 percent transparent");
