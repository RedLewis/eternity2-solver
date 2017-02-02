#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include "cell.h"
#include "e2squarepieces.h"

class Board
{
    Cell *_board[16][16];
    int fitness;
public:
    Board();
    ~Board();
    int getFitness();
    std::pair<Board, Board> operator*(const Board& other);
private:
    int evaluateFitness();
    void mutate();
};


#endif // BOARD_H
