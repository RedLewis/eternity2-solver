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

std::pair<Board*, Board*> regionExchangeCrossover(const Board& board1, const Board& board2)
{
}

void Board::rotateSquare(int posX, int posY,int size)
{
    if ((posX < 0 || posX >= 16) ||
        (posY < 0 || posY >= 16) ||
        (size < 0 || size > 16) ||
        (posX + size > 16) ||
        (posY + size > 16)){
        std::cerr << "mutation parameter invalid" << std::endl;
        return;
    }

    for ( int i = 0; i < size; ++i ) {
      for ( int j = 0; j < size; ++j ) {
          _board[posY + i][posX + j]->setRotation(_board[posY + i][posX + j]->getRotation() + 1);
        }
    }
    // Transpose the matrix
    for ( int i = 0; i < size; ++i ) {
      for ( int j = i + 1; j < size; ++j ) {
        Cell* tmp = _board[posY + i][posX + j];
        _board[posY + i][posX + j] = _board[posX + j][posY + i];
        _board[posX + j][posY + i] = tmp;
      }
    }
    // Swap the columns
    for ( int i = 0; i < size; ++i ) {
      for ( int j = 0; j < size/2; ++j ) {
         Cell* tmp = _board[posY + i][posX + j];
        _board[posY + i][posX + j] = _board[posY + i][posY + (size-1-j)];
        _board[posY + i][posY + (size-1-j)] = tmp;
      }
    }
}
/*
// ne gere pas les overlap
void Board::swapSquare(int posXa, int posYa,int posXb, int posYb, int sizeX, int sizeY)
{
    if ((posXa < 0 || posXa >= 16) ||
        (posYa < 0 || posYa >= 16) ||
        (posXb < 0 || posXb >= 16) ||
        (posYb < 0 || posYb >= 16) ||
        (sizeX < 0 || sizeX > 16) ||
        (sizeY < 0 || sizeY > 16) ||
        (posXa + sizeX > 16) ||
        (posYa + sizeY > 16) ||
        (posXb + sizeX > 16) ||
        (posYb + sizeY > 16)){
        std::cerr << "mutation parameter invalid" << std::endl;
        return;
    }

    if (((posXb >= posXa && posXb < (posXa + sizeX)) && (posYb >= posYa && posYb < (posYa + sizeY))) ||
        ((posXa >= posXb && posXa < (posXb + sizeX)) && (posYa >= posYb && posYa < (posYb + sizeY)))){
        std::cerr << "mutation parameter invalid overlap not handled" << std::endl;
        return;
    }
    for ( int i = 0; i < sizeY; i++ ) {
      for ( int j = 0; j < sizeX; j++ ) {
          Cell* tmp = _board[posYa + i][posXa + j];
          _board[posYa + i][posXa + j] = _board[posYb + i][posXb + j];
          _board[posYb + i][posXb + j] = tmp;
        }
    }
}
*/
void Board::swapSquare(int posXa, int posYa,int posXb, int posYb, int sizeX, int sizeY)
{
    if ((posXa < 0 || posXa >= 16) ||
        (posYa < 0 || posYa >= 16) ||
        (posXb < 0 || posXb >= 16) ||
        (posYb < 0 || posYb >= 16) ||
        (sizeX < 0 || sizeX > 16) ||
        (sizeY < 0 || sizeY > 16) ||
        (posXa + sizeX > 16) ||
        (posYa + sizeY > 16) ||
        (posXb + sizeX > 16) ||
        (posYb + sizeY > 16)){
        std::cerr << "mutation parameter invalid" << std::endl;
        return;
    }
    Cell *cpy[sizeY][sizeX] = {};

    for ( int i = 0; i < sizeY; ++i ) {
      for ( int j = 0; j < sizeX; ++j ) {
          cpy[i][j] = _board[posYa + i][posXa + j];
          _board[posYa + i][posXa + j] = _board[posYb + i][posXb + j];
        }
    }

    for ( int i = 0; i < sizeY; ++i ) {
      for ( int j = 0; j < sizeX; ++j ) {
          _board[posYb + i][posXb + j] = cpy[i][j];
        }
    }

}

void Board::rotateRegionMutation(int posX, int posY,int size)
{
    rotateSquare(posX, posY, size);
}


int Board::evaluateFitness()
{
    const Cell* topCell;
    const Cell* rightCell;
    const Cell* downCell;
    const Cell* leftCell;
    const Cell* currentCell;

    _fitness = 0;
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            //Get neighbor cells
            currentCell = _board[y][x];
            topCell = (y == 0) ? NULL : _board[y - 1][x];
            rightCell = (x == 15) ? NULL : _board[y][x + 1];
            downCell = (y == 15) ? NULL : _board[y + 1][x];
            leftCell = (x == 0) ? NULL : _board[y][x - 1];
            //Check for matching edges
            if (currentCell->getTop() == Cell::EDGE_VALUE && topCell == NULL)
                _fitness += 2;
            else if (currentCell->getTop() != Cell::EDGE_VALUE && topCell != NULL && currentCell->getTop() == topCell->getDown())
                _fitness += 1;
            if (currentCell->getRight() == Cell::EDGE_VALUE && rightCell == NULL)
                _fitness += 2;
            else if (currentCell->getRight() != Cell::EDGE_VALUE && rightCell != NULL && currentCell->getRight() == rightCell->getLeft())
                _fitness += 1;
            if (currentCell->getDown() == Cell::EDGE_VALUE && downCell == NULL)
                _fitness += 2;
            else if (currentCell->getDown() != Cell::EDGE_VALUE && downCell != NULL && currentCell->getDown() == downCell->getTop())
                _fitness += 1;
            if (currentCell->getLeft() == Cell::EDGE_VALUE && leftCell == NULL)
                _fitness += 2;
            else if (currentCell->getLeft() != Cell::EDGE_VALUE && leftCell != NULL && currentCell->getLeft() == leftCell->getRight())
                _fitness += 1;
        }
    }

    return _fitness;
}

std::ostream& Board::_stringify(std::ostream& os)const
{
    int cellLine = 0;
    int boardLine = 0;
    int boardRaw = 0;
    std::string bgr;
    std::string wht("\e[100m");
    std::string blk("\e[47m");

    std::string red;
    std::string grn;
    std::string blu;

    std::string red_w("\e[1;100;31m");
    std::string grn_w("\e[1;102;90m");
    std::string blu_w("\e[1;104;90m");

    std::string red_b("\e[1;47;91m");
    std::string grn_b("\e[1;42;37m");
    std::string blu_b("\e[1;44;37m");

    std::string rst("\e[0m");

    os << "fitness: " << _fitness << "/" << MAX_FITNESS << std::endl;

    while (boardLine < 16){
        cellLine = 0;
        while (cellLine < 3){
            boardRaw = 0;
            while(boardRaw < 16){
                if (( (boardRaw % 2) &&  (boardLine % 2)) ||
                    (!(boardRaw % 2) && !(boardLine % 2))){
                    bgr = wht;
                    red = red_w;
                    blu = blu_w;
                    grn = grn_w;
                }else{
                    bgr = blk;
                    red = red_b;
                    blu = blu_b;
                    grn = grn_b;
                }
                //os << "  ";
                if (cellLine == 0){
                    os << bgr << "  ";
                    if (_board[boardLine][boardRaw]->getTop() == 0){
                        if (boardLine == 0){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
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
                        if (boardRaw == 0){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
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
                        if (boardRaw == 15){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
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
                        if (boardLine ==15){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
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
            os << rst;
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
