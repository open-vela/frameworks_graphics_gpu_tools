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

#include "gpu_color.h"
#include "gpu_log.h"

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

uint32_t gpu_color_format_get_bpp(gpu_color_format_t format)
{
    switch (format) {
    case GPU_COLOR_FORMAT_BGR565:
        return sizeof(gpu_color_bgr565_t) * 8;

    case GPU_COLOR_FORMAT_BGR888:
        return sizeof(gpu_color_bgr888_t) * 8;

    case GPU_COLOR_FORMAT_BGRA8888:
    case GPU_COLOR_FORMAT_BGRX8888:
        return sizeof(gpu_color_bgra8888_t) * 8;

    case GPU_COLOR_FORMAT_BGRA5658:
        return sizeof(gpu_color_bgra5658_t) * 8;

    case GPU_COLOR_FORMAT_INDEX_8:
        return sizeof(uint8_t) * 8;

    case GPU_COLOR_FORMAT_A4:
        return 4;

    case GPU_COLOR_FORMAT_A8:
        return 8;

    default:
        GPU_LOG_ERROR("Unsupported color format: %d", format);
        break;
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
