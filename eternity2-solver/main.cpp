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
#include "fpstimer.h"
#include "board.h"

int main()
{
    //Board::getSolvedEdgesBoards();

    FPSTimer timer;

    //Board::unitTestSwap();;
    int oldBest = 0;
    int since = 0;
    srand(time(NULL));
    Population population(10000);
    while (population.getBestBoard().getFitness() != Board::MAX_FITNESS && population.getGeneration() < 10)
    {
        population.stepGeneration();
        auto  t = timer.update();
        std::cout << std::setw(5) << std::left << "Gen:"
                  << std::setw(12) << std::left << population.getGeneration()
                  << std::setw(6) << std::left << "Best:"
                  << std::setw(8) << std::left << population.getBestBoard().getFitness()
                  << std::setw(6) << std::left << "Since:"
                  << std::setw(8) << std::left << since
                  << std::setw(5) << std::left << "Avg:"
                  << std::setw(8) << std::left << population.getAverageFitness()
                  << std::setw(7) << std::left << "Worst:"
                  << std::setw(8) << std::left << population.getWorstBoard().getFitness()
                  << "DeltaT: " << t
                  << std::endl;
        ++since;;
        if (oldBest < population.getBestBoard().getFitness()){
            since = 0;
            oldBest = population.getBestBoard().getFitness();
            std::cerr << population.getBestBoard() << std::endl;
        }
    }
}
