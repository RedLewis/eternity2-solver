# eternity2-solver

This project is an Ethernity2 solver in C++.

This project was started as a student project.
It aims to find the closest solution using genetic algorithms.
Our implementation is based on the paper [Solving the Eternity II Puzzle using Evolutionary Computing Techniques](https://github.com/RedLewis/eternity2-solver/raw/master/docs/Solving_the_Eternity_II_Puzzle.pdf) by Papa Ousmane Niang.

# Features

####BASE POPULATION
- Each individual has randomly placed tiles. With edge tiles placed on the edges
and corner tiles placed on the corners with proper orientation.
- A Border Solver was created to find all possible outer borders of the board.
Then one population would be created for each solution and would try to solve
the board treating the given border as correct. We found that we often had to many
solutions.

####EVALUATION
- Tile score scales logarithmicaly according to the vertical and horizontal position.
The closer a tile is to the edge, the higher it's score. This scale was chosen because
border tiles have less possibilities then inner tiles. Therefore they are
more likely to be correct.

####SELECTION
- Selection by tournament. We keep at maximum 10% of the population including the fittest
and other fittest individuals with at least 90% different tile settings form all
other kept individuals.

####CROSSOVER
- Region Exchange Crossover. The crossover is more likely to exchange small regions than big ones.
- Edge edge tiles are kept on the edges and corner tiles are kept on the corners with proper orientation.

####MUTATIONS
- Rotate Region Mutation
- Swap Region Mutation
- Swap Rotate Region Mutation
- Simple Outer Mutation
- Swap Chunk Outer Mutation
- Swap & Rotate Angle Mutation
- Edge edge tiles are kept on the edges and corner tiles are kept on the corners with proper orientation.

# Benchmarks

We have evaluated the performance of the solver running for a fixed time.
We have also looked at the efficiency of the genetic algorithm implementation by
running it for a fixed number of generations.

####Performance Benchmark

After 15min of calculation:

| Population Size | Generation | Completed Edges | Best Fitness | Average Fitness |
|----------------:|-----------:| ---------------:|-------------:|----------------:|
|               3 |    6794965 |             328 |         1454 |            1272 |
|              30 |     535978 |             278 |         1250 |             962 |
|             300 |      54509 |             230 |         1031 |             675 |
|            3000 |       5762 |             178 |          828 |             531 |
|           30000 |        553 |             145 |          690 |             548 |

####Algorithme Evaluation

After 1000 generations:

| Population Size | Completed Edges | Best Fitness| Average Fitness |  Color |
|----------------:|----------------:|------------:|----------------:|-------:|
|               3 |             144 |         669 |             650 |  green |
|              30 |             150 |         686 |             582 |   cyan |
|             300 |             154 |         709 |             542 |   blue |
|            3000 |             147 |         689 |             605 | yellow |
|           30000 |             156 |         735 |             565 |    red |

![graph](https://github.com/RedLewis/eternity2-solver/raw/master/docs/figure_1-1.png)

#Program Input Output

The program catches the signal SIGINT to stop. On most terminals it's CTRL+C.

The program outputs a graphical representation of the best board on the `error` output at each generation where the best board improved. Written in red are the tile values that are not connected and colored in green are the valid tile connections.

![best-raw](https://github.com/RedLewis/eternity2-solver/raw/master/docs/best-board.png)

The program outputs population statics of the best board on the `standart` outputs at each generation where the best board improved or every 1000 generations.

![debug](https://github.com/RedLewis/eternity2-solver/raw/master/docs/debug.png)

We also display a graph (generated by python) as show above in the benchmark section.

