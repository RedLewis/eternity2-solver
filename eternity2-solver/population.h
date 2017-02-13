#ifndef POPULATION_H
#define POPULATION_H

#include "board.h"
#include <list>
#include <algorithm>

class Population
{
    std::list<Board*> _pop;
    unsigned int _generation;
    unsigned int _mutationRate;
    unsigned int _mutationPotency;

    Board*  _best;
    Board*  _worst;
    unsigned int _averageFitness;
public:
    Population();
    Population(int size);
    ~Population();
    unsigned int getGeneration()const;
    unsigned int getAverageFitness()const;
    void evaluate();
private:
};

#endif // POPULATION_H
