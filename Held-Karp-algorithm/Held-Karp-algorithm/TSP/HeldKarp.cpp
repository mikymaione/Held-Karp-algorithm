/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.
*/
#pragma once

#include "HeldKarp.hpp"

namespace TSP
{
	HeldKarp::HeldKarp(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	void HeldKarp::CalcPath(const unsigned int code, const unsigned short π, float &opt, string &path)
	{
		unsigned short p = 0;

		path = "";
		opt = 0;

		float f;

		for (const auto e : C.front()[code][π].path)
		{
			f = distance[p][e];
			opt += f;
			path += to_string(e) + " ";

			p = e;
		}

		path = "0 " + path + "0";
		opt += distance[p][0];
	}

	void HeldKarp::AddNewToQueue()
	{
		map<unsigned int, map<unsigned short, sInfo>> new_map;
		C.push(new_map);
	}

	void HeldKarp::Combinations_FreeMem(stack<unsigned short> &Q, vector<unsigned short> &S, const unsigned short K, const unsigned short N, const unsigned short sCur)
	{
		size_t i;
		unsigned short s;
		unsigned int code;
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

	void HeldKarp::Combinations(const unsigned short K, const unsigned short N)
	{
		size_t i;
		unsigned short π, s;
		float opt, tmp;
		unsigned int code, code_k;

		vector<unsigned short> S(K);
		stack<unsigned short> Q;
		Q.push(0);

		// mem opt	
		vector<unsigned short> S_freeMem(K - 1);
		stack<unsigned short> Q_freeMem;

		const auto tempC = &C.front();
		map<unsigned short, sInfo> *tempC_k, *tempCBack;
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
						opt = FLT_MAX;

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
	Algo. from: https://en.wikipedia.org/wiki/Held%E2%80%93Karp_algorithm
	Algo.:
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
	void HeldKarp::Solve(float &opt, string &path)
	{
		// TSP ================================================================================================================================
		// ALGO[01:02]
		{
			AddNewToQueue();

			auto CF1 = &C.front();
			for (unsigned short k = 1; k < numberOfNodes; k++)
				(*CF1)[1 << k][k].cost = distance[0][k];
		}
		// ALGO[01:02]

		maxCardinality = numberOfNodes;

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
			unsigned short π = 0;
			float tmp;
			opt = FLT_MAX;

			vector<unsigned short> FullSet;
			for (unsigned short z = 1; z < numberOfNodes; z++)
				FullSet.push_back(z);

			const auto code = Powered2Code(FullSet);

			for (unsigned short k = 1; k < numberOfNodes; k++) // min(k≠0) {C({1, ..., n-1}, k) + d[k,0]} ALGO[07]			
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

			CalcPath(code, π, opt, path);
		}
		// ALGO[07:08]
		// PATH ===============================================================================================================================	
	}
}