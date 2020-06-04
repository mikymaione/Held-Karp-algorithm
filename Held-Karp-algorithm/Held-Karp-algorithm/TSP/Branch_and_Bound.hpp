/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <utility>
#include <vector>

#include "Base/TSP.hpp"
#include "../ADS/sGraph.hpp"

using namespace std;
using namespace ADS;

namespace TSP
{
	class Branch_and_Bound : public Base::TSP
	{
	private:
		unsigned short CountElements(vector<sEdge> &edges, vector<unsigned short> &from, unsigned short i)
		{
			unsigned short x = 0;

			for (auto f : edges)
			{
				if (f.from == i)
				{
					from[f.to] = 1;
					x++;
				}

				if (f.to == i)
				{
					from[f.from] = 1;
					x++;
				}
			}

			return x;
		}

	private:
		bool Bound(sNode &node, vector<unsigned short> &δ, float t, unsigned short const steps);
		vector<sNode> Branch(sTree &T, vector<unsigned short> &δ, sNode &current_node, unsigned short n);

		float t1();

		void PQ_Add(vector<sNode> &L, sNode &new_elem);

	protected:
		string PrintPath(vector<sEdge> &path);

		void Solve(float &opt, string &path);

	public:
		Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D);

	};
}