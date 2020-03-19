/*
MIT License
Copyright (c) 2019: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
unsigned short
16 bit
0 to 65.535
%hu

unsigned int
32 bit
0 to 4.294.967.295
%u
*/
#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class HeldKarp
{
private:
	map<unsigned char, unsigned long long> FACT;

	string PrintTour(map<unsigned long, map<unsigned char, unsigned char>> P, const unsigned char N)
	{
		string path;
		unsigned char s = 0;
		set<unsigned char>  S;

		for (unsigned char i = 0; i < N; i++)
			S.insert(i);

		while (true)
		{
			S.erase(s);
			s = P[Powered2Code(S)][s];

			path += to_string(s) + " ";

			if (s == 0)
				break;
		}

		return path;
	}

	unsigned long long factorial(unsigned char n)
	{
		if (FACT.count(n) == 0)
		{
			unsigned long long r = 1;

			for (unsigned char i = 1; i <= n; i++)
				r *= i;

			FACT[n] = r;
		}

		return FACT[n];
	}

	unsigned long long combinations(unsigned char N, unsigned char K)
	{
		return factorial(N) / (factorial(K) * factorial(N - K));
	}

	unsigned char ** GetAllCombos_Permutation(const unsigned long long tot_combinations, const unsigned char N, const unsigned char K)
	{
		string bitmask(K, 1);
		bitmask.resize(N, 0);

		unsigned long z = 0;
		short e = -1;

		auto sets = new unsigned char*[tot_combinations];

		do
		{
			e = -1;
			sets[z] = new unsigned char[K];

			for (unsigned char i = 0; i < N; ++i)
				if (bitmask[i])
					sets[z][e += 1] = (i + 1);

			z++;
		} while (prev_permutation(bitmask.begin(), bitmask.end()));

		return sets;
	}

	template <class T>
	unsigned long Powered2Code(const unsigned char N, T * S)
	{
		return Powered2Code(N, S, UCHAR_MAX);
	}

	template <class T>
	unsigned long Powered2Code(const unsigned char N, T * S, const unsigned char exclude)
	{
		unsigned long code = 0;

		for (unsigned long i = 0; i < N; i++)
			if (S[i] != exclude)
				code += 1 << S[i];

		return code;
	}

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable S)
	{
		return Powered2Code(S, UCHAR_MAX);
	}

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable S, const unsigned char exclude)
	{
		unsigned long code = 0;

		for each(auto e in S)
			if (e != exclude)
				code += 1 << e;

		return code;
	}

	template <class T>
	bool arrayContains(const unsigned long N, T * arr, T e)
	{
		for (unsigned long i = 0; i < N; i++)
			if (arr[i] == e)
				return true;

		return false;
	}

public:
	/*
	Implementation of [Held-Karp](https://en.wikipedia.org/wiki/Held-Karp_algorithm), an algorithm that solves the Traveling Salesman Problem using dynamic programming with memoization.
	Held-Karp solves the problem by calculating lowest cost on subsets of the larger problem, and re-uses the intermediate results.
	T(n) = O(n²2ⁿ)
	S(n) = O(n2ⁿ)
	*/
	template <class Matrix2D>
	void TSP(Matrix2D distance, const unsigned char N)
	{
		auto begin = chrono::steady_clock::now();

		const unsigned char N0 = N - 1;

		unsigned char π;
		unsigned short opt;

		map<unsigned long, map<unsigned char, unsigned short>> C;
		map<unsigned long, map<unsigned char, unsigned char>> P;

		vector<unsigned char> FullSet(N0);

		for (unsigned char z = 1; z < N; z++)
			FullSet[z - 1] = z;

		for (unsigned char k = 1; k < N; k++)
			C[0][k] = distance[k][0];

		for (unsigned char K = 1; K < N; K++) // cardinalità O(N)
		{
			auto tot_combinations = combinations(N0, K);
			auto permutation_of_k = GetAllCombos_Permutation(tot_combinations, N0, K);

			for (unsigned long long pk = 0; pk < tot_combinations; pk++) // sets O(2ⁿ)
			{
				auto S = permutation_of_k[pk];

				for (unsigned char k = 1; k < N; k++) // nodi O(N)
					if (!arrayContains(K, S, k))
					{
						π = 0;
						opt = USHRT_MAX;

						for (unsigned char b = 0; b < K; b++)
						{
							auto m = S[b];
							auto tmp = C[Powered2Code(K, S, m)][m] + distance[k][m];

							if (tmp < opt)
							{
								opt = tmp;
								π = m;
							}
						}

						C[Powered2Code(K, S)][k] = opt;
						P[Powered2Code(K, S)][k] = π;
					}
			}

			for (unsigned long long i = 0; i < tot_combinations; i++)
				delete[] permutation_of_k[i];

			delete[] permutation_of_k;
		}

		opt = USHRT_MAX;
		π = 0;

		for each(auto k in FullSet) // O(N)
		{
			auto tmp = C[Powered2Code(FullSet, k)][k] + distance[0][k];

			if (tmp < opt)
			{
				opt = tmp;
				π = k;
			}
		}

		P[Powered2Code(FullSet)][0] = π;

		auto path = PrintTour(P, N);

		cout << "Grafo di " << to_string(N) << " nodi, costo: " << to_string(opt) << " tempo: " << chrono::duration_cast<chrono::milliseconds> (chrono::steady_clock::now() - begin).count() << "ms, percorso: " << path << endl;
	}

};