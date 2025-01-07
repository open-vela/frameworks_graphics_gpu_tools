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

#ifndef GPU_BUFFER_H
#define GPU_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "gpu_color.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_buffer_s {
    enum gpu_color_format_e format;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    void* data;
    void* data_unaligned;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Allocate a new GPU buffer with the given format, width, height, and stride.
 * The buffer will be initialized with zeros.
 * @param width The width of the buffer in pixels.
 * @param height The height of the buffer in pixels.
 * @param format The color format of the buffer.
 * @param stride The stride of the buffer in bytes.
 * @param align The alignment of the start address of the buffer.
 * @return A pointer to the new GPU buffer, or NULL if there was an error.
 */
struct gpu_buffer_s* gpu_buffer_alloc(uint32_t width, uint32_t height, enum gpu_color_format_e format, uint32_t stride, uint32_t align);

/**
 * Free a GPU buffer.
 * @param buffer The GPU buffer to free.
 */
void gpu_buffer_free(struct gpu_buffer_s* buffer);

/**
 * Get the pixel at the given position in the buffer.
 * @param buffer The GPU buffer to get the pixel from.
 * @param x The x position of the pixel.
 * @param y The y position of the pixel.
 * @return The pixel data (BGRA8888 format).
 */
uint32_t gpu_buffer_get_pixel(struct gpu_buffer_s* buffer, uint32_t x, uint32_t y);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_TEMPL_H*/
