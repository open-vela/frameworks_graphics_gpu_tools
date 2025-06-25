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

#ifndef GPU_BUFFER_BLUR_H
#define GPU_BUFFER_BLUR_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdbool.h>
#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_buffer_s;

typedef enum {
    GPU_BUF_BLUR_TYPE_EXP,
    GPU_BUF_BLUR_TYPE_STACK,
    GPU_BUF_BLUR_TYPE_GAUSSIAN,
} gpu_buf_blur_type_t;

typedef struct {
    gpu_buf_blur_type_t type; /**< The type of the blur effect*/
    uint16_t radius; /**< The radius of the blur*/
    uint8_t aprec; /**< The number of bits of the alpha channel (for exp blur only)*/
    uint8_t zprec; /**< The number of bits of the z-buffer (for exp blur only)*/
} gpu_buf_blur_args_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the effect arguments to default values.
 * @param args  the effect arguments to initialize
 */
void gpu_buf_blur_args_init(gpu_buf_blur_args_t* args);

/**
 * Apply a blur effect to the draw buffer.
 * @param dst_buf  the dst draw buffer to apply the effect
 * @param src_buf  the src draw buffer to apply the effect
 * @param args     the effect arguments
 * @return         the result of the effect (true for success, false for failure)
 */
bool gpu_buf_blur(
    struct gpu_buffer_s* dst_buf,
    const struct gpu_buffer_s* src_buf,
    const gpu_buf_blur_args_t* args);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_BUFFER_BLUR_H*/
