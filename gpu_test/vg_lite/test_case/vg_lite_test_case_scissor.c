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

#include "../resource/image_circle_a8.h"
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
#if VGLITE_RELEASE_VERSION <= VGLITE_MAKE_VERSION(4, 0, 57)
    /**
     * In the new version of VG-Lite, vg_lite_set_scissor no longer needs to call vg_lite_enable_scissor and
     * vg_lite_disable_scissor APIs.
     *
     * Original description in the manual:
     * Description: This is a legacy scissor API function that can be used to set and enable a single scissor rectangle
     * for the render target. This scissor API is supported by a different hardware mechanism other than the mask layer,
     * and it is not enabled/disabled by vg_lite_enable_scissor and vg_lite_disable_scissor APIs.
     */
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_enable_scissor());
#endif

    vg_lite_test_context_load_src_image(
        ctx,
        image_circle_a8_map,
        IMAGE_CIRCLE_A8_WIDTH,
        IMAGE_CIRCLE_A8_HEIGHT,
        IMAGE_CIRCLE_A8_FORMAT,
        IMAGE_CIRCLE_A8_STRIDE);

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);

    float x1 = 100;
    float y1 = 100;
    float x2 = 300;
    float y2 = 300;

    vg_lite_test_transform_point(&x1, &y1, &matrix);
    vg_lite_test_transform_point(&x2, &y2, &matrix);

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_set_scissor(x1, y1, x2, y2));

    struct vg_lite_test_path_s* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);
    vg_lite_test_path_set_bounding_box(path, 0, 0, 200, 200);
    vg_lite_test_path_append_circle(path, 100, 100, 100, 100);
    vg_lite_test_path_end(path);

    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw(
            target_buffer,
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFFFF0000));

    vg_lite_buffer_t* image = vg_lite_test_context_get_src_buffer(ctx);
    vg_lite_translate(250, 250, &matrix);
    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_blit(
            target_buffer,
            image,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFF0000FF,
            VG_LITE_FILTER_BI_LINEAR));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    /* Reset the scissor to the full screen */
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_set_scissor(0, 0, target_buffer->width, target_buffer->height));
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(scissor, SCISSOR, "Test scissor clipping of path and image");
