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
    _pop.remove_if([](Board* pop){
        delete pop;
        return true;
    });
}

void Population::evaluate(){
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
