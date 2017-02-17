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



int TileRef::getTop() const {
    if (_ptr == NULL)
        return -1;
    return _ptr->s_pattern[_rotation].top;
}

int TileRef::getRight() const {
    if (_ptr == NULL)
        return -1;
    return _ptr->s_pattern[_rotation].right;
}

int TileRef::getDown() const {
    if (_ptr == NULL)
        return -1;
    return _ptr->s_pattern[_rotation].down;
}

int TileRef::getLeft() const {
    if (_ptr == NULL)
        return -1;
    return _ptr->s_pattern[_rotation].left;
}

int TileRef::getRotation() const {
    if (_ptr == NULL)
        return -1;
    return _rotation;
}

void TileRef::setRotation(unsigned char rotation) {
    _rotation = rotation % 4;
}

