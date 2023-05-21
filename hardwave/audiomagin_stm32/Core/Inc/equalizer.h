//
// Created by wengz on 2023/5/6.
//

#ifndef AUDIOMAGIC_EQUALIZER_H
#define AUDIOMAGIC_EQUALIZER_H

#include "wave_struct.h"
#include <vector>

#if USE_CMSIS_DSP

#include "stm32f407xx.h"
#include "arm_math.h"

#endif

using std::vector;
constexpr uint16_t samples_per_packet = 512;//缓冲区大小
void peak_filter_design();

float EQ_process(float x);

float Gain_at(float freq);

class equalizer {
public:
    equalizer() {
        for (auto &i: Q) {
            i = 1.414f;
        }
    }

    ~equalizer() = default;

    float EQ_process(float x);

#if USE_CMSIS_DSP

    void EQ_process(q15_t *x, q15_t *y, size_t n);

#endif

    float gain_at(float freq);

    void peak_filter_design();


    float Q[10]{};

    void set_gain(uint8_t index, float gain);

    void set_freq(uint8_t index, float freq);

    void set_q(uint8_t index, float q);

    float get_enegry_fix();

    float alpha_r;
    float alpha_a;

private:
    float peak_detector(float x);

    float gain_computer(float x);

    float iir_process(float x, int i);

    float enegry_fix{1.0f};
    float gains[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float freqs[10] = {31.25, 62.5, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
#if !USE_CMSIS_DSP
    float coffs[10][6]{};
#endif

    float tmp[10][4]{};//x1,x2,y1,y2
#if USE_CMSIS_DSP
    float coffs[60]{};
    q15_t coffs_q15[60]{};
    q15_t state[40]{};
    arm_biquad_casd_df1_inst_q15 df_inst_ptr[10];
    q15_t scale_fact;
#endif

};


#endif //AUDIOMAGIC_EQUALIZER_H
