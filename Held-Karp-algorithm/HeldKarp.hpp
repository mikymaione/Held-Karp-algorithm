﻿/*
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
#include <unordered_map>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class HeldKarp
{
private:
	string PrintTour(unordered_map<unsigned long, unordered_map<unsigned char, unsigned char>> P, const unsigned char N)
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

	void Combinations(const int K, const int N, function<void(const unsigned char S[], const unsigned long long elements)> CALLBACK)
	{
		unsigned long long index;
		unsigned char value;

		auto result = new unsigned char[K];

		stack<unsigned char> stack;
		stack.push(0);

		while (stack.size() > 0)
		{
			index = stack.size() - 1;
			value = stack.top();
			stack.pop();

			while (value < N)
			{
				result[index++] = ++value;
				stack.push(value);

				if (index == K)
				{
					CALLBACK(result, index);
					break;
				}
			}
		}

		delete[] result;
	}

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable S)
	{
		return Powered2Code(S, UCHAR_MAX);
	}

	template <class IEnumerable>
	unsigned int Powered2Code(IEnumerable S, const unsigned char exclude)
	{
		unsigned long code = 0;

		for each(auto e in S)
			if (e != exclude)
				code += 1 << e;

		return code;
	}

	template <class T>
	unsigned long Powered2Code(const T S[], unsigned long long len)
	{
		return Powered2Code(S, len, UCHAR_MAX);
	}

	template <class T>
	unsigned long Powered2Code(const T S[], unsigned long long len, const unsigned char exclude)
	{
		unsigned long code = 0;

		for (unsigned long long i = 0; i < len; i++)
		{
			auto e = S[i];

			if (e != exclude)
				code += 1 << e;
		}

		return code;
	}

	template <class T>
	bool binSearch(const T arr[], unsigned long long len, T what)
	{
		long long low = 0;
		long long high = len - 1;
		long long mid;

		while (low <= high)
		{
			mid = (low + high) / 2;

			if (arr[mid] > what)
				high = mid - 1;
			else if (arr[mid] < what)
				low = mid + 1;
			else
				return true;
		}

		return false;
	}

public:
	/*
	Implementation of [Held-Karp](https://en.wikipedia.org/wiki/Held-Karp_algorithm), an algorithm that solves the Traveling Salesman Problem using dynamic programming with memoization.
	Held-Karp solves the problem by calculating lowest cost on subsets of the larger problem, and re-uses the intermediate results.
	T(n) = O(n²2ⁿ)
	S(n) = O(n2ⁿ)
	*/
	template <class T, size_t N, size_t W>
	void TSP(T(&distance)[N][W])
	{
		auto begin = chrono::steady_clock::now();

		unsigned char π, m, k;
		unsigned short opt, tmp;
		unsigned int code;
		unsigned long long i;

		const unsigned char N0 = N - 1;

		unordered_map<unsigned long, unordered_map<unsigned char, unsigned short>> C;
		unordered_map<unsigned long, unordered_map<unsigned char, unsigned char>> P;

		vector<unsigned char> FullSet(N0);

		for (unsigned char z = 1; z < N; z++)
			FullSet[z - 1] = z;

		for (k = 1; k < N; k++)
			C[0][k] = distance[k][0];

		for (unsigned char K = 1; K < N; K++) // cardinalità O(N)
		{
			Combinations(K, N0, [&](const unsigned char S[], const unsigned long long elements)
			{
				for (k = 1; k < N; k++) // nodi O(N)
					if (!binSearch(S, elements, k))
					{
						π = 0;
						opt = USHRT_MAX;

						for (i = 0; i < elements; i++)
						{
							m = S[i];
							tmp = C[Powered2Code(S, elements, m)][m] + distance[k][m];

							if (tmp < opt)
							{
								opt = tmp;
								π = m;
							}
						}

						code = Powered2Code(S, elements);

						C[code][k] = opt;
						P[code][k] = π;
					}
			});
		}

		π = 0;
		opt = USHRT_MAX;

		for each(auto e in FullSet) // O(N)
		{
			tmp = C[Powered2Code(FullSet, e)][e] + distance[0][e];

			if (tmp < opt)
			{
				opt = tmp;
				π = e;
			}
		}

		P[Powered2Code(FullSet)][0] = π;

		auto path = PrintTour(P, N);

		cout << "Grafo di " << to_string(N) << " nodi, costo: " << to_string(opt) << " tempo: " << chrono::duration_cast<chrono::milliseconds> (chrono::steady_clock::now() - begin).count() << "ms, percorso: " << path << endl;
	}

};