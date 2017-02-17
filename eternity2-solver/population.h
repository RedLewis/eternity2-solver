#ifndef POPULATION_H
#define POPULATION_H

#include "board.h"
#include <list>
#include <algorithm>

class Population
{
    std::vector<Board*> _boards;
    unsigned int _generation = 1;
    Board* _best = NULL;
    Board* _worst = NULL;
    float _averageFitness;
public:
    Population(int size = 100);
    ~Population();
    unsigned int getGeneration()const;
    unsigned int getAverageFitness()const;
    const Board& getBestBoard()const;
    const Board& getWorstBoard()const;

    void stepGeneration();
    Board* tournamentSelection(int tournamentSize)const;
private:
    void selection();
    void mutate();
    void evaluate();
private:
};

#endif // POPULATION_H
