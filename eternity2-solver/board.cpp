#include "board.h"
#include "e2squarepieces.h"
#include <array>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
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
}

void Board::mutate()
{
}

int Board::evaluateFitness()
{
    //_fitness =
}

std::ostream& Board::_stringify(std::ostream& os)const
{
    int cellLine = 0;
    int boardLine = 0;
    int boardRaw = 0;
    std::string bgr;
    std::string wht("\e[100m");
    std::string blk("\e[47m");
    std::string red("\e[101m");
    std::string grn("\e[42m");
    std::string blu("\e[44m");
    std::string rst("\e[49m");

    os << "board"<< std::endl;

    while (boardLine < 16){
        cellLine = 0;
        while (cellLine < 3){
            boardRaw = 0;
            while(boardRaw < 16){
                if (( (boardRaw % 2) &&  (boardLine % 2)) ||
                    (!(boardRaw % 2) && !(boardLine % 2)))
                    bgr = wht;
                else
                    bgr = blk;
                //os << "  ";
                if (cellLine == 0){
                    os << bgr << "  ";
                    if (_board[boardLine][boardRaw]->getTop() == 0){
                        os << blu;
                    }else{
                        if (boardLine != 0 && _board[boardLine][boardRaw]->getTop() == _board[boardLine - 1][boardRaw]->getDown()){
                            os << grn;
                        }else{
                            os << red;
                        }
                    }
                    os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_board[boardLine][boardRaw]->getTop();
                    os << bgr << "  ";
                }else if(cellLine == 1){
                    if (_board[boardLine][boardRaw]->getLeft() == 0){
                        os << blu;
                    }else{
                        if (boardRaw != 0 && _board[boardLine][boardRaw]->getLeft() == _board[boardLine][boardRaw - 1]->getRight()){
                            os << grn;
                        }else{
                            os << red;
                        }
                    }
                    os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_board[boardLine][boardRaw]->getLeft();
                    os << bgr << "  ";
                    if (_board[boardLine][boardRaw]->getRight() == 0){
                        os << blu;
                    }else{
                        if (boardRaw < 15 && _board[boardLine][boardRaw]->getRight() == _board[boardLine][boardRaw + 1]->getLeft()){
                            os << grn;
                        }else{
                            os << red;
                        }                    }
                     os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_board[boardLine][boardRaw]->getRight();
                }else{
                    os << bgr << "  ";
                    if (_board[boardLine][boardRaw]->getDown() == 0){
                        os << blu;
                    }else{

                        if (boardLine < 15 && _board[boardLine][boardRaw]->getDown() == _board[boardLine + 1][boardRaw]->getTop()){
                            os << grn;
                        }else{
                            os << red;
                        }
                   }
                   os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_board[boardLine][boardRaw]->getDown();
                   os << bgr <<"  ";
                }
                os << rst;
                ++boardRaw;
            }
            ++cellLine;
            os << std::endl;
        }
        ++boardLine;
        //os << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Board& other)
{

    return other._stringify(os);
}
