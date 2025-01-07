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

#ifndef VG_LITE_TEST_PATH_H
#define VG_LITE_TEST_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "vg_lite_test_utils.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/

typedef struct vg_lite_test_path_s vg_lite_test_path_t;

typedef void (*vg_lite_test_path_iter_cb_t)(void* user_data, uint8_t op_code, const float* data, uint32_t len);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Create a new path object.
 * @param data_format The data format of the path.
 * @return The new path object.
 */
vg_lite_test_path_t* vg_lite_test_path_create(vg_lite_format_t data_format);

/**
 * @brief Destroy a path object.
 * @param path The path object to destroy.
 */
void vg_lite_test_path_destroy(vg_lite_test_path_t* path);

/**
 * @brief Reset a path object.
 * @param path The path object to reset.
 * @param data_format The data format of the path.
 */
void vg_lite_test_path_reset(vg_lite_test_path_t* path, vg_lite_format_t data_format);

/**
 * @brief Set the bounding box of a path object.
 * @param path The path object to set the bounding box.
 * @param min_x The minimum x value of the bounding box.
 * @param min_y The minimum y value of the bounding box.
 * @param max_x The maximum x value of the bounding box.
 * @param max_y The maximum y value of the bounding box.
 */
void vg_lite_test_path_set_bounding_box(vg_lite_test_path_t* path,
    float min_x, float min_y,
    float max_x, float max_y);

/**
 * @brief Get the bounding box of a path object.
 * @param path The path object to get the bounding box.
 * @param min_x The minimum x value of the bounding box.
 * @param min_y The minimum y value of the bounding box.
 * @param max_x The maximum x value of the bounding box.
 * @param max_y The maximum y value of the bounding box.
 */
void vg_lite_test_path_get_bounding_box(vg_lite_test_path_t* path,
    float* min_x, float* min_y,
    float* max_x, float* max_y);

/**
 * @brief Update the bounding box of a path object.
 * @param path The path object to update the bounding box.
 * @return True if the bounding box is updated, false otherwise.
 */
bool vg_lite_test_path_update_bounding_box(vg_lite_test_path_t* path);

/**
 * @brief Set the transform of a path object.
 * @param path The path object to set the transform.
 * @param matrix The transform matrix.
 */
void vg_lite_test_path_set_transform(vg_lite_test_path_t* path, const vg_lite_matrix_t* matrix);

/**
 * @brief Set the quality of a path object.
 * @param path The path object to set the quality.
 * @param quality The quality of the path.
 */
void vg_lite_test_path_set_quality(vg_lite_test_path_t* path, vg_lite_quality_t quality);

/**
 * @brief Get vg-lite path object.
 * @param path The path object to get the vg-lite path object.
 * @return The vg-lite path object.
 */
vg_lite_path_t* vg_lite_test_path_get_path(vg_lite_test_path_t* path);

/**
 * @brief Move to a point.
 * @param path The path object to move to a point.
 * @param x The x value of the point.
 * @param y The y value of the point.
 */
void vg_lite_test_path_move_to(vg_lite_test_path_t* path,
    float x, float y);

/**
 * @brief Line to a point.
 * @param path The path object to line to a point.
 * @param x The x value of the point.
 * @param y The y value of the point.
 */
void vg_lite_test_path_line_to(vg_lite_test_path_t* path,
    float x, float y);

/**
 * @brief Quad to a point.
 * @param path The path object to quad to a point.
 * @param cx The x value of the control point.
 * @param cy The y value of the control point.
 * @param x The x value of the point.
 * @param y The y value of the point.
 */
void vg_lite_test_path_quad_to(vg_lite_test_path_t* path,
    float cx, float cy,
    float x, float y);

/**
 * @brief Cubic to a point.
 * @param path The path object to cubic to a point.
 * @param cx1 The x value of the first control point.
 * @param cy1 The y value of the first control point.
 * @param cx2 The x value of the second control point.
 * @param cy2 The y value of the second control point.
 * @param x The x value of the point.
 * @param y The y value of the point.
 */
void vg_lite_test_path_cubic_to(vg_lite_test_path_t* path,
    float cx1, float cy1,
    float cx2, float cy2,
    float x, float y);

/**
 * @brief Close the path.
 * @param path The path object to close.
 */
void vg_lite_test_path_close(vg_lite_test_path_t* path);

/**
 * @brief End the path.
 * @param path The path object to end.
 */
void vg_lite_test_path_end(vg_lite_test_path_t* path);

/**
 * @brief Append a rectangle to the path.
 * @param path The path object to append a rectangle.
 * @param x The x value of the rectangle.
 * @param y The y value of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param r The radius of the corners of the rectangle.
 */
void vg_lite_test_path_append_rect(vg_lite_test_path_t* path,
    float x, float y,
    float w, float h,
    float r);

/**
 * @brief Append a circle to the path.
 * @param path The path object to append a circle.
 * @param cx The x value of the center of the circle.
 * @param cy The y value of the center of the circle.
 * @param rx The radius of the x axis of the circle.
 * @param ry The radius of the y axis of the circle.
 */
void vg_lite_test_path_append_circle(vg_lite_test_path_t* path,
    float cx, float cy,
    float rx, float ry);

/**
 * @brief Append an ellipse to the path.
 * @param path The path object to append an ellipse.
 * @param cx The x value of the center of the ellipse.
 * @param cy The y value of the center of the ellipse.
 * @param rx The radius of the x axis of the ellipse.
 * @param ry The radius of the y axis of the ellipse.
 */
void vg_lite_test_path_append_arc_right_angle(vg_lite_test_path_t* path,
    float start_x, float start_y,
    float center_x, float center_y,
    float end_x, float end_y);

/**
 * @brief Append an arc to the path.
 * @param path The path object to append an arc.
 * @param cx The x value of the center of the arc.
 * @param cy The y value of the center of the arc.
 * @param radius The radius of the arc.
 * @param start_angle The start angle of the arc.
 * @param sweep The sweep angle of the arc.
 * @param pie True if the arc is a pie, false normal arc.
 */
void vg_lite_test_path_append_arc(vg_lite_test_path_t* path,
    float cx, float cy,
    float radius,
    float start_angle,
    float sweep,
    bool pie);

/**
 * @brief Append a path to the path.
 * @param dest The destination path object to append a path.
 * @param src The source path object to append.
 */
void vg_lite_test_path_append_path(vg_lite_test_path_t* dest, const vg_lite_test_path_t* src);

/**
 * @brief Get operation code and argument length of a vg-lite path object.
 * @param vlc_op The operation code
 * @return The length of the argument of the operation code.
 */
uint8_t vg_lite_test_vlc_op_arg_len(uint8_t vlc_op);

/**
 * @brief Get the format length of a vg-lite path object.
 * @param format The data format of the path.
 * @return The length of the format.
 */
uint8_t vg_lite_test_path_format_len(vg_lite_format_t format);

/**
 * @brief Iterate over the data of a vg-lite path object.
 * @param path The path object to iterate over.
 * @param cb The callback function to call for each data.
 * @param user_data The user data to pass to the callback function.
 */
void vg_lite_test_path_for_each_data(const vg_lite_path_t* path, vg_lite_test_path_iter_cb_t cb, void* user_data);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*VG_LITE_TEST_PATH_H*/
