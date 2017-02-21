# eternity2-solver

This project is an Ethernity2 solver in c++.

This project was started as a student project.
It aims to find the closest solution using genetic algorithms.

# Benchmarks

We have evaluated the performance of the solver running for a fixed time.
We have also looked at the efficiency of the genetic algorithm implementation by
running it a fixed number of generations.

After 15min of calculation:
+-----------------+------------+-----------------+----------------+-----------------+
| Population Size | Generation | Completed Edges |  Best Fitness  | Average Fitness |
+-----------------+------------+-----------------+----------------+-----------------+
|               3 |            |                 |                |                 |
|              30 |            |                 |                |                 |
|             300 |            |                 |                |                 |
|            3000 |            |                 |                |                 |
|           30000 |            |                 |                |                 |
+-----------------+------------+-----------------+----------------+-----------------+

After 5000 generations:
+-----------------+------------+-----------------+----------------+-----------------+
| Population Size |    Time    | Completed Edges |  Best Fitness  | Average Fitness |
+-----------------+------------+-----------------+----------------+-----------------+
|               3 |            |                 |                |                 |
|              30 |            |                 |                |                 |
|             300 |            |                 |                |                 |
|            3000 |            |                 |                |                 |
|           30000 |            |                 |                |                 |
+-----------------+------------+-----------------+----------------+-----------------+

