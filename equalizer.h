//
// Created by wengz on 2023/5/6.
//

#ifndef AUDIOMAGIC_EQUALIZER_H
#define AUDIOMAGIC_EQUALIZER_H

#include "wave_struct.h"
#include <vector>

using std::vector;

void peak_filter_design();

float EQ_process(float x);

float Gain_at(float freq);

class equalizer {
public:
    equalizer() = default;

    ~equalizer() = default;

    float EQ_process(float x);

    float gain_at(float freq);

    void peak_filter_design();

    float Q = 1.414f;

    void set_gain(float freq, float gain);

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
    float coffs[10][6]{};
    float tmp[10][4]{};//x1,x2,y1,y2
};


#endif //AUDIOMAGIC_EQUALIZER_H
