/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.
*/
#pragma once

#include <stack>

#include "Christofides.hpp"
#include "../MST/Prim.hpp"
#include "../Matching/Blossom.hpp"

namespace TSP
{
	Christofides::Christofides(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	float Christofides::CalcCost(vector<unsigned short> &circuit) // O(V)
	{
		unsigned short da, a;
		float cost = 0;

		for (unsigned short i = 0; i < numberOfNodes; i++)
		{
			da = circuit[i];
			a = circuit[i + 1];

			cost += distance[da][a];
		}

		return cost;
	}

	string Christofides::PrintPath(vector<unsigned short> &circuit) // O(V)
	{
		string s;

		for (auto const e : circuit)
			s = to_string(e) + " " + s;

		return s;
	}

	// 1. Create a minimum spanning tree T of G.
	vector<set<unsigned short>> Christofides::MST(Graph &G) // O(E ㏒ V)
	{
		vector<set<unsigned short>> T(numberOfNodes);

		MST::Prim prim;
		prim.Solve(distance, G, 0); // O(E ㏒ V)

		for (auto u : G.V)
			if (u->π)
			{
				T[u->id].insert(u->π->id);
				T[u->π->id].insert(u->id);
			}

		return T;
	}

	// 2. Let O be the set of vertices with odd degree in T.
	set<unsigned short> Christofides::OddVertices(vector<set<unsigned short>> &T) // O(V)
	{
		set<unsigned short> O;

		for (unsigned short i = 0; i < numberOfNodes; i++)
			if (T[i].size() % 2 != 0)
				O.insert(i);

		return O;
	}

	// 3.b induced subgraph given by the vertices from O.
	shared_ptr<Graph> Christofides::SubGraph(Graph &G, set<unsigned short> O) // O(E)
	{
		Graph I(O);
		I.MakeConnected(distance);

		return make_shared<Graph>(I);
	}

	// 3. Find a minimum - weight perfect matching M in the induced subgraph given by the vertices from O.		
	set<shared_ptr<Edge>> Christofides::PerfectMatching(shared_ptr<Graph> G) // O(V²E)
	{
		Matching::Blossom blossom;
		return blossom.Solve(G); // O(V²E)
	}

	// 4. Combine the edges of M and T to form a connected multigraph H
	vector<set<unsigned short>> Christofides::Multigraph(vector<set<unsigned short>> &T, set<shared_ptr<Edge>> &M) // O(E)
	{
		auto H = T; // copy

		for (auto m : M)
		{
			if (H[m->from->id].count(m->to->id) == 0)
				H[m->from->id].insert(m->to->id);

			if (H[m->to->id].count(m->from->id) == 0)
				H[m->to->id].insert(m->from->id);
		}

		return H;
	}

	// 5. Form an Eulerian circuit in H.
	// 6. Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
	void Christofides::Hamiltonian(vector<set<unsigned short>> &H, vector<unsigned short> &E, set<unsigned short> &visited, unsigned short c) // O(V)
	{
		visited.insert(c);
		E.push_back(c);

		for (auto e : H[c])
			if (visited.count(e) == 0)
				Hamiltonian(H, E, visited, e);
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
	void Christofides::Solve(float &opt, string &path) // O(V²E)
	{
		maxCardinality = 11;

		Graph G(distance); // O(V+E)
		currentCardinality = 1;

		// 1. Create a minimum spanning tree T of G.
		auto T = MST(G); // O(E ㏒ V)
		currentCardinality = 3;

		// 2. Let O be the set of vertices with odd degree in T.
		auto O = OddVertices(T);
		currentCardinality = 4;

		// 3.b induced subgraph given by the vertices from O.
		auto IG = SubGraph(G, O);
		currentCardinality = 5;

		// 3. Find a minimum - weight perfect matching M in the induced subgraph given by the vertices from O.		
		auto M = PerfectMatching(IG); // O(V²E)
		currentCardinality = 6;

		// 4. Combine the edges of M and T to form a connected multigraph H
		auto H = Multigraph(T, M);
		currentCardinality = 7;

		// 5. Form an Eulerian circuit in H.
		// 6. Make the circuit found in previous step into a Hamiltonian circuit by skipping repeated vertices (shortcutting).
		vector<unsigned short> E;
		{
			set<unsigned short> visited;
			Hamiltonian(H, E, visited, 0);
			E.push_back(0);
		}
		currentCardinality = 10;

		opt = CalcCost(E);
		path = PrintPath(E);
		currentCardinality = 11;
	}

}