/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>
#include <memory>
#include <list>
#include <map>

using namespace std;

namespace ADS
{
	struct Node
	{
		unsigned short id, rank = 0;
		float key = FLT_MAX;
		Node *π = NULL;

		bool operator < (const Node &n) const
		{
			return (key < n.key);
		}
	};

	struct Edge
	{
		Node *from, *to;

		const vector<vector<float>> *distance;

		Edge(const vector<vector<float>> *distance_, Node *from_, Node *to_) :
			distance(distance_),
			from(from_),
			to(to_) {}

		bool operator < (const Edge &e) const
		{
			auto w1 = distance[from->id][to->id];
			auto w2 = distance[e.from->id][e.to->id];

			return w1 < w2;
		}
	};

	struct Graph
	{
		list<Node> V;
		vector<Edge> E; // sortable
		//map<unsigned short, list<Node *>> Adj;

		const vector<vector<float>> *distance;

		Graph(const vector<vector<float>> *distance_, unsigned short N, unsigned short from, unsigned to) : distance(distance_)
		{
			for (unsigned short d = from; d <= to; d++)
			{
				Node n;
				n.id = d;

				V.push_back(n);
			}
		}

		void AddEdge(Node *from, Node *to)
		{
			E.push_back(Edge(distance, from, to));
			//Adj[from->id].push_back(to);
		}

		void SortEdgeByWeight()
		{
			sort(E.begin(), E.end());
		}

		void MakeConnected()
		{
			for (auto d : V)
				for (auto a : V)
					if (d.id != a.id)
						AddEdge(&d, &a);
		}

		Node *NodeById(unsigned short id)
		{
			for (auto v : V)
				if (v.id == id)
					return &v;
		}
	};
}