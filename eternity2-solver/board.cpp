#include "board.h"
#include "e2squarepieces.h"
#include <array>
#include <algorithm>
#include <chrono>
#include <iostream>

Board::Board()
{
    std::array<Cell*, 256 > tmp;
    for(int i=0; i < 256; ++i) {
        tmp[i] = (new Cell(e2SquarePieces[i][0], e2SquarePieces[i][1], e2SquarePieces[i][2], e2SquarePieces[i][3], rand() % 4));
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

Board::Board(const Board& other) {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            _board[y][x] = new Cell(*(other._board[y][x]));
        }
    }
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

std::pair<Board, Board> regionExchangeCrossover(const Board& board1, const Board& board2)
{
    std::pair<Board, Board> childBorads(board1, board2);
}

int Board::evaluateFitness()
{
    //_fitness =
}

void Board::mutate()
{

}
