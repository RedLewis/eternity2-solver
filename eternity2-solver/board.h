#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include "tile.h"
#include "e2tiles.h"

template <typename NumType>
struct Point {
    NumType x = 0;
    NumType y = 0;
    Point(NumType x, NumType y) : x(x), y(y) {}
    Point(const Point& p) : x(p.x), y(p.y) {}
    Point() {}
};

class Board
{
    Tile *_tiles[16][16] = {};
    int _fitness = -1;

public:

    enum  {
        MAX_FITNESS = 1088
    };

    Board();
    Board(const Board& other);
    ~Board();
    int getFitness() const;
    std::ostream& _stringify(std::ostream& os) const;
    static std::pair<Board*, Board*> regionExchangeCrossover(const Board& board1, const Board& board2);
    void rotateRegionMutation();
    void swapRegionMutation();
    int evaluate();

    bool checkIntegrity();

public:
    bool swapRectangle(int posXa, int posYa, int posXb, int posYb, int sizeX, int sizeY);
    bool rotateSquare(int posX, int posY, int size);
};

std::ostream& operator<<(std::ostream& os, const Board& other);

#endif // BOARD_H
