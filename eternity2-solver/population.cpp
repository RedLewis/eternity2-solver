#include "population.h"
#include <iostream>
#include <algorithm>

Population::Population(int size): _boards(size)
{
    std::generate(_boards.begin(), _boards.end(), [](){
        return new Board();
    });
    evaluate();
}

Population::~Population()
{
    for (Board* board : _boards)
        delete board;
}

Board* Population::tournamentSelection(int tournamentSize)const
{
    Board* best = NULL;
    for (int i = 0; i < tournamentSize; ++i)
    {
        Board* ind = _boards[std::rand() % _boards.size()];
        if (best == NULL || best->getFitness() < ind->getFitness())
        {
            best = ind;
        }
    }
    return best;
}

void Population::selection()
{
    std::vector<Board*> newBoards(0);
    newBoards.reserve(_boards.size());
    newBoards.push_back(_best);

    std::pair<Board*, Board*> children;
    //Skip best (which is number 0)
    do {
        Board* parentA = tournamentSelection(2);
        Board* parentB = tournamentSelection(2);
        children = Board::regionExchangeCrossover(*parentA, *parentB);
        newBoards.push_back(children.first);
        if (newBoards.size() < _boards.size()) {
            newBoards.push_back(children.second);
        }
        else
            delete children.second;
    }while(newBoards.size() < _boards.size());
    //Skip best (which is number 0)
    for (unsigned int i = 0; i < _boards.size(); ++i){
        if (_boards[i] != _best){
            delete _boards[i];
        }
    }
    _boards = std::move(newBoards);
}

void Population::mutate()
{
    //Skip best (which is number 0)
    for (unsigned int i = 1; i < _boards.size(); ++i) {
        if (std::rand() % 2)
        _boards[i]->rotateInnerRegionMutation();
        if (std::rand() % 2)
        _boards[i]->swapInnerRegionMutation();
        if (std::rand() % 2)
        _boards[i]->swapAndRotateInnerRegionMutation();
        if (std::rand() % 2)
        _boards[i]->mutateOuter();

    }
}

void Population::evaluate() {
    _averageFitness = 0;
    _best = NULL;
    _worst = NULL;
    for (Board* board : _boards){
        board->evaluate();
        _averageFitness += board->getFitness();
        if (_best == NULL || _best->getFitness() < board->getFitness()){
        _best = board;
       }
        if (_worst == NULL || _worst->getFitness() > board->getFitness()){
        _worst = board;
       }
    }
    _averageFitness /=_boards.size();
}

unsigned int Population::getGeneration()const
{
    return _generation;
}

unsigned int Population::getAverageFitness()const
{
    return _averageFitness;
}

const Board& Population::getBestBoard()const
{
    return *(*_boards.begin());
}

const Board& Population::getWorstBoard()const
{
    return *(*std::prev(_boards.end()));
}

void Population::stepGeneration()
{
    selection();
    mutate();
    evaluate();
    _generation += 1;
}
