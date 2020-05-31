﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <queue>
#include <set>

#include "BranchAndBound.hpp"
#include "Kruskal.hpp"

namespace TSP
{
	BranchAndBound::BranchAndBound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	Node *BranchAndBound::OneTree(Graph &G)
	{
		MST::Kruskal kruskal;

		Graph guard(&distance, numberOfNodes, 0, numberOfNodes - 1);
		Graph onetree(&distance, numberOfNodes - 1, 1, numberOfNodes - 1);

		for (auto v0 : guard.V)
			if (v0.id == 0)
			{
				for (auto a : guard.V)
					if (a.id != 0)
						guard.AddEdge(&v0, &a);

				break;
			}

		for (auto d : onetree.V)
			for (auto a : onetree.V)
				if (a.id != d.id)
					onetree.AddEdge(&d, &a);

		auto result = kruskal.Solve(&onetree);
		auto result1 = kruskal.Solve(&guard);

		return NULL;
	}

	string BranchAndBound::PrintPath()
	{
		string s;

		return s;
	}

	void BranchAndBound::Solve(float &opt, string &path)
	{
		Graph G(&distance, numberOfNodes, 0, numberOfNodes - 1);
		G.MakeConnected();

		auto busca = OneTree(G);

		opt = 0;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}