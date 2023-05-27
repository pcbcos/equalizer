//
// Created by wengz on 2023/5/6.
//

#include "equalizer.h"
#include <complex>
#include <cmath>
#include <algorithm>

#if USE_CMSIS_DSP

#include "arm_math.h"
#include "stm32f4xx_ll_tim.h"

#endif

constexpr uint32_t Fs = 50000;


void equalizer::peak_filter_design() {
#if !USE_CMSIS_DSP
    for (int i = 0; i < 10; i++) {
        float A = std::pow(10.0f, gains[i] / 20);
        float w0 = 2 * std::numbers::pi_v<float> * freqs[i] / Fs;
        float alpha = std::sin(w0) / (2 * Q[i]);
        float b0 = 1 + alpha * A;
        float b1 = -2 * std::cos(w0);
        float b2 = 1 - alpha * A;
        float a0 = 1 + alpha / A;
        float a1 = -2 * std::cos(w0);
        float a2 = 1 - alpha / A;
        coffs[i][0] = b0 / a0;
        coffs[i][1] = b1 / a0;
        coffs[i][2] = b2 / a0;
        coffs[i][3] = 1.0f;
        coffs[i][4] = a1 / a0;
        coffs[i][5] = a2 / a0;
    }
#else
    arm_fill_q15(0, state, 40);
    for (int i = 0; i < 10; i++) {
        float A = std::pow(10.0f, gains[i] / 20);
        float w0 = 2 * std::numbers::pi_v<float> * freqs[i] / Fs;
        float alpha = std::sin(w0) / (2 * Q[i]);
        float b0 = 1 + alpha * A;
        float b1 = -2 * std::cos(w0);
        float b2 = 1 - alpha * A;
        float a0 = 1 + alpha / A;
        float a1 = -2 * std::cos(w0);
        float a2 = 1 - alpha / A;
        coffs[i * 6 + 0] = b0 / a0;
        coffs[i * 6 + 1] = 0;
        coffs[i * 6 + 2] = b1 / a0;
        coffs[i * 6 + 3] = b2 / a0;
        coffs[i * 6 + 4] = -a1 / a0;
        coffs[i * 6 + 5] = -a2 / a0;

        int8_t shift = 0;
        auto t = std::abs(*std::max_element(coffs + i * 6, coffs + i * 6 + 6,
                                            [](float a, float b) { return std::abs(a) < std::abs(b); }));
        while (t >= 1) {
            shift++;
            t /= 2;
            for (int j = 0; j < 6; j++) {
                coffs[i * 6 + j] /= 2;
            }
        }
        arm_float_to_q15(coffs + i * 6, coffs_q15 + i * 6, 6);
        arm_biquad_cascade_df1_init_q15(df_inst_ptr + i, 1, coffs_q15 + i * 6, state + i * 4, shift);
    }
    auto max_gain = *std::max_element(gains, gains + 10);
    auto scale = std::pow(10.0f, -max_gain / 20);
    arm_float_to_q15(&scale, &scale_fact, 1);
#endif
}

//float equalizer::iir_process(float x, int i) {
//    float y = coffs[i][0] * x + coffs[i][1] * tmp[i][0] + coffs[i][2] * tmp[i][1] - coffs[i][4] * tmp[i][2] -
//              coffs[i][5] * tmp[i][3];
//    tmp[i][1] = tmp[i][0];//x2 = x1;
//    tmp[i][0] = x;//x1 = x;
//    tmp[i][3] = tmp[i][2];//y2 = y1;
//    tmp[i][2] = y;//y1 = y;
//
//    return y;
//}

//float equalizer::gain_at(float freq) {
//    float gain = 1;
//    auto j = std::complex<float>(0, 1);
//    auto z_inv = std::exp(-j * 2.0f * std::numbers::pi_v<float> * freq / (float) Fs);
//    for (int i = 0; i < 10; i++) {
//        gain *= std::abs(
//                (coffs[i][0] + coffs[i][1] * z_inv + coffs[i][2] * z_inv * z_inv) /
//                (1.0f + coffs[i][4] * z_inv + coffs[i][5] * z_inv * z_inv));
//    }
//    return gain;
//}


float equalizer::EQ_process(float x) {
    float y = x / 32767;
    y /= 2.512f;
    y = iir_process(y, 0);
    for (int i = 1; i < 10; i++) {
        y = iir_process(y, i);
    }
    if (y > 1) {
        y = 1;
    } else if (y < -1) {
        y = -1;
    }
    return y * 32767;
}

#if USE_CMSIS_DSP

void equalizer::EQ_process(q15_t *x, q15_t *y, size_t n) {
    arm_scale_q15(x, scale_fact, -1, y, n);
    for (int i = 0; i < 10; i++) {
        arm_biquad_cascade_df1_q15(df_inst_ptr + i, y, y, n);
    }
    //arm_shift_q15(y, 1, y, n);
    arm_shift_q15(y, -4, y, n);
}

#endif

void equalizer::set_gain(uint8_t index, float gain) {
    this->gains[index] = gain;
}

float equalizer::get_enegry_fix() {
    return enegry_fix;
}

float equalizer::peak_detector(float x) {
    static float y1_last = 0;
    float y1 = std::max(x, alpha_r * y1_last + (1 - alpha_r) * x);
    float y = alpha_a * y1_last + (1 - alpha_a) * y1;
    y1_last = y1;
    return y;
}

float equalizer::gain_computer(float x) {
    static float T = 25000;
    static float W = 1000;
    static float R = 3000;
    float abs_x = std::abs(x);
    int8_t sign = x > 0 ? 1 : -1;
    if (2 * (abs_x - T) < -W) {
        return 1;
    } else if (2 * (abs_x - T) > W) {
        return (T + (abs_x - T) / R) / abs_x;
    } else {
        return (1 / R - 1) * (abs_x - T + W / 2) * (abs_x - T + W / 2) / (2 * W) / abs_x;
    }
}

void equalizer::set_freq(uint8_t index, float freq) {
    this->freqs[index] = freq;
}

void equalizer::set_q(uint8_t index, float q) {
    this->Q[index] = q;
}

float equalizer::get_gain(uint8_t index) {
    return this->gains[index];
}

float equalizer::get_freq(uint8_t index) {
    return this->freqs[index];
}

float equalizer::get_q(uint8_t index) {
    return this->Q[index];
}