#include <cstdint>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "numeric"
#include "wave_struct.h"
#include "equalizer.h"
#include "fstream"
#include <tuple>
#include <random>
#include <algorithm>
#include "wave_gen.h"




//#define PWM_Hz 200000 //20kHz
//#define SAMPLE_PER_PWM_PERIOD (TEST_SAMPLE_RATE/PWM_Hz) //每个PWM周期内含有的采样点个数


class wave_file {
public:
    wave_file() = delete;

    ~wave_file() = default;

    explicit wave_file(const std::string &file_name) {
        std::ifstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "open file failed" << std::endl;
            return;
        }
        file.read(reinterpret_cast<char *>(&wavHdr), sizeof(SimpleWavHdr));
        if (wavHdr.RiffHdr != FOURCC_RIFF || wavHdr.WavHdr != FOURCC_WAVE || wavHdr.FmtHdr != FOURCC_FMT ||
            wavHdr.dataHdr != FOURCC_DATA) {
            std::cout << "not a wav file" << std::endl;
            return;
        }
        if (wavHdr.DataType != 1) {
            std::cout << "not a pcm wav file" << std::endl;
            return;
        }
        if (wavHdr.BitsPerSample != 16 && wavHdr.BitsPerSample != 32) {
            std::cout << "not a 16/32 bit pcm wav file" << std::endl;
            return;
        }
        pcm_data.resize(wavHdr.RawSize);
        file.read(reinterpret_cast<char *>(pcm_data.data()), wavHdr.RawSize);
        sample_count = wavHdr.RawSize / (wavHdr.BitsPerSample / 8) / wavHdr.ChanNo;
    }

    std::tuple<int16_t, int16_t> get_sample_16bit_2ch(uint32_t index) {
        if (wavHdr.BitsPerSample == 16) {
            auto *data = reinterpret_cast<int16_t *>(pcm_data.data());
            return std::make_tuple(data[index * 2], data[index * 2 + 1]);//左右声道
        } else {
            return {0, 0};
        }
    }

    std::tuple<int32_t, int32_t> get_sample_32bit_2ch(uint32_t index) {
        if (wavHdr.BitsPerSample == 16) {
            auto *data = reinterpret_cast<int32_t *>(pcm_data.data());
            return std::make_tuple(data[index * 2], data[index * 2 + 1]);//左右声道
        }
    }


    SimpleWavHdr wavHdr{};
    std::vector<int8_t> pcm_data;
    uint64_t sample_count{};
};


void noise_gen() {
    const float mean = 0;
    const float stddev = 1;
    std::default_random_engine generator;
    std::normal_distribution<float> dist(mean, stddev);

    std::vector<float> d;
    for (int i = 0; i < 44100 * 5; i++) {
        auto f = dist(generator);
        d.push_back(f);
        d.push_back(f);
    }
    auto Max = std::abs(
            *std::max_element(d.begin(), d.end(), [](float a, float b) { return std::abs(a) < std::abs(b); }));
    std::vector<int16_t> di(d.size());
    for (auto &i: d) {
        i = i / Max * 32767;
    }
    std::transform(d.begin(), d.end(), di.begin(), [](float a) { return static_cast<int16_t>(a); });

    wave_gen("noise.wav", di, d.size() / 2);
}

int main(int argc, char **argv) {
    equalizer eq1, eq2;
    eq1.set_gain(31.25, 8);
    eq1.set_gain(62.5, 8);
    eq1.set_gain(125, 8);

    eq1.set_gain(2000, 0);
    eq1.set_gain(4000, 0);
    eq1.set_gain(8000, 0);

    eq2.set_gain(31.25,8);
    eq2.set_gain(62.5, 8);
    eq2.set_gain(125, 8);

    eq2.set_gain(2000, 0);
    eq2.set_gain(4000, 0);
    eq2.set_gain(8000, 0);

    eq1.peak_filter_design();
    eq2.peak_filter_design();

    wave_file wav("test.wav");
    std::cout << wav.sample_count << std::endl;
    std::cout << wav.wavHdr.ChanNo << std::endl;
    std::cout << wav.wavHdr.BitsPerSample << std::endl;
    std::vector<int16_t> d;
    d.reserve(wav.sample_count * wav.wavHdr.ChanNo);
    for (uint32_t i = 0; i < wav.sample_count; i++) {
        auto [left, right] = wav.get_sample_16bit_2ch(i);
        d.push_back(eq1.EQ_process(left));
        d.push_back(eq2.EQ_process(right));
    }
    wave_gen("out.wav", d, wav.sample_count);
    std::cout<<eq1.get_enegry_fix()<<std::endl;
    std::cout<< *std::max_element(d.begin(), d.end(), [](int16_t a, int16_t b) { return std::abs(a) < std::abs(b); })<<std::endl;


    return 0;
}