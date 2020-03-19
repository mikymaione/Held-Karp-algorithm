/*
MIT License
Copyright (c) 2020: Michele Maione
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
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stack>
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

	void Combinations(const int K, const int N, function<void(vector<unsigned char>)> CALLBACK)
	{
		vector<unsigned char> result(K);

		stack<unsigned char> stack;
		stack.push(0);

		while (stack.size() > 0)
		{
			auto index = stack.size() - 1;
			auto value = stack.top();
			stack.pop();

			while (value < N)
			{
				result[index++] = ++value;
				stack.push(value);

				if (index == K)
				{
					CALLBACK(result);
					break;
				}
			}
		}
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

		unsigned char π;;
		unsigned short opt;

		const unsigned char N0 = N - 1;

		map<unsigned long, map<unsigned char, unsigned short>> C;
		map<unsigned long, map<unsigned char, unsigned char>> P;

		vector<unsigned char> FullSet(N0);

		for (unsigned char z = 1; z < N; z++)
			FullSet[z - 1] = z;

		for (unsigned char k = 1; k < N; k++)
			C[0][k] = distance[k][0];

		for (unsigned char K = 1; K < N; K++) // cardinalità O(N)
		{
			Combinations(K, N0, [&](vector<unsigned char> &S)
			{
				for (unsigned char k = 1; k < N; k++) // nodi O(N)
					if (!binary_search(S.begin(), S.end(), k))
					{
						π = 0;
						opt = USHRT_MAX;

						for each (auto m in S)
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
			});
		}

		π = 0;
		opt = USHRT_MAX;

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