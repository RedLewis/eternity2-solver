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

    enum {
        EDGE_VALUE = 0
    };

    //Creates a cell using top right down and left patterns.
    //Will also create all 4 possible rotations of these patterns
    Cell(unsigned char top, unsigned char right,
         unsigned char down, unsigned char left,
         unsigned char rotation);
    Cell(const Cell& other);


    unsigned char getTop() const;
    unsigned char getRight() const;
    unsigned char getDown() const;
    unsigned char getLeft() const;
    unsigned char getRotation() const;
    void setRotation(unsigned char rotation);

};

#endif // CELL_H
