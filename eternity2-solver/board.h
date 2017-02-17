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
    Point& operator=(const Point& other) {
        x = other.x; y = other.y;
    }
    bool operator==(const Point& other) {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) {
        return x != other.x || y != other.y;
    }
};

class Board
{
    TileRef _tiles[16][16] = {};
    float _fitness = -1;

public:

    static constexpr float MAX_FITNESS = 2530.22f;

    Board(bool empty = false);
    Board(const Board& other);
    ~Board();
    float getFitness() const;
    std::ostream& _stringify(std::ostream& os) const;
    static std::pair<Board*, Board*> regionExchangeCrossover(const Board& board1, const Board& board2);

    void simpleOuterMutation();
    void swapAndRotateAngleMutation();
    void inversionInnerRegionMutation();
    void rotateInnerRegionMutation();
    void swapInnerRegionMutation();
    void swapAndRotateInnerRegionMutation();
    void rawAndColumnInversionInnerRegionMutation();
    bool swapRectangle(int posXa, int posYa, int posXb, int posYb, int sizeX, int sizeY);
    bool rotateSquare(int posX, int posY, int size);  
    void swapChunkOuterMutation();
    float evaluate();
    bool isValid();
    float cmp(const Board& other) const;
private:
    static bool isTileInBoardEdge(const Board& board, const TileRef& tile);
    static void getSolvedEdgesForBoard(Board& currBoard, Point<int> edgeIndex, std::list<Board*>& solvedEdgesBoardsForBoard);

public:
    static void unitTestSwap();
    static std::list<Board*> getSolvedEdgesBoards();
};

std::ostream& operator<<(std::ostream& os, const Board& other);

#endif // BOARD_H
