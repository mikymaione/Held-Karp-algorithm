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

#include <unordered_map>
#include <vector>

using namespace std;

class HeldKarp
{
private:
	struct sInfo
	{
		unsigned short cost;
		vector<unsigned char> path;
	};

	// <Cardinality, <Node, <Code, Data>>>
	unordered_map<unsigned char, unordered_map<unsigned int, unordered_map<unsigned char, sInfo>>> C;

	const vector<vector<unsigned char>> distance;
	const unsigned char numberOfNodes;

private:
	template <class IEnumerable>
	unsigned int Powered2Code(const IEnumerable &S);

	template <class IEnumerable>
	unsigned int Powered2Code(const IEnumerable &S, const unsigned char exclude);

	unsigned int Powered2Code(unsigned int code, const unsigned char exclude);

	void CombinationPart(const vector<unsigned char> &S, const unsigned char s);
	void Combinations(const unsigned char K, const unsigned char N);

	string PrintPath(const unsigned int code, const unsigned char π);

	template <class T>
	static T generateRandomNumber(const T startRange, const T endRange, const T limit);

public:
	HeldKarp(const vector<vector<unsigned char>> &DistanceMatrix2D);

	void TSP();

	static vector<vector<unsigned char>> New_RND_Distances(const unsigned char Size_of_RandomDistanceCosts);

};