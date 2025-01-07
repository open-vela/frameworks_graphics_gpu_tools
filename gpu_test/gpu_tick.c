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

#include "gpu_tick.h"
#include <time.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static uint32_t tick_get_cb_default(void);

/**********************
 *  STATIC VARIABLES
 **********************/

static gpu_tick_get_cb_t tick_get_cb = tick_get_cb_default;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void gpu_tick_set_cb(gpu_tick_get_cb_t cb)
{
    tick_get_cb = cb;
}

uint32_t gpu_tick_get(void)
{
    return tick_get_cb();
}

uint32_t gpu_tick_elaps(uint32_t prev_tick)
{
    uint32_t act_time = gpu_tick_get();

    if (act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    } else {
        prev_tick = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}

void gpu_delay(uint32_t ms)
{
    usleep(ms * 1000);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static uint32_t tick_get_cb_default(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}
