﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <list>
#include <map>
#include <vector>

#include "TSP.hpp"

using namespace std;

namespace TSP
{
	class BranchAndBound : public TSP
	{
	private:
		struct Node
		{
			unsigned short id;
			float key;
			Node *π;

			bool operator < (const Node &n) const
			{
				return (key < n.key);
			}
		};

		struct Graph
		{
			list<Node> V;
			map<Node *, list<Node *>> Adj;

			Graph(unsigned short N, unsigned short from, unsigned to)
			{
				for (unsigned short d = from; d <= to; d++)
				{
					Node n;
					n.id = d;
					n.key = FLT_MAX;
					n.π = NULL;

					V.push_back(n);
				}
			}

			Node *NodeById(unsigned short id)
			{
				for (auto v : V)
					if (v.id == id)
						return &v;
			}
		};

	private:
		Node *OneTree(Graph &G);
		list<Node> MST_Prim(Graph &G, Node *r);

	protected:
		string PrintPath();

		void Solve(float &opt, string &path);

	public:
		BranchAndBound(const vector<vector<float>> &DistanceMatrix2D);

	};
}