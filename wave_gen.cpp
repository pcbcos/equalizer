//
// Created by wengz on 2023/5/5.
//

#include <string>
#include "wave_gen.h"
#include <cmath>
#include "vector"


#ifndef M_PI
#define M_PI                3.14159265358979323846
#endif
#define TEST_SAMPLE_RATE    (44100)
#define TEST_SAMPLE_LEN_SEC    4 //时长
#define TEST_SAMPLE_NUM (TEST_SAMPLE_RATE*TEST_SAMPLE_LEN_SEC)
#define CHAN_NO 2 //声道数
#define SAMPLE_SIZE_IN_BYTE_CH_MONO    2
#define SAMPLE_SIZE_IN_BYTE_ALL_CH (SAMPLE_SIZE_IN_BYTE_CH_MONO * CHAN_NO)
#define BITS_PER_BYTE   8
#define SAMPLE_SIZE_IN_BITS_CH_MONO    (SAMPLE_SIZE_IN_BYTE_CH_MONO * BITS_PER_BYTE)
#define SAMPLE_SIZE_IN_BITS_ALL_CH (SAMPLE_SIZE_IN_BITS_CH_MONO * CHAN_NO)
#define AUDIO_HZ  440
#define AUDIO_CYCLE (TEST_SAMPLE_RATE/AUDIO_HZ) //每个正弦周期有多少采样点
#define UINT8_HIGH  (UINT8_MAX/2)
#define UINT8_LOW   0
#define UINT16_HIGH  (UINT16_MAX/2)
#define UINT16_LOW   0
#define INT16_HIGH  (INT16_MAX/2)
#define INT16_LOW   (0-INT16_HIGH)

void wave_gen(const std::string &filename, const std::vector<int16_t> &data, uint32_t samples) {
    FILE *fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr) {
        printf("open file %s failed\n", filename.c_str());
        return;
    }
    SimpleWavHdr wavHdr;
    wavHdr.RiffHdr = FOURCC_RIFF;
    wavHdr.ChunkSize = samples * SAMPLE_SIZE_IN_BYTE_ALL_CH + sizeof(SimpleWavHdr) - 8;
    wavHdr.WavHdr = ((uint32_t) (uint8_t) ('W') | ((uint32_t) (uint8_t) ('A') << 8) |
                     ((uint32_t) (uint8_t) ('V') << 16) | ((uint32_t) (uint8_t) ('E') << 24));
    wavHdr.FmtHdr = ((uint32_t) (uint8_t) ('f') | ((uint32_t) (uint8_t) ('m') << 8) |
                     ((uint32_t) (uint8_t) ('t') << 16) | ((uint32_t) (uint8_t) (' ') << 24));;
    wavHdr.HdrLen = 16;
    wavHdr.DataType = 1;
    wavHdr.ChanNo = CHAN_NO;
    wavHdr.SampleRate = TEST_SAMPLE_RATE;
    wavHdr.SamplePerSec = TEST_SAMPLE_RATE * SAMPLE_SIZE_IN_BYTE_ALL_CH;
    wavHdr.BytePerSample = SAMPLE_SIZE_IN_BYTE_ALL_CH;
    wavHdr.BitsPerSample = SAMPLE_SIZE_IN_BITS_CH_MONO;
    wavHdr.dataHdr = ((uint32_t) (uint8_t) ('d') | ((uint32_t) (uint8_t) ('a') << 8) |
                      ((uint32_t) (uint8_t) ('t') << 16) | ((uint32_t) (uint8_t) ('a') << 24));
    wavHdr.RawSize = samples * SAMPLE_SIZE_IN_BYTE_ALL_CH;
    fwrite(&wavHdr, sizeof(SimpleWavHdr), 1, fp);
    uint16_t test_16bit_2ch[2]{};
//    for (int i = 0; i < TEST_SAMPLE_NUM; ++i) {
//        sample = (int16_t) (INT16_MAX * std::sin(2 * M_PI * AUDIO_HZ * i / TEST_SAMPLE_RATE));
//        fwrite(&sample, sizeof(int16_t), 1, fp);
//        sample = (int16_t) (INT16_MAX * std::sin(2 * M_PI * AUDIO_HZ * i / TEST_SAMPLE_RATE));
//        fwrite(&sample, sizeof(int16_t), 1, fp);
//    }
    for (auto &i: data) {
        fwrite(&i, sizeof(int16_t), 1, fp);
        //fwrite(&i, sizeof(float), 1, fp);
    }
    fclose(fp);
}