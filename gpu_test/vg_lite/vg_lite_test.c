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

#include "../gpu_context.h"
#include "../gpu_log.h"
#include "../gpu_recorder.h"
#include "../gpu_screenshot.h"
#include "../gpu_tick.h"
#include "vg_lite_test_context.h"
#include "vg_lite_test_utils.h"
#include <stdlib.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct vg_lite_test_iter_s {
    enum gpu_test_mode_e mode;
    const struct vg_lite_test_item_s* item;
    const struct vg_lite_test_item_s** group;
    int group_size;
    int name_to_index;
    int current_index;
    int current_loop_count;
    int total_loop_count;
    int failed_count;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void vg_lite_test_run_group(struct gpu_test_context_s* ctx);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int vg_lite_test_run(struct gpu_test_context_s* ctx)
{
    vg_lite_test_dump_info();
    vg_lite_test_run_group(ctx);
    GPU_LOG_INFO("GPU test finish");
    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int vg_lite_test_name_to_index(const struct vg_lite_test_item_s** group, int group_size, const char* name)
{
    if (!name) {
        return -1;
    }

    for (int i = 0; i < group_size; i++) {
        if (strcmp(group[i]->name, name) == 0) {
            return i;
        }
    }

    return -1;
}

static bool vg_lite_test_iter_next(struct vg_lite_test_iter_s* iter)
{
    iter->current_loop_count++;

    switch (iter->mode) {
    case GPU_TEST_MODE_DEFAULT: {
        /* Check if there is a specific test case to run */
        if (iter->name_to_index >= 0) {
            if (iter->current_loop_count == 1) {
                iter->item = iter->group[iter->name_to_index];
                return true;
            }

            return false;
        }

        if (iter->current_index >= iter->group_size) {
            return false;
        }

        iter->item = iter->group[iter->current_index++];
        return true;
    }

    case GPU_TEST_MODE_STRESS: {
        if (iter->failed_count > 0) {
            return false;
        }

        GPU_LOG_INFO("Test loop count: %d/%d", iter->current_loop_count, iter->total_loop_count);
        if (iter->current_loop_count >= iter->total_loop_count) {
            GPU_LOG_INFO("Test loop count reached, exit");
            return false;
        }

        iter->current_index = iter->name_to_index >= 0 ? iter->name_to_index : (rand() % iter->group_size);
        iter->item = iter->group[iter->current_index];
        return true;
    }

    default:
        GPU_LOG_ERROR("Unsupported test mode: %d", iter->mode);
        break;
    }

    return false;
}

static void vg_lite_test_run_group(struct gpu_test_context_s* ctx)
{
    /* Import testcase entry */

#define ITEM_DEF(NAME) extern struct vg_lite_test_item_s vg_lite_test_case_item_##NAME;
#include "test_case/vg_lite_test_case.inc"
#undef ITEM_DEF

#define ITEM_DEF(NAME) &vg_lite_test_case_item_##NAME,
    static const struct vg_lite_test_item_s* vg_lite_test_group[] = {
#include "test_case/vg_lite_test_case.inc"
    };
#undef ITEM_DEF

    const int group_size = sizeof(vg_lite_test_group) / sizeof(vg_lite_test_group[0]);
    const int name_to_index = vg_lite_test_name_to_index(vg_lite_test_group, group_size, ctx->param.testcase_name);

    /* Check if test case is valid */
    if (ctx->param.testcase_name && name_to_index < 0) {
        GPU_LOG_WARN("Test case not found: %s, Available test cases:", ctx->param.testcase_name);
        for (int i = 0; i < group_size; i++) {
            GPU_LOG_WARN("[%d/%d]: %s", i, group_size, vg_lite_test_group[i]->name);
        }
        return;
    }

    struct vg_lite_test_iter_s iter = { 0 };
    iter.mode = ctx->param.mode;
    iter.group = vg_lite_test_group;
    iter.group_size = group_size;
    iter.name_to_index = name_to_index;
    iter.total_loop_count = ctx->param.run_loop_count;

    struct vg_lite_test_context_s* vg_lite_ctx = vg_lite_test_context_create(ctx);

    while (vg_lite_test_iter_next(&iter)) {
        if (!vg_lite_test_context_run_item(vg_lite_ctx, iter.item)) {
            iter.failed_count++;
        }
    }

    vg_lite_test_context_destroy(vg_lite_ctx);

    GPU_LOG_WARN("Test result: %d failed / %d total", iter.failed_count, iter.current_loop_count - 1);
}
