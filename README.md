# Traveling Salesman Problem Solvers
TSP is an extension of the Hamiltonian circuit problem. The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.

## Benchmarks

### PCs
| CPU | CPU GHz | RAM | HD | OS | 
| --- | ---------: | -- | -- | -- |
| AMD Ryzen5 2500U | 2.00 | 8GB DDR4 | SSD | Windows 10 |
| Intel Celeron J1900 | 2.00 | 8GB DDR3 | SSD | Windows 10 |
| Intel Core2 Quad Q6600 | 2.40 | 4GB DDR2 | SSD | Windows 10 |
| Intel Core2 Duo E6550 | 2.33 | 4GB DDR2 | SSD | Windows 10 |


## Held–Karp algorithm
The Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).

### Complexity
* T(n) = O(2ⁿn²)
* S(n) = O(2ⁿ√n)

### Results
| PC | Nodes  | Time | 
| -- | -----: | ---: |
| Core2 Quad | 20 | 00'07"27 |
| Core2 Duo | 20 | 00'07"89 |
| Ryzen5 | 20 | 00'08"47 |
| Celeron | 20 | 00'13"43 |
|||
| Ryzen5 | 25 | 14'51"70 |
| Celeron | 25 | 31'14"42 |


## Christofides algorithm
The Christofides algorithm, 1976, is an algorithm for finding approximate solutions to the euclidean travelling salesman problem. It is an approximation algorithm that guarantees that its solutions will be within a factor of 3/2 of the optimal solution length.

### Complexity
* T(n) = O(n⁴)

### Results
| PC | Nodes  | Time | 
| -- | -----: | ---: |
| Core2 Quad | 20 | 00'00"01 |
| Core2 Quad | 25 | 00'00"01 |
| Core2 Quad | 100 | 00'00"02 |
| Core2 Quad | 500 | 00'00"08 |
| Core2 Quad | 1000 | 00'00"35 |
| Core2 Quad | 5000 | 00'15"52 |

## IDE
1. C++14
2. Visual Studio 2015 (http://visualstudio.microsoft.com)


## Run the software
1. Run the program:

	```Held-Karp-algorithm.exe algorithm = {H, C} type = {E, A} [graph to solve = {4, 10, 15, 20, 25, all}]```


## License
Copyright 2020 (c) [MAIONE MIKY]. All rights reserved.

Licensed under the [MIT](LICENSE) License.