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

#include "gpu_buffer.h"
#include "gpu_assert.h"
#include "gpu_log.h"
#include "gpu_utils.h"
#include <stdlib.h>
#include <string.h>

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

struct gpu_buffer_s* gpu_buffer_alloc(uint32_t width, uint32_t height, enum gpu_color_format_e format, uint32_t stride, uint32_t align)
{
    GPU_ASSERT(width > 0);
    GPU_ASSERT(height > 0);
    GPU_ASSERT(stride > 0);

    struct gpu_buffer_s* buffer = calloc(1, sizeof(struct gpu_buffer_s));
    GPU_ASSERT_NULL(buffer);

    buffer->format = format;
    buffer->width = width;
    buffer->height = height;
    buffer->stride = stride;

    buffer->data_unaligned = calloc(1, stride * height + align);
    GPU_ASSERT_NULL(buffer->data_unaligned);
    buffer->data = (void*)GPU_ALIGN_UP(buffer->data_unaligned, align);

    GPU_LOG_DEBUG("Allocated buffer %p, format %d, size W%dxH%d, stride %d, data %p",
        buffer, format, width, height, stride, buffer->data);

    return buffer;
}

void gpu_buffer_free(struct gpu_buffer_s* buffer)
{
    GPU_ASSERT_NULL(buffer);
    GPU_ASSERT_NULL(buffer->data);
    GPU_ASSERT_NULL(buffer->data_unaligned);

    GPU_LOG_DEBUG("Freed buffer %p, format %d, size W%dxH%d, stride %d, data %p",
        buffer, buffer->format, buffer->width, buffer->height, buffer->stride, buffer->data);
    free(buffer->data_unaligned);

    memset(buffer, 0, sizeof(struct gpu_buffer_s));
    free(buffer);
}

uint32_t gpu_buffer_get_pixel(struct gpu_buffer_s* buffer, uint32_t x, uint32_t y)
{
    GPU_ASSERT_NULL(buffer);

    if (x >= buffer->width || y >= buffer->height) {
        GPU_LOG_ERROR("Invalid coordinates X%d Y%d for buffer W%dxH%d", x, y, buffer->width, buffer->height);
        return 0;
    }

    const void* pixel = (const uint8_t*)buffer->data + y * buffer->stride + x * gpu_color_format_get_bpp(buffer->format) / 8;

    switch (buffer->format) {
    case GPU_COLOR_FORMAT_BGR565: {
        const gpu_color_bgr565_t* c16 = pixel;
        gpu_color_bgra8888_t c32;
        c32.ch.blue = c16->ch.blue * 0xFF / 0x1F;
        c32.ch.green = c16->ch.green * 0xFF / 0x3F;
        c32.ch.red = c16->ch.red * 0xFF / 0x1F;
        c32.ch.alpha = 0xFF;
        return c32.full;
    }

    case GPU_COLOR_FORMAT_BGR888: {
        const gpu_color_bgr888_t* c24 = pixel;
        gpu_color_bgra8888_t c32;
        c32.ch.blue = c24->ch.blue;
        c32.ch.green = c24->ch.green;
        c32.ch.red = c24->ch.red;
        c32.ch.alpha = 0xFF;
        return c32.full;
    }

    case GPU_COLOR_FORMAT_BGRA8888:
        return *(const uint32_t*)pixel;

    case GPU_COLOR_FORMAT_BGRX8888: {
        gpu_color_bgra8888_t c32 = *(gpu_color_bgra8888_t*)pixel;
        c32.ch.alpha = 0xFF;
        return c32.full;
    }

    case GPU_COLOR_FORMAT_BGRA5658: {
        const gpu_color_bgra5658_t* c16a = pixel;
        gpu_color_bgra8888_t c32;
        c32.ch.blue = c16a->ch.blue * 0xFF / 0x1F;
        c32.ch.green = c16a->ch.green * 0xFF / 0x3F;
        c32.ch.red = c16a->ch.red * 0xFF / 0x1F;
        c32.ch.alpha = c16a->ch.alpha;
        return c32.full;
    }

    default:
        GPU_LOG_ERROR("Unsupported color format: %d", buffer->format);
        break;
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
