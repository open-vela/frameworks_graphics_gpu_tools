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

#include "vg_lite_test_context.h"
#include "../gpu_assert.h"
#include "../gpu_buffer.h"
#include "../gpu_cache.h"
#include "../gpu_context.h"
#include "../gpu_recorder.h"
#include "../gpu_screenshot.h"
#include "../gpu_tick.h"
#include "../gpu_utils.h"
#include "vg_lite_test_path.h"
#include "vg_lite_test_utils.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/

#define REF_IMAGES_DIR "/ref_images"

/**********************
 *      TYPEDEFS
 **********************/

struct vg_lite_test_context_s {
    struct gpu_test_context_s* gpu_ctx;
    struct gpu_buffer_s* target_gpu_buffer;
    struct gpu_buffer_s* src_gpu_buffer;
    vg_lite_buffer_t target_buffer;
    vg_lite_buffer_t src_buffer;
    struct vg_lite_test_path_s* path;
    vg_lite_matrix_t matrix;
    uint32_t setup_tick;
    uint32_t draw_tick;
    uint32_t finish_tick;
    char vg_error_remark_text[64];
    char screenshot_remark_text[192];
    void* user_data;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void vg_lite_test_context_cleanup(struct vg_lite_test_context_s* ctx);
static void vg_lite_test_context_record(
    struct vg_lite_test_context_s* ctx,
    const struct vg_lite_test_item_s* item,
    vg_lite_error_t error,
    const char* result_str);
static void vg_lite_test_context_error_to_remark(struct vg_lite_test_context_s* ctx, vg_lite_error_t error);
static bool vg_lite_test_context_check_screenshot(struct vg_lite_test_context_s* ctx, const char* name);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

struct vg_lite_test_context_s* vg_lite_test_context_create(struct gpu_test_context_s* gpu_ctx)
{
    GPU_ASSERT_NULL(gpu_ctx);

    struct vg_lite_test_context_s* ctx = malloc(sizeof(struct vg_lite_test_context_s));
    GPU_ASSERT_NULL(ctx);
    memset(ctx, 0, sizeof(struct vg_lite_test_context_s));
    ctx->gpu_ctx = gpu_ctx;

    if (gpu_ctx->target_buffer.data) {
        GPU_LOG_INFO("Using external target buffer");
        vg_lite_test_gpu_buffer_to_vg_buffer(&ctx->target_buffer, &gpu_ctx->target_buffer);
    } else {
        ctx->target_gpu_buffer = vg_lite_test_buffer_alloc(
            &ctx->target_buffer,
            ctx->gpu_ctx->param.target_width,
            ctx->gpu_ctx->param.target_height,
            VG_LITE_BGRA8888,
            VG_LITE_TEST_STRIDE_AUTO);
    }

    /* Scale the output image to the design resolution */
    vg_lite_identity(&ctx->matrix);
    vg_lite_scale(
        ctx->target_buffer.width / (float)GPU_TEST_DESIGN_WIDTH,
        ctx->target_buffer.height / (float)GPU_TEST_DESIGN_HEIGHT,
        &ctx->matrix);

    if (ctx->gpu_ctx->recorder) {
        gpu_recorder_write_string(ctx->gpu_ctx->recorder,
            "Testcase,"
            "Instructions,"
            "Target Format,Source Format,"
            "Target Address,Source Address,"
            "Target Area,Source Area,"
            "Setup Time(ms),Draw Time(ms),Finish Time(ms),"
            "VG-Lite Result,VG-Lite Remark,"
            "Screenshot Result,"
            "Result"
            "\n");
    }

    char path[256];
    snprintf(path, sizeof(path), "%s" REF_IMAGES_DIR, ctx->gpu_ctx->param.output_dir);
    gpu_dir_create(path);

    return ctx;
}

void vg_lite_test_context_destroy(struct vg_lite_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);
    if (ctx->target_gpu_buffer) {
        gpu_buffer_free(ctx->target_gpu_buffer);
        ctx->target_gpu_buffer = NULL;
    }

    if (ctx->src_gpu_buffer) {
        gpu_buffer_free(ctx->src_gpu_buffer);
        ctx->src_gpu_buffer = NULL;
    }

    if (ctx->path) {
        vg_lite_test_path_destroy(ctx->path);
        ctx->path = NULL;
    }

    memset(ctx, 0, sizeof(struct vg_lite_test_context_s));
    free(ctx);
}

bool vg_lite_test_context_run_item(struct vg_lite_test_context_s* ctx, const struct vg_lite_test_item_s* item)
{
    vg_lite_test_context_cleanup(ctx);

    if (item->feature != gcFEATURE_BIT_VG_NONE && !vg_lite_query_feature(item->feature)) {
        snprintf(ctx->vg_error_remark_text, sizeof(ctx->vg_error_remark_text), "Feature '%s' not supported", vg_lite_test_feature_string(item->feature));
        GPU_LOG_WARN("Skipping test case: %s %s", item->name, ctx->vg_error_remark_text);
        if (ctx->gpu_ctx->param.mode == GPU_TEST_MODE_DEFAULT) {
            vg_lite_test_context_record(ctx, item, VG_LITE_NOT_SUPPORT, "SKIP");
        }
        return true;
    }

    GPU_LOG_INFO("Running test case: %s", item->name);

    vg_lite_error_t error = VG_LITE_SUCCESS;
    {
        uint32_t start_tick = gpu_tick_get();
        error = item->on_setup(ctx);
        ctx->setup_tick = gpu_tick_elaps(start_tick);
    }

    if (error == VG_LITE_SUCCESS) {
        uint32_t start_tick = gpu_tick_get();
        error = item->on_draw(ctx);
        ctx->draw_tick = gpu_tick_elaps(start_tick);
    }

    if (error == VG_LITE_SUCCESS) {
        uint32_t start_tick = gpu_tick_get();
        error = vg_lite_finish();
        ctx->finish_tick = gpu_tick_elaps(start_tick);
    }

    if (item->on_teardown) {
        item->on_teardown(ctx);
    }

    if (error == VG_LITE_SUCCESS) {
        GPU_LOG_INFO("Test case '%s' render success", item->name);
    } else {
        GPU_LOG_ERROR("Test case '%s' render failed: %d (%s)", item->name, error, vg_lite_test_error_string(error));
        vg_lite_test_context_error_to_remark(ctx, error);
    }

    bool screenshot_cmp_pass = vg_lite_test_context_check_screenshot(ctx, item->name);

    bool passed = (error == VG_LITE_SUCCESS && screenshot_cmp_pass);

    if (ctx->gpu_ctx->param.mode == GPU_TEST_MODE_DEFAULT || !passed) {
        vg_lite_test_context_record(ctx, item, error, passed ? "PASS" : "FAIL");
    }

    return passed;
}

vg_lite_buffer_t* vg_lite_test_context_get_target_buffer(struct vg_lite_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);
    return &ctx->target_buffer;
}

vg_lite_buffer_t* vg_lite_test_context_get_src_buffer(struct vg_lite_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);
    return &ctx->src_buffer;
}

vg_lite_buffer_t* vg_lite_test_context_alloc_src_buffer(
    struct vg_lite_test_context_s* ctx,
    uint32_t width,
    uint32_t height,
    vg_lite_buffer_format_t format,
    uint32_t stride)
{
    GPU_ASSERT_NULL(ctx);
    GPU_ASSERT(width > 0);
    GPU_ASSERT(height > 0);

    /* Check if the source buffer is already created */
    GPU_ASSERT(ctx->src_gpu_buffer == NULL);
    ctx->src_gpu_buffer = vg_lite_test_buffer_alloc(&ctx->src_buffer, width, height, format, stride);
    return &ctx->src_buffer;
}

void vg_lite_test_context_load_src_image(
    struct vg_lite_test_context_s* ctx,
    const void* image_data,
    uint32_t width,
    uint32_t height,
    vg_lite_buffer_format_t format,
    uint32_t image_stride)
{
    vg_lite_test_context_alloc_src_buffer(ctx, width, height, format, VG_LITE_TEST_STRIDE_AUTO);
    vg_lite_buffer_t* buffer = vg_lite_test_context_get_src_buffer(ctx);

    /* Check if the buffer is large enough to hold the image data. */
    GPU_ASSERT((height * image_stride) <= (buffer->stride * buffer->height));

    const uint8_t* src = image_data;
    uint8_t* dest = buffer->memory;

    for (uint32_t y = 0; y < height; y++) {
        memcpy(dest, src, image_stride);
        dest += buffer->stride;
        src += image_stride;
    }

    /* Make sure the buffer is flushed to memory */
    gpu_cache_flush(buffer->memory, buffer->stride * buffer->height);
}

void vg_lite_test_context_set_transform(struct vg_lite_test_context_s* ctx, const vg_lite_matrix_t* matrix)
{
    GPU_ASSERT_NULL(ctx);
    ctx->matrix = *matrix;
}

void vg_lite_test_context_get_transform(struct vg_lite_test_context_s* ctx, vg_lite_matrix_t* matrix)
{
    GPU_ASSERT_NULL(ctx);
    *matrix = ctx->matrix;
}

struct vg_lite_test_path_s* vg_lite_test_context_init_path(struct vg_lite_test_context_s* ctx, vg_lite_format_t format)
{
    GPU_ASSERT_NULL(ctx);

    /* Check if the path is already created */
    if (ctx->path == NULL) {
        ctx->path = vg_lite_test_path_create(format);
    } else {
        vg_lite_test_path_reset(ctx->path, format);
    }

    return ctx->path;
}

struct vg_lite_test_path_s* vg_lite_test_context_get_path(struct vg_lite_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);
    GPU_ASSERT_NULL(ctx->path);
    return ctx->path;
}

void vg_lite_test_context_set_user_data(struct vg_lite_test_context_s* ctx, void* user_data)
{
    GPU_ASSERT_NULL(ctx);
    ctx->user_data = user_data;
}

void* vg_lite_test_context_get_user_data(struct vg_lite_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);
    return ctx->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void vg_lite_test_context_cleanup(struct vg_lite_test_context_s* ctx)
{
    GPU_ASSERT_NULL(ctx);

    /* Clear the target buffer */
    size_t target_size = ctx->target_buffer.stride * ctx->target_buffer.height;
    memset(ctx->target_buffer.memory, 0, target_size);
    gpu_cache_flush(ctx->target_buffer.memory, target_size);

    /* Clear the source buffer info */
    memset(&ctx->src_buffer, 0, sizeof(vg_lite_buffer_t));

    ctx->vg_error_remark_text[0] = '\0';
    ctx->screenshot_remark_text[0] = '\0';
    ctx->setup_tick = 0;
    ctx->draw_tick = 0;
    ctx->finish_tick = 0;
    ctx->user_data = NULL;

    if (ctx->src_gpu_buffer) {
        gpu_buffer_free(ctx->src_gpu_buffer);
        ctx->src_gpu_buffer = NULL;
    }

    if (ctx->path) {
        vg_lite_test_path_reset(ctx->path, VG_LITE_FP32);
    }

    if (vg_lite_query_feature(gcFEATURE_BIT_VG_SCISSOR)) {
#if VGLITE_RELEASE_VERSION <= VGLITE_MAKE_VERSION(4, 0, 57)
        VG_LITE_TEST_CHECK_ERROR(vg_lite_enable_scissor());
#endif

        /* Reset the scissor to the full screen */
        VG_LITE_TEST_CHECK_ERROR(vg_lite_set_scissor(0, 0, ctx->target_buffer.width, ctx->target_buffer.height));
    }
}

static void vg_lite_test_context_record(
    struct vg_lite_test_context_s* ctx,
    const struct vg_lite_test_item_s* item,
    vg_lite_error_t error,
    const char* result_str)
{
    GPU_ASSERT_NULL(ctx);
    GPU_ASSERT_NULL(item);

    if (!ctx->gpu_ctx->recorder) {
        return;
    }

    char result[512];
    snprintf(result, sizeof(result),
        "%s," /* Testcase */
        "%s," /* Instructions */
        "%s,%s," /* Target Format, Source Format */
        "%p,%p," /* Target Address, Source Address */
        "%dx%d,%dx%d," /* Target Area, Source Area */
        "%0.3f," /* Setup Time(ms) */
        "%0.3f," /* Draw Time(ms) */
        "%0.3f," /* Finish Time(ms) */
        "%s," /* VG-Lite Result */
        "%s," /* VG-Lite Remark */
        "%s," /* Screenshot Result */
        "%s\n", /* Result */
        item->name,
        item->instructions,
        vg_lite_test_buffer_format_string(ctx->target_buffer.format),
        vg_lite_test_buffer_format_string(ctx->src_buffer.format),
        ctx->target_buffer.memory,
        ctx->src_buffer.memory,
        (int)ctx->target_buffer.width,
        (int)ctx->target_buffer.height,
        (int)ctx->src_buffer.width,
        (int)ctx->src_buffer.height,
        ctx->setup_tick / 1000.0f,
        ctx->draw_tick / 1000.0f,
        ctx->finish_tick / 1000.0f,
        vg_lite_test_error_string(error),
        ctx->vg_error_remark_text,
        ctx->screenshot_remark_text,
        result_str);

    gpu_recorder_write_string(ctx->gpu_ctx->recorder, result);
}

static void vg_lite_test_context_error_to_remark(struct vg_lite_test_context_s* ctx, vg_lite_error_t error)
{
    if (error == VG_LITE_SUCCESS) {
        return;
    }

    if (error == VG_LITE_TIMEOUT) {
        VG_LITE_TEST_CHECK_ERROR(vg_lite_dump_command_buffer());
        snprintf(ctx->vg_error_remark_text, sizeof(ctx->vg_error_remark_text), "See log for command buffer dump");
        return;
    }

    if (error == VG_LITE_OUT_OF_MEMORY || error == VG_LITE_OUT_OF_RESOURCES) {
        vg_lite_uint32_t mem_size = 0;
        VG_LITE_TEST_CHECK_ERROR(vg_lite_get_mem_size(&mem_size));
        GPU_LOG_WARN("Memory available: %" PRIu32 " bytes", (uint32_t)mem_size);
        snprintf(ctx->vg_error_remark_text, sizeof(ctx->vg_error_remark_text),
            "Memory not enough: %" PRIu32 " bytes", (uint32_t)mem_size);
        return;
    }
}

static bool vg_lite_test_context_check_screenshot(struct vg_lite_test_context_s* ctx, const char* name)
{
    if (!ctx->gpu_ctx->param.screenshot_en) {
        return true;
    }

    bool retval = false;
    char path[128];
    snprintf(path, sizeof(path), "%s" REF_IMAGES_DIR "/%s.png", ctx->gpu_ctx->param.output_dir, name);

    struct gpu_buffer_s target_buffer;
    vg_lite_test_vg_buffer_to_gpu_buffer(&target_buffer, &ctx->target_buffer);

    struct gpu_buffer_s* loaded_buffer = gpu_screenshot_load(path);
    if (!loaded_buffer) {
        int ret = gpu_screenshot_save(path, &target_buffer);
        snprintf(ctx->screenshot_remark_text, sizeof(ctx->screenshot_remark_text),
            "Create: %s - %s", path, ret == 0 ? "SUCCESS" : "FAILED");
        return true;
    }

    if (target_buffer.width != loaded_buffer->width || target_buffer.height != loaded_buffer->height) {
        snprintf(ctx->screenshot_remark_text, sizeof(ctx->screenshot_remark_text),
            "Size not matched: %s target: W%dxH%d vs loaded: W%dxH%d",
            path,
            (int)target_buffer.width, (int)target_buffer.height,
            (int)loaded_buffer->width, (int)loaded_buffer->height);

        GPU_LOG_ERROR("%s", ctx->screenshot_remark_text);
        goto failed;
    }

    /* Make sure the buffer fully loaded to memory */
    gpu_cache_invalidate(target_buffer.data, target_buffer.stride * target_buffer.height);

    for (int y = 0; y < target_buffer.height; y++) {
        for (int x = 0; x < target_buffer.width; x++) {
            gpu_color_bgra8888_t target_pixel;
            target_pixel.full = gpu_buffer_get_pixel(&target_buffer, x, y);

            gpu_color_bgra8888_t loaded_pixel;
            loaded_pixel.full = gpu_buffer_get_pixel(loaded_buffer, x, y);

            if (!gpu_color_bgra8888_compare(target_pixel, loaded_pixel, ctx->gpu_ctx->param.color_tolerance)) {
                snprintf(ctx->screenshot_remark_text, sizeof(ctx->screenshot_remark_text),
                    "Pixel not match in (X%d Y%d) "
                    "target: 0x%08" PRIX32 "(A%d R%d G%d B%d) vs "
                    "loaded: 0x%08" PRIX32 "(A%d R%d G%d B%d)",
                    x, y,
                    target_pixel.full, target_pixel.ch.alpha, target_pixel.ch.red, target_pixel.ch.green, target_pixel.ch.blue,
                    loaded_pixel.full, loaded_pixel.ch.alpha, loaded_pixel.ch.red, loaded_pixel.ch.green, loaded_pixel.ch.blue);
                GPU_LOG_ERROR("%s", ctx->screenshot_remark_text);

                snprintf(path, sizeof(path), "%s" REF_IMAGES_DIR "/%s_err.png", ctx->gpu_ctx->param.output_dir, name);
                gpu_screenshot_save(path, &target_buffer);
                goto failed;
            }
        }
    }

    retval = true;
    GPU_LOG_INFO("Screenshot check PASS: %s", path);
    snprintf(ctx->screenshot_remark_text, sizeof(ctx->screenshot_remark_text), "SUCCESS");

failed:
    gpu_buffer_free(loaded_buffer);
    return retval;
}
