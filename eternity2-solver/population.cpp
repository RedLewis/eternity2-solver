#include "population.h"
#include <iostream>
#include <algorithm>

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

void Population::crossover()
{
    std::vector<Board*> newBoards(_boards.size());
    newBoards[0] = _best;

    auto parentItA = _boards.begin();
    auto parentItB = std::next(_boards.begin());
    std::pair<Board*, Board*> children;

    //Skip best (which is number 0)
    for (unsigned int i = 1; i < _boards.size(); i += 2)
    {
        children = Board::regionExchangeCrossover(**parentItA, **parentItB);
        newBoards[i] = children.first;
        if ((i + 1) < _boards.size()) {
            newBoards[i + 1] = children.second;
        }
        else
            delete children.second;
        ++parentItA;
        ++parentItB;
    }

    //Skip best (which is number 0)
    for (unsigned int i = 1; i < _boards.size(); ++i)
        delete _boards[i];
    _boards = std::move(newBoards);
}

void Population::mutate()
{
    //Skip best (which is number 0)
    for (unsigned int i = 1; i < _boards.size(); ++i) {
        _boards[i]->rotateInnerRegionMutation();
        _boards[i]->swapInnerRegionMutation();
    }
}

void Population::evaluate() {
    for (auto board : _boards)
        board->evaluate();

    std::sort(_boards.begin(), _boards.end(), [](Board*& first, Board*& second){
        return first->getFitness() > second->getFitness();
    });

    _best = _boards.front();
    _worst = _boards.back();

    _averageFitness = 0;
    for (auto board : _boards)
        _averageFitness += board->getFitness();
    _averageFitness /=_boards.size();
}

unsigned int Population::getGeneration()const
{
    return _generation;
}

unsigned int Population::getAverageFitness()const
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

void Population::stepGeneration()
{
    crossover();
    mutate();
    evaluate();
    _generation += 1;
}
