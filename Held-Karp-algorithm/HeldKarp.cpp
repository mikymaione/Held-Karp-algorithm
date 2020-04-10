/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <chrono>
#include <iostream>
#include <set>
#include <stack>
#include <string>

#include "HeldKarp.hpp"

using namespace chrono;

HeldKarp::HeldKarp(const vector<vector<uint8_t>> &DistanceMatrix2D) : numberOfNodes(DistanceMatrix2D.size()), distance(DistanceMatrix2D) {}

string HeldKarp::PrintPath(const uint32_t code, const uint8_t π)
{
	string s;

	for (const auto e : C[numberOfNodes - 1][code][π].path)
		s = to_string(e) + " " + s;

	return "0 " + s + "0";
}

template <class IEnumerable>
uint32_t HeldKarp::Powered2Code(const IEnumerable &S)
{
	return Powered2Code(S, UCHAR_MAX);
}

template <class IEnumerable>
uint32_t HeldKarp::Powered2Code(const IEnumerable &S, const uint8_t exclude)
{
	uint32_t code = 0;

	for (const auto e : S)
		if (e != exclude)
			code += 1 << e;

	return code;
}

uint32_t HeldKarp::Powered2Code(const uint32_t code, const uint8_t exclude)
{
	return code - (1 << exclude);
}

void HeldKarp::Combinations(const uint8_t K, const uint8_t N)
{
	size_t i;
	uint8_t π, s;
	uint16_t opt, tmp;
	uint32_t code_k, code;

	vector<uint8_t> R(K);
	stack<uint8_t> S;
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
				code = Powered2Code(R);

				for (const auto k : R)
				{
					π = 0;
					opt = USHRT_MAX;
					code_k = Powered2Code(code, k);

					for (const auto m : R) // min(m≠k, m∈R) {C(R\{k}, m) + d[m,k]}
						if (m != k)
						{
							tmp = C[K - 1][code_k][m].cost + distance[k][m];

							if (tmp < opt)
							{
								opt = tmp;
								π = m;
							}
						}

					C[K][code][k] = C[K - 1][code_k][π];
					C[K][code][k].path.push_back(π);
					C[K][code][k].cost = opt;
				}

				break;
			}
		}
	}
}

template <class T>
T HeldKarp::generateRandomNumber(const T startRange, const T endRange, const T limit)
{
	const T r = rand();
	const T range = 1 + endRange - startRange;
	const T num = r % range + startRange;

	return num;
}

/*
Held–Karp algorithm

The Held–Karp algorithm, also called Bellman–Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).
TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.

T(n) = O(2ⁿn²)
S(n) = O(2ⁿ√n)
*/
void HeldKarp::TSP()
{
	const auto begin = steady_clock::now();

	cout
		<< "Solving a graph of "
		<< to_string(numberOfNodes)
		<< " nodes:"
		<< endl;

	// TSP ================================================================================================================================
	// insieme vuoto
	for (uint8_t k = 1; k < numberOfNodes; k++)
		C[1][1 << k][k].cost = distance[k][0];

	for (uint8_t s = 2; s < numberOfNodes; s++) // O(N) cardinalità degli insiemi
	{
		Combinations(s, numberOfNodes - 1); // O(2ⁿ) genera (2^s)-1 insiemi differenti di cardinalità s		
		C.erase(s - 1);

		cout
			<< "ET: "
			<< duration_cast<milliseconds>(steady_clock::now() - begin).count() / 1000
			<< "s ETL: "
			<< (duration_cast<milliseconds>(steady_clock::now() - begin).count() / s) * (numberOfNodes - s) / 1000
			<< "s\r";
		fflush(stdin);
	}
	// TSP ================================================================================================================================

	// PATH ===============================================================================================================================
	uint8_t π = 0;
	uint16_t tmp, opt = USHRT_MAX;

	set<uint8_t> FullSet;
	for (uint8_t z = 1; z < numberOfNodes; z++)
		FullSet.insert(z);

	const auto code = Powered2Code(FullSet);

	for (const auto k : FullSet) // min(k≠0) {C({1, ..., n-1}, k) + d[k,0]}
		if (C[numberOfNodes - 1][code][k].cost > 0)
		{
			tmp = C[numberOfNodes - 1][code][k].cost + distance[0][k];

			if (tmp < opt)
			{
				opt = tmp;
				π = k;
			}
		}

	C[numberOfNodes - 1][code][π].cost = opt;
	C[numberOfNodes - 1][code][π].path.push_back(π);

	const auto path = PrintPath(code, π);

	cout
		<< "-Cost: "
		<< to_string(opt)
		<< " time: "
		<< duration_cast<milliseconds>(steady_clock::now() - begin).count()
		<< "ms, path: "
		<< path
		<< endl;
	// PATH ===============================================================================================================================	
}

vector<vector<uint8_t>> HeldKarp::New_RND_Distances(const uint8_t Size_of_RandomDistanceCosts)
{
	vector<vector<uint8_t>> A(Size_of_RandomDistanceCosts, vector<uint8_t>(Size_of_RandomDistanceCosts, 0));

	for (auto x = 0; x < Size_of_RandomDistanceCosts; x++)
		for (auto y = 0; y < Size_of_RandomDistanceCosts; y++)
			A[x][y] = (x == y ? 0 : generateRandomNumber(1, 25, UCHAR_MAX));

	return A;
}