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

#ifndef GPU_SCREENSHOT_H
#define GPU_SCREENSHOT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_buffer_s;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Take a screenshot of the given buffer and save it to the given directory with the given name.
 * @param name The name of the screenshot file.
 * @param buffer The buffer to take the screenshot of.
 * @return 0 on success, -1 on failure.
 */
int gpu_screenshot_save(const char* path, const struct gpu_buffer_s* buffer);

/**
 * @brief Load a screenshot from the given directory with the given name.
 * @param name The name of the screenshot file.
 * @return The buffer containing the screenshot, or NULL on failure.
 */
struct gpu_buffer_s* gpu_screenshot_load(const char* path);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* GPU_SCREENSHOT_H */
