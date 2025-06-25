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

#include "gpu_buf_blur.h"
#include "gpu_assert.h"
#include "gpu_buffer.h"
#include "gpu_cache.h"
#include "gpu_log.h"
#include <math.h>
#include <string.h>

#ifdef CONFIG_ARM_HAVE_MVE
#include <arm_mve.h>
#define GPU_BUF_BLUR_MVE_OPT 1
#else
#define GPU_BUF_BLUR_MVE_OPT 0
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

static void exp_blur(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t radius,
    int32_t aprec,
    int32_t zprec);

#if GPU_BUF_BLUR_MVE_OPT
static void exp_blur_q8_mve(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t radius);

static void exp_blur_q16_mve(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t radius);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void gpu_buf_blur_args_init(gpu_buf_blur_args_t* args)
{
    GPU_ASSERT_NULL(args);
    memset(args, 0, sizeof(gpu_buf_blur_args_t));
    args->type = GPU_BUF_BLUR_TYPE_EXP;
    args->radius = 10;
    args->aprec = 16;
    args->zprec = 7;
}

bool gpu_buf_blur(
    struct gpu_buffer_s* dst_buf,
    const struct gpu_buffer_s* src_buf,
    const gpu_buf_blur_args_t* args)
{
    GPU_ASSERT_NULL(dst_buf);
    GPU_ASSERT_NULL(src_buf);
    GPU_ASSERT_NULL(args);

    if (src_buf->format != GPU_COLOR_FORMAT_BGRA8888 && src_buf->format != GPU_COLOR_FORMAT_BGRX8888) {
        GPU_LOG_WARN("Unsupported color format: %d", src_buf->format);
        return false;
    }

    if (src_buf->width != dst_buf->width || src_buf->height != dst_buf->height
        || src_buf->stride != dst_buf->stride || src_buf->format != dst_buf->format) {
        GPU_LOG_WARN("The header info is different of src and dst");
        return false;
    }

    if (args->radius < 1) {
        return true;
    }

    switch (args->type) {
    case GPU_BUF_BLUR_TYPE_EXP:
#if GPU_BUF_BLUR_MVE_OPT
        if (args->aprec == 8 && args->zprec == 0) {
            exp_blur_q8_mve(
                dst_buf->data,
                src_buf->data,
                src_buf->width,
                src_buf->height,
                src_buf->stride,
                args->radius);
        } else if (args->aprec == 16 && args->zprec == 7) {
            exp_blur_q16_mve(
                dst_buf->data,
                src_buf->data,
                src_buf->width,
                src_buf->height,
                src_buf->stride,
                args->radius);
        } else
#endif
        {
            exp_blur(
                dst_buf->data,
                src_buf->data,
                src_buf->width,
                src_buf->height,
                src_buf->stride,
                args->radius,
                args->aprec,
                args->zprec);
        }
        break;

    default:
        GPU_LOG_WARN("Unsupported blur type: %d", args->type);
        return false;
    }

    gpu_cache_flush(dst_buf->data, dst_buf->stride * dst_buf->height);
    return true;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static inline void exp_blur_inner(
    uint8_t* dst,
    const uint8_t* src,
    int32_t* zB,
    int32_t* zG,
    int32_t* zR,
    int32_t* zA,
    int32_t alpha,
    int32_t aprec,
    int32_t zprec)
{
    int32_t B = *src;
    int32_t G = *(src + 1);
    int32_t R = *(src + 2);
    int32_t A = *(src + 3);

    *zB += (alpha * ((B << zprec) - *zB)) >> aprec;
    *zG += (alpha * ((G << zprec) - *zG)) >> aprec;
    *zR += (alpha * ((R << zprec) - *zR)) >> aprec;
    *zA += (alpha * ((A << zprec) - *zA)) >> aprec;

    *dst = *zB >> zprec;
    *(dst + 1) = *zG >> zprec;
    *(dst + 2) = *zR >> zprec;
    *(dst + 3) = *zA >> zprec;
}

static inline void exp_blur_row(
    uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t /* height */, /* TODO: This seems very strange. Why is height not used as it is in _blurcol() ? */
    int32_t stride,
    int32_t line,
    int32_t alpha,
    int32_t aprec,
    int32_t zprec)
{
    const uint8_t* input = &(src[line * stride]);
    uint8_t* output = &(dst[line * stride]);
    int32_t zB = *input << zprec;
    int32_t zG = *(input + 1) << zprec;
    int32_t zR = *(input + 2) << zprec;
    int32_t zA = *(input + 3) << zprec;

    for (int32_t index = 0; index < width; index++) {
        exp_blur_inner(
            &output[index * sizeof(uint32_t)],
            &input[index * sizeof(uint32_t)],
            &zB, &zG, &zR, &zA,
            alpha, aprec, zprec);
    }

    for (int32_t index = width - 2; index >= 0; index--) {
        exp_blur_inner(
            &output[index * sizeof(uint32_t)],
            &output[index * sizeof(uint32_t)],
            &zB, &zG, &zR, &zA,
            alpha, aprec, zprec);
    }
}

static inline void exp_blur_col(
    uint8_t* dst,
    int32_t /*width*/,
    int32_t height,
    int32_t stride,
    int32_t x,
    int32_t alpha,
    int32_t aprec,
    int32_t zprec)
{
    uint8_t* ptr = dst + x * sizeof(uint32_t);
    int32_t zB = *((uint8_t*)ptr) << zprec;
    int32_t zG = *((uint8_t*)ptr + 1) << zprec;
    int32_t zR = *((uint8_t*)ptr + 2) << zprec;
    int32_t zA = *((uint8_t*)ptr + 3) << zprec;

    for (int32_t index = 1; index < height; index++) {
        exp_blur_inner(
            &ptr[index * stride],
            &ptr[index * stride],
            &zB, &zG, &zR, &zA,
            alpha, aprec, zprec);
    }

    for (int32_t index = height - 2; index >= 0; index--) {
        exp_blur_inner(
            &ptr[index * stride],
            &ptr[index * stride],
            &zB, &zG, &zR, &zA,
            alpha, aprec, zprec);
    }
}

static void exp_blur(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t radius,
    int32_t aprec,
    int32_t zprec)
{
    if (radius < 1)
        return;

    /**
     * calculate the alpha such that 90% of
     * the kernel is within the radius.
     * (Kernel extends to infinity)
     */
    const int32_t alpha = (int32_t)((1 << aprec) * (1.0f - expf(-2.3f / (radius + 1.f))));

    for (int32_t row = 0; row < height; row++) {
        exp_blur_row(dst, src, width, height, stride, row, alpha, aprec, zprec);
    }

    for (int32_t col = 0; col < width; col++) {
        exp_blur_col(dst, width, height, stride, col, alpha, aprec, zprec);
    }
}

#if GPU_BUF_BLUR_MVE_OPT

static inline void exp_blur_row_q8_mve(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t /* height */,
    int32_t stride,
    int32_t line,
    int32_t alpha,
    int32_t inv_alpha)
{
    const uint8_t* input0 = &(src[line * stride]);
    const uint8_t* input1 = &(src[(line + 1) * stride]);
    uint8_t* output0 = &(dst[line * stride]);
    uint8_t* output1 = &(dst[(line + 1) * stride]);

    uint32x4_t sum0 = vldrbq_u32(input0);
    uint32x4_t sum1 = vldrbq_u32(input1);

    vstrbq_u32(output0, sum0);
    vstrbq_u32(output1, sum1);

    sum1 = vshlq_n_u32(sum1, 16);
    uint16x8_t sum = vorrq_u16(vreinterpretq_u16_u32(sum0), vreinterpretq_u16_u32(sum1));
    uint32x4_t zRGBA0, zRGBA1;
    uint16x8_t zRGBA;

    for (int32_t index = 1; index < width; index++) {
        zRGBA0 = vldrbq_u32(&(input0[index * 4]));
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&(input1[index * 4])), 16);
        zRGBA = vorrq_u16(vreinterpretq_u16_u32(zRGBA0), vreinterpretq_u16_u32(zRGBA1));
        sum = vmulq_n_u16(sum, inv_alpha);
        sum = vmlaq_n_u16(sum, zRGBA, alpha);
        sum = vshrq_n_u16(sum, 8);
        vstrbq_u32(&(output0[index * 4]), vreinterpretq_u32_u16(sum));
        vstrbq_u32(&(output1[index * 4]), vshrq_n_u32(vreinterpretq_u32_u16(sum), 16));
    }

    for (int32_t index = width - 2; index >= 0; index--) {
        zRGBA0 = vldrbq_u32(&(output0[index * 4]));
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&(output1[index * 4])), 16);
        zRGBA = vorrq_u16(vreinterpretq_u16_u32(zRGBA0), vreinterpretq_u16_u32(zRGBA1));
        sum = vmulq_n_u16(sum, inv_alpha);
        sum = vmlaq_n_u16(sum, zRGBA, alpha);
        sum = vshrq_n_u16(sum, 8);
        vstrbq_u32(&(output0[index * 4]), vreinterpretq_u32_u16(sum));
        vstrbq_u32(&(output1[index * 4]), vshrq_n_u32(vreinterpretq_u32_u16(sum), 16));
    }
}

static inline void exp_blur_col_q8_mve(uint8_t* dst,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t x,
    int32_t alpha,
    int32_t inv_alpha)
{
    uint8_t* scancol = dst + x * 4;
    uint16x8_t sum = vldrbq_u16(scancol);
    uint16x8_t zRGBA;

    for (int32_t index = stride; index < (height - 1) * stride; index += stride) {
        zRGBA = vldrbq_u16(&scancol[index]);
        sum = vmulq_n_u16(sum, inv_alpha);
        sum = vmlaq_n_u16(sum, zRGBA, alpha);
        sum = vshrq_n_u16(sum, 8);
        vstrbq_u16(&(scancol[index]), sum);
    }

    for (int32_t index = (height - 2) * stride; index >= 0; index -= stride) {
        zRGBA = vldrbq_u16(&scancol[index]);
        sum = vmulq_n_u16(sum, inv_alpha);
        sum = vmlaq_n_u16(sum, zRGBA, alpha);
        sum = vshrq_n_u16(sum, 8);
        vstrbq_u16(&(scancol[index]), sum);
    }
}

static inline void exp_blur_8col_q8_mve(uint8_t* dst,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t x,
    int32_t alpha,
    int32_t inv_alpha)
{
    uint8_t* scancol = dst + x * 4;
    uint16x8_t sum0 = vldrbq_u16(scancol); /* load 2 pixels of rgba8888*/
    uint16x8_t sum1 = vldrbq_u16(scancol + 8); /* load 2 pixels of rgba8888*/
    uint16x8_t sum2 = vldrbq_u16(scancol + 16); /* load 2 pixels of rgba8888*/
    uint16x8_t sum3 = vldrbq_u16(scancol + 24); /* load 2 pixels of rgba8888*/
    uint16x8_t zRGBA;

    for (int32_t index = stride; index < (height - 1) * stride; index += stride) {
        __builtin_prefetch(&scancol[index]);

        zRGBA = vldrbq_u16(&scancol[index]);
        sum0 = vmulq_n_u16(sum0, inv_alpha);
        sum0 = vmlaq_n_u16(sum0, zRGBA, alpha);
        sum0 = vshrq_n_u16(sum0, 8);
        vstrbq_u16(&(scancol[index]), sum0);

        zRGBA = vldrbq_u16(&scancol[index + 8]);
        sum1 = vmulq_n_u16(sum1, inv_alpha);
        sum1 = vmlaq_n_u16(sum1, zRGBA, alpha);
        sum1 = vshrq_n_u16(sum1, 8);
        vstrbq_u16(&(scancol[index + 8]), sum1);

        zRGBA = vldrbq_u16(&scancol[index + 16]);
        sum2 = vmulq_n_u16(sum2, inv_alpha);
        sum2 = vmlaq_n_u16(sum2, zRGBA, alpha);
        sum2 = vshrq_n_u16(sum2, 8);
        vstrbq_u16(&(scancol[index + 16]), sum2);

        zRGBA = vldrbq_u16(&scancol[index + 24]);
        sum3 = vmulq_n_u16(sum3, inv_alpha);
        sum3 = vmlaq_n_u16(sum3, zRGBA, alpha);
        sum3 = vshrq_n_u16(sum3, 8);
        vstrbq_u16(&(scancol[index + 24]), sum3);
    }

    for (int32_t index = (height - 2) * stride; index >= 0; index -= stride) {
        __builtin_prefetch(&scancol[index]);

        zRGBA = vldrbq_u16(&scancol[index]);
        sum0 = vmulq_n_u16(sum0, inv_alpha);
        sum0 = vmlaq_n_u16(sum0, zRGBA, alpha);
        sum0 = vshrq_n_u16(sum0, 8);
        vstrbq_u16(&(scancol[index]), sum0);

        zRGBA = vldrbq_u16(&scancol[index + 8]);
        sum1 = vmulq_n_u16(sum1, inv_alpha);
        sum1 = vmlaq_n_u16(sum1, zRGBA, alpha);
        sum1 = vshrq_n_u16(sum1, 8);
        vstrbq_u16(&(scancol[index + 8]), sum1);

        zRGBA = vldrbq_u16(&scancol[index + 16]);
        sum2 = vmulq_n_u16(sum2, inv_alpha);
        sum2 = vmlaq_n_u16(sum2, zRGBA, alpha);
        sum2 = vshrq_n_u16(sum2, 8);
        vstrbq_u16(&(scancol[index + 16]), sum2);

        zRGBA = vldrbq_u16(&scancol[index + 24]);
        sum3 = vmulq_n_u16(sum3, inv_alpha);
        sum3 = vmlaq_n_u16(sum3, zRGBA, alpha);
        sum3 = vshrq_n_u16(sum3, 8);
        vstrbq_u16(&(scancol[index + 24]), sum3);
    }
}

static inline void _blurrow_mve(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t /* height */, // TODO: This seems very strange. Why is height not used as it is in _blurcol() ?
    int32_t stride,
    int32_t line,
    int32_t alpha,
    int32_t inv_alpha,
    int32_t aprec,
    int32_t zprec)
{
    int32_t index;

    const uint8_t* input = &(src[line * stride]);
    uint8_t* output = &(dst[line * stride]);

    uint32x4_t sum = vldrbq_u32(input);
    uint32x4_t zRGBA, zRGBA1;

    vstrbq_u32(output, sum);
    sum = vshlq_n_u32(sum, zprec);

    for (index = 1; (index + 2) < width; index += 2) {
        zRGBA = vshlq_n_u32(vldrbq_u32(&(input[index * 4])), zprec);
        sum = vmulq_n_u32(sum, inv_alpha);
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&(input[(index + 1) * 4])), zprec);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA, alpha), aprec);
        vstrbq_u32(&(output[index * 4]), vshrq_n_u32(sum, zprec));

        sum = vmulq_n_u32(sum, inv_alpha);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA1, alpha), aprec);
        vstrbq_u32(&(output[(index + 1) * 4]), vshrq_n_u32(sum, zprec));
    }

    for (; index < width; index++) {
        zRGBA = vshlq_n_u32(vldrbq_u32(&(input[index * 4])), zprec);
        sum = vmulq_n_u32(sum, inv_alpha);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA, alpha), aprec);
        vstrbq_u32(&(output[index * 4]), vshrq_n_u32(sum, zprec));
    }

    for (index = width - 2; (index - 2) >= 0; index -= 2) {
        zRGBA = vshlq_n_u32(vldrbq_u32(&(output[index * 4])), zprec);
        sum = vmulq_n_u32(sum, inv_alpha);
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&(output[(index - 1) * 4])), zprec);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA, alpha), aprec);
        vstrbq_u32(&(output[index * 4]), vshrq_n_u32(sum, zprec));

        sum = vmulq_n_u32(sum, inv_alpha);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA1, alpha), aprec);
        vstrbq_u32(&(output[(index - 1) * 4]), vshrq_n_u32(sum, zprec));
    }

    for (; index >= 0; index--) {
        zRGBA = vshlq_n_u32(vldrbq_u32(&(output[index * 4])), zprec);
        sum = vmulq_n_u32(sum, inv_alpha);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA, alpha), aprec);
        vstrbq_u32(&(output[index * 4]), vshrq_n_u32(sum, zprec));
    }
}

static inline void _blurcol_mve(uint8_t* dst,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t x,
    int32_t alpha,
    int32_t inv_alpha,
    int32_t aprec,
    int32_t zprec)
{
    int32_t index;
    uint8_t *ptr, *ptr1, *ptr2, *ptr3;

    ptr = dst + x * 4;
    ptr1 = ptr + 1 * 4;
    ptr2 = ptr + 2 * 4;
    ptr3 = ptr + 3 * 4;

    uint32x4_t sum = vshlq_n_u32(vldrbq_u32(ptr), zprec);
    uint32x4_t sum1 = vshlq_n_u32(vldrbq_u32(ptr1), zprec);
    uint32x4_t sum2 = vshlq_n_u32(vldrbq_u32(ptr2), zprec);
    uint32x4_t sum3 = vshlq_n_u32(vldrbq_u32(ptr3), zprec);
    uint32x4_t zRGBA, zRGBA1;

    for (index = stride; index < (height - 1) * stride; index += stride) {

        zRGBA = vshlq_n_u32(vldrbq_u32(&ptr[index]), zprec);
        sum = vmulq_n_u32(sum, inv_alpha);
        // load next ptr, insert between mla instruction to improve execution speed
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&ptr1[index]), zprec);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA, alpha), aprec);
        vstrbq_u32(&ptr[index], vshrq_n_u32(sum, zprec));

        /* handle the next pixel */
        sum1 = vmulq_n_u32(sum1, inv_alpha);
        // load next ptr, insert between mla instruction to improve execution speed
        zRGBA = vshlq_n_u32(vldrbq_u32(&ptr2[index]), zprec);
        sum1 = vshrq_n_u32(vmlaq_n_u32(sum1, zRGBA1, alpha), aprec);
        vstrbq_u32(&ptr1[index], vshrq_n_u32(sum1, zprec));

        /* handle the next pixel */
        sum2 = vmulq_n_u32(sum2, inv_alpha);
        // load next ptr, insert between mla instruction to improve execution speed
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&ptr3[index]), zprec);
        sum2 = vshrq_n_u32(vmlaq_n_u32(sum2, zRGBA, alpha), aprec);
        vstrbq_u32(&ptr2[index], vshrq_n_u32(sum2, zprec));

        /* handle the next pixel */
        sum3 = vmulq_n_u32(sum3, inv_alpha);
        sum3 = vshrq_n_u32(vmlaq_n_u32(sum3, zRGBA1, alpha), aprec);
        vstrbq_u32(&ptr3[index], vshrq_n_u32(sum3, zprec));
    }

    for (index = (height - 2) * stride; index >= 0; index -= stride) {
        zRGBA = vshlq_n_u32(vldrbq_u32(&ptr[index]), zprec);
        sum = vmulq_n_u32(sum, inv_alpha);
        // load next ptr, insert between mla instruction to improve execution speed
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&ptr1[index]), zprec);
        sum = vshrq_n_u32(vmlaq_n_u32(sum, zRGBA, alpha), aprec);
        vstrbq_u32(&ptr[index], vshrq_n_u32(sum, zprec));

        /* handle the next pixel */
        sum1 = vmulq_n_u32(sum1, inv_alpha);
        // load next ptr, insert between mla instruction to improve execution speed
        zRGBA = vshlq_n_u32(vldrbq_u32(&ptr2[index]), zprec);
        sum1 = vshrq_n_u32(vmlaq_n_u32(sum1, zRGBA1, alpha), aprec);
        vstrbq_u32(&ptr1[index], vshrq_n_u32(sum1, zprec));

        /* handle the next pixel */
        sum2 = vmulq_n_u32(sum2, inv_alpha);
        // load next ptr, insert between mla instruction to improve execution speed
        zRGBA1 = vshlq_n_u32(vldrbq_u32(&ptr3[index]), zprec);
        sum2 = vshrq_n_u32(vmlaq_n_u32(sum2, zRGBA, alpha), aprec);
        vstrbq_u32(&ptr2[index], vshrq_n_u32(sum2, zprec));

        /* handle the next pixel */
        sum3 = vmulq_n_u32(sum3, inv_alpha);
        sum3 = vshrq_n_u32(vmlaq_n_u32(sum3, zRGBA1, alpha), aprec);
        vstrbq_u32(&ptr3[index], vshrq_n_u32(sum3, zprec));
    }
}

static void exp_blur_q8_mve(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t radius)
{
    const uint8_t alpha = (uint8_t)((1 << 8) * (1.0f - expf(-2.3f / (radius + 1.f))));
    const uint8_t inv_alpha = (uint8_t)((1 << 8) - alpha);
    int32_t row;
    int32_t col;

    for (row = 0; row < height; row += 2) {
        exp_blur_row_q8_mve(dst, src, width, height, stride, row, alpha, inv_alpha);
    }

    for (col = 0; (col + 8) < width; col += 8) {
        exp_blur_8col_q8_mve(dst, width, height, stride, col, alpha, inv_alpha);
    }

    for (; col < width; col += 2) {
        exp_blur_col_q8_mve(dst, width, height, stride, col, alpha, inv_alpha);
    }
}

static void exp_blur_q16_mve(uint8_t* dst,
    const uint8_t* src,
    int32_t width,
    int32_t height,
    int32_t stride,
    int32_t radius)
{
    const uint32_t alpha = (uint32_t)((1 << 16) * (1.0f - expf(-2.3f / (radius + 1.f))));
    const uint32_t inv_alpha = (uint32_t)((1 << 16) - alpha);
    int32_t row;
    int32_t col;

    for (row = 0; row < height; row++)
        _blurrow_mve(dst, src, width, height, stride, row, alpha, inv_alpha, 16, 7);

    for (col = 0; col < width; col += 4)
        _blurcol_mve(dst, width, height, stride, col, alpha, inv_alpha, 16, 7);
}

#endif /* GPU_BUF_BLUR_MVE_OPT */
