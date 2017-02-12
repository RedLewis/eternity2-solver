#include "cell.h"
#include "board.h"
#include <iostream>
#include <list>
#include <time.h>
#include <algorithm>


int main() {
    //srand(time(NULL));
    std::list<Board*> population(100);
    std::generate(population.begin(), population.end(), [](){
        return new Board();
    });
    population.sort([](Board*& first, Board*& second){
        return first->getFitness() > second->getFitness();
    });

    while ((*population.begin())->getFitness() != Board::MAX_FITNESS){
        for( auto individual : population){
            std::cout << individual->getFitness() << std::endl;
        }
        std::cout << (*population.begin())->getFitness() << std::endl;
    }

    population.remove_if([](Board* pop){
        delete pop;
        return true;
    });
}
