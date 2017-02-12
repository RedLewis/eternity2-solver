#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include "cell.h"
#include "e2squarepieces.h"

class Board
{
    Cell *_board[16][16] = {};
    int _fitness = -1;

public:
    Board();
    Board(const Board& other);
    ~Board();
    int getFitness();
    std::pair<Board, Board> regionExchangeCrossover(const Board& board1, const Board& board2);

protected:
    int evaluateFitness();
    void mutate();
};


#endif // BOARD_H
