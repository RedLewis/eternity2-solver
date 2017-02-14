#ifndef POPULATION_H
#define POPULATION_H

#include "board.h"
#include <list>
#include <algorithm>

class Population
{
    std::list<Board*> _boards;
    unsigned int _generation = 1;
    float _mutationRate;
    float _mutationPotency;

    Board* _best;
    Board* _worst;
    float _averageFitness;
public:
    Population(int size = 100);
    ~Population();
    unsigned int getGeneration()const;
    unsigned int getAverageFitness()const;
    const Board& getBestBoard()const;
    const Board& getWorstBoard()const;

    void stepGeneration();

private:
    void crossover();
    void mutate();
    void evaluate();
private:
};

#endif // POPULATION_H
