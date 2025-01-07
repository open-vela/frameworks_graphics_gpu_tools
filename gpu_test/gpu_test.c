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

#include "gpu_test.h"
#include "gpu_context.h"
#include "gpu_recorder.h"
#include "gpu_tick.h"
#include "vg_lite/vg_lite_test.h"
#include <stdio.h>
#include <stdlib.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void gpu_test_write_header(struct gpu_test_context_s* ctx);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int gpu_test_run(struct gpu_test_context_s* ctx)
{
    switch (ctx->param.mode) {
    case GPU_TEST_MODE_DEFAULT:
        ctx->recorder = gpu_recorder_create(ctx->param.output_dir, "vg_lite");
        gpu_test_write_header(ctx);
        break;

    case GPU_TEST_MODE_STRESS:
        /* Seed the random number generator with the current time */
        srand(gpu_tick_get());
        break;

    default:
        break;
    }

    int ret = vg_lite_test_run(ctx);

    if (ctx->recorder) {
        gpu_recorder_delete(ctx->recorder);
    }

    return ret;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void gpu_test_write_header(struct gpu_test_context_s* ctx)
{
    if (!ctx->recorder) {
        return;
    }

    gpu_recorder_write_string(ctx->recorder, "Command Line,");

    for (int i = 0; i < ctx->param.argc; i++) {
        gpu_recorder_write_string(ctx->recorder, ctx->param.argv[i]);
        gpu_recorder_write_string(ctx->recorder, " ");
    }

    gpu_recorder_write_string(ctx->recorder, "\n\n");
}
