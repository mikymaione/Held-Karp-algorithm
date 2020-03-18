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

	set<unsigned char> * GetAllCombos(vector<unsigned char> FullSet, const unsigned long N)
	{
		unsigned long i, b;
		size_t j;

		auto stop = N + 1;
		auto list_size = FullSet.size();
		auto sets = new set<unsigned char>[N];

		set<unsigned char> * last;

		for (i = 1; i < stop; i++)
		{
			last = new set<unsigned char>();

			for (j = 0; j < list_size; j++)
			{
				b = i & (1 << j);

				if (b == 0)
					continue;

				last->insert(FullSet[j]);
			}

			sets[i - 1] = *last;
		}

		sort(sets, sets + N, [](set<unsigned char> x, set<unsigned char> y)
		{
			return x.size() < y.size();
		});

		return sets;
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

		unsigned char π;
		unsigned short opt;
		map<unsigned long, map<unsigned char, unsigned char>> C, P;

		vector<unsigned char> FullSet(N - 1);

		for (unsigned char z = 1; z < N; z++)
			FullSet[z - 1] = z;

		const unsigned long setsCount = (1 << (N - 1)) - 1;
		auto sets = GetAllCombos(FullSet, setsCount);

		for (unsigned char k = 1; k < N; k++)
			C[0][k] = distance[k][0];

		set<unsigned char> * S;

		for (unsigned long s = 0; s < setsCount; s++)
		{
			S = &sets[s];

			for (unsigned char k = 1; k < N; k++)
				if (S->count(k) == 0)
				{
					π = 0;

					if (S->empty())
					{
						opt = distance[k][0];
					}
					else
					{
						opt = USHRT_MAX;

						for each(auto m in *S)
						{
							auto tmp = C[Powered2Code(*S, m)][m] + distance[k][m];

							if (tmp < opt)
							{
								opt = tmp;
								π = m;
							}
						}
					}

					C[Powered2Code(*S)][k] = opt;
					P[Powered2Code(*S)][k] = π;
				}
		}

		opt = USHRT_MAX;
		π = 0;

		for each(auto k in FullSet)
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