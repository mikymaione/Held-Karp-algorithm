/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <map>

#include "Blossom.hpp"

namespace Matching
{

	set<shared_ptr<Node>> Blossom::FindBlossom(shared_ptr<Node> p, shared_ptr<Node> i, shared_ptr<Node> j)
	{
		set<shared_ptr<Node>> B;

		for (auto k : G->V)
			k->blossom = false;

		auto k = i;
		k->blossom = true;

		do
		{
			k = k->π;
			k->blossom = true;
		} while (k != p);

		k = j;

		do
		{
			k->blossom = true;
			k = k->π;
		} while (!k->blossom);

		auto w = k;

		do
		{
			k = k->π;
			k->blossom = false;
		} while (k != p);

		for (auto k : G->V)
			if (k->blossom)
			{
				B.insert(k);
				k->active = false;
			}

		return B;
	}

	void Blossom::Contract(shared_ptr<Node> p, shared_ptr<Node> i, shared_ptr<Node> j)
	{
		shared_ptr<Node> b; // memory node

		b->B = FindBlossom(p, i, j);
		b->pseudo = true;

		V_.push_back(b);
		Adj_[b].clear();

		for (auto k : G->V)
			k->marked = false;

		for (auto y : b->B)
			for (auto k : Adj_[y])
				k->marked = true;

		for (auto k : G->V)
			if (k->marked)
			{
				Adj_[b].push_back(j);
				Adj_[j].push_back(b);
			}
	}

	shared_ptr<Node> Blossom::ExamineEven(shared_ptr<Node> p, shared_ptr<Node> i, bool &found)
	{
		for (auto j : Adj_[i])
		{
			if (j->label == BlossomLabel::even)
			{
				Contract(p, i, j);

				return nullptr;
			}

			if (j->active && j->mate == nullptr)
			{
				auto q = j;
				q->π = i;
				found = true;

				return q;
			}

			if (j->active && j->label == BlossomLabel::not_setted)
			{
				j->π = i;
				j->label = BlossomLabel::odd;

				L.push(j);
			}
		}

		return nullptr;
	}

	void Blossom::ExamineOdd(shared_ptr<Node> p, shared_ptr<Node> i)
	{
		auto j = i->mate;

		if (j->label == BlossomLabel::odd)
		{
			j->π = i;
			Contract(p, i, j);

			return;
		}

		if (j->mate == nullptr && j->label == BlossomLabel::not_setted)
		{
			j->π = i;
			j->label = BlossomLabel::even;

			L.push(j);
		}
	}

	void Blossom::Search(shared_ptr<Node> p)
	{
		auto found = false;
		shared_ptr<Node> q;

		V_ = G->V;
		Adj_ = G->Adj;

		p->label = BlossomLabel::even;
		L.push(p);

		while (!L.empty() && !found)
		{
			auto i = L.top();
			L.pop();

			if (i->label == BlossomLabel::even)
				q = ExamineEven(p, i, found);
			else
				ExamineOdd(p, i);
		}

		if (found)
			Augment(p, q);
	}

	shared_ptr<Node> Blossom::FindMarked(list<shared_ptr<Node>> Z)
	{
		for (auto z : Z)
			if (z->marked)
				return z;

		return nullptr;
	}

	void Blossom::ExpandPred(shared_ptr<Node> k)
	{
		auto b = k->π;

		for (auto i : V_)
			i->marked = false;

		for (auto i : b->B)
			i->marked = true;

		auto j = FindMarked(Adj_[k]);
		k->π = j;
	}

	bool Blossom::SetContainsThisEdge(set<shared_ptr<Edge>> S, shared_ptr<Edge> e)
	{
		for (auto x : S)
			if (x->from == e->from ||
				x->to == e->to ||
				x->from == e->to ||
				x->to == e->from)
				return true;

		return false;
	}

	set<shared_ptr<Edge>> Blossom::SymmetricDifference(set<shared_ptr<Edge>> &S1, set<shared_ptr<Edge>> &S2)
	{
		set<shared_ptr<Edge>> symmetricDifference;

		for (auto e : S1)
			symmetricDifference.insert(e);

		for (auto e : S2)
			if (!SetContainsThisEdge(S1, e))
				symmetricDifference.insert(e);

		return symmetricDifference;
	}

	void Blossom::Augment(shared_ptr<Node> p, shared_ptr<Node> q)
	{
		set<shared_ptr<Edge>> P;
		auto k = q;

		do
		{
			if (k->π->pseudo)
			{
				ExpandPred(k);
			}
			else
			{
				Edge e(0, k->π, k);
				P.insert(make_shared<Edge>(e));
				k = k->π;
			}
		} while (k != p);

		M = SymmetricDifference(M, P);
	}


	set<shared_ptr<Edge>> Blossom::Solve(shared_ptr<Graph> graph)
	{
		set<unsigned short> done;

		G = graph;

	reiterate:
		for (auto p : G->V)
			if (done.count(p->id) == 0)
			{
				done.insert(p->id);

				if (p->mate == nullptr)
				{
					Search(p);

					if (p->mate == nullptr)
					{
						G->V.remove(p);
						G->Adj.erase(p);

						for (auto v : G->V)
							G->Adj[v].remove(p);

						G->NumberOfNodes--;
						goto reiterate;
					}
				}
			}

		return M;
	}
}