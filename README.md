# eternity2-solver

This project is an Ethernity2 solver in c++.

This project was started as a student project.
It aims to find the closest solution using genetic algorithms.

# Features

BASE POPULATION
- Each idividual has randomly placed tiles. With edge tiles placed on edges
and corner tiles placed on the corners with proper orientation.
- A Border Solver was created to find all possible outer borders of the board.
Then one population would be created for each solution and would try to solve
the board treating the given border as correct.

EVALUATION
- Tile score scales according to position. Logarithmic in x and y position.
The closer to the edge, higher the score. This scale was chosen because
border tiles have less possibilites then inner tiles. Thereofre they are
more likely to be correct.

SELECTION
- Selection by tournament. We keep at maximum 10% of the population including the fittest
and other fittest individuals with at least 90% different tiles settings form all
other kept individuals.

CROSSOVER ()
- Region Exchange Crossover.
* The crossover is more likely to echanger small region than big ones.
Edge tiles are kept on the edges and corner tiles are kept on the corners with proper orientation.

MUTATIONS
- Rotate Region Mutation
- Swap Region Mutation
- Swap Rotate Region Mutation
- Simple Outer Mutation
- Swap Chunk Outer Mutation
- Swap & Rotate Angle Mutation
* Edge tiles are kept on the edges and corner tiles are kept on the corners with proper orientation.

# Benchmarks

We have evaluated the performance of the solver running for a fixed time.
We have also looked at the efficiency of the genetic algorithm implementation by
running it a fixed number of generations.

After 15min of calculation:
+-----------------+------------+-----------------+----------------+-----------------+
| Population Size | Generation | Completed Edges |  Best Fitness  | Average Fitness |
+-----------------+------------+-----------------+----------------+-----------------+
|               3 |    6794965 |             328 |           1454 |            1272 |
|              30 |     535978 |             278 |           1250 |             962 |
|             300 |      54509 |             230 |           1031 |             675 |
|            3000 |       5762 |             178 |            828 |             531 |
|           30000 |        553 |             145 |            690 |             548 |
+-----------------+------------+-----------------+----------------+-----------------+

After 1000 generations:
+-----------------+-----------------+----------------+-----------------+
| Population Size | Completed Edges |  Best Fitness  | Average Fitness |
+-----------------+-----------------+----------------+-----------------+
|               3 |             144 |            669 |             650 |
|              30 |             150 |            686 |             582 |
|             300 |             154 |            709 |             542 |
|            3000 |             147 |            689 |             605 |
|           30000 |             156 |            735 |             565 |
+-----------------+-----------------+----------------+-----------------+

