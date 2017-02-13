#include "tile.h"
#include "board.h"
#include <iostream>
#include <list>
#include <time.h>
#include <algorithm>
#include "population.h"
#include <chrono>
int main()
{
    srand(time(NULL));
    int lastBestFitness = 0;
    Population pop(10000);
    while (pop.getBestBoard()->getFitness() != Board::MAX_FITNESS)
    {
        pop.mutate();
        pop.evaluate();
       if (lastBestFitness < pop.getBestBoard()->getFitness())
       {
           std::cout << "Fitness:" << std::endl;
           std::cout << "\tbest:" << pop.getBestBoard()->getFitness() << std::endl;
           std::cout << "\taverage:" << pop.getAverageFitness() << std::endl;
           std::cout << "\tworst:" << pop.getWorstBoard()->getFitness() << std::endl;

           lastBestFitness = pop.getBestBoard()->getFitness();
            std::cout << *pop.getBestBoard() << std::endl;
       }
    }
}
