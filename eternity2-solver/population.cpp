#include "population.h"
#include <iostream>
Population::Population(): Population(100)
{}

Population::Population(int size): _pop(size)
{
    std::generate(_pop.begin(), _pop.end(), [](){
        return new Board();
    });
    evaluate();
}

Population::~Population()
{
    _pop.remove_if([](Board* b){
        delete b;
        return true;
    });
}

void Population::mutate()
{
    for (auto individual : _pop)
    {
        if (individual != _best)
            individual->rotateRegionMutation();
    }
}

void Population::evaluate(){
    for (auto individual : _pop)
    {
        individual->evaluate();
    }
    _pop.sort([](Board*& first, Board*& second){
        return first->getFitness() > second->getFitness();
    });
    _best = *_pop.begin();
    _worst = *_pop.end();
    _averageFitness = 0;
    for (auto individual : _pop)
    {
        _averageFitness += individual->getFitness();
    }
    _averageFitness /=_pop.size();
}

unsigned int Population::getGeneration()const
{
    return _generation;
}

unsigned int Population::getAverageFitness()const
{
    return _averageFitness;
}

Board& Population::getBestBoard()const
{
    return *(*_pop.begin());
}

Board& Population::getWorstBoard()const
{
    return *(*std::prev(_pop.end()));
}
void Population::generation()
{
    mutate();
    evaluate();
}
