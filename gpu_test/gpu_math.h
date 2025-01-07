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

#ifndef GPU_MATH_H
#define GPU_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <float.h>
#include <math.h>
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/

#define MATH_PI 3.14159265358979323846f
#define MATH_HALF_PI 1.57079632679489661923f
#define MATH_TWO_PI 6.28318530717958647692f
#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f

#define MATH_TANF(x) tanf(x)
#define MATH_SINF(x) sinf(x)
#define MATH_COSF(x) cosf(x)
#define MATH_ASINF(x) asinf(x)
#define MATH_ACOSF(x) acosf(x)
#define MATH_FABSF(x) fabsf(x)
#define MATH_SQRTF(x) sqrtf(x)

#define MATH_RADIANS(deg) ((deg)*DEG_TO_RAD)
#define MATH_DEGREES(rad) ((rad)*RAD_TO_DEG)

#define MATH_MIN(a, b) ((a) < (b) ? (a) : (b))
#define MATH_MAX(a, b) ((a) > (b) ? (a) : (b))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Check if a float value is zero.
 * @param a The float value to check.
 * @return True if the value is zero, false otherwise.
 */
static inline bool math_zero(float a)
{
    return (MATH_FABSF(a) < FLT_EPSILON);
}

/**
 * @brief Check if two float values are equal.
 * @param a The first float value to compare.
 * @param b The second float value to compare.
 * @return True if the values are equal, false otherwise.
 */
static inline bool math_equal(float a, float b)
{
    return math_zero(a - b);
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GPU_MATH_H*/
