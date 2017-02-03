#include "board.h"
#include "e2squarepieces.h"
#include <array>
#include <algorithm>
#include <chrono>
#include <iostream>

Board::Board()
{
    std::array<Cell*, E2SQUAREPIECES_NBR> tmp;
    for(int i=0; i < E2SQUAREPIECES_NBR; ++i) {
        tmp[i] = (new Cell(e2SquarePieces[i][0], e2SquarePieces[i][1], e2SquarePieces[i][2], e2SquarePieces[i][3]));
        //std::cout << tmp[i] << std::endl;
    }

    std::shuffle(tmp.begin(), tmp.end(), std::default_random_engine(rand()));
    int index = 0;
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            _board[y][x] = tmp[index++];
        }
    }
    evaluateFitness();
}

Board::~Board()
{
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            delete _board[y][x];
        }
    }
}

int Board::getFitness()
{
    return _fitness;
}

int Board::evaluateFitness()
{
    //_fitness =
}
std::pair<Board, Board> Board::operator*(const Board& other)
{

}

void Board::mutate()
{

}
