/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "pch.h"
#include "HeldKarp.hpp"

void TSP(vector<vector<unsigned char>> DistanceMatrix2D, const int numThreads)
{
	HeldKarp hk(DistanceMatrix2D, numThreads);
	hk.TSP();
}

void TSP_RND(const unsigned char NumberOfNodes, const int numThreads)
{
	auto DistanceMatrix2D = HeldKarp::New_RND_Distances(NumberOfNodes);
	TSP(DistanceMatrix2D, numThreads);
}

void StartElaboration(const int numThreads)
{
	vector<vector<unsigned char>> distance3 =
	{
		{ 0, 1, 3 },
		{ 3, 0, 2 },
		{ 5, 1, 0 }
	};

	vector<vector<unsigned char>> distance4 =
	{
		{ 0, 2, 9, 10 },
		{ 1, 0, 6, 4 },
		{ 15, 7, 0, 8 },
		{ 6, 3, 12, 0 }
	};

	vector<vector<unsigned char>> distance6 =
	{
		{ 0, 59, 75, 22, 46, 38 },
		{ 25, 0, 79, 60, 75, 12 },
		{ 85, 26, 0, 58, 56, 27 },
		{ 5, 82, 18, 0, 90, 90 },
		{ 74, 77, 1, 46, 0, 79 },
		{ 76, 23, 94, 61, 47, 0 }
	};

	vector<vector<unsigned char>> distance20 =
	{
		{ 0, 44, 12, 11, 44, 5, 63, 74, 50, 43, 77, 36, 47, 50, 94, 61, 75, 80, 51, 39 },
		{ 58, 0, 90, 24, 86, 24, 97, 67, 85, 17, 85, 37, 58, 60, 39, 6, 8, 16, 9, 30 },
		{ 48, 37, 0, 76, 45, 23, 32, 11, 43, 14, 82, 43, 19, 17, 64, 1, 76, 18, 46, 91 },
		{ 69, 100, 74, 0, 74, 63, 11, 96, 53, 36, 94, 73, 73, 67, 62, 17, 51, 48, 21, 20 },
		{ 95, 38, 55, 48, 0, 6, 65, 16, 99, 54, 85, 18, 95, 82, 11, 30, 24, 53, 62, 76 },
		{ 38, 30, 77, 52, 68, 0, 1, 87, 79, 68, 8, 98, 73, 2, 30, 16, 29, 19, 90, 36 },
		{ 79, 31, 3, 82, 97, 40, 0, 47, 7, 30, 53, 49, 60, 82, 24, 78, 1, 73, 76, 60 },
		{ 71, 44, 54, 90, 38, 87, 72, 0, 79, 7, 88, 71, 7, 68, 78, 50, 91, 28, 24, 10 },
		{ 14, 87, 97, 87, 74, 84, 29, 90, 0, 13, 85, 92, 43, 62, 37, 62, 8, 15, 10, 35 },
		{ 16, 1, 70, 44, 50, 37, 96, 8, 91, 0, 62, 67, 44, 68, 70, 98, 21, 75, 23, 69 },
		{ 44, 100, 82, 81, 32, 22, 7, 67, 93, 23, 0, 7, 96, 6, 51, 3, 48, 53, 30, 1 },
		{ 12, 32, 42, 59, 64, 36, 14, 58, 15, 50, 7, 0, 35, 67, 92, 59, 98, 14, 75, 71 },
		{ 40, 1, 32, 34, 7, 41, 8, 87, 30, 78, 89, 27, 0, 6, 34, 40, 44, 73, 84, 58 },
		{ 18, 36, 100, 75, 3, 77, 78, 29, 14, 74, 28, 11, 19, 0, 9, 14, 20, 20, 47, 96 },
		{ 67, 32, 95, 89, 41, 83, 77, 40, 78, 17, 51, 69, 40, 55, 0, 51, 43, 53, 87, 39 },
		{ 37, 87, 97, 11, 3, 10, 23, 69, 58, 69, 76, 21, 82, 90, 70, 0, 35, 94, 92, 99 },
		{ 44, 30, 36, 43, 31, 77, 36, 19, 84, 15, 45, 62, 79, 18, 35, 12, 0, 92, 54, 4 },
		{ 79, 52, 17, 73, 33, 100, 16, 85, 52, 5, 20, 44, 37, 28, 94, 8, 57, 0, 93, 1 },
		{ 8, 44, 3, 67, 92, 53, 15, 41, 39, 95, 41, 75, 22, 41, 72, 39, 35, 5, 0, 67 },
		{ 52, 78, 77, 78, 12, 13, 93, 13, 98, 60, 87, 85, 59, 61, 60, 82, 23, 88, 88, 0 }
	};

	TSP(distance3, numThreads);
	TSP(distance4, numThreads);
	TSP(distance6, numThreads);
	TSP_RND(10, numThreads);
	TSP_RND(15, numThreads);
	TSP(distance20, numThreads);
}

void main(int argc, char ** argv)
{
	cout << "Multi-threaded Held-Karp algorithm to solve the Traveling Salesman Problem" << endl;
	cout << "Copyright 2020 (c) [MAIONE MIKY]. All rights reserved." << endl;
	cout << "Licensed under the MIT License." << endl << endl;

	auto numThreads = (argc == 2 ? atoi(argv[1]) : -1);

	StartElaboration(numThreads);

	cout << endl << "End." << endl;
	system("pause");
}