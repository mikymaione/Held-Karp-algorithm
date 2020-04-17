/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <atomic>

#include <chrono>

#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;


class HeldKarp
{
private:
	struct sInfo
	{
		uint_least16_t cost;
		vector<uint_least8_t> path;
	};

	// <Code, <Node, Data>>
	queue<unordered_map<uint_least32_t, unordered_map<uint_least8_t, sInfo>>> C;

	const vector<vector<uint_least8_t>> distance;
	const uint_least8_t numberOfNodes;

	time_point<system_clock> begin;
	atomic<uint_least8_t> currentCardinality = 2;
	atomic<bool> writingBuffer = false;

private:
	template <class IEnumerable>
	uint_least32_t Powered2Code(const IEnumerable &S);

	template <class IEnumerable>
	uint_least32_t Powered2Code(const IEnumerable &S, const uint_least8_t exclude);

	uint_least32_t Powered2Code(const uint_least32_t code, const uint_least8_t exclude);

	void Combinations(const uint_least8_t K, const uint_least8_t N);
	void AddNewToQueue();
	void Combinations_FreeMem(stack<uint_least8_t> &Q, vector<uint_least8_t> &S, const uint_least8_t K, const uint_least8_t N, const uint_least8_t sCur);

	string PrintPath(const uint_least32_t code, const uint_least8_t π);
	void ETL();
	void ETLw();

	template <class T>
	static T generateRandomNumber(const T startRange, const T endRange, const T limit);

public:
	HeldKarp(const vector<vector<uint_least8_t>> &DistanceMatrix2D);

	void TSP();

	static vector<vector<uint_least8_t>> New_RND_Distances(const uint_least8_t Size_of_RandomDistanceCosts);

};