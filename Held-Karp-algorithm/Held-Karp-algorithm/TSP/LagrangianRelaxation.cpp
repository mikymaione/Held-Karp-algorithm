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

#include "LagrangianRelaxation.hpp"
#include "ApproxTSP.hpp"

#include "../ADS/Graph.hpp"
#include "../MST/Kruskal.hpp"

namespace TSP
{
	LagrangianRelaxation::LagrangianRelaxation(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string LagrangianRelaxation::PrintPath()
	{
		string s;

		return s;
	}

	float LagrangianRelaxation::UpperBound()
	{
		float opt;
		string path;

		ApproxTSP approxTSP(distance);
		approxTSP.SilentSolve(opt, path);

		return opt;
	}

	shared_ptr<Graph> LagrangianRelaxation::LagrangeSubGradient(const unsigned short N, Graph &G, float &best_zero_tree_cost)
	{
		auto t_1 = 0.0f;
		auto t_k = 0.0f;

		unsigned short k = 0;
		unsigned short M = ((N * N) / 50) + N + 16;

		maxCardinality = M;

		auto constraint1 = 2.0f * (M - 1.0f) * (M - 2.0f);
		auto constraint2 = M * (2.0f * M - 3.0f);

		MST::Kruskal kruskal;

		map<unsigned short, unsigned short> d_k_prev;
		vector<float> π(N, 0);

		shared_ptr<Graph> best_zero_tree;

		while (k < M)
		{
			k++;
			currentCardinality++;

			auto zero_tree = kruskal.Solve(G);

			if (zero_tree->E.size() == 0)
				break;

			auto d_k = zero_tree->Degree();

			if (k == 1)
				d_k_prev = d_k;

			auto zero_tree_cost = zero_tree->Cost();

			for (unsigned short i = 0; i < N; i++)
				zero_tree_cost += π[i] * 2.0f;

			if (zero_tree_cost > best_zero_tree_cost || k == 1)
			{
				best_zero_tree_cost = zero_tree_cost;
				best_zero_tree = zero_tree;

				t_1 = 0.01f * zero_tree_cost;

				if (zero_tree_cost > UB)
					break;
			}

			if (zero_tree->HaveCycle())
				break;

			t_k = t_1 * ((k * k - 3.0f * (M - 1.0f) * k + constraint2) / constraint1);

			for (auto i : zero_tree->V)
				π[i->id] += 0.6f * t_k * (2 - d_k[i->id]) + 0.4f * t_k * (2 - d_k_prev[i->id]);

			d_k_prev.clear();
			for (auto d : d_k)
				d_k_prev[d.first] = d.second;

			for (auto e : G.E)
				e->cost = distance[e->from->id][e->to->id] - π[e->from->id] - π[e->to->id];
		}

		return best_zero_tree;
	}

	/*
	From: Volgenant, T. and Jonker, R., 1982. A branch and bound algorithm for the symmetric traveling salesman problem based on the 1-tree relaxation. European Journal of Operational Research, 9(1):83–89.
	*/
	void LagrangianRelaxation::Solve(float &opt, string &path)
	{
		const auto FirstUB = UpperBound();
		UB = FirstUB;

		Graph G0(numberOfNodes - 1);
		G0.MakeConnected(distance);

		auto LB = 0.0f;
		auto zero_tree = LagrangeSubGradient(numberOfNodes - 1, G0, LB);

		{
			unsigned short minv1, minv2;

			float minv1c = FLT_MAX;
			float minv2c = FLT_MAX;

			// n - v1
			for (unsigned short x = 0; x < numberOfNodes - 1; x++)
				if (distance[x][numberOfNodes - 1] < minv1c)
				{
					minv1c = distance[x][numberOfNodes - 1];
					minv1 = x;
				}

			// n - v2
			for (unsigned short x = 0; x < numberOfNodes - 1; x++)
				if (x != minv1)
					if (distance[x][numberOfNodes - 1] < minv2c)
					{
						minv2c = distance[x][numberOfNodes - 1];
						minv2 = x;
					}

			zero_tree->AddNode(numberOfNodes - 1);

			Edge minv1e(
				minv1c,
				zero_tree->NodeById(minv1),
				zero_tree->NodeById(numberOfNodes - 1)
				);

			Edge minv2e(
				minv2c,
				zero_tree->NodeById(minv2),
				zero_tree->NodeById(numberOfNodes - 1)
				);

			zero_tree->E.push_back(make_shared<Edge>(minv1e));
			zero_tree->E.push_back(make_shared<Edge>(minv2e));

			LB = zero_tree->Cost(distance);
		}

		auto gap = 100.0f * (UB - LB) / LB;

		opt = LB;
		path = PrintPath();
	}

}