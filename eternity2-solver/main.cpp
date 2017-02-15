#include "tile.h"
#include "board.h"
#include <iostream>
#include <list>
#include <time.h>
#include <algorithm>
#include "population.h"
#include <chrono>
#include <assert.h>
#include <iomanip>

int main()
{
    //Board::unitTestSwap();;
    int oldBest = 0;

    srand(time(NULL));
    Population population(10);
    unsigned int m = 0;
    while (population.getBestBoard().getFitness() != Board::MAX_FITNESS && m < 10)
    {
        ++m;
        population.stepGeneration();
        std::cout << std::setw(5) << std::left << "Gen:"
                  << std::setw(12) << std::left << population.getGeneration()
                  << std::setw(6) << std::left << "Best:"
                  << std::setw(8) << std::left << population.getBestBoard().getFitness()
                  << std::setw(5) << std::left << "Avg:"
                  << std::setw(8) << std::left << population.getAverageFitness()
                  << std::setw(7) << std::left << "Worst:"
                  << std::setw(8) << std::left << population.getWorstBoard().getFitness()
                  << std::endl;
        if (oldBest < population.getBestBoard().getFitness()){
            oldBest = population.getBestBoard().getFitness();
            std::cerr << population.getBestBoard() << std::endl;
        }
    }
}
