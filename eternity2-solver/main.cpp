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

void showStat(Population* pop, float t, int since){
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
              << "DeltaT: " << t
              << std::endl;
}

int main()
{
    srand(time(NULL));
    Board tmp;
    tmp.swapAndRotateAngleMutation();
    //tmp.swapRectangle(0,0,11,0,5,5);
    //tmp.rotateSquare(0,0,6);
    //tmp.rotateSquare(0,0,6);
    //tmp.rotateSquare(0,0,6);
    std::cerr << tmp << std::endl;
    /*
    int ca = 4;
    while (ca--){
        tmp.rotateSquare(6,0,10);
        std::cerr << tmp << std::endl;
    }
*/
    exit(0);
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
    Population population(5);
    while (population.getBestBoard().getFitness() != Board::MAX_FITNESS)
    {
        population.stepGeneration();
        float t = timer.update();
        if (oldBest < population.getBestBoard().getFitness()){
            oldBest = population.getBestBoard().getFitness();
            std::cerr << population.getBestBoard() << std::endl;
            showStat(&population, t, since);
            since = 0;
        }else if ((population.getGeneration() % 1000) == 0){
            showStat(&population, t, since);
        }
        ++since;
    }
}
