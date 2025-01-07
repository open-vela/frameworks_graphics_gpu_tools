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

#ifndef GPU_FB_H
#define GPU_FB_H

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

struct gpu_fb_s;
struct gpu_buffer_s;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a GPU framebuffer object
 * @param path The path to the device to use
 * @return The framebuffer object or NULL if an error occurred
 */
struct gpu_fb_s* gpu_fb_create(const char* path);

/**
 * Destroy a GPU framebuffer object
 * @param fb The framebuffer object to destroy
 */
void gpu_fb_destroy(struct gpu_fb_s* fb);

/**
 * Get the GPU buffer object associated with the framebuffer
 * @param fb The framebuffer object
 * @param buffer The buffer object to fill in
 */
void gpu_fb_get_buffer(const struct gpu_fb_s* fb, struct gpu_buffer_s* buffer);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_FB_H*/
