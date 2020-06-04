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

using namespace std;

namespace TSP
{
	class Branch_and_Bound : public Base::TSP
	{
	private:
		struct Edge
		{
			unsigned short from, to;

			Edge() : Edge(UINT16_MAX, UINT16_MAX) {}
			Edge(unsigned short from, unsigned short to) : from(from), to(to) {}
		};

		struct Tree
		{
			vector<Edge> T;

			Tree(unsigned short size) : T(size) {}

			Edge &operator[](unsigned short i)
			{
				return T[i];
			}

			Edge operator[](unsigned short i) const
			{
				return T[i];
			}

			unsigned short size()
			{
				return T.size();
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

		struct Node
		{
			float bound = 0;

			vector<float> λ;
			vector<Edge> R, F;
			Tree oneTree;

			Node(unsigned short size) : oneTree(size), λ(size) {}

			Node(vector<Edge> R, vector<Edge> F, vector<float> λ, unsigned short size) : R(R), F(F), λ(λ), oneTree(size) {}

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

	private:
		pair<vector<Edge>, float> HKAlgo();
		bool Bound(Node &node, vector<unsigned short> &degree, float t, unsigned short const steps);
		vector<Node> Branch(Tree &tree, vector<unsigned short> &degrees, Node &current_node, unsigned short n);

		float t1();

		bool MST_Prim(Tree &tree, vector<vector<unsigned short>> &omitted, const vector<vector<float>> &Weights, const unsigned short req);
		void PQ_Add(vector<Node> &L, Node &new_elem);

	private:
		unsigned short CountElements(vector<Edge> &edges, vector<unsigned short> &from, unsigned short i)
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

	protected:
		string PrintPath(vector<Edge> &path);

		void Solve(float &opt, string &path);

	public:
		Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D);

	};
}