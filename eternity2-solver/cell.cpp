#include "cell.h"
#include <iostream>
Cell::Cell(unsigned char top, unsigned char right, unsigned char down, unsigned char left) {
    //std::cout << "t" << (int)top << "r" << (int)right << "d" << (int)down << "l" << (int)left << std::endl;
    data.s_pattern[0].top = top;
    data.s_pattern[0].right = right;
    data.s_pattern[0].down = down;
    data.s_pattern[0].left = left;
    //std::cout << data.i_pattern[0] << std::endl;

    data.s_pattern[1].top = left;
    data.s_pattern[1].right = top;
    data.s_pattern[1].down = right;
    data.s_pattern[1].left = down;
    //std::cout << data.i_pattern[1] << std::endl;

    data.s_pattern[2].top = down;
    data.s_pattern[2].right = left;
    data.s_pattern[2].down = top;
    data.s_pattern[2].left = right;
    //std::cout << data.i_pattern[2] << std::endl;

    data.s_pattern[3].top = right;
    data.s_pattern[3].right = down;
    data.s_pattern[3].down = left;
    data.s_pattern[3].left = top;
    //std::cout << data.i_pattern[3] << std::endl<< std::endl;

}

bool operator==(const Cell& lhs, const Cell& rhs)
{
    return (lhs.data.i_pattern[0] == rhs.data.i_pattern[0]) ||
            (lhs.data.i_pattern[0] == rhs.data.i_pattern[1]) ||
            (lhs.data.i_pattern[0] == rhs.data.i_pattern[2]) ||
            (lhs.data.i_pattern[0] == rhs.data.i_pattern[3]);
}
