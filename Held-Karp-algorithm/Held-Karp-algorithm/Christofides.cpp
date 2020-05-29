/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <queue>

#include "Christofides.hpp"

namespace TSP
{
	Christofides::Christofides(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) // O(V)
	{
		out_star.resize(numberOfNodes);
	}

	string Christofides::PrintPath(vector<unsigned short> circuit) // O(V)
	{
		string s;

		for (auto const e : circuit)
			s = to_string(e) + " " + s;

		return s;
	}

	void Christofides::MinimumSpanningTree_Prim() // O(E ㏒ V)
	{
		struct Node
		{
			unsigned short id, π;
			float key;

			bool operator < (const Node &n) const
			{
				return (key < n.key);
			}
		};

		priority_queue<Node*, vector<Node*>, less<Node*>> Q; // sorted min queue
		set<size_t> S; // elements available
		vector<Node> V(numberOfNodes);

		for (size_t u = 0; u < numberOfNodes; u++)
		{
			V[u].id = u;
			V[u].key = FLT_MAX;
			V[u].π = UINT16_MAX;

			S.insert(u);
		}

		auto r = &V[0];
		r->key = 0;
		Q.push(r);

		while (!Q.empty())
		{
			auto u = Q.top()->id; // min
			Q.pop();
			S.erase(u);

			for (size_t v = 0; v < V.size(); v++)
				if (u != v)
					if (S.count(v) && distance[u][v] < V[v].key)
					{
						V[v].π = u;
						V[v].key = distance[u][v];

						Q.push(&V[v]);
					}
		}

		// crea out-star
		for (unsigned short u = 0; u < numberOfNodes; u++)
		{
			auto v = V[u].π;

			if (v != UINT16_MAX)
			{
				out_star[u].push_back(v);
				out_star[v].push_back(u);
			}
		}
	}

	void Christofides::GreedyWeightedPerfectMatching() // O(V²)
	{
		float dist;
		unsigned short i, closest = 0;
		set<unsigned short> V_odd;

		for (i = 0; i < numberOfNodes; i++)
			if (out_star[i].size() % 2 != 0)
				V_odd.insert(i);

		while (!V_odd.empty())
			for (const auto v : V_odd)
			{
				dist = FLT_MAX;
				V_odd.erase(v);

				for (const auto u : V_odd)
					if (distance[u][v] < dist)
					{
						dist = distance[u][v];
						closest = u;
					}

				out_star[v].push_back(closest);
				out_star[closest].push_back(v);

				V_odd.erase(closest);
				break; // next element
			}
	}

	vector<unsigned short> Christofides::FindEulerCircuit(unsigned short start) // O((V + E)²)
	{
		size_t i;
		unsigned short neighbor, pos;

		stack<unsigned short> S;
		vector<unsigned short> path;

		path.push_back(start);

		pos = start;
		auto neighbours = out_star; // copy

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

				neighbor = neighbours[pos].back();

				neighbours[pos].pop_back();

				for (i = 0; i < neighbours[neighbor].size(); i++)
					if (neighbours[neighbor][i] == pos)
						neighbours[neighbor].erase(neighbours[neighbor].begin() + i);

				pos = neighbor;
			}

		path.push_back(pos);

		return path;
	}

	float Christofides::ToHamiltonianPath(vector<unsigned short> &path) // O(V)
	{
		vector<bool> visited(numberOfNodes, false);
		float opt = 0;

		auto curr = path.begin();
		auto next = path.begin() + 1;

		visited[*curr] = true;

		while (next != path.end())
			if (visited[*next])
			{
				next = path.erase(next);
			}
			else
			{
				opt += distance[*curr][*next];
				curr = next;

				visited[*curr] = true;

				next = curr + 1;
			}

		auto head = path.front();
		auto tail = path.back();

		path.push_back(head);

		opt += distance[tail][head];

		return opt;
	}

	float Christofides::findBestPath(unsigned short start) // O((V + E)²)
	{
		auto circuit = FindEulerCircuit(start); // O((V + E)²)
		auto opt = ToHamiltonianPath(circuit); // O(V)

		return opt;
	}

	/*
	Christofides algorithm
	The Christofides algorithm, 1976, is an algorithm for finding approximate solutions to the euclidean travelling salesman problem. It is an approximation algorithm that guarantees that its solutions will be within a factor of 3/2 of the optimal solution length.

	T(n) = O(n⁴)

	TSP is an extension of the Hamiltonian circuit problem.
	The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
	1. visit every city just once
	2. return to the starting point
	3. be of minimum distance.

	Algo. from https://en.wikipedia.org/wiki/Christofides_algorithm
	Algo.:
	1. Create a minimum spanning tree T of G.
	2. Let O be the set of vertices with odd degree in T. By the handshaking lemma, O has an even number of vertices.
	3. Find a minimum-weight perfect matching M in the induced subgraph given by the vertices from O.
	4. Combine the edges of M and T to form a connected multigraph H in which each vertex has even degree.
	5. Form an Eulerian circuit in H.
	6. Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
	*/
	void Christofides::Solve(float &opt, string &path) // O(V⁴)
	{
		// 1. Create a minimum spanning tree T of G.
		MinimumSpanningTree_Prim(); // O(E ㏒ V)

		// 2. Let O be the set of vertices with odd degree in T.
		// 3. Find a minimum - weight perfect matching M in the induced subgraph given by the vertices from O.
		// 4. Combine the edges of M and T to form a connected multigraph H in which each vertex has even degree.
		GreedyWeightedPerfectMatching(); // O(V²)

		unsigned short bestIndex;
		{
			float cost, min = FLT_MAX;

			for (unsigned short t = 0; t < numberOfNodes; t++)
			{
				cost = findBestPath(t); // O((V + E)²)

				if (cost < min)
				{
					bestIndex = t;
					min = cost;
				}
			}
		}

		// 5. Form an Eulerian circuit in H.
		auto circuit = FindEulerCircuit(bestIndex); // O((V + E)²)

		// 6. Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
		opt = ToHamiltonianPath(circuit); // O(V)
		path = PrintPath(circuit); // O(V)

		currentCardinality = numberOfNodes;
	}
}