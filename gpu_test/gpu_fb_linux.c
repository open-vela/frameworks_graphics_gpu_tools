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

#ifndef GPU_TEST_CONTEXT_LINUX_DISABLE

#include "gpu_assert.h"
#include "gpu_buffer.h"
#include "gpu_fb.h"
#include "gpu_log.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_fb_s {
    int fd;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    void* memory;
};

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

struct gpu_fb_s* gpu_fb_create(const char* path)
{
    struct gpu_fb_s* fb = malloc(sizeof(struct gpu_fb_s));
    GPU_ASSERT_NULL(fb);
    memset(fb, 0, sizeof(struct gpu_fb_s));

    fb->fd = open(path, O_RDWR);
    if (fb->fd < 0) {
        GPU_LOG_ERROR("Failed to open framebuffer device: %s, error: %d", path, errno);
        goto failed;
    }

    /* Get fixed screen information*/
    if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->finfo) < 0) {
        GPU_LOG_ERROR("ioctl FBIOGET_FSCREENINFO failed: %d", errno);
        goto failed;
    }

    /* Get variable screen information*/
    if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vinfo) < 0) {
        GPU_LOG_ERROR("ioctl FBIOGET_VSCREENINFO failed: %d", errno);
        goto failed;
    }

    /* Map the device to memory*/
    fb->memory = mmap(0, fb->finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
    if ((intptr_t)fb->memory == -1) {
        GPU_LOG_ERROR("mmap failed: %d", errno);
        goto failed;
    }

    GPU_LOG_INFO("Framebuffer device opened: %s, size: %ux%u, depth: %u, stride: %d",
        path, fb->vinfo.xres, fb->vinfo.yres, fb->vinfo.bits_per_pixel, fb->finfo.line_length);

    return fb;

failed:
    gpu_fb_destroy(fb);
    return NULL;
}

void gpu_fb_destroy(struct gpu_fb_s* fb)
{
    GPU_ASSERT_NULL(fb);
    if (fb->memory) {
        GPU_LOG_INFO("munmap memory: %p, size: %u", fb->memory, fb->finfo.smem_len);
        munmap(fb->memory, fb->finfo.smem_len);
    }

    if (fb->fd >= 0) {
        GPU_LOG_INFO("close fd: %d", fb->fd);
        close(fb->fd);
    }

    GPU_LOG_INFO("free fb: %p", fb);
    memset(fb, 0, sizeof(struct gpu_fb_s));
    free(fb);
}

void gpu_fb_get_buffer(const struct gpu_fb_s* fb, struct gpu_buffer_s* buffer)
{
    GPU_ASSERT_NULL(fb);
    GPU_ASSERT_NULL(buffer);

    buffer->data = fb->memory;
    buffer->width = fb->vinfo.xres;
    buffer->height = fb->vinfo.yres;
    buffer->stride = fb->finfo.line_length;

    switch (fb->vinfo.bits_per_pixel) {
    case 16:
        buffer->format = GPU_COLOR_FORMAT_BGR565;
        break;

    case 24:
        buffer->format = GPU_COLOR_FORMAT_BGR888;
        break;

    case 32:
        buffer->format = GPU_COLOR_FORMAT_BGRA8888;
        break;

    default:
        buffer->format = GPU_COLOR_FORMAT_UNKNOWN;
        GPU_LOG_ERROR("Unsupported color depth: %d", fb->vinfo.bits_per_pixel);
        break;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /* GPU_TEST_CONTEXT_LINUX_DISABLE */
