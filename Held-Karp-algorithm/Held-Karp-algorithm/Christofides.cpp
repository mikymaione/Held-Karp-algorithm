/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <stack>
#include <string>

#include "Christofides.hpp"

namespace TSP
{
	Christofides::Christofides(const vector<vector<uint_least8_t>> &DistanceMatrix2D) : TSP(DistanceMatrix2D)
	{
		Adj.resize(numberOfNodes);
	}

	string Christofides::PrintPath()
	{
		string s;

		for (auto const e : circuit)
			s = to_string(e) + " " + s;

		return s;
	}

	void Christofides::MinimumSpanningTree_Prim()
	{
		uint_least8_t i, u, v, opt;

		vector<uint_least8_t> parent(numberOfNodes, UINT_LEAST8_MAX);
		vector<uint_least8_t> dist(numberOfNodes, UINT_LEAST8_MAX);
		vector<bool> in_mst(numberOfNodes, false);

		dist[0] = 0;

		for (i = 0; i < numberOfNodes - 1; i++)
		{
			opt = UINT_LEAST8_MAX;

			for (u = 0; u < numberOfNodes; u++)
				if (!in_mst[u] && dist[u] < opt)
				{
					opt = dist[u];
					v = u;
				}

			in_mst[v] = true;

			for (u = 0; u < numberOfNodes; u++)
				if (!in_mst[u] && distance[v][u] < dist[u])
				{
					parent[u] = v;
					dist[u] = distance[v][u];
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

	void Christofides::WeightedPerfectMatching()
	{
		uint_least8_t i, dist, closest = 0;
		set<uint_least8_t> V_odd;

		for (i = 0; i < numberOfNodes; i++)
			if (Adj[i].size() % 2 != 0)
				V_odd.insert(i);

		while (!V_odd.empty())
			for (const auto v : V_odd)
			{
				dist = UINT_LEAST8_MAX;
				V_odd.erase(v);

				for (const auto u : V_odd)
					if (distance[u][v] < dist)
					{
						dist = distance[u][v];
						closest = u;
					}

				Adj[v].push_back(closest);
				Adj[closest].push_back(v);

				V_odd.erase(closest);
				break; // next element
			}
	}

	vector<uint_least8_t> Christofides::FindEulerCircuit(uint_least8_t start)
	{
		stack<uint_least8_t> S;
		vector<uint_least8_t> path;

		path.push_back(start);

		auto pos = start;
		auto neighbours = Adj; // copy

		while (!S.empty() || neighbours[pos].size() > 0)
			if (neighbours[pos].empty())
			{
				path.push_back(pos);
				pos = S.top();
				S.pop();
			}
			else
			{
				S.push(pos);

				auto neighbor = neighbours[pos].back();

				neighbours[pos].pop_back();

				for (size_t i = 0; i < neighbours[neighbor].size(); i++)
					if (neighbours[neighbor][i] == pos)
						neighbours[neighbor].erase(neighbours[neighbor].begin() + i);

				pos = neighbor;
			}

		path.push_back(pos);

		return path;
	}

	uint_least16_t Christofides::ToHamiltonianPath(vector<uint_least8_t> &path)
	{
		vector<bool> visited(numberOfNodes, false);
		uint_least16_t pathCost = 0;

		auto cur = path.begin();
		auto next = path.begin() + 1;

		visited[*cur] = 1;

		while (next != path.end())
			if (visited[*next])
			{
				next = path.erase(next);
			}
			else
			{
				pathCost += distance[*cur][*next];
				cur = next;
				visited[*cur] = 1;
				next = cur + 1;
			}

		if (next != path.end())
			pathCost += distance[*cur][*next];

		return pathCost;
	}

	uint_least16_t Christofides::findBestPath(uint_least8_t start)
	{
		auto path = FindEulerCircuit(start);
		auto len = ToHamiltonianPath(path);

		return len;
	}

	void Christofides::Solve(uint_least16_t &opt, string &path)
	{
		uint_least8_t bestIndex;

		MinimumSpanningTree_Prim();
		WeightedPerfectMatching();

		{
			struct e2
			{
				uint_least8_t node;
				uint_least16_t cost;
			};

			vector<e2> path_vals(numberOfNodes);
			uint_least16_t min = UINT_LEAST16_MAX;

			for (uint_least8_t t = 0; t < numberOfNodes; t++)
			{
				path_vals[t].node = t;
				path_vals[t].cost = findBestPath(t);

				if (path_vals[t].cost < min)
				{
					bestIndex = path_vals[t].node;
					min = path_vals[t].cost;
				}
			}
		}

		circuit = FindEulerCircuit(bestIndex);
		opt = ToHamiltonianPath(circuit);

		{
			auto head = circuit.front();
			auto tail = circuit.back();

			circuit.push_back(head);

			opt += distance[tail][head];
		}

		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}