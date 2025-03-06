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

#ifndef GPU_COLOR_H
#define GPU_COLOR_H

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

typedef enum gpu_color_format_e {
    GPU_COLOR_FORMAT_UNKNOWN,
    GPU_COLOR_FORMAT_BGR565,
    GPU_COLOR_FORMAT_BGR888,
    GPU_COLOR_FORMAT_BGRA8888,
    GPU_COLOR_FORMAT_BGRX8888,
    GPU_COLOR_FORMAT_BGRA5658,
    GPU_COLOR_FORMAT_INDEX_8,
    GPU_COLOR_FORMAT_A4,
    GPU_COLOR_FORMAT_A8,
} gpu_color_format_t;

#pragma pack(1)

typedef union gpu_color_bgra8888_u {
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } ch;
    uint32_t full;
} gpu_color32_t, gpu_color_bgra8888_t;

typedef union gpu_color_bgr888_u {
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    } ch;
    uint8_t full[3];
} gpu_color24_t, gpu_color_bgr888_t;

typedef union gpu_color_bgr565_u {
    struct
    {
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
    } ch;
    uint16_t full;
} gpu_color16_t, gpu_color_bgr565_t;

typedef struct gpu_color_bgra5658_u {
    struct
    {
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
        uint16_t alpha : 8;
    } ch;
    uint8_t full[3];
} gpu_color16_alpha_t, gpu_color_bgra5658_t;

#pragma pack()

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Get the number of bits per pixel for a given color format
 * @param format The color format to get the bits per pixel for
 * @return The number of bits per pixel for the given color format
 */
uint32_t gpu_color_format_get_bpp(gpu_color_format_t format);

/**
 * @brief Compare two colors(bgra8888) for equality
 * @param color1 The first color to compare
 * @param color2 The second color to compare
 * @return True if the colors are equal, false otherwise
 */
 bool gpu_color_bgra8888_compare(gpu_color_bgra8888_t color1, gpu_color_bgra8888_t color2, int tolerance);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_COLOR_H*/
