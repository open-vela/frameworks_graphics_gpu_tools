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
    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);
    vg_lite_rectangle_t rect = { 0, 0, target_buffer->width, target_buffer->height };

    /* Black */
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, NULL, 0xFF000000));

    /* White */
    rect.width /= 2;
    rect.height /= 2;
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, &rect, 0xFFFFFFFF));

    /* Blue */
    rect.width /= 2;
    rect.height /= 2;
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, &rect, 0xFFFF0000));

    /* Green */
    rect.width /= 2;
    rect.height /= 2;
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, &rect, 0xFF00FF00));

    /* Red */
    rect.width /= 2;
    rect.height /= 2;
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear(target_buffer, &rect, 0xFF0000FF));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(clear, NONE, "Clear 5 areas with different color");
