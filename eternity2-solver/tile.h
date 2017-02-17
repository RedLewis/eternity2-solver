#ifndef CELL_H
#define CELL_H

#include "aligned.h"
#include "e2tiles.h"

class TileRef : public Aligned<Alignment::SSE>
{
private:

    const TileData* _ptr = NULL;
    unsigned char _rotation = 0; //From 0 to 3

public:

    enum {
        EDGE_VALUE = 0
    };

    static const TileRef empty;

    //Creates a cell using top right down and left patterns.
    //Will also create all 4 possible rotations of these patterns
    TileRef();
    TileRef(const TileData& tileData);
    TileRef(const TileData& tileData, unsigned char rotation);
    TileRef(const TileRef& other);
    TileRef(const TileRef& other, unsigned char rotation);

    bool operator==(const TileRef& other) const;
    bool operator!=(const TileRef& other) const;
    TileRef& operator=(const TileRef& other);
    TileRef& operator=(const TileData& tileData);

    int getTop() const;
    int getRight() const;
    int getDown() const;
    int getLeft() const;
    int getRotation() const;
    void setRotation(unsigned char rotation);

};

#endif // CELL_H
