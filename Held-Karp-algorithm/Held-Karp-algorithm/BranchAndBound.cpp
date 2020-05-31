/*
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

	list<BranchAndBound::Node> BranchAndBound::MST_Prim(Graph &G, Node *r) // O(E ㏒ V)
	{
		priority_queue<Node*, vector<Node*>, less<Node*>> Q; // sorted min queue
		set<size_t> S; // elements available		

		list<Node> R;

		for (auto u : G.V)
		{
			u.key = FLT_MAX;
			u.π = NULL;

			S.insert(u.id);
		}

		r->key = 0;
		Q.push(r);

		while (!Q.empty())
		{
			auto u = Q.top(); // min
			Q.pop();
			S.erase(u->id);

			for (auto v : G.Adj[u])
				if (u != v)
					if (S.count(v->id) > 0 && distance[u->id][v->id] < v->key)
					{
						v->π = u;
						v->key = distance[u->id][v->id];

						Q.push(v);
					}
		}

		return R;
	}

	BranchAndBound::Node *BranchAndBound::OneTree(Graph &G)
	{
		Graph guard(numberOfNodes, 0, numberOfNodes - 1);
		Graph onetree(numberOfNodes - 1, 1, numberOfNodes - 1);

		{
			auto v0 = guard.NodeById(0);

			for (auto a : guard.V)
				if (a.id != 0)
					guard.Adj[v0].push_back(&a);
		}

		for (auto d : onetree.V)
			for (auto a : onetree.V)
				if (a.id != d.id)
					onetree.Adj[&d].push_back(&a);

		auto result = MST_Prim(onetree, onetree.NodeById(1));
		auto result1 = MST_Prim(guard, guard.NodeById(0));

		return NULL;
	}

	string BranchAndBound::PrintPath()
	{
		string s;

		return s;
	}

	void BranchAndBound::Solve(float &opt, string &path)
	{
		Graph G(numberOfNodes, 0, numberOfNodes - 1);

		for (auto d : G.V)
			for (auto a : G.V)
				if (a.id != d.id)
					G.Adj[&d].push_back(&a);

		auto busca = OneTree(G);

		opt = 0;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}