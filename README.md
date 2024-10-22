# Algorithms for the resolution of the TSP
TSP is an extension of the Hamiltonian circuit problem. The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.

## Algorithms
### D.P. Held–Karp algorithm
The Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 by Held and Karp to solve the Traveling Salesman Problem (TSP), the complexities are: T(n) = O(2ⁿn²), S(n) = O(2ⁿ√n).
### Held–Karp MST algorithm
In 1969 Held and Karp proposed a new approach to solve the symmetric Traveling Salesman Problem (sTSP) using an ascent method and costruct a branch and bound method to control the search for an optimum tour.
### Volgenant–Jonker 1-tree relaxation
In 1980 Volgenant and Jonker proposed a simple and fast algorithm to solve the symmetric Traveling Salesman Problem (sTSP) using a branch and bound method to control the search for an optimum tour based on 1-tree relaxation.
### Christofides algorithm
The Christofides algorithm, 1976, is an algorithm for finding approximate solutions to the euclidean travelling salesman problem. It is an approximation algorithm that guarantees that its solutions will be within a factor of 3/2 of the optimal solution length in O(n³).
### Kruskal algorithm for MST
In 1959 Kruskal proposed a greedy algorithm to find a minimum spanning tree for a connected weighted graph adding increasing cost arcs at each step.
### Prim algorithm for MST
A greedy algorithm of the 1957 that finds a minimum spanning tree for a weighted undirected graph.
### Blossom algorithm
An algorithm for constructing maximum matchings on graphs. The algorithm was developed by Jack Edmonds in 1961.

## IDE
1. C++14
2. Visual Studio 2015 (http://visualstudio.microsoft.com)


## Run the software
1. Run the program:

	```Held-Karp-algorithm.exe algorithm = {H, C, A, B, L} type = {T, E, A} [graph to solve = {4, 10, 15, 20, 25, all} OR {TSPLib File name}]```


## License
Copyright 2020 (c) [MAIONE MIKY]. All rights reserved.

Licensed under the [MIT](LICENSE) License.