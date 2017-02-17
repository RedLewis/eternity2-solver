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

void showStat(Population* pop, float t, int since, bool found){
    std::cout << std::setw(5) << std::left << "Gen:"
              << std::setw(12) << std::left << pop->getGeneration()
              << std::setw(6) << std::left << "Best:"
              << std::setw(10) << std::left << pop->getBestBoard().getFitness()
              << std::setw(7) << std::left << "Since:"
              << std::setw(8) << std::left << since
              << std::setw(5) << std::left << "Avg:"
              << std::setw(8) << std::left << pop->getAverageFitness()
              << std::setw(7) << std::left << "Worst:"
              << std::setw(8) << std::left << pop->getWorstBoard().getFitness()
              << "DeltaT: " << t;
    if (found){std::cout << std::setw(10) << std::left << "/!\\";}

    std::cout << std::endl;
}

int main()
{
    //Board::getSolvedEdgesBoards();
    std::cout << std::endl << std::endl
    << "###############################" << std::endl
    << "            restart            " << std::endl
    << "###############################" << std::endl;
    FPSTimer timer;

    //Board::unitTestSwap();;
    float oldBest = 0;
    int since = 0;
    srand(time(NULL));
    Population population(100);
    while (population.getBestBoard().getFitness() < Board::MAX_FITNESS)
    {
        population.stepGeneration();
        float t = timer.update();
        if (oldBest < population.getBestBoard().getFitness()){
            oldBest = population.getBestBoard().getFitness();
            std::cerr << population.getBestBoard() << std::endl;
            showStat(&population, t, since, true);
            since = 0;
        }else if ((population.getGeneration() % 1000) == 0){
            showStat(&population, t, since, false);
        }
        ++since;
    }
}
