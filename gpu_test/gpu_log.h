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

#ifndef GPU_LOG_H
#define GPU_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

#define GPU_LOG_DEBUG(format, ...)      \
    gpu_log_printf(GPU_LOG_LEVEL_DEBUG, \
        __func__,                       \
        format,                         \
        ##__VA_ARGS__)
#define GPU_LOG_INFO(format, ...)      \
    gpu_log_printf(GPU_LOG_LEVEL_INFO, \
        __func__,                      \
        format,                        \
        ##__VA_ARGS__)
#define GPU_LOG_WARN(format, ...)      \
    gpu_log_printf(GPU_LOG_LEVEL_WARN, \
        __func__,                      \
        format,                        \
        ##__VA_ARGS__)
#define GPU_LOG_ERROR(format, ...)      \
    gpu_log_printf(GPU_LOG_LEVEL_ERROR, \
        __func__,                       \
        format,                         \
        ##__VA_ARGS__)

/**********************
 *      TYPEDEFS
 **********************/

enum gpu_log_level_type_e {
    GPU_LOG_LEVEL_DEBUG,
    GPU_LOG_LEVEL_INFO,
    GPU_LOG_LEVEL_WARN,
    GPU_LOG_LEVEL_ERROR,
    _GPU_LOG_LEVEL_LAST
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Print log message
 * @param level Log level
 * @param func Function name
 * @param format Log message format
 * @param... Variable arguments
 */
void gpu_log_printf(enum gpu_log_level_type_e level, const char* func, const char* format, ...);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* GPU_LOG_H */
