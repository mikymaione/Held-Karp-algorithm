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

#include "BranchAndBound.hpp"
#include "Christofides.hpp"

#include "../ADS/Graph.hpp"
#include "../MST/Kruskal.hpp"

namespace TSP
{
	BranchAndBound::BranchAndBound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string BranchAndBound::PrintPath()
	{
		string s;

		return s;
	}

	float BranchAndBound::UpperBound()
	{
		float opt;
		string path;

		Christofides christofides(distance);

		christofides.SilentSolve(opt, path);

		return opt;
	}

	/*
	From: Volgenant, T. and Jonker, R., 1982. A branch and bound algorithm for the symmetric traveling salesman problem based on the 1-tree relaxation. European Journal of Operational Research, 9(1):83–89.
	*/
	void BranchAndBound::Solve(float &opt, string &path)
	{
		auto best_one_tree = 0.0f;
		auto t_1 = 0.0f;
		auto t_k = 0.0f;

		unsigned short k = 0;
		unsigned short M = ((numberOfNodes * numberOfNodes) / 50) + numberOfNodes + 16;

		maxCardinality = M;

		auto constraint1 = 2.0f * (M - 1.0f) * (M - 2.0f);
		auto constraint2 = M * (2.0f * M - 3.0f);

		auto UB = UpperBound();

		MST::Kruskal kruskal;

		Graph G(numberOfNodes);
		G.MakeConnected(distance);

		map<shared_ptr<Node>, unsigned short> d_k_prev;
		vector<float> π(numberOfNodes, 0);

		shared_ptr<Graph> best_1t, one_tree;

		while (k < M)
		{
			k++;
			currentCardinality++;

			one_tree = kruskal.Solve(G);

			if (one_tree->E.size() == 0)
				break;

			auto d_k = one_tree->Degree();

			if (k == 1)
				d_k_prev = d_k;

			auto z = one_tree->Cost();

			for (unsigned short i = 0; i < numberOfNodes; i++)
				z += π[i] * 2.0f;

			if (z > best_one_tree || k == 1)
			{
				best_one_tree = z;
				best_1t = one_tree;

				t_1 = 0.01f * z;

				if (z > UB)
					break;
			}

			if (one_tree->HaveCycle())
				break;

			t_k = t_1 * ((k * k - 3.0f * (M - 1.0f) * k + constraint2) / constraint1);

			for (auto i : one_tree->V)
				π[i->id] += 0.6f * t_k * (2 - d_k[i]) + 0.4f * t_k * (2 - d_k_prev[i]);

			d_k_prev = d_k;

			for (auto e : G.E)
				e->cost = distance[e->from->id][e->to->id] - π[e->from->id] - π[e->to->id];
		}

		opt = best_one_tree;
		path = PrintPath();
	}
}