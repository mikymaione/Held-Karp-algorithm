/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <string>

#include "Christofides.hpp"


Christofides::Christofides(const vector<vector<uint_least8_t>> &DistanceMatrix2D) : TSP(DistanceMatrix2D)
{
	adjlist.resize(numberOfNodes);
}

string Christofides::PrintPath(const uint_least32_t code, const uint_least8_t π)
{
	string s;

	for (auto const e : circuit)
		s = to_string(e) + " " + s;

	return s;
}

uint_least8_t Christofides::getMinIndex(vector<uint_least8_t> &key, vector<bool> &mst)
{
	uint_least8_t i;
	uint_least8_t opt = UINT8_MAX;

	for (uint_least8_t x = 0; x < numberOfNodes; x++)
		if (!mst[x] && key[x] < opt)
		{
			opt = key[x];
			i = x;
		}

	return i;
}

void Christofides::findMST()
{
	vector<uint_least8_t> key(numberOfNodes, UINT8_MAX);
	vector<int_least16_t> parent(numberOfNodes, -1);
	vector<bool> included(numberOfNodes, false);

	key[0] = 0;

	for (uint_least8_t i = 0; i < numberOfNodes - 1; i++)
	{
		auto k = getMinIndex(key, included);

		included[k] = true;

		for (uint_least8_t j = 0; j < numberOfNodes; j++)
			if (distance[k][j] && included[j] == false && distance[k][j] < key[j])
			{
				parent[j] = k;
				key[j] = distance[k][j];
			}
	}

	for (uint_least8_t i = 0; i < numberOfNodes; i++)
	{
		auto j = parent[i];

		if (j != -1)
		{
			adjlist[i].push_back(j);
			adjlist[j].push_back(i);
		}
	}
}

vector<uint_least8_t> Christofides::findOdds()
{
	vector<uint_least8_t> odds;

	for (uint_least8_t i = 0; i < numberOfNodes; i++)
		if (adjlist[i].size() % 2 != 0)
			odds.push_back(i);

	return odds;
}

void Christofides::perfectMatching()
{
	uint_least8_t closest = 0;

	auto odds = findOdds();

	while (!odds.empty())
	{
		auto first = odds.begin();
		auto it = odds.begin() + 1;
		auto tmp = it;
		auto end = odds.end();

		uint_least8_t length = UINT8_MAX;

		for (; it != end; ++it)
			if (distance[*first][*it] < length)
			{
				length = distance[*first][*it];
				closest = *it;
				tmp = it;
			}

		adjlist[*first].push_back(closest);
		adjlist[closest].push_back(*first);

		odds.erase(tmp);
		odds.erase(first);
	}
}

vector<uint_least8_t> Christofides::euler_tour(uint_least8_t start)
{
	stack<uint_least8_t> stack;
	vector<uint_least8_t> path;

	path.push_back(start);

	auto pos = start;
	auto tempList = adjlist; // copy

	while (!stack.empty() || tempList[pos].size() > 0)
		if (tempList[pos].empty())
		{
			path.push_back(pos);
			pos = stack.top();
			stack.pop();
		}
		else
		{
			stack.push(pos);

			auto neighbor = tempList[pos].back();

			tempList[pos].pop_back();

			for (size_t i = 0; i < tempList[neighbor].size(); i++)
				if (tempList[neighbor][i] == pos)
					tempList[neighbor].erase(tempList[neighbor].begin() + i);

			pos = neighbor;
		}

	path.push_back(pos);

	return path;
}

uint_least16_t Christofides::make_hamiltonian(vector<uint_least8_t> &path)
{
	vector<bool> visited(numberOfNodes, false);
	uint_least16_t pathCost = 0;

	auto root = path.front();
	auto cur = path.begin();
	auto iter = path.begin() + 1;

	visited[root] = 1;

	while (iter != path.end())
		if (visited[*iter])
		{
			iter = path.erase(iter);
		}
		else
		{
			pathCost += distance[*cur][*iter];
			cur = iter;
			visited[*cur] = 1;
			iter = cur + 1;
		}

	if (iter != path.end())
		pathCost += distance[*cur][*iter];

	return pathCost;
}

uint_least16_t Christofides::findBestPath(uint_least8_t start)
{
	auto path = euler_tour(start);
	auto len = make_hamiltonian(path);

	return len;
}

void Christofides::Solve(uint_least16_t &opt, string &path)
{
	uint_least16_t bestIndex;

	findMST();
	perfectMatching();

	{
		vector<Christofides::e2<uint_least8_t, uint_least16_t>> path_vals(numberOfNodes);
		uint_least16_t best = UINT16_MAX;

		for (uint_least8_t t = 0; t < numberOfNodes; t++)
		{
			path_vals[t].a = t;
			path_vals[t].b = findBestPath(t);

			if (path_vals[t].b < best)
			{
				bestIndex = path_vals[t].a;
				best = path_vals[t].b;
			}
		}
	}

	circuit = euler_tour(bestIndex);
	opt = make_hamiltonian(circuit);
	path = PrintPath(0, 0);

	currentCardinality = numberOfNodes;
}