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

#include "../../gpu_math.h"
#include "../vg_lite_test_context.h"
#include "../vg_lite_test_utils.h"
#include "../vg_lite_test_path.h"
#include <string.h>

/*********************
 *      DEFINES
 *********************/

#ifdef SQUARE
#undef SQUARE
#endif
#define SQUARE(x) ((x) * (x))

#ifndef M_PI
#define M_PI 3.1415926f
#endif

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

static void grad_point_to_matrix(vg_lite_matrix_t* grad_matrix, float x1, float y1, float x2, float y2)
{
    vg_lite_translate(x1, y1, grad_matrix);

    float angle = atan2f(y2 - y1, x2 - x1) * 180.0f / (float)M_PI;
    vg_lite_rotate(angle, grad_matrix);
    float length = MATH_SQRTF(SQUARE(x2 - x1) + SQUARE(y2 - y1));
    vg_lite_scale(length / 256.0f, 1, grad_matrix);
}

static vg_lite_error_t on_setup(struct vg_lite_test_context_s* ctx)
{
    static vg_lite_linear_gradient_t linear_grad;
    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_init_grad(&linear_grad));

    vg_lite_test_context_set_user_data(ctx, &linear_grad);

    vg_lite_uint32_t colors[] = {
        0xFFFF0000,
        0xFF00FF00,
        0xFF0000FF,
    };

    vg_lite_uint32_t stops[] = {
        64,
        128,
        192,
    };

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_set_grad(&linear_grad, 3, colors, stops));

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_update_grad(&linear_grad));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_linear_gradient_t* linear_grad = vg_lite_test_context_get_user_data(ctx);

    vg_lite_matrix_t* grad_mat_p = vg_lite_get_grad_matrix(linear_grad);
    vg_lite_identity(grad_mat_p);
    grad_point_to_matrix(grad_mat_p, 0, 0, 100, 100);

    struct vg_lite_test_path_s* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);
    vg_lite_test_path_set_bounding_box(path, 0, 0, 100, 100);
    vg_lite_test_path_append_rect(path, 0, 0, 100, 100, 10);
    vg_lite_test_path_end(path);

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw_grad(
            vg_lite_test_context_get_target_buffer(ctx),
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            linear_grad,
            VG_LITE_BLEND_SRC_OVER));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    vg_lite_linear_gradient_t* linear_grad = vg_lite_test_context_get_user_data(ctx);
    if (linear_grad) {
        VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear_grad(linear_grad));
    }
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(gradient_linear, NONE, "Draw a RGB linear gradient");
