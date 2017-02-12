#include "cell.h"
#include "board.h"
#include <iostream>
#include <stack>
#include <time.h>
int main() {
    /*
    Cell cell(1,2,3,4);
    Cell cella(4,1,2,3);
    if (cell == cella)
        std::cout << "equal" << std::endl;
    else
        std::cout << "not equal" << std::endl;
    */
    //srand(time(NULL));
    std::stack<Board*> population;
    while (population.size() < 1){
        Board* tmp = new Board();
        std::cout << *tmp << std::endl;
        population.push(tmp);
    }
    population.top()->swapSquare(0,0,1,0,1,1);
        /*
    population.top()->rotateRegionMutation(0,0,1,1);
    population.top()->rotateRegionMutation(0,0,1,1);
    population.top()->rotateRegionMutation(0,0,1,1);
    */
    std::cout << *population.top() << std::endl;
    while (population.size() > 0){
        delete population.top();
        population.pop();
    }
}
