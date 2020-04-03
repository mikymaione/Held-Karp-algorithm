﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <stack>
#include <string>

#include "HeldKarp.hpp"

using namespace chrono;

unsigned char HeldKarp::CombinationPath(set<unsigned char> &S, const unsigned char s, const unsigned char k)
{
	unsigned char π = 0;
	unsigned short tmp;

	auto opt = USHRT_MAX;
	auto code = Powered2Code(S);

	for (auto m : S) // min(m≠k, m∈S) {C(S\{k}, m) + d[m,k]}		
	{
		tmp = CGet(s, code, m) + distance[k][m];

		if (tmp < opt)
		{
			opt = tmp;
			π = m;
		}
	}

	return π;
}

string HeldKarp::PrintTour(set<unsigned char> S, const unsigned char π) // O(N³)
{
	unsigned char s = π;
	string path = "0 " + to_string(s) + " ";

	while (S.size() > 1)
	{
		S.erase(s);
		s = CombinationPath(S, S.size(), s);

		path += to_string(s) + " ";
	}

	return path + "0";
}

template <class IEnumerable>
unsigned long HeldKarp::Powered2Code(IEnumerable &S)
{
	return Powered2Code(S, UCHAR_MAX);
}

template <class IEnumerable>
unsigned long HeldKarp::Powered2Code(IEnumerable &S, const unsigned char exclude)
{
	unsigned long code = 0;

	for (auto e : S)
		if (e != exclude)
			code += 1 << e;

	return code;
}

unsigned long HeldKarp::Powered2Code(unsigned long code, const unsigned char exclude)
{
	return code - (1 << exclude);
}

void HeldKarp::CombinationPart(vector<unsigned char> S, const unsigned char s)
{
	unsigned char π;
	unsigned short opt, tmp;
	unsigned long code_k, code;

	code = Powered2Code(S);

	for (auto k : S)
	{
		π = 0;
		opt = USHRT_MAX;
		code_k = Powered2Code(code, k);

		for (auto m : S) // min(m≠k, m∈S) {C(S\{k}, m) + d[m,k]}
			if (m != k)
			{
				tmp = CGet(s - 1, code_k, m) + distance[k][m];

				if (tmp < opt)
				{
					opt = tmp;
					π = m;
				}
			}

		CSet(s, code, k, opt);
	}
}

void HeldKarp::Combinations(const unsigned char K, const unsigned char N)
{
	size_t i;
	unsigned char s;

	auto useMultiThreadingForK = (useMultiThreading && N > 10 && K > 3);

	vector<thread> threads;

	vector<unsigned char> R(K);
	stack<unsigned char> S;
	S.push(0);

	while (S.size() > 0)
	{
		i = S.size() - 1;
		s = S.top();
		S.pop();

		while (s < N)
		{
			s++;
			R[i] = s;
			S.push(s);
			i++;

			if (i == K)
			{
				if (useMultiThreadingForK)
				{
					threads.push_back(thread(&HeldKarp::CombinationPart, this, R, K));

					if (threads.size() == concurentThreadsSupported)
						waitForThreads(threads);
				}
				else
				{
					CombinationPart(R, K);
				}

				break;
			}
		}
	}

	if (useMultiThreadingForK)
		waitForThreads(threads);
}

void HeldKarp::waitForThreads(vector<thread> &threads)
{
	for (auto & th : threads)
		if (th.joinable())
			th.join();

	threads.clear();
}

template <class T>
T HeldKarp::generateRandomNumber(T startRange, T endRange, T limit)
{
	T r = rand();

	T range = endRange - startRange;
	range++;

	T num = r % range + startRange;

	return num;
}

HeldKarp::HeldKarp(vector<vector<unsigned char>> &DistanceMatrix2D, const unsigned int numThreads)
{
	distance = DistanceMatrix2D;
	numberOfNodes = (unsigned char)distance.size();

	auto hc = thread::hardware_concurrency();

	if (numThreads < hc)
		hc = numThreads;

	concurentThreadsSupported = (hc >= minCpus ? hc - 2 : 0);

	useMultiThreading = (concurentThreadsSupported > 0);
}

/*
Held–Karp algorithm

The Held–Karp algorithm, also called Bellman–Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).
TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.

T(n) = O(2ⁿn²) + O(n³)
S(n) = O(2ⁿn)
*/
void HeldKarp::TSP()
{
	auto begin = steady_clock::now();

	cout
		<< "Using "
		<< to_string(concurentThreadsSupported)
		<< " threads to solve a graph of "
		<< to_string(numberOfNodes)
		<< " nodes:"
		<< endl;

	// TSP ================================================================================================================================
	// insieme vuoto
	for (auto k = 1; k < numberOfNodes; k++)
		CSet(1, 1 << k, k, distance[k][0]);

	for (unsigned char s = 2; s < numberOfNodes; s++) // O(N) cardinalità degli insiemi
	{
		Combinations(s, numberOfNodes - 1); // O(2ⁿ) genera (2^s)-1 insiemi differenti di cardinalità s		

		cout
			<< "ET: "
			<< duration_cast<milliseconds>(steady_clock::now() - begin).count()
			<< "ms ETL: "
			<< (duration_cast<milliseconds>(steady_clock::now() - begin).count() / s) * (numberOfNodes - s)
			<< "ms\r";
		fflush(stdin);
	}
	// TSP ================================================================================================================================

	// PATH ===============================================================================================================================
	unsigned char π = 0;
	unsigned short tmp, opt = USHRT_MAX;
	unsigned long code;

	set<unsigned char> FullSet;
	for (unsigned char z = 1; z < numberOfNodes; z++)
		FullSet.insert(z);

	code = Powered2Code(FullSet);

	for (auto k : FullSet) // min(k≠0) {C({1, ..., n-1}, k) + d[k,0]}
		if (CGet(numberOfNodes - 1, code, k) > 0)
		{
			tmp = CGet(numberOfNodes - 1, code, k) + distance[0][k];

			if (tmp < opt)
			{
				opt = tmp;
				π = k;
			}
		}

	auto path = PrintTour(FullSet, π);

	cout
		<< " Cost: "
		<< to_string(opt)
		<< " time: "
		<< duration_cast<milliseconds>(steady_clock::now() - begin).count()
		<< "ms, path: "
		<< path
		<< endl;
	// PATH ===============================================================================================================================
}

vector<vector<unsigned char>> HeldKarp::New_RND_Distances(const unsigned char Size_of_RandomDistanceCosts)
{
	vector<vector<unsigned char>> A(Size_of_RandomDistanceCosts, vector<unsigned char>(Size_of_RandomDistanceCosts, 0));

	for (auto x = 0; x < Size_of_RandomDistanceCosts; x++)
		for (auto y = 0; y < Size_of_RandomDistanceCosts; y++)
			A[x][y] = (x == y ? 0 : generateRandomNumber(1, 25, UCHAR_MAX));

	return A;
}