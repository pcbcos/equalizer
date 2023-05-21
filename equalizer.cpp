//
// Created by wengz on 2023/5/6.
//

#include "equalizer.h"
#include <complex>
#include <cmath>
#include <algorithm>

constexpr uint32_t Fs = 44100;


void equalizer::peak_filter_design() {
    for (int i = 0; i < 10; i++) {
        float A = std::pow(10.0f, gains[i] / 20);
        float w0 = 2 * std::numbers::pi_v<float> * freqs[i] / Fs;
        float alpha = std::sin(w0) / (2 * Q);
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
}

float equalizer::iir_process(float x, int i) {
    float y = coffs[i][0] * x + coffs[i][1] * tmp[i][0] + coffs[i][2] * tmp[i][1] - coffs[i][4] * tmp[i][2] -
              coffs[i][5] * tmp[i][3];
    tmp[i][1] = tmp[i][0];//x2 = x1;
    tmp[i][0] = x;//x1 = x;
    tmp[i][3] = tmp[i][2];//y2 = y1;
    tmp[i][2] = y;//y1 = y;

    return y;
}

float equalizer::gain_at(float freq) {
    float gain = 1;
    auto j = std::complex<float>(0, 1);
    auto z_inv = std::exp(-j * 2.0f * std::numbers::pi_v<float> * freq / (float) Fs);
    for (int i = 0; i < 10; i++) {
        gain *= std::abs(
                (coffs[i][0] + coffs[i][1] * z_inv + coffs[i][2] * z_inv * z_inv) /
                (1.0f + coffs[i][4] * z_inv + coffs[i][5] * z_inv * z_inv));
    }
    return gain;
}


float equalizer::EQ_process(float x) {
    float y = x / 32767;
    y/=std::pow(10.0f,*std::max_element(gains,gains+9)/20);
    y = iir_process(y, 0);
    for (int i = 1; i < 10; i++) {
        y = iir_process(y, i);
    }
    if (y > 1) {
        y = 1;
    } else if (y < -1) {
        y = -1;
    }
    return y*32767;
}

void equalizer::set_gain(float freq, float gain) {
    for (int i = 0; i < 10; i++) {
        if (this->freqs[i] == freq) {
            this->gains[i] = gain;
            break;
        }
    }
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
