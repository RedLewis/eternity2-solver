#include "population.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Population::Population(int size): _boards(size)
{
    std::generate(_boards.begin(), _boards.end(), [](){
        return new Board();
    });
    evaluate();
}

Population::~Population()
{
    for (Board* board : _boards)
        delete board;
}

Board* Population::tournamentSelection(int tournamentSize)const
{
    Board* best = NULL;
    for (int i = 0; i < tournamentSize; ++i)
    {
        Board* ind = _boards[std::rand() % _boards.size()];
        if (best == NULL || best->getFitness() < ind->getFitness())
        {
            best = ind;
        }
    }
    return best;
}

void Population::selection()
{
    _preserveds.clear();
    std::vector<Board*> newBoards(0);
    newBoards.reserve(_boards.size());


    //Preserve the best
    _preserveds.push_back(_best);
    newBoards.push_back(_best);
    //Preserve individuals different from other bests
    for (Board *board : _boards) {
        if (!(_preserveds.size() < std::roundf(_boards.size() / 10.f)))
            break;
        bool toPreserve = true;
        for (Board* preserved : _preserveds) {
            if (board == preserved ||
                ((float)board->getFitness()/(float)preserved->getFitness()) < 0.90f ||
                preserved->cmp(*board) > 0.10f) {
                toPreserve = false;
                break;
            }
        }
        if (toPreserve) {
            _preserveds.push_back(board);
            newBoards.push_back(board);
        }
    }
    //std::cout << "best saved: " << _preserveds.size() << std::endl;

    std::pair<Board*, Board*> children;
    while(newBoards.size() < _boards.size()){
        Board* parentA = tournamentSelection(2);
        Board* parentB = tournamentSelection(2);
        children = Board::regionExchangeCrossover(*parentA, *parentB);
        newBoards.push_back(children.first);
        if (newBoards.size() < _boards.size()) {
            newBoards.push_back(children.second);
        }
        else
            delete children.second;
    }
    for (unsigned int i = 0; i < _boards.size(); ++i) {
        if (std::find(_preserveds.begin(), _preserveds.end(), _boards[i]) == _preserveds.end()){
            delete _boards[i];
        }
    }
    _boards = std::move(newBoards);
}

void Population::mutate()
{
    for (Board* b : _boards){
            if (std::find(_preserveds.begin(), _preserveds.end(), b) != _preserveds.end()){
                continue;
            }

        switch(std::rand() % 6){
            case 5:b->rotateInnerRegionMutation();break;
            case 4:b->swapInnerRegionMutation();break;
            case 3:b->swapAndRotateInnerRegionMutation();break;
            case 2:b->simpleOuterMutation();break;
            case 1:b->swapChunkOuterMutation();break;
            case 0:b->swapAndRotateAngleMutation();break;
        }
    }
}

void Population::evaluate() {
    _averageFitness = 0;
    _best = NULL;
    _worst = NULL;
    for (Board* board : _boards){
        board->evaluate();
        _averageFitness += board->getFitness();
        if (_best == NULL || _best->getFitness() < board->getFitness()){
        _best = board;
       }
        if (_worst == NULL || _worst->getFitness() > board->getFitness()){
        _worst = board;
       }
    }
    _averageFitness /=_boards.size();
}

unsigned int Population::getGeneration()const
{
    return _generation;
}

float Population::getAverageFitness()const
{
    return _averageFitness;
}

const Board& Population::getBestBoard()const
{
    return *(*_boards.begin());
}

const Board& Population::getWorstBoard()const
{
    return *(*std::prev(_boards.end()));
}

int Population::getPreserved()const
{
    return _preserveds.size();
}


void Population::stepGeneration()
{
    selection();
    mutate();
    evaluate();
    _generation += 1;
}
