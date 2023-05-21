//
// Created by wengz on 2023/5/5.
//

#ifndef AUDIOMAGIC_WAVE_GEN_H
#define AUDIOMAGIC_WAVE_GEN_H
#include "wave_struct.h"
#include "vector"
void wave_gen(const std::string &filename, const std::vector<int16_t> &data, uint32_t samples);

#endif //AUDIOMAGIC_WAVE_GEN_H
