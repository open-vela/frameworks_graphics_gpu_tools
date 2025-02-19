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

#ifdef GPU_TEST_CONTEXT_NUTTX_ENABLE

/*********************
 *      INCLUDES
 *********************/

#include "gpu_assert.h"
#include "gpu_context.h"
#include "gpu_fb.h"
#include "gpu_log.h"
#include "gpu_tick.h"
#include <inttypes.h>
#include <nuttx/arch.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

#define TICK_TO_USEC(tick) ((tick) / g_cpu_freq_mhz)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static uint32_t tick_get_cb(void);
static uint32_t calc_avg_cpu_freq(void);

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t g_cpu_freq_mhz = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool gpu_test_context_setup(struct gpu_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);

    if (ctx->param.fbdev_path) {
        ctx->fb = gpu_fb_create(ctx->param.fbdev_path);

        if (!ctx->fb) {
            return false;
        }

        gpu_fb_get_buffer(ctx->fb, &ctx->target_buffer);
    }

    static bool initialized = false;

#ifdef CONFIG_ARCH_SIM
    /* In simulation mode, we need to initialize the GPU all the time */
    initialized = false;
#endif

    if (initialized) {
        GPU_LOG_INFO("GPU already initialized");
        return true;
    }

    initialized = true;

#ifdef CONFIG_GPU_TEST_CUSTOM_INIT
    GPU_LOG_INFO("Initializing GPU");
    extern void gpu_init(void);
    gpu_init();
#endif

    uint32_t cpu_freq_hz = 1;

    if (ctx->param.cpu_freq > 0) {
        g_cpu_freq_mhz = ctx->param.cpu_freq;
    } else {
        /* Enable performance counter */
        up_perf_init((void*)(uintptr_t)cpu_freq_hz);

        /* Calculate average CPU frequency */
        g_cpu_freq_mhz = calc_avg_cpu_freq() / 1000000;
    }

    GPU_LOG_INFO("CPU frequency: %" PRIu32 " MHz", g_cpu_freq_mhz);

    if (g_cpu_freq_mhz == 0) {
        GPU_LOG_ERROR("CPU frequency error");
        goto failed;
    }

    cpu_freq_hz = g_cpu_freq_mhz * 1000000;

    up_perf_init((void*)(uintptr_t)cpu_freq_hz);

    gpu_tick_set_cb(tick_get_cb);
    return true;

failed:
    if (ctx->fb) {
        gpu_fb_destroy(ctx->fb);
        ctx->fb = NULL;
    }

    return false;
}

void gpu_test_context_teardown(struct gpu_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);

    if (ctx->fb) {
        gpu_fb_destroy(ctx->fb);
        ctx->fb = NULL;
    }

#ifdef CONFIG_ARCH_SIM
    extern void gpu_deinit(void);
    GPU_LOG_INFO("Deinitializing GPU");
    gpu_deinit();
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static uint32_t tick_get_cb(void)
{
    static uint32_t prev_tick = 0;
    static uint32_t cur_tick_us = 0;
    uint32_t act_time = up_perf_gettime();
    uint32_t elaps;

    /*If there is no overflow in sys_time simple subtract*/
    if (act_time >= prev_tick) {
        elaps = act_time - prev_tick;
    } else {
        elaps = UINT32_MAX - prev_tick + 1;
        elaps += act_time;
    }

    cur_tick_us += TICK_TO_USEC(elaps);
    prev_tick = act_time;
    return cur_tick_us;
}

static uint32_t calc_avg_cpu_freq(void)
{
    uint32_t start_tick = up_perf_gettime();

    /* Wait 1 second*/
    usleep(1000 * 1000);

    uint32_t elapsed_tick = up_perf_gettime() - start_tick;

    GPU_LOG_INFO("perf elapsed_tick: %" PRIu32, elapsed_tick);

    return elapsed_tick;
}

#endif /* GPU_TEST_CONTEXT_NUTTX_ENABLE */
