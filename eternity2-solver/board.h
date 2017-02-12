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
    enum{MAX_FITNESS = 1088};
    Board();
    Board(const Board& other);
    ~Board();
    int getFitness();
    std::ostream& _stringify(std::ostream& os)const;
    std::pair<Board*, Board*> regionExchangeCrossover(const Board& board1, const Board& board2);
    void rotateRegionMutation(int posX, int posY,int size);

protected:
    int evaluateFitness();
    void mutate();

public:
    void swapSquare(int posXa, int posYa, int posXb, int posYb, int sizeX, int sizeY);
    void rotateSquare(int posX, int posY, int size);
};

std::ostream& operator<<(std::ostream& os, const Board& other);

#endif // BOARD_H
