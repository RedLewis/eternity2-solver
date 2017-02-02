#ifndef CELL_H
#define CELL_H

#include "aligned.h"

class Cell : public Aligned<Alignment::SSE>
{
public:

    struct Data : public Aligned<Alignment::SSE> {

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

    };

    Data data;
    unsigned char orientation = 0; //From 0 to 3
    bool used = false;

    //Creates a cell using top right down and left patterns.
    //Will also create all 4 possible rotations of these patterns
    Cell(unsigned char top, unsigned char right, unsigned char down, unsigned char left);

};

#endif // CELL_H
