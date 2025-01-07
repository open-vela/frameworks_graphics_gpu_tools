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

#include "gpu_utils.h"
#include "gpu_log.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

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

const char* gpu_get_localtime_str(void)
{
    static char str_buf[32];
    time_t rawtime;
    time(&rawtime);

    struct tm* timeinfo = localtime(&rawtime);
    if (!timeinfo) {
        GPU_LOG_ERROR("localtime() failed");
        return NULL;
    }

    snprintf(str_buf, sizeof(str_buf), "%04d%02d%02d_%02d%02d%02d",
        1900 + timeinfo->tm_year,
        timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour,
        timeinfo->tm_min, timeinfo->tm_sec);

    return str_buf;
}

int gpu_dir_create(const char* dir_path)
{
    if (access(dir_path, F_OK) == 0) {
        GPU_LOG_INFO("directory: %s already exists", dir_path);
        return 0;
    }

    GPU_LOG_WARN("can't access directory: %s, creating...", dir_path);

    int retval = mkdir(dir_path, 0777);
    if (retval == 0) {
        GPU_LOG_INFO("OK");
    } else {
        GPU_LOG_ERROR("failed: %d", errno);
    }

    return retval;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
