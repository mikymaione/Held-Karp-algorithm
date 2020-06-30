/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <memory>
#include <list>
#include <set>
#include <stack>

#include "../ADS/Graph.hpp"

using namespace std;
using namespace ADS;

namespace Matching
{
	class Blossom
	{
	private:
		map<shared_ptr<Node>, list<shared_ptr<Node>>> Adj_;
		list<shared_ptr<Node>> V_;
		set<shared_ptr<Edge>> M;
		stack<shared_ptr<Node>> L;
		shared_ptr<Graph> G;

	private:
		shared_ptr<Node> FindMarked(list<shared_ptr<Node>> Z);
		set<shared_ptr<Node>> FindBlossom(shared_ptr<Node> p, shared_ptr<Node> i, shared_ptr<Node> j);

		void Augment(shared_ptr<Node> p, shared_ptr<Node> q);
		void Contract(shared_ptr<Node> p, shared_ptr<Node> i, shared_ptr<Node> j);

		void ExpandPred(shared_ptr<Node> k);

		shared_ptr<Node> ExamineEven(shared_ptr<Node> p, shared_ptr<Node> i, bool &found);
		void ExamineOdd(shared_ptr<Node> p, shared_ptr<Node> i);

		bool SetContainsThisEdge(set<shared_ptr<Edge>> S, shared_ptr<Edge> e);
		set<shared_ptr<Edge>> SymmetricDifference(set<shared_ptr<Edge>> &S1, set<shared_ptr<Edge>> &S2);

		void Search(shared_ptr<Node> p);
	public:
		set<shared_ptr<Edge>> Solve(shared_ptr<Graph> graph);

	};
}