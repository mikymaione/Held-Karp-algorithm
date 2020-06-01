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
		auto max_num_step = ((numberOfNodes * numberOfNodes) / 50) + numberOfNodes + 16;
		auto num_step = 0;

		MST::Kruskal kruskal;

		Graph G(numberOfNodes);
		G.MakeConnected(distance);

		while (num_step < max_num_step)
		{
			num_step++;

			/* compute minimum 1-tree */
			auto curr_1t = kruskal.Solve(G);
		}

		opt = 0;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}