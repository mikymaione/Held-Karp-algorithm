/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "BranchAndBound.hpp"
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

	void BranchAndBound::Solve(float &opt, string &path)
	{
		auto best_lb = 0.0f;
		auto base_step = 0.0f;
		auto UB = FLT_MAX;
		auto step = 0.0f;

		auto M = ((numberOfNodes * numberOfNodes) / 50) + numberOfNodes + 16;
		auto K = 0;

		auto constraint1 = 2.0f * (M - 1.0f) * (M - 2.0f);
		auto constraint2 = M * (2.0f * M - 3.0f);

		MST::Kruskal kruskal;

		Graph G(numberOfNodes);
		G.MakeConnected(distance);

		map<shared_ptr<Node>, unsigned short> oldDegree;
		vector<float> π(numberOfNodes, 0);

		shared_ptr<Graph> best_1t;

		while (K < M)
		{
			K++;

			/* compute minimum 1-tree */
			auto curr_1t = kruskal.Solve(G);

			if (curr_1t->E.size() == 0)
				break;

			auto vrtx_deg = curr_1t->Degree();

			if (K == 1)
				oldDegree = vrtx_deg;

			auto z = curr_1t->Cost();

			for (unsigned short i = 0; i < numberOfNodes; i++)
				z += π[i] * 2;

			/* update solution */
			if (z > best_lb || K == 1)
			{
				best_lb = z;
				best_1t = curr_1t;

				base_step = 0.01 * z;

				if (z > UB)
					break;
			}

			/* stop condition */
			if (curr_1t->onetree_is_cycle())
				break;

			/* update step */
			step = base_step * ((K * K - 3.0f * (M - 1.0f) * K + constraint2) / constraint1);

			for (auto v : curr_1t->V)
				if (vrtx_deg[v] != 2)
					π[v->id] += 0.5 * step * (2 - vrtx_deg[v]) + 0.5 * step * (2 - oldDegree[v]);

			/* store degrees */
			oldDegree = vrtx_deg;

			/* update edge-costs */
			for (auto e : G.E)
				e->cost = distance[e->from->id][e->to->id] - π[e->from->id] - π[e->to->id];
		}

		opt = best_lb;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}