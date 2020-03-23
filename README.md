# Held–Karp algorithm
The Held–Karp algorithm, also called Bellman–Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP). TSP is an extension of the Hamiltonian circuit problem. The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.


## Features
* Supports the multi-thread: for every set S ⊆ {2, ..., n} of cardinality |S| = s, s threads are executed.
* Supports SQLite storage for memorize the data for graph of cardinality greater than 25.


## IDE
1. C++17
2. Microsoft Concurrency Runtime
3. SQLite3
4. Visual Studio 2017 (http://visualstudio.microsoft.com)


## Run the software
1. Run the program:

	```Held-Karp-algorithm.exe [number of threads = {0 - 32}] [use SQLite = {0 - 1}] [graph to solve = {3, 4, 6, 20, 25, 40, all, random}] [number of node of random graph = {3 - 255}]```


## License
Copyright 2020 (c) [MAIONE MIKY]. All rights reserved.

Licensed under the [MIT](LICENSE) License.