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

#include "gpu_log.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef GPU_LOG_USE_SYSLOG
#include <syslog.h>
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

void gpu_log_printf(enum gpu_log_level_type_e level, const char* func, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    char buf[256];
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

#ifdef GPU_LOG_USE_SYSLOG
    static const int priority[_GPU_LOG_LEVEL_LAST] = {
        LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERR
    };

    syslog(priority[level], "[GPU] %s: %s\n", func, buf);

#else
    static const char* prompt[_GPU_LOG_LEVEL_LAST] = {
        "DEBUG", "INFO", "WARN", "ERROR"
    };

    printf("[GPU][%s] %s: %s\n", prompt[level], func, buf);
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
