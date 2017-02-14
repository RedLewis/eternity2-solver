#include "board.h"
#include "e2tiles.h"
#include <array>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>

#include <climits>
#include <assert.h>
#include <list>
#include <cassert>
#include "tile.h"

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

    evaluate();
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

int Board::getFitness() const
{
    return _fitness;
}

std::pair<Board*, Board*> Board::regionExchangeCrossover(const Board& parentA, const Board& parentB)
{

    // Select a random region (two points and a random width and height)
    /*const unsigned char width = 1 + rand() % 16;
    const unsigned char height = 1 + rand() % 16;
    const Point<unsigned char> pointA(rand() % (16 - (width - 1)),
                                rand() % (16 - (height - 1)));
    const Point<unsigned char> pointB(rand() % (16 - (width - 1)),
                                rand() % (16 - (height - 1)));*/

    const unsigned char width = 2;
    const unsigned char height = 2;
    const Point<unsigned char> pointA(0, 0);
    const Point<unsigned char> pointB(0, 0);

    //Clone the two parents
    std::pair<Board*, Board*> children(new Board(parentA), new Board(parentB));

    //Remove from child A all tiles that are inside the region in parent B
    for (int yB = pointB.y; yB < (pointB.y + height); ++yB) {
        for (int xB = pointB.x; xB < (pointB.x + width); ++xB) {
            bool removed = false;
            for (int yA = 0; removed == false && (yA < 16); ++yA) {
                for (int xA = 0; removed == false && (xA < 16); ++xA) {
                    if (children.first->_tiles[yA][xA] != NULL
                        && (*(children.first->_tiles[yA][xA]) == *(parentB._tiles[yB][xB]))) {
                        delete children.first->_tiles[yA][xA];
                        children.first->_tiles[yA][xA] = NULL;
                        removed = true;
                    }
                }
            }
        }
    }

    //Remove from child B all tiles that are inside the region in parent A
    for (int yA = pointA.y; yA < (pointA.y + height); ++yA) {
        for (int xA = pointA.x; xA < (pointA.x + width); ++xA) {
            bool removed = false;
            for (int yB = 0; removed == false && (yB < 16); ++yB) {
                for (int xB = 0; removed == false && (xB < 16); ++xB) {
                    if (children.second->_tiles[yB][xB] != NULL
                        && (*(children.second->_tiles[yB][xB]) == *(parentA._tiles[yA][xA]))) {
                        delete children.second->_tiles[yB][xB];
                        children.second->_tiles[yB][xB] = NULL;
                        removed = true;
                    }
                }
            }
        }
    }

    //Add the tiles remaining in both regions to two separate lists: list A and list B

    //Add the tiles remaining in child A's region to list A
    std::list<Tile*> listA;
    for (int y = pointA.y; y < (pointA.y + height); ++y) {
        for (int x = pointA.x; x < (pointA.x + width); ++x) {
            if (children.first->_tiles[y][x] != NULL) {
                listA.push_back(children.first->_tiles[y][x]);
                children.first->_tiles[y][x] = NULL;
            }
        }
    }

    //Add the tiles remaining in child B's region to list B
    std::list<Tile*> listB;
    for (int y = pointB.y; y < (pointB.y + height); ++y) {
        for (int x = pointB.x; x < (pointB.x + width); ++x) {
            if (children.second->_tiles[y][x] != NULL) {
                listB.push_back(children.second->_tiles[y][x]);
                children.second->_tiles[y][x] = NULL;
            }
        }
    }

    //Copy to child A’s region all tiles that in parent B’s region
    //Copy to child B’s region all tiles that in parent A’s region
    {
        Point<unsigned char> indexA;
        Point<unsigned char> indexB;
        for (int y = 0; y < height; ++y) {
            indexA.y = pointA.y + y;
            indexB.y = pointB.y + y;
            for (int x = 0; x < width; ++x) {
                indexA.x = pointA.x + x;
                indexB.x = pointB.x + x;
                //Copy to child A’s region all tiles that in parent B’s region
                assert(children.first->_tiles[indexA.y][indexA.x] == NULL);
                children.first->_tiles[indexA.y][indexA.x] = new Tile(*(parentB._tiles[indexB.y][indexB.x]));
                //Copy to child B’s region all tiles that in parent A’s region
                assert(children.second->_tiles[indexB.y][indexB.x] == NULL);
                children.second->_tiles[indexB.y][indexB.x] = new Tile(*(parentA._tiles[indexA.y][indexA.x]));
            }
        }
    }

    //Fill the empty places in child A with the tiles from list A
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            if (children.first->_tiles[y][x] == NULL) {
                assert(listA.size() > 0);
                children.first->_tiles[y][x] = listA.front();
                listA.pop_front();
            }
        }
    }
    assert(listA.size() == 0);

    //Fill the empty places in child B with the tiles from list B
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            if (children.second->_tiles[y][x] == NULL) {
                assert(listB.size() > 0);
                children.second->_tiles[y][x] = listB.front();
                listB.pop_front();
            }
        }
    }
    assert(listB.size() == 0);

    children.first->evaluate();
    children.second->evaluate();
    return children;
}

bool Board::rotateSquare(int posX, int posY,int size)
{
    if ((posX < 0 || posX >= 16) ||
        (posY < 0 || posY >= 16) ||
        (size < 0 || size > 16) ||
        (posX + size > 16) ||
        (posY + size > 16)){
        std::cerr << "rotate mutation parameter invalid" << std::endl;
        return false;
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
    return true;
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
bool Board::swapRectangle(int posXa, int posYa,int posXb, int posYb, int sizeX, int sizeY)
{
    if ((posXa < 0 || posXa >= 16) ||
        (posYa < 0 || posYa >= 16) ||
        (posXb < 0 || posXb >= 16) ||
        (posYb < 0 || posYb >= 16))
    {
        std::cerr << "pX:" << posXa << " pY:" << posYa << std::endl;
        std::cerr << "pX:" << posXb << " pY:" << posYb << std::endl;
        std::cerr << "sX:" << sizeX << " sY:" << sizeY << std::endl;
        std::cerr << "a point is not in the board" << std::endl;
        return false;
    }
    if ((sizeX < 1 || sizeX > 16) ||
        (sizeY < 1 || sizeY > 16))
    {
        std::cerr << "pX:" << posXa << " pY:" << posYa << std::endl;
        std::cerr << "pX:" << posXb << " pY:" << posYb << std::endl;
        std::cerr << "sX:" << sizeX << " sY:" << sizeY << std::endl;
        std::cerr << "size is invalid" << std::endl;
        return false;
    }
    if((posXa + sizeX > 16) ||
        (posYa + sizeY > 16) ||
        (posXb + sizeX > 16) ||
        (posYb + sizeY > 16)){
        std::cerr << "pX:" << posXa << " pY:" << posYa << std::endl;
        std::cerr << "pX:" << posXb << " pY:" << posYb << std::endl;
        std::cerr << "sX:" << sizeX << " sY:" << sizeY << std::endl;
        std::cerr << "swap mutation parameter invalid" << std::endl;
        return false;
    }
    if (posXa == posXb && posYa == posYb)
    {
        std::cerr << "pX:" << posXa << " pY:" << posYa << std::endl;
        std::cerr << "pX:" << posXb << " pY:" << posYb << std::endl;
        std::cerr << "sX:" << sizeX << " sY:" << sizeY << std::endl;
        std::cerr << "rectangles same pos overlap" << std::endl;
        return false;
    }

    if (
            ((posXa < posXb && posXa + sizeX > posXb) && (posYa < posYb && posYa + sizeY >= posYb)) ||
            ((posXa < posXb + sizeX && posXa + sizeX >= posXb + sizeX) && (posYa < posYb + sizeY && posYa + sizeY >= posYb + sizeY)) ||
            ((posXb < posXa && posXb + sizeX > posXa) && (posYb < posYa && posYb + sizeY >= posYa)) ||
            ((posXb < posXa + sizeX && posXb + sizeX >= posXa + sizeX) && (posYb < posYa + sizeY && posYb + sizeY >= posYa + sizeY))
)
    {
        std::cerr << "pX:" << posXa << " pY:" << posYa << std::endl;
        std::cerr << "pX:" << posXb << " pY:" << posYb << std::endl;
        std::cerr << "sX:" << sizeX << " sY:" << sizeY << std::endl;

        std::cerr << "rectangles overlap" << std::endl;
        return false;
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
    return true;
}

void Board::rotateRegionMutation()
{
    int x = std::rand() % 16;
    int y = std::rand() % 16;
    //todo
    int size;
    if (x > y)
        size = std::rand() % (16 - x);
    else
        size = std::rand() % (16 - y);
    rotateSquare(x, y, size);
}

void Board::swapRegionMutation()
{
    int xa;
    int ya;
    int xb;
    int yb;
    int sizeX = 0;
    int sizeY = 0;
    do{
        xa = std::rand() % 16;
        ya = std::rand() % 16;
        xb = std::rand() % 16;
        yb = std::rand() % 16;
    }while (xa == xb && ya == yb);

    sizeX = 1;
    sizeY = 1;

    if (swapRectangle(xa, ya, xb, yb, sizeX, sizeY) ==false)
        exit(0);
}


int Board::evaluate()
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

bool Board::isValid() {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            //check if _tiles[y][x] is unique
            if (_tiles[y][x] == NULL)
                return false;
            for (int yp = 0; yp < 16; ++yp) {
                for (int xp = 0; xp < 16; ++xp) {
                    if (yp != y && xp != x) {
                        if (_tiles[yp][xp] == NULL)
                            return false;
                        if (*(_tiles[y][x]) == *(_tiles[yp][xp]))
                            return false;
                    }
                }
            }
        }
    }
    return true;
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
                if (_tiles[boardLine][boardRaw] == NULL){
                    os << "\e[41m";
                    os << "  " << "  " << "  ";
                    os << rst;
                }else{
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
                        if (_tiles[boardLine - 1][boardRaw] != NULL && boardLine != 0 && _tiles[boardLine][boardRaw]->getTop() == _tiles[boardLine - 1][boardRaw]->getDown()){
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
                        if (_tiles[boardLine][boardRaw - 1] != NULL && boardRaw != 0 && _tiles[boardLine][boardRaw]->getLeft() == _tiles[boardLine][boardRaw - 1]->getRight()){
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
                        if (_tiles[boardLine][boardRaw + 1] != NULL && boardRaw < 15 && _tiles[boardLine][boardRaw]->getRight() == _tiles[boardLine][boardRaw + 1]->getLeft()){
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

                        if (_tiles[boardLine + 1][boardRaw] != NULL && boardLine < 15 && _tiles[boardLine][boardRaw]->getDown() == _tiles[boardLine + 1][boardRaw]->getTop()){
                            os << grn;
                        }else{
                            os << red;
                        }
                   }
                   os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw]->getDown();
                   os << bgr <<"  ";
                }
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
