﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <list>
#include <stack>
#include <vector>
#include <map>

using namespace std;

namespace ADS
{
	struct Node
	{
		unsigned short id;
		shared_ptr<Node> π;

		unsigned short rank = 0; // Kruskal
		float key = FLT_MAX; // Prim		
	};

	struct Edge
	{
		float cost;
		shared_ptr<Node> from, to;

		Edge(float cost_, shared_ptr<Node> from_, shared_ptr<Node> to_) :
			cost(cost_),
			from(from_),
			to(to_) {}
	};

	struct Graph
	{
		list<shared_ptr<Node>> V;
		vector<shared_ptr<Edge>> E; // sortable

		map<shared_ptr<Node>, list<shared_ptr<Node>>> Adj;
		vector<vector<unsigned short>> AdjIDs;

		unsigned short NumberOfNodes;

		Graph(list<shared_ptr<Node>> V_)
		{
			NumberOfNodes = V_.size();

			for (auto v : V_)
			{
				Node n;
				n.id = v->id;

				V.push_back(make_shared<Node>(n));
			}
		}

		Graph(unsigned short NumberOfNodes_) : Graph(NumberOfNodes_, 0, NumberOfNodes_ - 1) {}

		Graph(unsigned short NumberOfNodes_, unsigned short from, unsigned to) : NumberOfNodes(NumberOfNodes_)
		{
			AdjIDs.resize(NumberOfNodes_);

			for (unsigned short d = from; d <= to; d++)
			{
				Node n;
				n.id = d;

				V.push_back(make_shared<Node>(n));
			}
		}

		void AddEdge(float cost, shared_ptr<Node> from, shared_ptr<Node> to)
		{
			Edge e(cost, from, to);

			AddEdge(e);
		}

		void AddEdge(Edge &e)
		{
			E.push_back(make_shared<Edge>(e));
			Adj[e.from].push_back(e.to);
			AdjIDs[e.from->id].push_back(e.to->id);
		}

		void MakeConnected(const vector<vector<float>> &DistanceMatrix2D)
		{
			for (auto d : V)
				for (auto a : V)
					if (d->id != a->id)
						AddEdge(DistanceMatrix2D[d->id][a->id], d, a);
		}

		shared_ptr<Node> NodeById(unsigned short id)
		{
			for (auto v : V)
				if (v->id == id)
					return v;
		}

		shared_ptr<Node> GetANode()
		{
			for (auto v : V)
				return v;
		}

		void PreVisit(stack<size_t> &R, unsigned short r)
		{
			R.push(r);

			for (auto v : V)
				if (v->π)
					if (v->π->id == r)
						PreVisit(R, v->id);
		}

		unsigned short degree_out(shared_ptr<Node> n)
		{
			return Adj[n].size();
		}

		void SortEdgeByWeight()
		{
			sort(E.begin(), E.end(), [](shared_ptr<Edge> l, shared_ptr<Edge> r) {
				return l->cost < r->cost;
			});
		}

		map<shared_ptr<Node>, unsigned short> Degree()
		{
			map<shared_ptr<Node>, unsigned short> R;

			for (auto e : E)
			{
				R[e->from] = 0;
				R[e->to] = 0;
			}

			for (auto e : E)
				R[e->from]++;

			for (auto e : E)
				R[e->to]++;

			return R;
		}

		bool HaveCycle()
		{
			auto D = Degree();

			for (auto v : V)
				if (D[v] != 2)
					return false;

			return true;
		}

		float Cost()
		{
			float cost = 0;

			for (auto e : E)
				cost += e->cost;

			return cost;
		}
	};
}