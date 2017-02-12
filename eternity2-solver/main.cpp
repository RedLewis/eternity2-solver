#include "cell.h"
#include "board.h"
#include <iostream>
#include <stack>

int main() {
    /*
    Cell cell(1,2,3,4);
    Cell cella(4,1,2,3);
    if (cell == cella)
        std::cout << "equal" << std::endl;
    else
        std::cout << "not equal" << std::endl;
    */

    std::stack<Board*> population;
    while (population.size() < 1){
        Board* tmp = new Board();
        std::cout << *tmp << std::endl;
        population.push(tmp);
    }

    while (population.size() > 0){
        delete population.top();
        population.pop();
    }
}
