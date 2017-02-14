#include "tile.h"
#include "board.h"
#include <iostream>
#include <list>
#include <time.h>
#include <algorithm>
#include "population.h"
#include <chrono>
#include <assert.h>
int main()
{
    //srand(time(NULL));
    /*
    Board tmp;
    //wrong size + overlap
    assert(tmp.swapRectangle(0,0,0,0,0,0) == false);
    assert(tmp.swapRectangle(0,0,0,0,0,1) == false);
    assert(tmp.swapRectangle(0,0,0,0,1,0) == false);
    //rectangle same pos
    assert(tmp.swapRectangle(0,0,0,0,1,1) == false);

    //should work [easy]
    assert(tmp.swapRectangle(1,0,0,0,1,1) == true);
    assert(tmp.swapRectangle(0,1,0,0,1,1) == true);
    assert(tmp.swapRectangle(0,0,1,0,1,1) == true);
    assert(tmp.swapRectangle(0,0,0,1,1,1) == true);
    assert(tmp.swapRectangle(0,0,1,1,1,1) == true);
    assert(tmp.swapRectangle(1,1,0,0,1,1) == true);

    assert(tmp.swapRectangle(1,1,0,0,1,1) == true);
    assert(tmp.swapRectangle(1,1,0,1,1,1) == true);
    assert(tmp.swapRectangle(1,1,0,2,1,1) == true);

    assert(tmp.swapRectangle(1,1,1,0,1,1) == true);
    assert(tmp.swapRectangle(1,1,1,1,1,1) == false);
    assert(tmp.swapRectangle(1,1,1,2,1,1) == true);

    assert(tmp.swapRectangle(1,1,2,0,1,1) == true);
    assert(tmp.swapRectangle(1,1,2,1,1,1) == true);
    assert(tmp.swapRectangle(1,1,2,2,1,1) == true);
    //should overlap [medium]
    // sqr shape
    assert(tmp.swapRectangle(0,0,0,1,2,2) == false);
    assert(tmp.swapRectangle(0,0,1,0,2,2) == false);
    assert(tmp.swapRectangle(0,1,0,0,2,2) == false);
    assert(tmp.swapRectangle(1,0,0,0,2,2) == false);
    // rect shape
    assert(tmp.swapRectangle(0,0,0,1,1,2) == false);
    assert(tmp.swapRectangle(0,0,1,0,2,1) == false);
    assert(tmp.swapRectangle(0,1,0,0,1,2) == false);
    assert(tmp.swapRectangle(1,0,0,0,2,1) == false);
    // random
    assert(tmp.swapRectangle(8,12,9,13,1,1) == true);
    */

    /*
    srand(time(NULL));
    int lastBestFitness = 0;
    Population pop(10000);
    while (pop.getBestBoard().getFitness() != Board::MAX_FITNESS)
    {
        pop.stepGeneration();
       if (lastBestFitness < pop.getBestBoard().getFitness())
       {
           std::cout << "Fitness:" << std::endl;
           std::cout << "\tbest:" << pop.getBestBoard().getFitness() << std::endl;
           std::cout << "\taverage:" << pop.getAverageFitness() << std::endl;
           std::cout << "\tworst:" << pop.getWorstBoard().getFitness() << std::endl;

           lastBestFitness = pop.getBestBoard().getFitness();
            std::cout << pop.getBestBoard() << std::endl;
       }
    }
    */

    //*
    Board parentA;
    Board parentB;
    std::pair<Board*, Board*> children;

    std::cout << parentA << std::endl;
    std::cout << parentB << std::endl;

    children = Board::regionExchangeCrossover(parentA, parentB);

    std::cout << *children.first << std::endl;
    std::cout << *children.second << std::endl;

    assert(parentA.isValid());
    assert(parentB.isValid());
    assert(children.first->isValid());
    assert(children.second->isValid());

    delete children.first;
    delete children.second;
    //*/
}
