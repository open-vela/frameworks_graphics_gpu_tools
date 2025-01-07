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

#ifndef GPU_UTILS_H
#define GPU_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stddef.h>

/*********************
 *      DEFINES
 *********************/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#define GPU_ALIGN_UP(number, align_bytes) \
    ((((uintptr_t)number) + ((align_bytes) - 1)) & ~(((uintptr_t)align_bytes) - 1))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Get the current local time as a string.
 * @return A string representing the current local time.
 */
const char* gpu_get_localtime_str(void);

/**
 * @brief Create a directory if it does not exist.
 * @param dir_path The path of the directory to create.
 * @return 0 if the directory was created successfully, -1 otherwise.
 */
int gpu_dir_create(const char* dir_path);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* GPU_UTILS_H */
