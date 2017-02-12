#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include "cell.h"
#include "e2squarepieces.h"

class Board
{
    Cell *_board[16][16];
    int _fitness;
public:
    Board();
    ~Board();
    int getFitness();
    std::pair<Board, Board> operator*(const Board& other);
    std::ostream& _stringify(std::ostream& os)const;
protected:
    int evaluateFitness();
    void mutate();
};

std::ostream& operator<<(std::ostream& os, const Board& other);

#endif // BOARD_H
