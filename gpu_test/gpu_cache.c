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

#include "gpu_cache.h"
#include <stdint.h>

#ifdef GPU_CACHE_INCLUDE_H
#include GPU_CACHE_INCLUDE_H
#endif

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

void gpu_cache_invalidate(void* addr, size_t len)
{
#ifdef GPU_CACHE_INVALIDATE_FUNC
    GPU_CACHE_INVALIDATE_FUNC((uintptr_t)addr, (uintptr_t)addr + len);
#else
    (void)addr;
    (void)len;
#endif
}

void gpu_cache_clean(void* addr, size_t len)
{
#ifdef GPU_CACHE_CLEAN_FUNC
    GPU_CACHE_CLEAN_FUNC((uintptr_t)addr, (uintptr_t)addr + len);
#else
    (void)addr;
    (void)len;
#endif
}

void gpu_cache_flush(void* addr, size_t len)
{
#ifdef GPU_CACHE_FLUSH_FUNC
    GPU_CACHE_FLUSH_FUNC((uintptr_t)addr, (uintptr_t)addr + len);
#else
    (void)addr;
    (void)len;
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
