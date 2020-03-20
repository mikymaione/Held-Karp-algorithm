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
#include <mutex>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace std;

class HeldKarp
{
private:
	mutex MUTEX;

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

	template <class T>
	T * clonaArray(const T S[], unsigned char len)
	{
		auto R = new T[len];

		for (unsigned char i = 0; i < len; i++)
			R[i] = S[i];

		return R;
	}

	void Combinations(const unsigned char K, const unsigned char N, function<void(const unsigned char[])> CALLBACK)
	{
		unsigned long long i;
		unsigned char s;

		auto R = new unsigned char[K];

		vector<unsigned char *> mem;
		vector<thread> threads;

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
					auto Z = clonaArray(R, K);
					mem.push_back(Z);

					threads.push_back(thread(CALLBACK, Z));
					break;
				}
			}
		}

		for (auto x = 0; x < threads.size(); x++)
			if (threads.at(x).joinable())
				threads.at(x).join();

		for each (auto Z in mem)
			delete[] Z;

		delete[] R;
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
	unsigned long Powered2Code(const T S[], unsigned char len)
	{
		return Powered2Code(S, len, UCHAR_MAX);
	}

	template <class T>
	unsigned long Powered2Code(const T S[], unsigned char len, const unsigned char exclude)
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
	bool binSearch(const T arr[], unsigned char len, T what)
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
	Held–Karp algorithm

	The Held–Karp algorithm, also called Bellman–Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).
	TSP is an extension of the Hamiltonian circuit problem.
	The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
		1. visit every city just once
		2. return to the starting point
		3. be of minimum distance.

	T(n) = O(n²2ⁿ)
	S(n) = O(n2ⁿ)
	*/
	template <class T, size_t N, size_t W>
	void TSP(T(&distance)[N][W])
	{
		auto begin = chrono::steady_clock::now();
		const unsigned char N0 = N - 1;

		unordered_map<unsigned long, unordered_map<unsigned char, unsigned short>> C;
		unordered_map<unsigned long, unordered_map<unsigned char, unsigned char>> P;

		// insieme vuoto
		for (auto k = 1; k < N; k++)
			C[0][k] = distance[k][0];

		for (unsigned char s = 1; s < N; s++) // O(N) cardinalità degli insiemi
		{
			Combinations(s, N0, [&](const unsigned char S[]) // O(2ⁿ) genera (2^s)-1 insiemi differenti di cardinalità s
			{
				for (unsigned char k = 1; k < N; k++)
					if (!binSearch(S, s, k)) // S\{k}
					{
						unsigned char π = 0;
						unsigned short opt = USHRT_MAX;

						for (unsigned char i = 0; i < s; i++) // min(m≠k, m∈S) {C(S\{k}, m) + d[m,k]}
						{
							auto m = S[i];

							MUTEX.lock();
							auto tmp = C[Powered2Code(S, s, m)][m] + distance[k][m];
							MUTEX.unlock();

							if (tmp < opt)
							{
								opt = tmp;
								π = m;
							}
						}

						auto code = Powered2Code(S, s);

						MUTEX.lock();
						C[code][k] = opt;
						P[code][k] = π;
						MUTEX.unlock();
					}
			});
		}

		unsigned char π = 0;
		unsigned short opt = USHRT_MAX;

		vector<unsigned char> FullSet(N0);
		for (unsigned char z = 1; z < N; z++)
			FullSet[z - 1] = z;

		for each(auto e in FullSet) // min(k≠0) {C({1, ..., n-1}, k) + d[k,0]}
		{
			auto tmp = C[Powered2Code(FullSet, e)][e] + distance[0][e];

			if (tmp < opt)
			{
				opt = tmp;
				π = e;
			}
		}

		P[Powered2Code(FullSet)][0] = π;

		auto path = PrintTour(P, N);

		cout
			<< "Grafo di "
			<< to_string(N)
			<< " nodi, costo: "
			<< to_string(opt)
			<< " tempo: "
			<< chrono::duration_cast<chrono::milliseconds> (chrono::steady_clock::now() - begin).count()
			<< "ms, percorso: "
			<< path
			<< endl;
	}

	template <class T>
	T generateRandomNumber(T startRange, T endRange, T limit)
	{
		T r = rand();

		T range = endRange - startRange;
		range++;

		T num = r % range + startRange;

		return num;
	}

	template <class T, size_t rows, size_t cols>
	void grafoRND(T(&A)[rows][cols])
	{
		for (auto x = 0; x < rows; x++)
			for (auto y = 0; y < cols; y++)
				A[x][y] = (x == y ? 0 : generateRandomNumber(1, 25, UCHAR_MAX));
	}

};