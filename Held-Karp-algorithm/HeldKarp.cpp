/*
MIT License
Copyright (c) 2019: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "pch.h"
#include "HeldKarp.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

/*
Implementation of [Held-Karp](https://en.wikipedia.org/wiki/Held-Karp_algorithm), an algorithm that solves the Traveling Salesman Problem using dynamic programming with memoization.
Held-Karp solves the problem by calculating lowest cost on subsets of the larger problem, and re-uses the intermediate results.
T(n) = O(n²2ⁿ)
S(n) = O(n2ⁿ)
*/

void HeldKarp::TSP(vector<vector<unsigned short>> distance)
{
	auto begin = chrono::steady_clock::now();

	unsigned short π, opt;
	map<unsigned short, map<unsigned short, unsigned short>> C, P;

	const unsigned short N = distance.size();
	vector<unsigned short> FullSet(N - 1);

	for (unsigned short z = 1; z < N; z++)
		FullSet[z - 1] = z;

	auto sets = GetAllCombos(FullSet);

	for (unsigned short k = 1; k < N; k++)
		C[0][k] = distance[k][0];

	for each(auto S in sets)
		for (unsigned short k = 1; k < N; k++)
			if (S.count(k) == 0)
			{
				opt = USHRT_MAX;
				π = 0;

				if (S.empty())
					opt = distance[k][0];
				else
					for each(auto m in S)
					{
						auto tmp = C[Powered2Code(S, m)][m] + distance[k][m];

						if (tmp < opt)
						{
							opt = tmp;
							π = m;
						}
					}

				C[Powered2Code(S)][k] = opt;
				P[Powered2Code(S)][k] = π;
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

	cout << "Grafo di " << N << " nodi, costo: " << opt << " tempo: " << chrono::duration_cast<chrono::milliseconds> (chrono::steady_clock::now() - begin).count() << "ms, percorso: " << path << endl;
}

string HeldKarp::PrintTour(map<unsigned short, map<unsigned short, unsigned short>> P, unsigned short N)
{
	string path;
	unsigned short s = 0;
	set<unsigned short>  S;

	for (unsigned short i = 0; i < N; i++)
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

template <class IEnumerable>
unsigned short HeldKarp::Powered2Code(IEnumerable S)
{
	return Powered2Code(S, USHRT_MAX);
}

template <class IEnumerable>
unsigned short HeldKarp::Powered2Code(IEnumerable S, const unsigned short exclude)
{
	unsigned short code = 0;

	for each(auto e in S)
		if (e != exclude)
			code += 1 << e;

	return code;
}

template <class T>
vector<set<T>> HeldKarp::GetAllCombos(vector<T> list)
{
	auto s = -1;
	auto N = 1 << list.size();
	vector<set<T>> sets(N - 1);

	for (auto i = 1; i < N; i++)
	{
		set<T> last;

		for (auto j = 0; j < list.size(); j++)
			if ((i >> j) % 2 != 0)
				last.insert(list[j]);

		sets[s += 1] = last;
	}

	sort(sets.begin(), sets.end(), [](set<T> x, set<T> y)
	{
		return x.size() < y.size();
	});

	return sets;
}