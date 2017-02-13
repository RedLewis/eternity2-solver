#include "population.h"
#include <iostream>
Population::Population(): Population(100)
{}

Population::Population(int size): _boards(size)
{
    std::generate(_boards.begin(), _boards.end(), [](){
        return new Board();
    });
    evaluate();
}

Population::~Population()
{
    _boards.remove_if([](Board* b){
        delete b;
        return true;
    });
}

void Population::mutate()
{
    for (auto individual : _boards)
    {
        if (individual != _best)
            individual->rotateRegionMutation();
    }
}

void Population::evaluate(){
    for (auto individual : _boards)
    {
        individual->evaluate();
    }
    _boards.sort([](Board*& first, Board*& second){
        return first->getFitness() > second->getFitness();
    });
    _best = *_boards.begin();
    _worst = *_boards.end();
    _averageFitness = 0;
    for (auto individual : _boards)
    {
        _averageFitness += individual->getFitness();
    }
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
    mutate();
    evaluate();
}
