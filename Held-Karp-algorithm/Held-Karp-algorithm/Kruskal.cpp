/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>

#include "Kruskal.hpp"

namespace MST
{

	Kruskal::Graph Kruskal::graphFromDistanceMatrix(const vector<vector<float>> &DistanceMatrix2D)
	{
		const unsigned short nodes = DistanceMatrix2D.size();

		Graph G;
		G.node.resize(nodes);
		G.edge.resize(nodes * nodes);

		size_t i = 0;
		for (unsigned short x = 0; x < nodes; x++)
			for (unsigned short y = 0; y < nodes; y++)
			{
				G.edge[i].u = x;
				G.edge[i].v = y;
				G.edge[i].w = (x == y ? FLT_MAX : DistanceMatrix2D[x][y]);

				i++;
			}

		return G;
	}

	void Kruskal::MakeSet(Graph &G, unsigned short x)
	{
		G.node[x].p = x;
		G.node[x].rank = 0;
	}

	unsigned short Kruskal::FindSet(Graph &G, unsigned short x)
	{
		if (G.node[x].p != x)
			G.node[x].p = FindSet(G, G.node[x].p);

		return G.node[x].p;
	}

	void Kruskal::Link(Graph &G, unsigned short x, unsigned short y)
	{
		if (G.node[x].rank > G.node[y].rank)
		{
			G.node[y].p = x;
		}
		else
		{
			G.node[x].p = y;

			if (G.node[x].rank == G.node[y].rank)
				G.node[y].rank++;
		}
	}

	void Kruskal::Union(Graph &G, unsigned short x, unsigned short y)
	{
		auto u = FindSet(G, x);
		auto v = FindSet(G, y);

		Link(G, u, v);
	}

	vector<Kruskal::Edge*> Kruskal::Solve(const vector<vector<float>> &DistanceMatrix2D) // O(E ㏒ V)
	{
		vector<Edge*> A;
		unsigned short u, v, i;

		auto G = graphFromDistanceMatrix(DistanceMatrix2D);

		for (v = 0; v < G.node.size(); v++)
			MakeSet(G, v);

		sort(G.edge.begin(), G.edge.end());

		for (i = 0; i < G.edge.size(); i++)
		{
			u = G.edge[i].u;
			v = G.edge[i].v;

			if (FindSet(G, u) != FindSet(G, v))
			{
				A.push_back(&G.edge[i]);
				Union(G, u, v);
			}
		}

		return A;
	}

}