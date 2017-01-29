#ifndef CELL_H
#define CELL_H

#include "aligned.h"

class Cell : Aligned<Alignment::SSE>
{
public:

    union {
        __m128 xmm;
        unsigned int alias[4];
    };

};

#endif // CELL_H
