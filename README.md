# Held–Karp algorithm
The Held–Karp algorithm, also called Bellman–Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP). TSP is an extension of the Hamiltonian circuit problem. The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.


## Complexity
* T(n) = O(2ⁿn²)
* S(n) = O(2ⁿ√n)

## Benchmarks

### PCs
| CPU | CPU GHz | RAM | HD | OS | 
| --- | ---------: | -- | -- | -- |
| AMD Ryzen5 2500U | 2.00 | 8GB DDR4 | SSD | Windows 10 |
| Intel Celeron J1900 | 2.00 | 8GB DDR3 | SSD | Windows 10 |
| Intel Core2 Duo E6550 | 2.33 | 4GB DDR2 | SSD | Windows 10 |

### Results
| PC | Nodes  | Time | 
| -- | -----: | ---: |
| Ryzen5 | 20 | 8473ms |
| Ryzen5 | 25 | 891070ms |
| Celeron | 20 | 13429ms |
| Celeron | 25 | 1874242ms |
| Core2 | 20 | 7895ms |


## IDE
1. C++14
2. Visual Studio 2015 (http://visualstudio.microsoft.com)


## Run the software
1. Run the program:

	```Held-Karp-algorithm.exe [graph to solve = {3, 4, 6, 20, 25, 40, all, random}] [number of node of random graph = {3 - 255}]```


## License
Copyright 2020 (c) [MAIONE MIKY]. All rights reserved.

Licensed under the [MIT](LICENSE) License.