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

#ifndef GPU_TICK_H
#define GPU_TICK_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef uint32_t (*gpu_tick_get_cb_t)(void);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Set the callback function to get the GPU tick
 * @param cb The callback function to get the GPU tick
 */
void gpu_tick_set_cb(gpu_tick_get_cb_t cb);

/**
 * @brief Get the GPU tick
 * @return The GPU tick
 */
uint32_t gpu_tick_get(void);

/**
 * @brief Get the elapsed time between two GPU ticks
 * @param prev_tick The previous GPU tick
 * @return The elapsed time in GPU ticks
 */
uint32_t gpu_tick_elaps(uint32_t prev_tick);

/**
 * @brief Delay for a specified number of milliseconds
 * @param ms The number of milliseconds to delay
 */
void gpu_delay(uint32_t ms);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_TICK_H*/
