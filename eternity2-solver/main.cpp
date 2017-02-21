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
#include <csignal>

static volatile bool keepRunning = true;
void sigintHandler(int i) {
    (void)i;
    keepRunning = false;
}


#define NO_GRAPH_

#ifdef _GRAPH_

#include "matplotlibcpp.h"

std::vector<double> gen;
std::vector<double> fitness;

#endif
void showStat(int nbr, Population* pop, float t, int since, bool found){
    std::cout << std::setw(18) << std::left << std::string("Population[") + std::to_string(nbr) + std::string("]")

              << std::setw(5) << std::left << "Gen:"
              << std::setw(13) << std::left << pop->getGeneration()

              << std::setw(7) << std::left << "Edges:"
              << std::setw(11) << std::left << std::to_string(pop->getBestBoard().getEdgeMatch()) + std::string("/") + std::to_string(Board::EDGE_NUMBER)

              << std::setw(6) << std::left << "Best:"
              << std::setw(12) << std::left << pop->getBestBoard().getFitness()

              << std::setw(7) << std::left << "Since:"
              << std::setw(11) << std::left << since

              << std::setw(5) << std::left << "Avg:"
              << std::setw(13) << std::left << pop->getAverageFitness()

              << std::setw(7) << std::left << "Preserved:"
              << std::setw(11) << std::left << pop->getPreserved()

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
    signal(SIGINT, sigintHandler);
  
    //Board::getSolvedEdgesBoards();
    //exit(0);

    std::cout << std::endl << std::endl
    << "###############################" << std::endl
    << "            restart            " << std::endl
    << "###############################" << std::endl;

    //Board::unitTestSwap();
    float oldBest = 0;
    int since = 0;
    srand(time(NULL));
    Population population1(3);
    Population population2(30);
    Population population3(300);
    Population population4(3000);
    Population population5(30000);
    Population* populations[5] = {
        &population1, &population2, &population3, &population4, &population5
    };
    FPSTimer timer;

#ifdef _GRAPH_
    matplotlibcpp::ion();
    matplotlibcpp::show();
#endif
    
#pragma omp parallel for num_threads(5) schedule(static) firstprivate(oldBest, since)
    for (unsigned int i = 0; i < 5; ++i) {

        while(keepRunning && populations[i]->getBestBoard().getEdgeMatch() < Board::EDGE_NUMBER) {
            populations[i]->stepGeneration();
            float t = 0;
            if (oldBest < populations[i]->getBestBoard().getFitness()){
                oldBest = populations[i]->getBestBoard().getFitness();

#pragma omp critical
                {
                //std::cerr << populations[i]->getBestBoard() << std::endl;
                showStat(i, populations[i], t, since, true);
#ifdef _GRAPH_
                matplotlibcpp::plot(gen, fitness);
                matplotlibcpp::draw();
                matplotlibcpp::pause(0.001);
#endif
                }
                since = 0;
            } else if ((populations[i]->getGeneration() % 5000) == 0){
                showStat(i, populations[i], t, since, false);
            }
            ++since;
        }

    }

    std::cout << "Program finished successfully." << std::endl;
}
