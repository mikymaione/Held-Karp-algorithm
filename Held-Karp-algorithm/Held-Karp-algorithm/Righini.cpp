/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>

#include "Righini.hpp"

namespace TSP
{
	Righini::Righini(const vector<vector<uint_least16_t>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string Righini::PrintPath(const uint_least32_t code, const uint_least16_t π)
	{
		string s;

		return "0 " + s + "0";
	}

	void Righini::makeSet(vector<Node> &node, uint_least16_t x)
	{
		node[x].r = 0;
		node[x].p = x;
	}

	uint_least16_t Righini::findSet(vector<Node> &node, uint_least16_t x)
	{
		if (node[x].p != x)
			node[x].p = findSet(node, node[x].p);

		return node[x].p;
	}

	void Righini::setUnion(vector<Node> &node, uint_least16_t x, uint_least16_t y)
	{
		if (node[x].r > node[y].r)
		{
			node[y].p = x;
		}
		else
		{
			node[x].p = y;

			if (node[x].r == node[y].r)
				node[y].r++;
		}
	}

	vector<Righini::Edge*> Righini::Kruskal(vector<Node> &node, vector<Edge> &edge)
	{
		vector<Edge*> A;
		uint_least16_t i, min = UINT_LEAST16_MAX;

		for (i = 0; i < node.size(); i++)
			makeSet(node, i);

		sort(edge.begin(), edge.end());

		for (i = 0; i < edge.size(); i++)
		{
			auto x = edge[i].u;
			auto y = edge[i].v;
			auto x_parent = findSet(node, x);
			auto y_parent = findSet(node, y);

			if (x_parent != y_parent)
			{
				if (edge[i].w < min)
					min = edge[i].w;

				A.push_back(&edge[i]);
				setUnion(node, x_parent, y_parent);
			}
		}

		for (i = 0; i < A.size(); i++)
			A[i]->w -= min;

		return A;
	}

	Righini::Graph Righini::graphFromDistanceMatrix(const uint_least16_t nodes)
	{
		vector<Node> node(nodes);
		vector<Edge> edge(nodes * nodes);

		size_t i = 0;
		for (uint_least16_t x = 0; x < nodes; x++)
			for (uint_least16_t y = 0; y < nodes; y++)
			{
				edge[i].u = x;
				edge[i].v = y;
				edge[i].w = distance[x][y];

				i++;
			}

		Graph G;
		G.edge = edge;
		G.node = node;

		return G;
	}

	void Righini::Solve(uint_least16_t &opt, string &path)
	{
		const uint_least16_t r = numberOfNodes - 1;

		auto G_k = graphFromDistanceMatrix(r);
		auto S = Kruskal(G_k.node, G_k.edge);

		vector<Edge> D(2);
		{
			// calculate D
			vector<Edge> toSort;
			auto G = graphFromDistanceMatrix(numberOfNodes);

			for (const auto e : G.edge)
				if (e.v == r && e.v != e.u)
					toSort.push_back(e);

			sort(toSort.begin(), toSort.end());

			for (size_t x = 0; x < 2; x++)
				D[x] = toSort[x];
		}



		currentCardinality = numberOfNodes;
	}
}