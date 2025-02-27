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

/* clang-format off */

/* '启' */
static const int16_t glphy_u542f_path_data[] = {
    VLC_OP_QUAD, 2048.00, -2408.00, 1822.00, -1531.00,
    VLC_OP_QUAD, 1597.00, -655.00, 1049.00, 434.00,
    VLC_OP_LINE, 377.00, -41.00,
    VLC_OP_QUAD, 745.00, -754.00, 937.00, -1323.00,
    VLC_OP_QUAD, 1130.00, -1892.00, 1212.00, -2531.00,
    VLC_OP_QUAD, 1294.00, -3170.00, 1294.00, -4096.00,
    VLC_OP_LINE, 1294.00, -6128.00,
    VLC_OP_LINE, 3981.00, -6128.00,
    VLC_OP_LINE, 3777.00, -6709.00,
    VLC_OP_LINE, 3703.00, -6914.00,
    VLC_OP_LINE, 4538.00, -7053.00,
    VLC_OP_LINE, 4604.00, -6865.00,
    VLC_OP_QUAD, 4702.00, -6586.00, 4874.00, -6128.00,
    VLC_OP_LINE, 7340.00, -6128.00,
    VLC_OP_MOVE, 2073.00, -4342.00,
    VLC_OP_LINE, 6545.00, -4342.00,
    VLC_OP_LINE, 6545.00, -5423.00,
    VLC_OP_LINE, 2073.00, -5423.00,
    VLC_OP_LINE, 2073.00, -4342.00,
    VLC_OP_MOVE, 3080.00, 606.00,
    VLC_OP_LINE, 2302.00, 606.00,
    VLC_OP_LINE, 2302.00, -2720.00,
    VLC_OP_LINE, 7176.00, -2720.00,
    VLC_OP_LINE, 7176.00, 606.00,
    VLC_OP_LINE, 6382.00, 606.00,
    VLC_OP_LINE, 6382.00, 131.00,
    VLC_OP_LINE, 3080.00, 131.00,
    VLC_OP_LINE, 3080.00, 606.00,
    VLC_OP_MOVE, 6382.00, -557.00,
    VLC_OP_LINE, 6382.00, -2007.00,
    VLC_OP_LINE, 3080.00, -2007.00,
    VLC_OP_LINE, 3080.00, -557.00,
    VLC_OP_LINE, 6382.00, -557.00,
    VLC_OP_END
};

/* clang-format on */

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
        sizeof(glphy_u542f_path_data),
        (void*)glphy_u542f_path_data, 372.36, 372.36, 7447.27, 744.73));

    vg_lite_matrix_t matrix = { 0 };
    matrix.m[0][0] = 0.002286;
    matrix.m[0][1] = 0.000000;
    matrix.m[0][2] = 117.164063;
    matrix.m[1][0] = 0.000000;
    matrix.m[1][1] = 0.002286;
    matrix.m[1][2] = 255.742188;
    matrix.m[2][0] = 0.000000;
    matrix.m[2][1] = 0.000000;
    matrix.m[2][2] = 1.000000;

    vg_lite_buffer_t* target_buffer = vg_lite_test_context_get_target_buffer(ctx);

    VG_LITE_TEST_CHECK_ERROR_RETURN(
        vg_lite_draw(
            target_buffer,
            &path,
            VG_LITE_FILL_NON_ZERO,
            &matrix,
            VG_LITE_BLEND_SRC_OVER,
            0xFF0000FF));

    return VG_LITE_SUCCESS;
}

static vg_lite_error_t on_teardown(struct vg_lite_test_context_s* ctx)
{
    return VG_LITE_SUCCESS;
}

VG_LITE_TEST_CASE_ITEM_DEF(path_bounding_box, NONE, "Draw '启' path and use boundbox to crop the bottom");
