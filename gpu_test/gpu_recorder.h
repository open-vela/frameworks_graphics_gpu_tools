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

#ifndef GPU_RECORDER_H
#define GPU_RECORDER_H

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

struct gpu_recorder_s;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Create a new gpu recorder
 * @param dir_path The directory path to save the record file
 * @param name The name of the record file
 * @return A pointer to the created recorder object on success, NULL on failure
 */
struct gpu_recorder_s* gpu_recorder_create(const char* dir_path, const char* name);

/**
 * @brief Delete a gpu recorder
 * @param recorder The recorder object to delete
 */
void gpu_recorder_delete(struct gpu_recorder_s* recorder);

/**
 * @brief Start recording
 * @param recorder The recorder object to start recording
 * @return 0 on success, -1 on failure
 */
int gpu_recorder_write_string(struct gpu_recorder_s* recorder, const char* str);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_RECORDER_H*/
