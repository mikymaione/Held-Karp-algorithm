/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <vector>

#include "TSP.hpp"

using namespace std;

namespace TSP
{
	class Righini : public TSP
	{
	protected:
		struct Node
		{
			unsigned short rank, p;
		};

		struct Edge
		{
			unsigned short u, v;
			float w;

			bool operator < (const Edge &e) const
			{
				return w < e.w;
			}
		};

		struct Graph
		{
			vector<Node> node;
			vector<Edge> edge;
		};

	protected:
		string PrintPath(const unsigned int code, const unsigned short π);

		void Solve(float &opt, string &path);

	private:
		Graph graphFromDistanceMatrix(const unsigned short nodes);

		// Kruskal
		unsigned short FindSet(Graph &G, unsigned short x);
		void MakeSet(Graph &G, unsigned short x);
		void Link(Graph &G, unsigned short x, unsigned short y);
		void Union(Graph &G, unsigned short x, unsigned short y);
		vector<Edge*> Kruskal(Graph &G);
		// Kruskal

	public:
		Righini(const vector<vector<float>> &DistanceMatrix2D);

	};
}