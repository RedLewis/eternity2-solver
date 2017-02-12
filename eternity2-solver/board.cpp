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
    _fitness = 0;
    return _fitness;
}

void Board::mutate()
{

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
                os << " ";
                if (cellLine == 0){
                    os << bgr << "##";
                    if (_board[boardLine][boardRaw]->data.s_pattern[_board[boardLine][boardRaw]->orientation].top == 0){
                        os << blu;
                    }else{
                        os << red;
                    }
                    os << "@@";
                    os << bgr << "##";
                }else if(cellLine == 1){
                    if (_board[boardLine][boardRaw]->data.s_pattern[_board[boardLine][boardRaw]->orientation].left == 0){
                        os << blu;
                    }else{
                        os << red;
                    }
                    os << "@@";
                    os << bgr << "##";
                    if (_board[boardLine][boardRaw]->data.s_pattern[_board[boardLine][boardRaw]->orientation].right == 0){
                        os << blu;
                    }else{
                        os << red;
                    }
                     os <<"@@";
                }else{
                    os << bgr << "##";
                    if (_board[boardLine][boardRaw]->data.s_pattern[_board[boardLine][boardRaw]->orientation].down == 0){
                        os << blu;
                    }else{
                        os << red;
                    }
                    os <<"@@";
                    os << bgr <<"##";
                }
                os << rst;
                ++boardRaw;
            }
            ++cellLine;
            os << std::endl;
        }
        ++boardLine;
        os << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Board& other)
{

    return other._stringify(os);
}
