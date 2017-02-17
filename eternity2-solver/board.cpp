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

Board::Board(bool empty)
{
    if (empty) {
        _fitness = 0;
        return;
    }

    std::array<TileRef, 196> inners;
    int innersIndex = 0;
    std::array<TileRef, 56> borders;
    int bordersIndex = 0;
    std::array<TileRef, 4> corners;
    int cornersIndex = 0;

    // Sort tiles in borders, conners and inners arrays
    for(const TileData& tileData: E2TILES.corners){
        corners[cornersIndex++] = tileData;
    }
    for(const TileData& tileData: E2TILES.borders){
        borders[bordersIndex++] = tileData;
    }
    for(const TileData& tileData: E2TILES.inners){
        inners[innersIndex++] = tileData;
    }
    // Shuffles tile arrays
    std::shuffle(corners.begin(), corners.end(), std::default_random_engine(rand()));
    std::shuffle(borders.begin(), borders.end(), std::default_random_engine(rand()));
    std::shuffle(inners.begin(), inners.end(), std::default_random_engine(rand()));

    // Place tiles on the board
    innersIndex = 0;
    cornersIndex = 0;
    bordersIndex = 0;
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {

            // Place corner tile
            if ((x == 0 && y == 0) || (x == 0 && y == 15) || (x == 15 && y == 0) || (x == 15 && y == 15)) {
                if (x == 15 && y == 0)
                    corners[cornersIndex].setRotation(0);
                else if (x == 15 && y == 15)
                    corners[cornersIndex].setRotation(1);
                else if (x == 0 && y == 15)
                    corners[cornersIndex].setRotation(2);
                else if (x == 0 && y == 0)
                    corners[cornersIndex].setRotation(3);
                _tiles[y][x] = corners[cornersIndex++];
            }

            // Place edge tile
            else if (x == 0 || x == 15 || y == 0 || y == 15) {
                if (y == 0)
                    borders[bordersIndex].setRotation(0);
                else if (x == 15)
                    borders[bordersIndex].setRotation(1);
                else if (y == 15)
                    borders[bordersIndex].setRotation(2);
                else if (x == 0)
                    borders[bordersIndex].setRotation(3);
                _tiles[y][x] = borders[bordersIndex++];
            }

            // Place inner tile
            else {
                _tiles[y][x] = inners[innersIndex++];
            }
        }
    }

    // Calculate board fitness
    evaluate();
}

Board::Board(const Board& other)
{
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            _tiles[y][x] = other._tiles[y][x];
        }
    }
    _fitness = other._fitness;
}

Board::~Board()
{
}

float Board::getFitness() const
{
    return _fitness;
}

std::pair<Board*, Board*> Board::regionExchangeCrossover(const Board& parentA, const Board& parentB)
{
    // Select a random region (two points and a random width and height)
    // Exculde edges
    const unsigned char width = 1 + rand() % 14;
    const unsigned char height = 1 + rand() % 14;
    const Point<unsigned char> pointA(1 + rand() % (14 - (width - 1)),
                                      1 + rand() % (14 - (height - 1)));
    const Point<unsigned char> pointB(1 + rand() % (14 - (width - 1)),
                                      1 + rand() % (14 - (height - 1)));

    //Clone the two parents
    std::pair<Board*, Board*> children(new Board(parentA), new Board(parentB));

    //Remove from child A all tiles that are inside the region in parent B
    for (int yB = pointB.y; yB < (pointB.y + height); ++yB) {
        for (int xB = pointB.x; xB < (pointB.x + width); ++xB) {
            bool removed = false;
            for (int yA = 0; removed == false && (yA < 16); ++yA) {
                for (int xA = 0; removed == false && (xA < 16); ++xA) {
                    if (children.first->_tiles[yA][xA] != TileRef::empty
                        && (children.first->_tiles[yA][xA] == parentB._tiles[yB][xB])) {
                        children.first->_tiles[yA][xA] = TileRef::empty;
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
                    if (children.second->_tiles[yB][xB] != TileRef::empty
                        && (children.second->_tiles[yB][xB] == parentA._tiles[yA][xA])) {
                        children.second->_tiles[yB][xB] = TileRef::empty;
                        removed = true;
                    }
                }
            }
        }
    }

    //Add the tiles remaining in both regions to two separate lists: list A and list B

    //Add the tiles remaining in child A's region to list A
    std::list<TileRef> listA;
    for (int y = pointA.y; y < (pointA.y + height); ++y) {
        for (int x = pointA.x; x < (pointA.x + width); ++x) {
            if (children.first->_tiles[y][x] != TileRef::empty) {
                listA.push_back(children.first->_tiles[y][x]);
                children.first->_tiles[y][x] = TileRef::empty;
            }
        }
    }

    //Add the tiles remaining in child B's region to list B
    std::list<TileRef> listB;
    for (int y = pointB.y; y < (pointB.y + height); ++y) {
        for (int x = pointB.x; x < (pointB.x + width); ++x) {
            if (children.second->_tiles[y][x] != TileRef::empty) {
                listB.push_back(children.second->_tiles[y][x]);
                children.second->_tiles[y][x]= TileRef::empty;
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
                assert(children.first->_tiles[indexA.y][indexA.x] == TileRef::empty);
                children.first->_tiles[indexA.y][indexA.x] = parentB._tiles[indexB.y][indexB.x];
                //Copy to child B’s region all tiles that in parent A’s region
                assert(children.second->_tiles[indexB.y][indexB.x] == TileRef::empty);
                children.second->_tiles[indexB.y][indexB.x] = parentA._tiles[indexA.y][indexA.x];
            }
        }
    }

    //Fill the empty places in child A with the tiles from list A
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            if (children.first->_tiles[y][x] == TileRef::empty) {
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
            if (children.second->_tiles[y][x] == TileRef::empty) {
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

void Board::simpleOuterMutation()
{
    int posA = 1 + (std::rand() % 14);
    int posB = 1 + (std::rand() % 14);
    int colA = std::rand() % 4;
    int colB = std::rand() % 4;

    TileRef* tmpa;
    TileRef* tmpb;
    switch (colA){
        case 0: tmpa = &_tiles[0][posA]; break;
        case 1: tmpa = &_tiles[15][posA]; break;
        case 2: tmpa = &_tiles[posA][0]; break;
        case 3: tmpa = &_tiles[posA][15]; break;
        default: assert(false);
    }

    switch (colB){
        case 0: tmpb = &_tiles[0][posB]; break;
        case 1: tmpb = &_tiles[15][posB]; break;
        case 2: tmpb = &_tiles[posB][0]; break;
        case 3: tmpb = &_tiles[posB][15]; break;
        default: assert(false);
    }
    int orientation = tmpa->getRotation();
    tmpa->setRotation(tmpb->getRotation());
    tmpb->setRotation(orientation);
    TileRef tmp;
    tmp = *tmpa;
    *tmpa = *tmpb;
    *tmpb = tmp;
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
          _tiles[posY + i][posX + j].setRotation(_tiles[posY + i][posX + j].getRotation() + 1);
        }
    }
    // Transpose the matrix
    for ( int i = 0; i < size; ++i ) {
      for ( int j = i + 1; j < size; ++j ) {
        TileRef tmp = _tiles[posY + i][posX + j];
        _tiles[posY + i][posX + j] = _tiles[posY + j][posX + i];
        _tiles[posY + j][posX + i] = tmp;
      }
    }
    // Swap the columns
    for ( int i = 0; i < size; ++i ) {
      for ( int j = 0; j < size/2; ++j ) {
         TileRef tmp = _tiles[posY + i][posX + j];
        _tiles[posY + i][posX + j] = _tiles[posY + i][posX + (size-1-j)];
        _tiles[posY + i][posX + (size-1-j)] = tmp;
      }
    }
    return true;
}

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


    TileRef cpy;
    for ( int i = 0; i < sizeY; ++i ) {
      for ( int j = 0; j < sizeX; ++j ) {
          cpy = _tiles[posYb + i][posXb + j];
          _tiles[posYb + i][posXb + j] = _tiles[posYa + i][posXa + j];
          _tiles[posYa + i][posXa + j] = cpy;
        }
    }
    return true;
}

void Board::rotateInnerRegionMutation()
{
    int x = 1 + (std::rand() % 14);
    int y = 1 + (std::rand() % 14);
    int size;
    if (x > y)
        size = std::rand() % (15 - x);
    else
        size = std::rand() % (15 - y);
    rotateSquare(x, y, size);
}

void Board::swapInnerRegionMutation()
{
    int xa;
    int ya;
    int xb;
    int yb;
    int sizeX = 0;
    int sizeY = 0;
    do{
        xa = 1 + (std::rand() % 14);
        ya = 1 + (std::rand() % 14);
        xb = 1 + (std::rand() % 14);
        yb = 1 + (std::rand() % 14);
    }while (xa == xb && ya == yb);
    int maxSizeX = std::min(std::max(1, std::abs(xa - xb)), 15 - std::max(xa, xb));
    int maxSizeY = std::min(std::max(1, std::abs(ya - yb)), 15 - std::max(ya, yb));

    sizeX = 1 + (std::rand() % maxSizeX);
    sizeY = 1 + (std::rand() % maxSizeY);

    if (swapRectangle(xa, ya, xb, yb, sizeX, sizeY) ==false)
        exit(0);
}

void Board::swapAndRotateInnerRegionMutation(){
    int xa;
    int ya;
    int xb;
    int yb;
    do{
        xa = 1 + (std::rand() % 14);
        ya = 1 + (std::rand() % 14);
        xb = 1 + (std::rand() % 14);
        yb = 1 + (std::rand() % 14);
    }while (xa == xb && ya == yb);
    swapRectangle(xa, ya, xb, yb, 1, 1);
    int rotate = std::rand() % 4;
    while (rotate--){
        rotateSquare(xa,ya,1);
    }
    rotate = std::rand() % 4;
    while (rotate--){
        rotateSquare(xb,yb,1);
    }
}
//to check...
void Board::rawAndColumnInversionInnerRegionMutation()
{
    int direction = std::rand() % 2;
    int x;
    int y;
    TileRef* tmpa;
    TileRef* tmpb;

    if (direction == 0)
    {
        x = 1 + (std::rand() % 14);
        y = 1 + (std::rand() % 13);
        tmpa = &(_tiles[y][x]);
        tmpb = &(_tiles[y + 1][x]);
    }else{
        x = 1 + (std::rand() % 13);
        y = 1 + (std::rand() % 14);
        tmpa = &(_tiles[y][x]);
        tmpb = &(_tiles[y][x + 1]);
    }
    TileRef tmp;
    tmp = *tmpa;
    *tmpa = *tmpb;
    *tmpb = tmp;
}
//to check...
void Board::inversionInnerRegionMutation()
{
    int x = 1 + (std::rand() % 13);
    int y = 1 + (std::rand() % 13);
    TileRef* ta = &(_tiles[y][x]);
    TileRef* tb = &(_tiles[y][x + 1]);
    TileRef* tc = &(_tiles[y + 1][x]);
    TileRef* td = &(_tiles[y + 1][x + 1]);

    TileRef tmpa = *ta;
    TileRef tmpb = *tb;
    *ta = *td;
    *tb = *tc;
    *tc = tmpb;
    *td = tmpa;
 }

float Board::evaluate()
{
    TileRef topCell;
    TileRef rightCell;
    TileRef downCell;
    TileRef leftCell;
    TileRef currentCell;

    _fitness = 0;
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            //Get neighbor cells
            currentCell = _tiles[y][x];

            if (currentCell == TileRef::empty)
                continue;
            topCell = (y == 0) ? TileRef::empty : _tiles[y - 1][x];
            rightCell = (x == 15) ? TileRef::empty : _tiles[y][x + 1];
            downCell = (y == 15) ? TileRef::empty : _tiles[y + 1][x];
            leftCell = (x == 0) ? TileRef::empty : _tiles[y][x - 1];
            //Check for matching edges
            float xFactor = ((x < 8) ? (8 - x) : (x - 7));
            float yFactor = ((y < 8) ? (8 - y) : (y - 7));
            // evaluation less aggressive
            float factor = std::log(xFactor + yFactor);

            // evaluation more aggressive
            //float factor = xFactor + yFactor;

            if (currentCell.getTop() == TileRef::EDGE_VALUE && topCell == TileRef::empty)
                _fitness += factor * 2;
            else if (currentCell.getTop() != TileRef::EDGE_VALUE && topCell != TileRef::empty && currentCell.getTop() == topCell.getDown())
                _fitness += factor;
            if (currentCell.getRight() == TileRef::EDGE_VALUE && rightCell == TileRef::empty)
                _fitness += factor * 2;
            else if (currentCell.getRight() != TileRef::EDGE_VALUE && rightCell != TileRef::empty && currentCell.getRight() == rightCell.getLeft())
                _fitness += factor;
            if (currentCell.getDown() == TileRef::EDGE_VALUE && downCell == TileRef::empty)
                _fitness += factor * 2;
            else if (currentCell.getDown() != TileRef::EDGE_VALUE && downCell != TileRef::empty && currentCell.getDown() == downCell.getTop())
                _fitness += factor;
            if (currentCell.getLeft() == TileRef::EDGE_VALUE && leftCell == TileRef::empty)
                _fitness += factor * 2;
            else if (currentCell.getLeft() != TileRef::EDGE_VALUE && leftCell != TileRef::empty && currentCell.getLeft() == leftCell.getRight())
                _fitness += factor;
        }
    }
    return _fitness;
}



bool Board::isValid() {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            //check if _tiles[y][x] is unique
            if (_tiles[y][x] == TileRef::empty)
                return false;
            for (int yp = 0; yp < 16; ++yp) {
                for (int xp = 0; xp < 16; ++xp) {
                    if (yp != y && xp != x) {
                        if (_tiles[yp][xp] == TileRef::empty)
                            return false;
                        if (_tiles[y][x] == _tiles[yp][xp])
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
                if (_tiles[boardLine][boardRaw] == TileRef::empty){
                    os << "\e[41m";
                    os << "  " << "  " << "  ";
                    os << rst;
                }else{
                if (cellLine == 0){
                    os << bgr << "  ";
                    if (_tiles[boardLine][boardRaw].getTop() == 0){
                        if (boardLine == 0){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{
                        if (_tiles[boardLine - 1][boardRaw] != TileRef::empty && boardLine != 0 && _tiles[boardLine][boardRaw].getTop() == _tiles[boardLine - 1][boardRaw].getDown()){
                            os << grn;
                        }else{
                            os << red;
                        }
                    }
                    os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw].getTop();
                    os << bgr << "  ";
                }else if(cellLine == 1){
                    if (_tiles[boardLine][boardRaw].getLeft() == 0){
                        if (boardRaw == 0){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{
                        if (_tiles[boardLine][boardRaw - 1] != TileRef::empty && boardRaw != 0 && _tiles[boardLine][boardRaw].getLeft() == _tiles[boardLine][boardRaw - 1].getRight()){
                            os << grn;
                        }else{
                            os << red;
                        }
                    }
                    os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw].getLeft();
                    os << bgr << "  ";
                    if (_tiles[boardLine][boardRaw].getRight() == 0){
                        if (boardRaw == 15){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{
                        if (_tiles[boardLine][boardRaw + 1] != TileRef::empty && boardRaw < 15 && _tiles[boardLine][boardRaw].getRight() == _tiles[boardLine][boardRaw + 1].getLeft()){
                            os << grn;
                        }else{
                            os << red;
                        }                    }
                     os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw].getRight();
                }else{
                    os << bgr << "  ";
                    if (_tiles[boardLine][boardRaw].getDown() == 0){
                        if (boardLine ==15){
                            //match
                            os << grn;
                        }else{
                            os << blu;
                        }
                    }else{

                        if (_tiles[boardLine + 1][boardRaw] != TileRef::empty && boardLine < 15 && _tiles[boardLine][boardRaw].getDown() == _tiles[boardLine + 1][boardRaw].getTop()){
                            os << grn;
                        }else{
                            os << red;
                        }
                   }
                   os << std::fixed << std::setw( 2 ) << std::setfill('0') << (int)_tiles[boardLine][boardRaw].getDown();
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

void Board::unitTestSwap()
{
    ///*
    Board tmp;
    //wrong position
    assert(tmp.swapRectangle(-1,0,0,0,0,0) == false);
    assert(tmp.swapRectangle(0,-1,0,0,0,0) == false);
    assert(tmp.swapRectangle(0,0,-1,0,0,0) == false);
    assert(tmp.swapRectangle(0,0,0,-1,0,0) == false);
    //wrong size + overlap
    assert(tmp.swapRectangle(0,0,0,0,0,0) == false);
    assert(tmp.swapRectangle(0,0,0,0,0,1) == false);
    assert(tmp.swapRectangle(0,0,0,0,1,0) == false);
    //rectangle same pos
    assert(tmp.swapRectangle(0,0,0,0,1,1) == false);

    //should work [easy]
    assert(tmp.swapRectangle(1,0,0,0,1,1) == true);
    assert(tmp.swapRectangle(0,1,0,0,1,1) == true);
    assert(tmp.swapRectangle(0,0,1,0,1,1) == true);
    assert(tmp.swapRectangle(0,0,0,1,1,1) == true);
    assert(tmp.swapRectangle(0,0,1,1,1,1) == true);
    assert(tmp.swapRectangle(1,1,0,0,1,1) == true);

    // around One test
    assert(tmp.swapRectangle(1,1,0,0,1,1) == true);
    assert(tmp.swapRectangle(1,1,0,1,1,1) == true);
    assert(tmp.swapRectangle(1,1,0,2,1,1) == true);

    assert(tmp.swapRectangle(1,1,1,0,1,1) == true);
    assert(tmp.swapRectangle(1,1,1,1,1,1) == false);
    assert(tmp.swapRectangle(1,1,1,2,1,1) == true);

    assert(tmp.swapRectangle(1,1,2,0,1,1) == true);
    assert(tmp.swapRectangle(1,1,2,1,1,1) == true);
    assert(tmp.swapRectangle(1,1,2,2,1,1) == true);
    //should overlap [medium]
    // sqr shape
    assert(tmp.swapRectangle(0,0,0,1,2,2) == false);
    assert(tmp.swapRectangle(0,0,1,0,2,2) == false);
    assert(tmp.swapRectangle(0,1,0,0,2,2) == false);
    assert(tmp.swapRectangle(1,0,0,0,2,2) == false);
    // rect shape
    assert(tmp.swapRectangle(0,0,0,1,1,2) == false);
    assert(tmp.swapRectangle(0,0,1,0,2,1) == false);
    assert(tmp.swapRectangle(0,1,0,0,1,2) == false);
    assert(tmp.swapRectangle(1,0,0,0,2,1) == false);
    // random
    assert(tmp.swapRectangle(8,12,9,13,1,1) == true);
    assert(tmp.swapRectangle(4,7,1,4,11,6) == false);
    //*/

}

void Board::swapAndRotateAngleMutation() {
    int size = 1 + (std::rand() % 7);
    int ca = std::rand() % 4;
    int cb = std::rand() % 4;

    while(ca == cb) {
        cb = std::rand() % 4;
    }
    int xa, xb,ya,yb;
    switch(ca){
        case 0:xa = 0; ya = 0; break;
        case 3:xa = 16 - size; ya =  0; break;
        case 2:xa = 16 - size; ya = 16 - size; break;
        case 1:xa = 0; ya = 16 - size; break;
    default: assert(false);
    }
    switch(cb){
        case 0:xb = 0; yb = 0; break;
        case 3:xb = 16 - size; yb = 0; break;
        case 2:xb = 16 - size; yb = 16 - size; break;
        case 1:xb = 0; yb = 16 - size; break;
    default: assert(false);
    }
    swapRectangle(xa, ya, xb, yb, size, size);
    for (int i = ca; (i % 4) != cb; ++i) {
        rotateSquare(xa, ya, size);
    }
    for (int i = cb; (i % 4) != ca; ++i) {
        rotateSquare(xb, yb, size);
    }
}

#include <omp.h>
void Board::getSolvedEdgesBoards() {

    //Create Board 0, the reference board out of six boards
    //Clear board moving corner and border tiles in respective arrays
    std::vector<Board*> refBoards(6);
    std::array<TileRef, 56> borderTiles;
    std::array<TileRef, 4> cornerTiles;
    {
        unsigned int borderTilesIndex = 0;
        unsigned int cornerTilesIndex = 0;
        refBoards[0] = new Board();
        refBoards[0]->_fitness = 0;
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) {

                //Delete inner tile
                if ((x > 0 && x < 15) && (y > 0 && y < 15)) {
                    if (refBoards[0]->_tiles[y][x] != TileRef::empty) {
                        refBoards[0]->_tiles[y][x] = TileRef::empty;
                    }
                }

                //Move corner tile to list
                else if ((x == 0 && y == 0) || (x == 15 && y == 0) || (x == 0 && y == 15) || (x == 15 && y == 15)) {
                    refBoards[0]->_tiles[y][x].setRotation(0);
                    cornerTiles[cornerTilesIndex++] = refBoards[0]->_tiles[y][x];
                    refBoards[0]->_tiles[y][x] = TileRef::empty;
                }

                //Move border tile to list
                else {
                    refBoards[0]->_tiles[y][x].setRotation(0);
                    borderTiles[borderTilesIndex++] = refBoards[0]->_tiles[y][x];
                    refBoards[0]->_tiles[y][x] = TileRef::empty;
                }
            }
        }
    }

    //Create the 6 boards with all corner combinations
    {
        for (int i = 1; i < 6; ++i) {
            refBoards[i] = new Board(true);
        }
        //Board0
        refBoards[0]->_tiles[0][15] = cornerTiles[0];
        refBoards[0]->_tiles[15][15] = cornerTiles[1];
        refBoards[0]->_tiles[15][0] = cornerTiles[2];
        refBoards[0]->_tiles[0][0] = cornerTiles[3];
        //Board1
        refBoards[1]->_tiles[0][15] = cornerTiles[0];
        refBoards[1]->_tiles[15][15] = cornerTiles[2];
        refBoards[1]->_tiles[15][0] = cornerTiles[1];
        refBoards[1]->_tiles[0][0] = cornerTiles[3];
        //Board2
        refBoards[2]->_tiles[0][15] = cornerTiles[0];
        refBoards[2]->_tiles[15][15] = cornerTiles[1];
        refBoards[2]->_tiles[15][0] = cornerTiles[3];
        refBoards[2]->_tiles[0][0] = cornerTiles[2];
        //Board3
        refBoards[3]->_tiles[0][15] = cornerTiles[0];
        refBoards[3]->_tiles[15][15] = cornerTiles[2];
        refBoards[3]->_tiles[15][0] = cornerTiles[3];
        refBoards[3]->_tiles[0][0] = cornerTiles[1];
        //Board4
        refBoards[4]->_tiles[0][15] = cornerTiles[0];
        refBoards[4]->_tiles[15][15] = cornerTiles[3];
        refBoards[4]->_tiles[15][0] = cornerTiles[1];
        refBoards[4]->_tiles[0][0] = cornerTiles[2];
        //Board5
        refBoards[5]->_tiles[0][15] = cornerTiles[0];
        refBoards[5]->_tiles[15][15] = cornerTiles[3];
        refBoards[5]->_tiles[15][0] = cornerTiles[2];
        refBoards[5]->_tiles[0][0] = cornerTiles[1];
    }

    //Solve refBoards into all possible edges solutions
    std::list<Board*> solvedEdgesBoards;
//#pragma omp parallel for num_threads(6) schedule(static)
    for (int i = 0; i < 6; ++i) {
        std::list<Board*> solvedEdgesBoardsForBoard;
        getSolvedEdgesForBoard(refBoards[i], borderTiles, solvedEdgesBoardsForBoard);
//#pragma omp critical
        solvedEdgesBoards.splice(solvedEdgesBoards.end(), solvedEdgesBoardsForBoard);
    }

    //Clear data
    for (Board* board : refBoards)
        delete board;

    std::cout << "Done" << std::endl;
}

float Board::cmp(const Board& other) const
{
    float res = 0;
    for (int j = 0; j < 16; ++j) {
        for (int i = 0; i < 16; ++i) {
            if (_tiles[j][i] != other._tiles[j][i]){
                ++res;
            }
        }
    }
    res /= 256;
    return res;
}

void Board::swapChunkOuterMutation() {
    return;
    int size = 1 + (std::rand() % 14);
    int ca = std::rand() % 4;
    int pa = std::rand() % (16 - size);
    int cb = std::rand() % 4;
    int pb = std::rand() % (16 - size);

    while (ca == cb) {
        cb == std::rand() % 4;
    }
    for (int i = ca; (i % 4) != cb; ++i) {

    }
    for (int i = cb; (i % 4) != ca; ++i) {

    }

}



void Board::getSolvedEdgesForBoard(Board* refBoard, std::array<TileRef, 56>& borderTiles, std::list<Board*>& solvedEdgesBoardsForBoard) {
    /*
    unsigned char nextValue = refBoard->_tiles[0][0]->getRight();
    std::list<TileRef*> candidateTiles;

    std::cout << (int)nextValue << std::endl;
    for (TileRef* borderTile : borderTiles) {
        std::cout << (int)borderTile->getLeft() << std::endl;
    }
*/
}
