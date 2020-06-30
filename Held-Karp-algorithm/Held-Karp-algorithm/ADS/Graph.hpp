/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <memory>
#include <list>
#include <set>
#include <stack>
#include <vector>
#include <map>

using namespace std;

namespace ADS
{
	enum BlossomLabel
	{
		not_setted = 0,
		even = 1,
		odd = 2
	};

	enum Constraints
	{
		Free = 0,
		Forced = 1,
		Forbidden = 2
	};

	struct Node
	{
		unsigned short id;
		shared_ptr<Node> π;

		unsigned short rank = 0; // Kruskal
		float key = FLT_MAX; // Prim		


		// Blossom
		set<shared_ptr<Node>> B;
		shared_ptr<Node> mate;
		bool active = true, pseudo = false, marked = false, blossom = false;
		BlossomLabel label = not_setted;
		// Blossom
	};

	struct Edge
	{
		float cost = FLT_MAX;
		shared_ptr<Node> from, to;
		Constraints constraint = Free;

		Edge(float cost_, shared_ptr<Node> from_, shared_ptr<Node> to_) :
			cost(cost_),
			from(from_),
			to(to_)
		{
			assert(from_->id != to_->id);
		}
	};

	struct Graph
	{
		list<shared_ptr<Node>> V;
		vector<shared_ptr<Edge>> E; // sortable

		map<unsigned short, int> vrtx_frc, vrtx_frb;

		map<shared_ptr<Node>, list<shared_ptr<Node>>> Adj;
		vector<vector<unsigned short>> AdjIDs;

		unsigned short NumberOfNodes = 0;


		Graph(set<unsigned short> V_)
		{
			for (auto v : V_)
				AddNode(v);
		}

		Graph(list<shared_ptr<Node>> V_)
		{
			for (auto v : V_)
				AddNode(v->id);
		}

		Graph(const vector<vector<float>> &DistanceMatrix2D) : Graph(DistanceMatrix2D.size())
		{
			MakeConnected(DistanceMatrix2D);
		}

		Graph(unsigned short NumberOfNodes_) : Graph(NumberOfNodes_, 0, NumberOfNodes_ - 1) {}

		Graph(unsigned short NumberOfNodes_, unsigned short from, unsigned to)
		{
			AdjIDs.resize(NumberOfNodes_);

			for (unsigned short d = from; d <= to; d++)
				AddNode(d);
		}

		void graph_set_edge_cstr(shared_ptr<Edge> ie, Constraints c)
		{
			auto c_old = ie->constraint;
			ie->constraint = c;

			if (c_old == Forced)
			{
				vrtx_frc[ie->from->id]--;
				vrtx_frc[ie->to->id]--;
			}
			else if (c_old == Forbidden)
			{
				vrtx_frb[ie->from->id]--;
				vrtx_frb[ie->to->id]--;
			}

			if (c == Forced)
			{
				vrtx_frc[ie->from->id]++;
				vrtx_frc[ie->to->id]++;
			}
			else if (c == Forbidden)
			{
				vrtx_frb[ie->from->id]++;
				vrtx_frb[ie->to->id]++;
			}
		}

		void AddNode(unsigned short id_)
		{
			Node n;
			n.id = id_;

			V.push_back(make_shared<Node>(n));

			NumberOfNodes++;
		}


		void AddEdge(Edge &e)
		{
			E.push_back(make_shared<Edge>(e));
			Adj[e.from].push_back(e.to);

			if (AdjIDs.size() > 0)
				AdjIDs[e.from->id].push_back(e.to->id);
		}

		void AddEdge(float cost, shared_ptr<Node> from, shared_ptr<Node> to)
		{
			Edge e(cost, from, to);
			AddEdge(e);
		}

		void AddEdge(float cost, unsigned short from_, unsigned short to_)
		{
			AddEdge(cost, NodeById(from_), NodeById(to_));
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

			return nullptr;
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

		map<unsigned short, unsigned short> Degree()
		{
			map<unsigned short, unsigned short> R;

			for (auto e : E)
			{
				R[e->from->id] = 0;
				R[e->to->id] = 0;
			}

			for (auto e : E)
				R[e->from->id]++;

			for (auto e : E)
				R[e->to->id]++;

			return R;
		}

		bool HaveCycle()
		{
			auto D = Degree();

			for (auto v : V)
				if (D[v->id] != 2)
					return false;

			return true;
		}

		float Cost(const vector<vector<float>> &DistanceMatrix2D)
		{
			float cost = 0;

			for (auto e : E)
				cost += DistanceMatrix2D[e->from->id][e->to->id];

			return cost;
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