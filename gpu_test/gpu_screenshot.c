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

#include "gpu_screenshot.h"
#include "gpu_assert.h"
#include "gpu_buffer.h"
#include "gpu_cache.h"
#include "gpu_log.h"
#include <png.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static bool convert_color_to_bgr888(struct gpu_buffer_s* dest_buffer, const struct gpu_buffer_s* src_buffer);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int gpu_screenshot_save(const char* path, const struct gpu_buffer_s* buffer)
{
    GPU_ASSERT_NULL(path);
    GPU_ASSERT_NULL(buffer);

    GPU_LOG_INFO("Taking screenshot of '%s' ...", path);

    /* Construct the PNG image structure. */
    png_image image;
    memset(&image, 0, sizeof(image));

    image.version = PNG_IMAGE_VERSION;
    image.width = buffer->width;
    image.height = buffer->height;

    const struct gpu_buffer_s* target_buffer = buffer;
    bool need_convert = false;

    switch (buffer->format) {
    case GPU_COLOR_FORMAT_BGR888:
        image.format = PNG_FORMAT_BGR;
        break;

    case GPU_COLOR_FORMAT_BGRA8888:
        image.format = PNG_FORMAT_BGRA;
        break;

    case GPU_COLOR_FORMAT_BGR565:
    case GPU_COLOR_FORMAT_BGRA5658:
    case GPU_COLOR_FORMAT_BGRX8888:
        image.format = PNG_FORMAT_BGR;
        need_convert = true;
        break;

    default:
        GPU_LOG_ERROR("Unsupported color format: %d", buffer->format);
        return -1;
    }

    /* Invalidate the cache to ensure that the buffer data is up-to-date. */
    gpu_cache_invalidate(buffer->data, buffer->stride * buffer->height);

    struct gpu_buffer_s* temp_buffer = NULL;
    if (need_convert) {
        temp_buffer = gpu_buffer_alloc(
            buffer->width,
            buffer->height,
            GPU_COLOR_FORMAT_BGR888,
            image.width * sizeof(gpu_color24_t),
            8);

        if (!temp_buffer) {
            GPU_LOG_ERROR("Failed to allocate temporary buffer for conversion");
            return -1;
        }

        if (!convert_color_to_bgr888(temp_buffer, buffer)) {
            gpu_buffer_free(temp_buffer);
            return -1;
        }

        /* Change the target buffer to the temporary buffer for conversion. */
        target_buffer = temp_buffer;
    }

    /* Write the PNG image. */
    int is_success = png_image_write_to_file(&image, path, 0, target_buffer->data, target_buffer->stride, NULL);
    if (is_success) {
        GPU_LOG_INFO("Successed");
    } else {
        GPU_LOG_ERROR("Failed");
    }

    if (temp_buffer) {
        gpu_buffer_free(temp_buffer);
    }

    return is_success ? 0 : -1;
}

struct gpu_buffer_s* gpu_screenshot_load(const char* path)
{
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    if (!png_image_begin_read_from_file(&image, path)) {
        GPU_LOG_WARN("Failed to read PNG image from %s", path);
        return NULL;
    }

    struct gpu_buffer_s* buffer = gpu_buffer_alloc(image.width, image.height, GPU_COLOR_FORMAT_BGRA8888, image.width * sizeof(uint32_t), 8);

    image.format = PNG_FORMAT_BGRA;

    if (!png_image_finish_read(&image, NULL, buffer->data, buffer->stride, NULL)) {
        GPU_LOG_WARN("Failed to finish reading PNG image from %s", path);
        gpu_buffer_free(buffer);
        return NULL;
    }

    png_image_free(&image);
    return buffer;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool convert_color_to_bgr888(struct gpu_buffer_s* dest_buffer, const struct gpu_buffer_s* src_buffer)
{
    switch (src_buffer->format) {
    case GPU_COLOR_FORMAT_BGR565: {
        for (int y = 0; y < src_buffer->height; y++) {
            const gpu_color16_t* src = (const gpu_color16_t*)((uint8_t*)src_buffer->data + y * src_buffer->stride);
            gpu_color24_t* dest = (gpu_color24_t*)((uint8_t*)dest_buffer->data + y * dest_buffer->stride);

            for (int x = 0; x < src_buffer->width; x++) {
                dest->ch.blue = src->ch.blue * 0xFF / 0x1F;
                dest->ch.green = src->ch.green * 0xFF / 0x3F;
                dest->ch.red = src->ch.red * 0xFF / 0x1F;
                src++;
                dest++;
            }
        }
    } break;

    case GPU_COLOR_FORMAT_BGRA5658: {
        for (int y = 0; y < src_buffer->height; y++) {
            const gpu_color16_alpha_t* src = (const gpu_color16_alpha_t*)((uint8_t*)src_buffer->data + y * src_buffer->stride);
            gpu_color24_t* dest = (gpu_color24_t*)((uint8_t*)dest_buffer->data + y * dest_buffer->stride);

            for (int x = 0; x < src_buffer->width; x++) {
                dest->ch.blue = src->ch.blue * 0xFF / 0x1F;
                dest->ch.green = src->ch.green * 0xFF / 0x3F;
                dest->ch.red = src->ch.red * 0xFF / 0x1F;
                src++;
                dest++;
            }
        }
    } break;

    case GPU_COLOR_FORMAT_BGRX8888: {
        for (int y = 0; y < src_buffer->height; y++) {
            const gpu_color32_t* src = (const gpu_color32_t*)((uint8_t*)src_buffer->data + y * src_buffer->stride);
            gpu_color24_t* dest = (gpu_color24_t*)((uint8_t*)dest_buffer->data + y * dest_buffer->stride);

            for (int x = 0; x < src_buffer->width; x++) {
                dest->ch.blue = src->ch.blue;
                dest->ch.green = src->ch.green;
                dest->ch.red = src->ch.red;
                src++;
                dest++;
            }
        }
    } break;

    default:
        GPU_LOG_ERROR("Unsupported color format: %d", src_buffer->format);
        return false;
    }

    return true;
}
