//
// Created by wengz on 2023/5/5.
//

#ifndef AUDIOMAGIC_WAVE_STRUCT_H
#define AUDIOMAGIC_WAVE_STRUCT_H

#define FOURCC_RIFF     ((uint32_t) (uint8_t) ('R') | ((uint32_t) (uint8_t) ('I') << 8) | ((uint32_t) (uint8_t) ('F') << 16) | ((uint32_t) (uint8_t) ('F') << 24))
#define FOURCC_WAVE           ((uint32_t) (uint8_t) ('W') | ((uint32_t) (uint8_t) ('A') << 8) | ((uint32_t) (uint8_t) ('V') << 16) | ((uint32_t) (uint8_t) ('E') << 24))
#define FOURCC_FMT           ((uint32_t) (uint8_t) ('f') | ((uint32_t) (uint8_t) ('m') << 8) | ((uint32_t) (uint8_t) ('t') << 16) | ((uint32_t) (uint8_t) (' ') << 24))
#define FOURCC_DATA           ((uint32_t) (uint8_t) ('d') | ((uint32_t) (uint8_t) ('a') << 8) | ((uint32_t) (uint8_t) ('t') << 16) | ((uint32_t) (uint8_t) ('a') << 24))
#include <stdint.h>

struct SimpleWavHdr {
    uint32_t RiffHdr; //"RIFF"
    uint32_t ChunkSize; //file size - 8
    uint32_t WavHdr;//"WAVE"
    uint32_t FmtHdr; //"fmt "
    uint32_t HdrLen; //16, length of above
    uint16_t DataType; //1->PCM
    uint16_t ChanNo; //2 Channel
    uint32_t SampleRate;//22050*2 Hz
    uint32_t SamplePerSec; //8000 sample per second
    uint16_t BytePerSample;//Bytes per sample
    uint16_t BitsPerSample;//Bits per sample
    uint32_t dataHdr;//"data"
    uint32_t RawSize;//data size from this point
};
#endif //AUDIOMAGIC_WAVE_STRUCT_H
