/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Kruskal.hpp"

namespace MST
{

	void Kruskal::MakeSet(Node *x)
	{
		x->π = x;
		x->rank = 0;
	}

	Node *Kruskal::FindSet(Node *x)
	{
		if (x->π->id != x->id)
			x->π = FindSet(x->π);

		return x->π;
	}

	void Kruskal::Link(Node *x, Node *y)
	{
		if (x->rank > y->rank)
		{
			y->π = x;
		}
		else
		{
			x->π = y;

			if (x->rank == y->rank)
				y->rank++;
		}
	}

	void Kruskal::Union(Node *x, Node *y)
	{
		auto u = FindSet(x);
		auto v = FindSet(y);

		Link(u, v);
	}

	vector<Edge *> Kruskal::Solve(Graph *G) // O(E ㏒ V)
	{
		vector<Edge *> A;

		for (auto v : G->V)
			MakeSet(&v);

		G->SortEdgeByWeight();

		for (auto e : G->E)
		{
			auto u = e.from;
			auto v = e.to;

			if (FindSet(u) != FindSet(v))
			{
				A.push_back(&e);
				Union(u, v);
			}
		}

		return A;
	}

}