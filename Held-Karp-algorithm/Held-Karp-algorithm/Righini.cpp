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

	void Righini::MakeSet(Graph &G, uint_least16_t x)
	{
		G.node[x].p = x;
		G.node[x].rank = 0;
	}

	uint_least16_t Righini::FindSet(Graph &G, uint_least16_t x)
	{
		if (G.node[x].p != x)
			G.node[x].p = FindSet(G, G.node[x].p);

		return G.node[x].p;
	}

	void Righini::Link(Graph &G, uint_least16_t x, uint_least16_t y)
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

	void Righini::Union(Graph &G, uint_least16_t x, uint_least16_t y)
	{
		auto u = FindSet(G, x);
		auto v = FindSet(G, y);

		Link(G, u, v);
	}

	vector<Righini::Edge*> Righini::Kruskal(Graph &G) // O(E ㏒ V)
	{
		vector<Edge*> A;
		uint_least16_t u, v, i;

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

	Righini::Graph Righini::graphFromDistanceMatrix(const uint_least16_t nodes)
	{
		Graph G;
		G.node.resize(nodes);
		G.edge.resize(nodes * nodes);

		size_t i = 0;
		for (uint_least16_t x = 0; x < nodes; x++)
			for (uint_least16_t y = 0; y < nodes; y++)
			{
				G.edge[i].u = x;
				G.edge[i].v = y;
				G.edge[i].w = (x == y ? UINT_LEAST16_MAX : distance[x][y]);

				i++;
			}

		return G;
	}

	void Righini::Solve(uint_least16_t &opt, string &path)
	{
		const uint_least16_t r = numberOfNodes - 1;

		auto G = graphFromDistanceMatrix(numberOfNodes);
		auto G_k = graphFromDistanceMatrix(r);

		auto S = Kruskal(G_k);

		vector<Edge*> D(2); // min cost edge incident to r
		{
			vector<Edge*> toSort;

			for (size_t i = 0; i < G.edge.size(); i++)
				if (G.edge[i].v == r && G.edge[i].v != G.edge[i].u)
					toSort.push_back(&G.edge[i]);

			sort(toSort.begin(), toSort.end(), [](Edge* x, Edge* y) {
				return x->w < y->w;
			});

			for (size_t x = 0; x < 2; x++)
				D[x] = toSort[x];
		}

		{	// reduce cost G_k to 0
			uint_least16_t min = UINT_LEAST16_MAX;

			for (size_t i = 0; i < G_k.edge.size(); i++)
				if (G_k.edge[i].w < min)
					min = G_k.edge[i].w;

			for (size_t i = 0; i < G_k.edge.size(); i++)
				G_k.edge[i].w -= min;
		}

		currentCardinality = numberOfNodes;
	}
}