#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include "tile.h"
#include "e2tiles.h"
#include <list>
#include <array>

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
    TileRef _tiles[16][16] = {};
    float _fitness = -1;

public:

    enum  {
        MAX_FITNESS = 1088
    };

    Board(bool empty = false);
    Board(const Board& other);
    ~Board();
    float getFitness() const;
    std::ostream& _stringify(std::ostream& os) const;
    static std::pair<Board*, Board*> regionExchangeCrossover(const Board& board1, const Board& board2);

    void mutateOuter();
    void swapAndRotateAngleMutation();
    void inversionInnerRegionMutation();
    void rotateInnerRegionMutation();
    void swapInnerRegionMutation();
    void swapAndRotateInnerRegionMutation();
    void rawAndColumnInversionInnerRegionMutation();
    bool swapRectangle(int posXa, int posYa, int posXb, int posYb, int sizeX, int sizeY);
    bool rotateSquare(int posX, int posY, int size);  

    float evaluate();
    bool isValid();
private:
    static void getSolvedEdgesForBoard(Board* refBoard, std::array<TileRef, 56>& borderTiles, std::list<Board*>& solvedEdgesBoardsForBoard);

public:
    static void unitTestSwap();
    static void getSolvedEdgesBoards();
};

std::ostream& operator<<(std::ostream& os, const Board& other);

#endif // BOARD_H
