/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <vector>

using namespace std;

namespace ADS
{
	struct sEdge
	{
		unsigned short from, to;

		sEdge() : sEdge(UINT16_MAX, UINT16_MAX) {}
		sEdge(unsigned short from, unsigned short to) : from(from), to(to) {}
	};

	struct sTree
	{
		vector<sEdge> T;

		sTree(unsigned short size) : T(size) {}

		sEdge &operator[](unsigned short i)
		{
			return T[i];
		}

		sEdge operator[](unsigned short i) const
		{
			return T[i];
		}

		unsigned short size()
		{
			return T.size();
		}

		void GetMin0Nodes(const vector<vector<float>> &distance, unsigned short &id1, unsigned short &id2)
		{
			float min1 = FLT_MAX;
			float min2 = FLT_MAX;

			for (unsigned short i = 1; i < size(); i++)
				if (distance[0][i] < min1)
				{
					min1 = distance[0][i];
					id1 = i;
				}

			for (unsigned short i = 1; i < size(); i++)
				if (i != id1)
					if (distance[0][i] < min2)
					{
						min2 = distance[0][i];
						id2 = i;
					}
		}

		bool Contains(unsigned short i, unsigned short j)
		{
			for (auto e : T)
				if ((e.from == i && e.to == j) || (e.from == j && e.to == i))
					return true;

			return false;
		}

		bool CheckTour()
		{
			vector<unsigned short> δ(T.size(), 0);

			for (auto e : T)
			{
				δ[e.from]++;
				δ[e.to]++;
			}

			for (auto d : δ)
				if (d != 2)
					return true;

			return false;
		}
	};

	struct sNode
	{
		float bound = 0;

		vector<float> λ;
		vector<sEdge> R, F;
		sTree oneTree;

		sNode(unsigned short size) : oneTree(size), λ(size) {}

		sNode(vector<sEdge> R, vector<sEdge> F, vector<float> λ, unsigned short size) : R(R), F(F), λ(λ), oneTree(size) {}

		bool Forbidden(unsigned short i, unsigned short j)
		{
			for (auto f : F)
				if ((f.from == i && f.to == j) || (f.from == j && f.to == i))
					return true;

			return false;
		}

		unsigned short IncidentToRequired(unsigned short v, unsigned short n)
		{
			for (auto r : R)
				if (v == r.from)
					return r.to;
				else if (v == r.to)
					return r.from;

			return n;
		}
	};
}