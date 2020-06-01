/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <queue>
#include <set>

#include "Prim.hpp"

namespace MST
{

	void Prim::Solve(const vector<vector<float>> &distance, Graph &G, unsigned short r_id) // O(E ㏒ V)
	{
		priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, less<shared_ptr<Node>>> Q; // sorted min queue
		set<size_t> S; // elements available

		for (auto u : G.V)
		{
			u->key = FLT_MAX;
			u->π = nullptr;

			S.insert(u->id);
		}

		auto r = G.NodeById(r_id);
		r->key = 0;
		Q.push(r);

		while (!Q.empty())
		{
			auto u = Q.top(); // min
			Q.pop();
			S.erase(u->id);

			for (auto v : G.Adj[u])
				if (u->id != v->id)
					if (S.count(v->id) && distance[u->id][v->id] < v->key)
					{
						v->π = u;
						v->key = distance[u->id][v->id];

						Q.push(v);
					}
		}
	}

}