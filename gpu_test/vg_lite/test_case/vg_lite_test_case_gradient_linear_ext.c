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
    vg_lite_color_ramp_t color_ramp[] = {
        { .stop = 0.25f, .red = 1, .green = 0, .blue = 0, .alpha = 1 },
        { .stop = 0.50f, .red = 0, .green = 1, .blue = 0, .alpha = 1 },
        { .stop = 0.75f, .red = 0, .green = 0, .blue = 1, .alpha = 1 }
    };

    const vg_lite_linear_gradient_parameter_t grad_param = {
        .X0 = 0,
        .Y0 = 0,
        .X1 = 100,
        .Y1 = 100,
    };

    static vg_lite_ext_linear_gradient_t linear_grad;

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_set_linear_grad(
            &linear_grad,
            sizeof(color_ramp) / sizeof(vg_lite_color_ramp_t),
            color_ramp,
            grad_param,
            VG_LITE_GRADIENT_SPREAD_PAD,
            1));

    vg_lite_matrix_t* grad_mat_p = vg_lite_get_linear_grad_matrix(&linear_grad);
    vg_lite_identity(grad_mat_p);

    vg_lite_test_context_set_user_data(ctx, &linear_grad);

    VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_update_linear_grad(&linear_grad));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_draw(struct vg_lite_test_context_s* ctx)
{
    vg_lite_ext_linear_gradient_t* linear_grad = vg_lite_test_context_get_user_data(ctx);

    struct vg_lite_test_path_s* path = vg_lite_test_context_init_path(ctx, VG_LITE_FP32);
    vg_lite_test_path_set_bounding_box(path, 0, 0, 100, 100);
    vg_lite_test_path_append_rect(path, 0, 0, 100, 100, 10);
    vg_lite_test_path_end(path);

    vg_lite_matrix_t matrix;
    vg_lite_test_context_get_transform(ctx, &matrix);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw_linear_grad(
            vg_lite_test_context_get_target_buffer(ctx),
            vg_lite_test_path_get_path(path),
            VG_LITE_FILL_EVEN_ODD,
            &matrix,
            linear_grad,
            0,
            VG_LITE_BLEND_SRC_OVER,
            VG_LITE_FILTER_BI_LINEAR));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    vg_lite_ext_linear_gradient_t* linear_grad = vg_lite_test_context_get_user_data(ctx);
    if (linear_grad) {
        VG_LITE_TEST_CHECK_ERROR_RETURN(vg_lite_clear_linear_grad(linear_grad));
    }
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(gradient_linear_ext, LINEAR_GRADIENT_EXT, "Draw a RGB linear gradient use LINEAR_GRADIENT_EXT feature");
