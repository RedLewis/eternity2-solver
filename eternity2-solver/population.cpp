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
    unsigned int i = 0;
    std::vector<Board*> newBoards(_boards.size());
    newBoards[i++] = _best;

    auto parentItA = _boards.begin();
    auto parentItB = std::next(_boards.begin());
    std::pair<Board*, Board*> children;

    while (i < _boards.size())
    {
        children = Board::regionExchangeCrossover(**parentItA, **parentItB);
        newBoards[i++] = children.first;
        if (i < _boards.size()) {
            newBoards[i++] = children.second;
        }
        else
            delete children.second;
        ++parentItA;
        ++parentItB;
    }

    i = 1;
    while (i < _boards.size())
        delete _boards[i++];
    _boards = std::move(newBoards);
}

void Population::mutate()
{
//#pragma omp parallel for num_threads(4) schedule(static)
    for (Board* individual : _boards)
    {
        if (individual != _best) {
            individual->rotateInnerRegionMutation();
            individual->swapInnerRegionMutation();
        }
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
