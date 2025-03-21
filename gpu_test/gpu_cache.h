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

#ifndef GPU_CACHE_H
#define GPU_CACHE_H

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

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Invalidate the cache for a given memory range.
 * @param addr The start address of the memory range.
 * @param len The length of the memory range.
 */
void gpu_cache_invalidate(void* addr, size_t len);

/**
 * Clean the cache for a given memory range.
 * @param addr The start address of the memory range.
 * @param len The length of the memory range.
 */
void gpu_cache_clean(void* addr, size_t len);

/**
 * Flush the cache for a given memory range.
 * @param addr The start address of the memory range.
 * @param len The length of the memory range.
 */
void gpu_cache_flush(void* addr, size_t len);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_CACHE_H*/
