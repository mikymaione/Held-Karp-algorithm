/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Held–Karp algorithm
The Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).

T(n) = O(n²)

TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.
*/
#pragma once

#include <set>
#include <stack>

#include "Christofides.hpp"
#include "../MST/Prim.hpp"
#include "../ADS/Graph.hpp"

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

	void Christofides::GreedyWeightedPerfectMatching() // O(V³)
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

	vector<unsigned short> Christofides::FindEulerCircuit(unsigned short start) // O(E)
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

	float Christofides::ToHamiltonianPath(vector<unsigned short> &path) // O(E)
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

	float Christofides::findBestPath(unsigned short start) // O(E)
	{
		auto circuit = FindEulerCircuit(start); // O(E)
		auto opt = ToHamiltonianPath(circuit); // O(E)

		return opt;
	}

	/*
	Christofides algorithm
	The Christofides algorithm, 1976, is an algorithm for finding approximate solutions to the euclidean travelling salesman problem. It is an approximation algorithm that guarantees that its solutions will be within a factor of 3/2 of the optimal solution length.

	From: Christofides, N., 1976. Worst-case analysis of a new heuristic for the travelling salesman problem. Report 388, Graduate School of Industrial Administration, CMU.

	Algo. from https://en.wikipedia.org/wiki/Christofides_algorithm
	Algo.:
	1. Create a minimum spanning tree T of G.
	2. Let O be the set of vertices with odd degree in T. By the handshaking lemma, O has an even number of vertices.
	3. Find a minimum-weight perfect matching M in the induced subgraph given by the vertices from O.
	4. Combine the edges of M and T to form a connected multigraph H in which each vertex has even degree.
	5. Form an Eulerian circuit in H.
	6. Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
	*/
	void Christofides::Solve(float &opt, string &path) // O(V³)
	{
		maxCardinality = 8;

		Graph G(numberOfNodes);
		G.MakeConnected(distance);
		currentCardinality++;

		// 1. Create a minimum spanning tree T of G.
		MST::Prim prim;
		prim.Solve(distance, G, 0); // O(E ㏒ V)
		currentCardinality++;

		// crea out-star
		for (auto u : G.V)
			if (u->π)
			{
				out_star[u->id].push_back(u->π->id);
				out_star[u->π->id].push_back(u->id);
			}
		currentCardinality++;

		// 2. Let O be the set of vertices with odd degree in T.
		// 3. Find a minimum - weight perfect matching M in the induced subgraph given by the vertices from O.
		// 4. Combine the edges of M and T to form a connected multigraph H in which each vertex has even degree.
		GreedyWeightedPerfectMatching(); // O(V³)
		currentCardinality++;

		unsigned short bestIndex;
		{
			float cost, min = FLT_MAX;

			for (unsigned short t = 0; t < numberOfNodes; t++)
			{
				cost = findBestPath(t); // O(E)

				if (cost < min)
				{
					bestIndex = t;
					min = cost;
				}
			}
		}
		currentCardinality++;

		// 5. Form an Eulerian circuit in H.
		auto circuit = FindEulerCircuit(bestIndex); // O(E)
		currentCardinality++;

		// 6. Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
		opt = ToHamiltonianPath(circuit); // O(E)
		currentCardinality++;

		path = PrintPath(circuit);
		currentCardinality++;
	}
}