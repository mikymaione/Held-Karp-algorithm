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
	Righini::Righini(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string Righini::PrintPath(const unsigned int code, const unsigned short π)
	{
		string s;

		return "0 " + s + "0";
	}

	void Righini::Solve(float &opt, string &path)
	{
		const unsigned short r = numberOfNodes - 1;

		//auto G = graphFromDistanceMatrix(numberOfNodes);
		//auto G_k = graphFromDistanceMatrix(r);

		//auto S_k = Kruskal(G_k);

		//vector<Edge*> D(2); // min cost edge incident to r
		//{
		//	vector<Edge*> toSort;

		//	for (size_t i = 0; i < G.edge.size(); i++)
		//		if (G.edge[i].v == r && G.edge[i].v != G.edge[i].u)
		//			toSort.push_back(&G.edge[i]);

		//	sort(toSort.begin(), toSort.end(), [](Edge* x, Edge* y)
		//	{
		//		return x->w < y->w;
		//	});

		//	for (size_t x = 0; x < 2; x++)
		//		D[x] = toSort[x];
		//}

		//{	// reduce cost G_k to 0
		//	auto min = FLT_MAX;

		//	for (size_t i = 0; i < G_k.edge.size(); i++)
		//		if (G_k.edge[i].w < min)
		//			min = G_k.edge[i].w;

		//	for (size_t i = 0; i < G_k.edge.size(); i++)
		//		G_k.edge[i].w -= min;
		//}

		currentCardinality = numberOfNodes;
	}
}