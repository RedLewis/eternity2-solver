#include "board.h"
#include "e2tiles.h"
#include <array>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <list>
#include <cassert>

Board::Board()
{
    std::array<Tile*, 256 > tmp;
    for(int i=0; i < 256; ++i) {
        tmp[i] = (new Tile(E2TILES[i][0], E2TILES[i][1], E2TILES[i][2], E2TILES[i][3], rand() % 4));
    }

    std::shuffle(tmp.begin(), tmp.end(), std::default_random_engine(rand()));
    int index = 0;
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            _tiles[y][x] = tmp[index++];
        }
    }
    evaluateFitness();
}

Board::Board(const Board& other) {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            _tiles[y][x] = new Tile(*(other._tiles[y][x]));
        }
    }
}

Board::~Board()
{
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            delete _tiles[y][x];
        }
    }
}

int Board::getFitness()
{
    return _fitness;
}

std::pair<Board*, Board*> Board::regionExchangeCrossover(const Board& parentA, const Board& parentB)
{

    // Select a random region
    unsigned char width = 1 + rand() % 16;
    unsigned char height = 1 + rand() % 16;
    Point<unsigned char> pointA(rand() % (16 - (width - 1)),
                                rand() % (16 - (height - 1)));
    Point<unsigned char> pointB(rand() % (16 - (width - 1)),
                                rand() % (16 - (height - 1)));

    //Clone the two parents
    std::pair<Board*, Board*> children(new Board(parentA), new Board(parentB));

    //Remove from parent A all tiles that are inside the region in parent B
    for (int y = pointB.y; y < (pointB.y + height); ++y) {
        for (int x = pointB.x; x < (pointB.x + width); ++x) {
            delete children.first->_tiles[y][x];
            children.first->_tiles[y][x] = NULL;
        }
    }

    //Remove from parent B all tiles that are inside the region in parent A
    for (int y = pointA.y; y < (pointA.y + height); ++y) {
        for (int x = pointA.x; x < (pointA.x + width); ++x) {
            delete children.second->_tiles[y][x];
            children.second->_tiles[y][x] = NULL;
        }
    }

    //Add the tiles remaining in both regions to two separate lists: list A and list B
    std::list<Tile*> listA;
    for (int y = pointA.y; y < (pointA.y + height); ++y) {
        for (int x = pointA.x; x < (pointA.x + width); ++x) {
            listA.push_back(children.first->_tiles[y][x]);
            children.first->_tiles[y][x] = NULL;
        }
    }
    std::list<Tile*> listB;
    for (int y = pointB.y; y < (pointB.y + height); ++y) {
        for (int x = pointB.x; x < (pointB.x + width); ++x) {
            listB.push_back(children.second->_tiles[y][x]);
            children.second->_tiles[y][x] = NULL;
        }
    }

    //Copy to parent A’s region all tiles that in parent B’s region
    //Copy to parent B’s region all tiles that in parent A’s region
    {
        Point<unsigned char> indexA(pointA);
        Point<unsigned char> indexB(pointB);
        for (int y = 0; y < height; ++y) {
            ++indexA.y;
            ++indexB.y;
            for (int x = 0; x < width; ++x) {
                ++indexA.x;
                ++indexB.x;
                //Copy to parent A’s region all tiles that in parent B’s region
                assert(children.first->_tiles[indexA.y][indexA.x] == NULL);
                assert(children.second->_tiles[indexB.y][indexB.x] != NULL);
                children.first->_tiles[indexA.y][indexA.x] = new Tile(*(children.second->_tiles[indexB.y][indexB.x]));
                //Copy to parent B’s region all tiles that in parent A’s region
                assert(children.second->_tiles[indexB.y][indexB.x] == NULL);
                assert(children.first->_tiles[indexA.y][indexA.x] != NULL);
                children.second->_tiles[indexB.y][indexB.x] = new Tile(*(children.first->_tiles[indexA.y][indexA.x]));

            }
        }
    }

    //Fill the empty places in child A and child B using the tiles from list A and list B


    return children;
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
          _tiles[posY + i][posX + j]->setRotation(_tiles[posY + i][posX + j]->getRotation() + 1);
        }
    }
    // Transpose the matrix
    for ( int i = 0; i < size; ++i ) {
      for ( int j = i + 1; j < size; ++j ) {
        Tile* tmp = _tiles[posY + i][posX + j];
        _tiles[posY + i][posX + j] = _tiles[posX + j][posY + i];
        _tiles[posX + j][posY + i] = tmp;
      }
    }
    // Swap the columns
    for ( int i = 0; i < size; ++i ) {
      for ( int j = 0; j < size/2; ++j ) {
         Tile* tmp = _tiles[posY + i][posX + j];
        _tiles[posY + i][posX + j] = _tiles[posY + i][posY + (size-1-j)];
        _tiles[posY + i][posY + (size-1-j)] = tmp;
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
    Tile *cpy[sizeY][sizeX] = {};

    for ( int i = 0; i < sizeY; ++i ) {
      for ( int j = 0; j < sizeX; ++j ) {
          cpy[i][j] = _tiles[posYa + i][posXa + j];
          _tiles[posYa + i][posXa + j] = _tiles[posYb + i][posXb + j];
        }
    }

    for ( int i = 0; i < sizeY; ++i ) {
      for ( int j = 0; j < sizeX; ++j ) {
          _tiles[posYb + i][posXb + j] = cpy[i][j];
        }
    }

}

void Board::rotateRegionMutation(int posX, int posY,int size)
{
    rotateSquare(posX, posY, size);
}


int Board::evaluateFitness()
{
    const Tile* topCell;
    const Tile* rightCell;
    const Tile* downCell;
    const Tile* leftCell;
    const Tile* currentCell;

    _fitness = 0;
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            //Get neighbor cells
            currentCell = _tiles[y][x];
            topCell = (y == 0) ? NULL : _tiles[y - 1][x];
            rightCell = (x == 15) ? NULL : _tiles[y][x + 1];
            downCell = (y == 15) ? NULL : _tiles[y + 1][x];
            leftCell = (x == 0) ? NULL : _tiles[y][x - 1];
            //Check for matching edges
            if (currentCell->getTop() == Tile::EDGE_VALUE && topCell == NULL)
                _fitness += 2;
            else if (currentCell->getTop() != Tile::EDGE_VALUE && topCell != NULL && currentCell->getTop() == topCell->getDown())
                _fitness += 1;
            if (currentCell->getRight() == Tile::EDGE_VALUE && rightCell == NULL)
                _fitness += 2;
            else if (currentCell->getRight() != Tile::EDGE_VALUE && rightCell != NULL && currentCell->getRight() == rightCell->getLeft())
                _fitness += 1;
            if (currentCell->getDown() == Tile::EDGE_VALUE && downCell == NULL)
                _fitness += 2;
            else if (currentCell->getDown() != Tile::EDGE_VALUE && downCell != NULL && currentCell->getDown() == downCell->getTop())
                _fitness += 1;
            if (currentCell->getLeft() == Tile::EDGE_VALUE && leftCell == NULL)
                _fitness += 2;
            else if (currentCell->getLeft() != Tile::EDGE_VALUE && leftCell != NULL && currentCell->getLeft() == leftCell->getRight())
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
                    if (_tiles[boardLine][boardRaw]->getTop() == 0){
                        if (boardLine == 0){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{
                        if (boardLine != 0 && _tiles[boardLine][boardRaw]->getTop() == _tiles[boardLine - 1][boardRaw]->getDown()){
                            os << grn;
                        }else{
                            os << red;
                        }
                    }
                    os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw]->getTop();
                    os << bgr << "  ";
                }else if(cellLine == 1){
                    if (_tiles[boardLine][boardRaw]->getLeft() == 0){
                        if (boardRaw == 0){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{
                        if (boardRaw != 0 && _tiles[boardLine][boardRaw]->getLeft() == _tiles[boardLine][boardRaw - 1]->getRight()){
                            os << grn;
                        }else{
                            os << red;
                        }
                    }
                    os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw]->getLeft();
                    os << bgr << "  ";
                    if (_tiles[boardLine][boardRaw]->getRight() == 0){
                        if (boardRaw == 15){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{
                        if (boardRaw < 15 && _tiles[boardLine][boardRaw]->getRight() == _tiles[boardLine][boardRaw + 1]->getLeft()){
                            os << grn;
                        }else{
                            os << red;
                        }                    }
                     os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw]->getRight();
                }else{
                    os << bgr << "  ";
                    if (_tiles[boardLine][boardRaw]->getDown() == 0){
                        if (boardLine ==15){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{

                        if (boardLine < 15 && _tiles[boardLine][boardRaw]->getDown() == _tiles[boardLine + 1][boardRaw]->getTop()){
                            os << grn;
                        }else{
                            os << red;
                        }
                   }
                   os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw]->getDown();
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
