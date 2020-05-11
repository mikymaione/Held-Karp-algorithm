/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <string>

#include "Christofides.hpp"

namespace TSP
{
	Christofides::Christofides(const vector<vector<uint_least8_t>> &DistanceMatrix2D) : TSP(DistanceMatrix2D)
	{
		Adj.resize(numberOfNodes);
	}

	string Christofides::PrintPath(const uint_least32_t code, const uint_least8_t π)
	{
		string s;

		for (auto const e : circuit)
			s = to_string(e) + " " + s;

		return s;
	}

	void Christofides::findMST()
	{
		uint_least8_t i, u, v, opt;

		vector<uint_least8_t> parent(numberOfNodes, UINT_LEAST8_MAX);
		vector<uint_least8_t> key(numberOfNodes, UINT_LEAST8_MAX);
		vector<bool> in_mst(numberOfNodes, false);

		key[0] = 0;

		for (i = 0; i < numberOfNodes - 1; i++)
		{
			opt = UINT_LEAST8_MAX;

			for (u = 0; u < numberOfNodes; u++)
				if (!in_mst[u] && key[u] < opt)
				{
					opt = key[u];
					v = u;
				}

			in_mst[v] = true;

			for (u = 0; u < numberOfNodes; u++)
				if (distance[v][u] && !in_mst[u] && distance[v][u] < key[u])
				{
					parent[u] = v;
					key[u] = distance[v][u];
				}
		}

		for (u = 0; u < numberOfNodes; u++)
		{
			v = parent[u];

			if (v != UINT_LEAST8_MAX)
			{
				Adj[u].push_back(v);
				Adj[v].push_back(u);
			}
		}
	}

	set<uint_least8_t> Christofides::findOdds()
	{
		set<uint_least8_t> odds;

		for (uint_least8_t i = 0; i < numberOfNodes; i++)
			if (Adj[i].size() % 2 != 0)
				odds.insert(i);

		return odds;
	}

	void Christofides::perfectMatching()
	{
		uint_least8_t closest = 0;

		auto odds = findOdds();

		while (!odds.empty())
			for (const auto v : odds)
			{
				uint_least8_t length = UINT_LEAST8_MAX;
				odds.erase(v);

				for (const auto u : odds)
					if (distance[u][v] < length)
					{
						length = distance[u][v];
						closest = u;
					}

				Adj[v].push_back(closest);
				Adj[closest].push_back(v);

				odds.erase(closest);
				break;
			}
	}

	vector<uint_least8_t> Christofides::euler_tour(uint_least8_t start)
	{
		stack<uint_least8_t> stack;
		vector<uint_least8_t> path;

		path.push_back(start);

		auto pos = start;
		auto tempList = Adj; // copy

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
		uint_least8_t bestIndex;

		findMST();
		perfectMatching();

		{
			vector<Christofides::e2<uint_least8_t, uint_least16_t>> path_vals(numberOfNodes);
			uint_least16_t min = UINT_LEAST16_MAX;

			for (uint_least8_t t = 0; t < numberOfNodes; t++)
			{
				path_vals[t].a = t;
				path_vals[t].b = findBestPath(t);

				if (path_vals[t].b < min)
				{
					bestIndex = path_vals[t].a;
					min = path_vals[t].b;
				}
			}
		}

		circuit = euler_tour(bestIndex);
		opt = make_hamiltonian(circuit);

		{
			auto head = circuit.front();
			auto tail = circuit.back();

			circuit.push_back(head);

			opt += distance[tail][head];
		}

		path = PrintPath(0, 0);

		currentCardinality = numberOfNodes;
	}
}