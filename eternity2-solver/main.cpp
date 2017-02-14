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
    srand(time(NULL));
    Population population(100);
    while (population.getBestBoard().getFitness() != Board::MAX_FITNESS)
    {
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

    }
}
