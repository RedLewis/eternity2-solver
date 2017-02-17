#include "tile.h"
#include <iostream>

TileRef const TileRef::empty;

TileRef::TileRef() :
    _ptr(NULL),
    _rotation(0)
{}

TileRef::TileRef(const TileData& tileData) :
    _ptr(&tileData),
    _rotation(0)
{}

TileRef::TileRef(const TileData& tileData, unsigned char rotation) :
    _ptr(&tileData),
    _rotation(rotation % 4)
{}

TileRef::TileRef(const TileRef& other) :
    _ptr(other._ptr),
    _rotation(other._rotation)
{}

TileRef::TileRef(const TileRef& other, unsigned char rotation) :
    _ptr(other._ptr),
    _rotation(rotation % 4)
{}

bool TileRef::operator==(const TileRef& other) const {
    return (this->_ptr == other._ptr);
}

bool TileRef::operator!=(const TileRef& other) const {
    return (this->_ptr != other._ptr);
}


TileRef& TileRef::operator=(const TileRef& other) {
    this->_ptr = other._ptr;
    this->_rotation = other._rotation;
    return *this;
}

TileRef& TileRef::operator=(const TileData& tileData) {
    this->_ptr = &tileData;
    return *this;
}



unsigned char TileRef::getTop() const {
    return _ptr->s_pattern[_rotation].top;
}

unsigned char TileRef::getRight() const {
    return _ptr->s_pattern[_rotation].right;
}

unsigned char TileRef::getDown() const {
    return _ptr->s_pattern[_rotation].down;
}

unsigned char TileRef::getLeft() const {
    return _ptr->s_pattern[_rotation].left;
}

unsigned char TileRef::getRotation() const {
    return _rotation;
}

void TileRef::setRotation(unsigned char rotation) {
    _rotation = rotation % 4;
}

