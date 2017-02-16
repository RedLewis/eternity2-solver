#ifndef E2SQUAREPIECES_H
#define E2SQUAREPIECES_H

#include <array>
#include "aligned.h"

struct TileData: public Aligned<Alignment::SSE> {
    struct Pattern {
        unsigned char top;
        unsigned char right;
        unsigned char down;
        unsigned char left;
    };
    union {
        __m128 xmm;
        unsigned int i_pattern[4];
        Pattern s_pattern[4];
    };

    TileData();
};


struct TileDataArrays {
    std::array<TileData, 256> all;
    std::array<TileData, 4> corners;
    std::array<TileData, 56> borders;
    std::array<TileData, 196> inners;
};

extern const TileDataArrays E2TILES;
#endif // E2SQUAREPIECES_H
