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

#define _GRAPH_

#ifdef _GRAPH_

#include "matplotlibcpp.h"

std::vector<double> gen;
std::vector<double> fitness;

#endif
void showStat(Population* pop, float t, int since, bool found){
    std::cout << std::setw(5) << std::left << "Gen:"
              << std::setw(13) << std::left << pop->getGeneration()

              << std::setw(7) << std::left << "Edges:"
              << std::setw(11) << std::left << std::to_string(pop->getBestBoard().getEdgeMatch()) + std::string("/") + std::to_string(Board::EDGE_NUMBER)

              << std::setw(6) << std::left << "Best:"
              << std::setw(12) << std::left << pop->getBestBoard().getFitness()

              << std::setw(7) << std::left << "Since:"
              << std::setw(11) << std::left << since

              << std::setw(5) << std::left << "Avg:"
              << std::setw(13) << std::left << pop->getAverageFitness()

              << std::setw(7) << std::left << "Worst:"
              << std::setw(11) << std::left << pop->getWorstBoard().getFitness()

              << std::setw(8) << std::left << "DeltaT:"
              << std::setw(10) << std::left << t;

    if (found){
        std::cout << "/!\\";
        #ifdef _GRAPH_
        gen.push_back(pop->getGeneration());
        fitness.push_back(pop->getBestBoard().getFitness());
        #endif
    }

    std::cout << std::endl;
}

int main()
{
    //Board::getSolvedEdgesBoards();
    //exit(0);
    std::cout << std::endl << std::endl
    << "###############################" << std::endl
    << "            restart            " << std::endl
    << "###############################" << std::endl;
    FPSTimer timer;

    //Board::unitTestSwap();;
    float oldBest = 0;
    int since = 0;
    bool run = true;
    srand(time(NULL));
    Population population(3);
#ifdef _GRAPH_
    matplotlibcpp::ion();
    matplotlibcpp::show();
#endif
    while (run && population.getBestBoard().getEdgeMatch() < Board::EDGE_NUMBER)
    {
        population.stepGeneration();
        float t = timer.update();
        if (oldBest < population.getBestBoard().getFitness()){
            oldBest = population.getBestBoard().getFitness();
            std::cerr << population.getBestBoard() << std::endl;
            showStat(&population, t, since, true);
            since = 0;
#ifdef _GRAPH_
                matplotlibcpp::plot(gen, fitness);
                matplotlibcpp::draw();
                matplotlibcpp::pause(0.001);
#endif
        }else if ((population.getGeneration() % 5000) == 0){
            showStat(&population, t, since, false);
        }
        ++since;
    }
}
