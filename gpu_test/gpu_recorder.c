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

#include "gpu_recorder.h"
#include "gpu_assert.h"
#include "gpu_log.h"
#include "gpu_utils.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct gpu_recorder_s {
    int fd;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

struct gpu_recorder_s* gpu_recorder_create(const char* dir_path, const char* name)
{
    struct gpu_recorder_s* recorder;
    char path[256];
    snprintf(path, sizeof(path), "%s/report_%s.csv",
        dir_path, name);

    int fd = open(path, O_CREAT | O_WRONLY | O_CLOEXEC, 0666);
    if (fd < 0) {
        GPU_LOG_ERROR("open %s failed: %d", path, errno);
        return NULL;
    }

    recorder = calloc(1, sizeof(struct gpu_recorder_s));
    GPU_ASSERT_NULL(recorder);
    recorder->fd = fd;
    GPU_LOG_INFO("recorder file: %s created, fd = %d", path, fd);
    return recorder;
}

void gpu_recorder_delete(struct gpu_recorder_s* recorder)
{
    GPU_ASSERT_NULL(recorder);

    /* Get current position of file */
    off_t current_position = lseek(recorder->fd, 0, SEEK_CUR);
    if (current_position >= 0) {
        GPU_LOG_INFO("current position of file: %d", (int)current_position);

        /* Truncate file to current position */
        if (ftruncate(recorder->fd, current_position) < 0) {
            GPU_LOG_ERROR("ftruncate failed: %d", errno);
        }
    } else {
        GPU_LOG_ERROR("lseek failed: %d", errno);
    }

    close(recorder->fd);
    GPU_LOG_INFO("recorder file closed, fd = %d", recorder->fd);

    memset(recorder, 0, sizeof(struct gpu_recorder_s));
    free(recorder);
    GPU_LOG_INFO("recorder deleted");
}

int gpu_recorder_write_string(struct gpu_recorder_s* recorder, const char* str)
{
    GPU_ASSERT_NULL(recorder);
    GPU_ASSERT_NULL(str);
    size_t len = strlen(str);
    size_t written = 0;

    while (written < len) {
        ssize_t ret = write(recorder->fd, str + written, len - written);

        if (ret < 0) {
            GPU_LOG_ERROR("write failed: %d", errno);
            return -1;
        }

        written += ret;
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
