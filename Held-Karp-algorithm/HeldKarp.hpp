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
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <vector>

using namespace std;

class HeldKarp
{
protected:
	// TSP ========================================================	
	vector<vector<unsigned char>> distance;
	unsigned char numberOfNodes;
	// TSP ========================================================


	// Multi Thread ===============================================
	const unsigned char minCpus = 3;
	unsigned char concurentThreadsSupported;
	bool useMultiThreading;
	// Multi Thread ===============================================

protected:
	virtual void CSave() = 0;
	virtual void CLoadAll() = 0;

	virtual void CSet(const unsigned long code, const unsigned char key, const unsigned short val) = 0;
	virtual void PSet(const unsigned long code, const unsigned char key, const unsigned char val) = 0;

	virtual unsigned short CGet(const unsigned long code, const unsigned char key) = 0;
	virtual unsigned char PGet(const unsigned long code, const unsigned char key) = 0;

	string PrintTour(const unsigned char N)
	{
		string path;
		unsigned char s = 0;
		set<unsigned char>  S;

		for (unsigned char i = 0; i < N; i++)
			S.insert(i);

		while (true)
		{
			S.erase(s);

			try
			{
				s = PGet(Powered2Code(S), s);
			}
			catch (...)
			{
				s = 0;
				// end.
			}

			path += to_string(s) + " ";

			if (s == 0)
				break;
		}

		return path;
	}

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable &S)
	{
		return Powered2Code(S, UCHAR_MAX);
	}

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable &S, const unsigned char exclude)
	{
		unsigned long code = 0;

		for (auto e : S)
			if (e != exclude)
				code += 1 << e;

		return code;
	}

	void CombinationPart(vector<unsigned char> S, const unsigned char s)
	{
		unsigned char k, π;
		unsigned short opt, tmp;
		unsigned long code;

		for (k = 1; k < numberOfNodes; k++)
			if (!binary_search(S.begin(), S.end(), k)) // S\{k}			
			{
				π = 0;
				opt = USHRT_MAX;

				for (auto m : S) // min(m≠k, m∈S) {C(S\{k}, m) + d[m,k]}
				{
					tmp = CGet(Powered2Code(S, m), m) + distance[k][m];

					if (tmp < opt)
					{
						opt = tmp;
						π = m;
					}
				}

				code = Powered2Code(S);

				CSet(code, k, opt);
				PSet(code, k, π);
			}
	}

	void Combinations(const unsigned char K, const unsigned char N)
	{
		unsigned long long i;
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

		CSave();
	}

	void waitForThreads(vector<thread> &threads)
	{
		for (auto & th : threads)
			if (th.joinable())
				th.join();

		threads.clear();
	}

	template <class T>
	static T generateRandomNumber(T startRange, T endRange, T limit)
	{
		T r = rand();

		T range = endRange - startRange;
		range++;

		T num = r % range + startRange;

		return num;
	}

public:
	HeldKarp(vector<vector<unsigned char>> &DistanceMatrix2D, int numThreads)
	{
		distance = DistanceMatrix2D;
		numberOfNodes = (unsigned char)distance.size();

		auto hc = thread::hardware_concurrency();

		if (numThreads > hc)
			numThreads = hc;

		concurentThreadsSupported = (numThreads >= minCpus ? numThreads - 2 : 0);

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

	T(n) = O(n²2ⁿ)
	S(n) = O(n2ⁿ)
	*/
	void TSP()
	{
		auto begin = chrono::steady_clock::now();

		cout
			<< "Using "
			<< to_string(concurentThreadsSupported)
			<< " threads to solve a graph of "
			<< to_string(numberOfNodes)
			<< " nodes... ";

		// TSP ================================================================================================================================				
		// insieme vuoto
		for (auto k = 1; k < numberOfNodes; k++)
			CSet(0, k, distance[k][0]);

		CSave();

		for (unsigned char s = 1; s < numberOfNodes; s++) // O(N) cardinalità degli insiemi				
			Combinations(s, numberOfNodes - 1); // O(2ⁿ) genera (2^s)-1 insiemi differenti di cardinalità s				
		// TSP ================================================================================================================================

		// PATH ===============================================================================================================================
		unsigned char π = 0;
		unsigned short tmp, opt = USHRT_MAX;
		unsigned long code;

		vector<unsigned char> FullSet(numberOfNodes - 1);
		for (unsigned char z = 1; z < numberOfNodes; z++)
			FullSet[z - 1] = z;

		CLoadAll();

		for (auto e : FullSet) // min(k≠0) {C({1, ..., n-1}, k) + d[k,0]}
		{
			code = Powered2Code(FullSet, e);

			if (CGet(code, e) > 0)
			{
				tmp = CGet(code, e) + distance[0][e];

				if (tmp < opt)
				{
					opt = tmp;
					π = e;
				}
			}
		}

		PSet(Powered2Code(FullSet), 0, π);

		auto path = PrintTour(numberOfNodes);
		// PATH ===============================================================================================================================

		cout
			<< "Solved! Cost: "
			<< to_string(opt)
			<< " time: "
			<< chrono::duration_cast<chrono::milliseconds> (chrono::steady_clock::now() - begin).count()
			<< "ms, path: "
			<< path
			<< endl;
	}

	static vector<vector<unsigned char>> New_RND_Distances(const unsigned char Size_of_RandomDistanceCosts)
	{
		vector<vector<unsigned char>> A(Size_of_RandomDistanceCosts, vector<unsigned char>(Size_of_RandomDistanceCosts, 0));

		for (auto x = 0; x < Size_of_RandomDistanceCosts; x++)
			for (auto y = 0; y < Size_of_RandomDistanceCosts; y++)
				A[x][y] = (x == y ? 0 : generateRandomNumber(1, 25, UCHAR_MAX));

		return A;
	}

};