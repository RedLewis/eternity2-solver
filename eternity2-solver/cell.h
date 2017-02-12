#ifndef CELL_H
#define CELL_H

#include "aligned.h"

class Cell : public Aligned<Alignment::SSE>
{
private:

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

    Data _data;
    unsigned char _rotation = 0; //From 0 to 3

public:
    //Creates a cell using top right down and left patterns.
    //Will also create all 4 possible rotations of these patterns
    Cell(unsigned char top, unsigned char right,
         unsigned char down, unsigned char left,
         unsigned char rotation);
    Cell(const Cell& other);


    unsigned char getTop();
    unsigned char getRight();
    unsigned char getDown();
    unsigned char getLeft();
    unsigned char getRotation();
    void setRotation(unsigned char rotation);

};

#endif // CELL_H
