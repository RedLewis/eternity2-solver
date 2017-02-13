#include "tile.h"
#include "board.h"
#include <iostream>
#include <list>
#include <time.h>
#include <algorithm>
#include "population.h"
#include <chrono>
int main() {
/*
    Board tmp;
    //std::cout << tmp << std::endl;
    tmp.swapRectangle(0,0,5,5,10,10);
    tmp.swapRectangle(5,5,0,0,10,10);

    tmp.swapRectangle(0,5,0,0,10,10);
    tmp.swapRectangle(5,0,0,0,10,10);

    tmp.swapRectangle(0,0,5,0,10,10);
    tmp.swapRectangle(0,0,0,5,10,10);

    tmp.swapRectangle(5,5,5,5,10,10);
    tmp.swapRectangle(0,0,0,0,10,10);
*/

    srand(time(NULL));

    Population pop(100);
    std::cout << pop.getAverageFitness() << std::endl;
    //srand(time(NULL));

    std::list<Board*> population(100);
    std::generate(population.begin(), population.end(), [](){
        return new Board();
    });
/*
    while ((*population.begin())->getFitness() != Board::MAX_FITNESS){

        for( auto individual : population){
            std::cout << individual->getFitness() << std::endl;
        }
        //std::cout << *((Board*)(*population.begin()))<< std::endl;
        std::cout << (*population.begin())->getFitness() << std::endl;

        population.sort([](Board*& first, Board*& second){
            return first->getFitness() > second->getFitness();
        });
    }
*/
    std::cout << (**population.begin()) << std::endl;
    population.remove_if([](Board* pop){
        delete pop;
        return true;
    });
}
