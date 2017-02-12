#include "cell.h"
#include <iostream>
Cell::Cell(unsigned char top, unsigned char right,
           unsigned char down, unsigned char left,
           unsigned char rotation) {
    _rotation = rotation % 4;

    _data.s_pattern[0].top = top;
    _data.s_pattern[0].right = right;
    _data.s_pattern[0].down = down;
    _data.s_pattern[0].left = left;

    _data.s_pattern[1].top = left;
    _data.s_pattern[1].right = top;
    _data.s_pattern[1].down = right;
    _data.s_pattern[1].left = down;

    _data.s_pattern[2].top = down;
    _data.s_pattern[2].right = left;
    _data.s_pattern[2].down = top;
    _data.s_pattern[2].left = right;

    _data.s_pattern[3].top = right;
    _data.s_pattern[3].right = down;
    _data.s_pattern[3].down = left;
    _data.s_pattern[3].left = top;
}

Cell::Cell(const Cell& other)
{
    _rotation = other._rotation;
    _data.i_pattern[0] = other._data.i_pattern[0];
    _data.i_pattern[1] = other._data.i_pattern[1];
    _data.i_pattern[2] = other._data.i_pattern[2];
    _data.i_pattern[3] = other._data.i_pattern[3];
}

unsigned char Cell::getTop() {
    return _data.s_pattern[_rotation].top;
}

unsigned char Cell::getRight() {
    return _data.s_pattern[_rotation].right;
}

unsigned char Cell::getDown() {
    return _data.s_pattern[_rotation].down;
}

unsigned char Cell::getLeft() {
    return _data.s_pattern[_rotation].left;
}

unsigned char Cell::getRotation() {
    return _rotation;
}

void Cell::setRotation(unsigned char rotation) {
    _rotation = rotation % 4;
}
