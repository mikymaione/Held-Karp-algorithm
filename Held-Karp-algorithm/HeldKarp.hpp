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

#include <set>
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
	virtual void RemoveCardinality(const unsigned char K) = 0;

	virtual void CSet(const unsigned char cardinality, const unsigned long code, const unsigned char key, const unsigned short val) = 0;
	virtual void PSet(const unsigned char cardinality, const unsigned long code, const unsigned char key, const unsigned char val) = 0;

	virtual unsigned short CGet(const unsigned char cardinality, const unsigned long code, const unsigned char key) = 0;
	virtual unsigned char PGet(const unsigned char cardinality, const unsigned long code, const unsigned char key) = 0;

	string PrintTour(set<unsigned char> S);

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable &S);

	template <class IEnumerable>
	unsigned long Powered2Code(IEnumerable &S, const unsigned char exclude);

	unsigned long Powered2Code(unsigned long code, const unsigned char exclude);

	void CombinationPart(vector<unsigned char> S, const unsigned char s);

	void Combinations(const unsigned char K, const unsigned char N);

	void waitForThreads(vector<thread> &threads);

	template <class T>
	static T generateRandomNumber(T startRange, T endRange, T limit);

public:
	HeldKarp(vector<vector<unsigned char>> &DistanceMatrix2D, const unsigned int numThreads);

	void TSP();

	static vector<vector<unsigned char>> New_RND_Distances(const unsigned char Size_of_RandomDistanceCosts);

};