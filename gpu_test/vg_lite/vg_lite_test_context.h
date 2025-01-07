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

#ifndef VG_LITE_TEST_CONTEXT_H
#define VG_LITE_TEST_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdbool.h>
#include <vg_lite.h>

/*********************
 *      DEFINES
 *********************/

#define gcFEATURE_BIT_VG_NONE -1

#define VG_LITE_TEST_CASE_ITEM_DEF(NAME, FEATURE, INSTRUCTIONS)  \
    struct vg_lite_test_item_s vg_lite_test_case_item_##NAME = { \
        .name = #NAME,                                           \
        .instructions = INSTRUCTIONS,                            \
        .feature = gcFEATURE_BIT_VG_##FEATURE,                   \
        .on_setup = on_setup,                                    \
        .on_draw = on_draw,                                      \
        .on_teardown = on_teardown,                              \
    }

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_test_context_s;
struct vg_lite_test_path_s;
struct vg_lite_test_context_s;

typedef vg_lite_error_t (*vg_lite_test_func_t)(struct vg_lite_test_context_s* ctx);

struct vg_lite_test_item_s {
    const char* name;
    const char* instructions;
    int feature;
    vg_lite_test_func_t on_setup;
    vg_lite_test_func_t on_draw;
    vg_lite_test_func_t on_teardown;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Create a new test context
 * @param gpu_ctx The GPU test context to use
 * @return The new test context
 */
struct vg_lite_test_context_s* vg_lite_test_context_create(struct gpu_test_context_s* gpu_ctx);

/**
 * @brief Destroy a test context
 * @param ctx The test context to destroy
 */
void vg_lite_test_context_destroy(struct vg_lite_test_context_s* ctx);

/**
 * @brief Run a test case item
 * @param ctx The test context to use
 * @param item The test case item to run
 * @return True if the test case passed, false if it failed
 */
bool vg_lite_test_context_run_item(struct vg_lite_test_context_s* ctx, const struct vg_lite_test_item_s* item);

/**
 * @brief Get the target buffer for the test case
 * @param ctx The test context to use
 * @return The target buffer for the test case
 */
vg_lite_buffer_t* vg_lite_test_context_get_target_buffer(struct vg_lite_test_context_s* ctx);

/**
 * @brief Get the source buffer for the test case
 * @param ctx The test context to use
 * @return The source buffer for the test case
 */
vg_lite_buffer_t* vg_lite_test_context_get_src_buffer(struct vg_lite_test_context_s* ctx);

/**
 * @brief Allocate a source buffer for the test case
 * @param ctx The test context to use
 * @param width The width of the buffer
 * @param height The height of the buffer
 * @param format The format of the buffer
 * @param stride The stride of the buffer
 * @return The allocated source buffer
 */
vg_lite_buffer_t* vg_lite_test_context_alloc_src_buffer(
    struct vg_lite_test_context_s* ctx,
    uint32_t width,
    uint32_t height,
    vg_lite_buffer_format_t format,
    uint32_t stride);

/**
 * @brief Load the source image for the test case
 * @param ctx The test context to use
 * @param image_data The image data to load
 * @param width The width of the image
 * @param height The height of the image
 * @param format The format of the image
 * @param image_stride The stride of the image
 */
void vg_lite_test_context_load_src_image(
    struct vg_lite_test_context_s* ctx,
    const void* image_data,
    uint32_t width,
    uint32_t height,
    vg_lite_buffer_format_t format,
    uint32_t image_stride);

/**
 * @brief Set the transform for the test case
 * @param ctx The test context to use
 * @param matrix The transform matrix to set
 */
void vg_lite_test_context_set_transform(struct vg_lite_test_context_s* ctx, const vg_lite_matrix_t* matrix);

/**
 * @brief Get the transform for the test case
 * @param ctx The test context to use
 * @param matrix The transform matrix to get
 */
void vg_lite_test_context_get_transform(struct vg_lite_test_context_s* ctx, vg_lite_matrix_t* matrix);

/**
 * @brief Get the test path for the given format
 * @param format The format of the test path
 * @return The test path for the given format
 */
struct vg_lite_test_path_s* vg_lite_test_context_init_path(struct vg_lite_test_context_s* ctx, vg_lite_format_t format);

/**
 * @brief Get the test path for the given format
 * @param ctx The test context to use
 * @return The test path for the given format
 */
struct vg_lite_test_path_s* vg_lite_test_context_get_path(struct vg_lite_test_context_s* ctx);

/**
 * @brief Set the user data for the test context
 * @param ctx The test context to use
 * @param user_data The user data to set
 */
void vg_lite_test_context_set_user_data(struct vg_lite_test_context_s* ctx, void* user_data);

/**
 * @brief Get the user data for the test context
 * @param ctx The test context to use
 * @return The user data for the test context
 */
void* vg_lite_test_context_get_user_data(struct vg_lite_test_context_s* ctx);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*VG_LITE_TEST_CONTEXT_H*/
