﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <string>
#include <set>

#include "HeldKarp.hpp"

namespace TSP
{
	HeldKarp::HeldKarp(const vector<vector<uint_least8_t>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string HeldKarp::PrintPath(const uint_least32_t code, const uint_least8_t π)
	{
		string s;

		for (const auto e : C.front()[code][π].path)
			s = to_string(e) + " " + s;

		return "0 " + s + "0";
	}

	void HeldKarp::AddNewToQueue()
	{
		map<uint_least32_t, map<uint_least8_t, sInfo>> new_map;
		C.push(new_map);
	}

	void HeldKarp::Combinations_FreeMem(stack<uint_least8_t> &Q, vector<uint_least8_t> &S, const uint_least8_t K, const uint_least8_t N, const uint_least8_t sCur)
	{
		size_t i;
		uint_least8_t s;
		uint_least32_t code;
		auto tempC = &C.front();

		Q.push(sCur - 1);

		while (Q.size() > 0)
		{
			i = Q.size() - 1;
			s = Q.top();
			Q.pop();

			while (s < N)
			{
				s++;

				if (i == 0 && s > sCur)
					return;

				S[i] = s;
				Q.push(s);
				i++;

				if (i == K)
				{
					code = Powered2Code(S);
					tempC->erase(code);

					break;
				}
			}
		}
	}

	void HeldKarp::Combinations(const uint_least8_t K, const uint_least8_t N)
	{
		size_t i;
		uint_least8_t π, s;
		uint_least16_t opt, tmp;
		uint_least32_t code, code_k;

		vector<uint_least8_t> S(K);
		stack<uint_least8_t> Q;
		Q.push(0);

		// mem opt	
		vector<uint_least8_t> S_freeMem(K - 1);
		stack<uint_least8_t> Q_freeMem;

		const auto tempC = &C.front();
		map<uint_least8_t, sInfo> *tempC_k, *tempCBack;
		sInfo *info;
		// mem opt

		while (Q.size() > 0)
		{
			i = Q.size() - 1;
			s = Q.top();
			Q.pop();

			while (s < N)
			{
				if (i == 0 && s > 0)
					Combinations_FreeMem(Q_freeMem, S_freeMem, K - 1, N, s);

				s++;
				S[i] = s;
				Q.push(s);
				i++;

				if (i == K)
				{
					code = Powered2Code(S);
					tempCBack = &C.back()[code];

					for (const auto k : S) // ALGO[05]
					{
						// ALGO[06]
						// min(m≠k, m∈S) {C(S\{k}, m) + d[m,k]}
						π = 0;
						opt = USHRT_MAX;

						code_k = Powered2Code(code, k);
						tempC_k = &tempC->at(code_k);

						for (const auto m : S)
							if (m != k)
							{
								tmp = tempC_k->at(m).cost + distance[m][k];

								if (tmp < opt)
								{
									opt = tmp;
									π = m;
								}
							}

						(*tempCBack)[k] = tempC_k->at(π); // copy path vector

						info = &(*tempCBack)[k];
						info->path.push_back(π);
						info->cost = opt;
						// ALGO[06]
					}

					break;
				}
			}
		}
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

	ALGO:
	00	function algorithm TSP (G, n) is
	01		for k := 2 to n
	02			C({k}, k) := d1,k
	03		for s := 2 to n−1
	04			for all S ⊆ {2, . . . , n}, |S| = s
	05				for all k ∈ S
	06					C(S, k) := minm≠k,m∈S [C(S\{k}, m) + dm,k]
	07		opt := mink≠1 [C({2, 3, . . . , n}, k) + dk, 1]
	08		return (opt)
	09	end function
	*/
	void HeldKarp::Solve(uint_least16_t &opt, string &path)
	{
		// TSP ================================================================================================================================
		// ALGO[01:02]
		{
			AddNewToQueue();

			auto CF1 = &C.front();
			for (uint_least8_t k = 1; k < numberOfNodes; k++)
				(*CF1)[1 << k][k].cost = distance[0][k];
		}
		// ALGO[01:02]

		// ALGO[03:06]
		for (currentCardinality = 2; currentCardinality < numberOfNodes; currentCardinality++) // O(N) cardinalità degli insiemi // ALGO[03]
		{
			AddNewToQueue();

			Combinations(currentCardinality, numberOfNodes - 1); // O(2ⁿ) genera (2^s)-1 insiemi differenti di cardinalità s // ALGO[04]

			C.pop();
			ETLw();
		}
		// ALGO[03:06]
		// TSP ================================================================================================================================

		// PATH ===============================================================================================================================
		// ALGO[07:08]
		{
			uint_least8_t π = 0;
			uint_least16_t tmp;
			opt = USHRT_MAX;

			vector<uint_least8_t> FullSet;
			for (uint_least8_t z = 1; z < numberOfNodes; z++)
				FullSet.push_back(z);

			const auto code = Powered2Code(FullSet);

			for (uint_least8_t k = 1; k < numberOfNodes; k++) // min(k≠0) {C({1, ..., n-1}, k) + d[k,0]} ALGO[07]			
				if (C.front()[code][k].cost > 0)
				{
					tmp = C.front()[code][k].cost + distance[k][0];

					if (tmp < opt)
					{
						opt = tmp;
						π = k;
					}
				}

			C.front()[code][π].cost = opt;
			C.front()[code][π].path.push_back(π);

			path = PrintPath(code, π);
		}
		// ALGO[07:08]
		// PATH ===============================================================================================================================	
	}
}