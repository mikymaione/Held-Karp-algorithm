/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
		uint16_t cost;
		vector<uint8_t> path;
	};

	// <Cardinality, <Code, <Node, Data>>>
	unordered_map<uint8_t, unordered_map<uint32_t, unordered_map<uint8_t, sInfo>>> C;

	const vector<vector<uint8_t>> distance;
	const uint8_t numberOfNodes;

private:
	template <class IEnumerable>
	uint32_t Powered2Code(const IEnumerable &S);

	template <class IEnumerable>
	uint32_t Powered2Code(const IEnumerable &S, const uint8_t exclude);

	uint32_t Powered2Code(const uint32_t code, const uint8_t exclude);

	void Combinations(const uint8_t K, const uint8_t N);

	string PrintPath(const uint32_t code, const uint8_t π);

	template <class T>
	static T generateRandomNumber(const T startRange, const T endRange, const T limit);

public:
	HeldKarp(const vector<vector<uint8_t>> &DistanceMatrix2D);

	void TSP();

	static vector<vector<uint8_t>> New_RND_Distances(const uint8_t Size_of_RandomDistanceCosts);

};