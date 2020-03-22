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

	vector<vector<unsigned char>> distance25 = {
		{ 0, 22, 4, 9, 7, 20, 14, 12, 11, 24, 6, 5, 18, 4, 16, 14, 1, 6, 23, 22, 10, 22, 20, 9, 20, },
		{ 12, 0, 17, 20, 16, 19, 13, 4, 14, 13, 5, 11, 3, 16, 20, 18, 12, 16, 23, 17, 5, 3, 21, 2, 14, },
		{ 23, 7, 0, 9, 8, 11, 3, 20, 3, 20, 13, 19, 4, 18, 21, 17, 11, 2, 16, 5, 23, 5, 12, 9, 10, },
		{ 6, 10, 19, 0, 23, 3, 4, 17, 16, 15, 6, 18, 12, 5, 20, 4, 10, 10, 9, 7, 3, 2, 14, 19, 20, },
		{ 6, 7, 3, 12, 0, 16, 3, 10, 11, 22, 23, 2, 1, 9, 21, 24, 16, 20, 4, 13, 13, 4, 6, 7, 7, },
		{ 12, 20, 14, 21, 5, 0, 6, 9, 17, 16, 24, 17, 21, 2, 5, 1, 12, 21, 7, 14, 6, 18, 3, 2, 13, },
		{ 9, 16, 8, 22, 1, 7, 0, 17, 1, 24, 20, 1, 21, 14, 13, 7, 16, 15, 10, 10, 10, 4, 3, 15, 16, },
		{ 4, 1, 16, 21, 16, 1, 13, 0, 10, 19, 1, 12, 14, 23, 11, 16, 10, 8, 17, 21, 6, 5, 4, 23, 8, },
		{ 24, 22, 16, 8, 17, 16, 10, 6, 0, 2, 17, 19, 24, 15, 20, 9, 24, 8, 15, 2, 14, 1, 18, 18, 5, },
		{ 19, 1, 3, 12, 7, 9, 11, 24, 3, 0, 9, 22, 18, 10, 5, 18, 3, 16, 7, 13, 7, 16, 16, 19, 12, },
		{ 8, 22, 23, 15, 3, 10, 7, 7, 5, 11, 0, 14, 16, 14, 22, 24, 17, 6, 20, 22, 6, 23, 15, 14, 3, },
		{ 21, 24, 23, 19, 6, 11, 13, 3, 6, 15, 4, 0, 9, 10, 20, 22, 18, 3, 15, 14, 23, 16, 3, 13, 18, },
		{ 7, 20, 2, 21, 19, 14, 14, 17, 8, 21, 19, 3, 0, 12, 12, 2, 19, 23, 11, 10, 24, 19, 16, 8, 11, },
		{ 23, 5, 4, 8, 2, 16, 11, 19, 11, 5, 13, 11, 10, 0, 17, 9, 17, 1, 11, 2, 14, 2, 19, 7, 15, },
		{ 14, 20, 1, 5, 17, 14, 23, 15, 12, 15, 22, 14, 10, 7, 0, 16, 17, 10, 24, 16, 7, 23, 15, 9, 6, },
		{ 2, 18, 23, 8, 15, 19, 9, 4, 22, 13, 18, 7, 16, 6, 3, 0, 21, 6, 22, 23, 19, 19, 19, 7, 19, },
		{ 20, 9, 4, 2, 3, 1, 16, 23, 11, 19, 4, 23, 16, 16, 1, 18, 0, 6, 19, 17, 21, 14, 10, 2, 4, },
		{ 22, 20, 1, 2, 17, 15, 11, 7, 12, 5, 13, 6, 15, 7, 1, 22, 22, 0, 19, 20, 21, 3, 4, 6, 15, },
		{ 22, 5, 13, 1, 3, 10, 5, 10, 8, 24, 5, 8, 24, 15, 6, 21, 9, 21, 0, 12, 11, 4, 20, 3, 23, },
		{ 18, 19, 24, 19, 14, 5, 2, 18, 14, 10, 4, 13, 2, 11, 5, 2, 24, 3, 22, 0, 16, 3, 18, 22, 7, },
		{ 13, 24, 13, 7, 17, 19, 12, 2, 2, 14, 7, 9, 10, 11, 21, 3, 22, 11, 18, 15, 0, 11, 18, 19, 22, },
		{ 17, 10, 12, 2, 22, 12, 21, 1, 21, 18, 6, 14, 17, 20, 2, 20, 23, 3, 11, 13, 5, 0, 20, 16, 14, },
		{ 6, 19, 18, 16, 24, 8, 21, 20, 6, 15, 14, 7, 8, 11, 4, 15, 18, 6, 12, 17, 5, 7, 0, 10, 3, },
		{ 10, 23, 16, 9, 7, 7, 17, 21, 19, 22, 20, 4, 18, 5, 2, 14, 22, 11, 9, 3, 6, 20, 12, 0, 13, },
		{ 23, 21, 20, 23, 9, 24, 10, 24, 9, 16, 7, 1, 16, 12, 13, 24, 22, 18, 13, 10, 15, 1, 22, 13, 0, },
	};

	TSP(distance3, numThreads);
	TSP(distance4, numThreads);
	TSP(distance6, numThreads);
	TSP(distance20, numThreads);
	//TSP(distance25, numThreads);
}

void main(int argc, char ** argv)
{
	cout << "Multi-threaded Held-Karp algorithm to solve the Traveling Salesman Problem" << endl;
	cout << "Copyright 2020 (c) [MAIONE MIKY]. All rights reserved." << endl;
	cout << "Licensed under the MIT License." << endl << endl;

	auto numThreads = (argc == 2 ? atoi(argv[1]) : -1);

	try
	{
		StartElaboration(numThreads);
	}
	catch (const exception &e)
	{
		cout << endl << "Exception occurred: " << e.what() << endl;
		throw;
	}

	cout << endl << "End." << endl;
}