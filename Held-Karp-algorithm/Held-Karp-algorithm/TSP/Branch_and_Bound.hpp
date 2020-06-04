/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <vector>
#include <utility>

#include "Base/TSP.hpp"

using namespace std;

namespace TSP
{
	class Branch_and_Bound : public Base::TSP
	{
	public:
		struct Node
		{
			//required edges
			vector<pair<unsigned short, unsigned short>> R;

			//forbidden edges
			vector<pair<unsigned short, unsigned short>> F;

			//initial λ vector
			vector<float> λ;

			//one_tree
			vector<pair<unsigned short, unsigned short>> one_tree;

			//Held-Karp-Bound
			float HK = 0;

			Node(vector<pair<unsigned short, unsigned short>> R, vector<pair<unsigned short, unsigned short>> F, vector<float> λ, unsigned short size) :
				R(R),
				F(F),
				λ(λ)
			{
				one_tree.resize(size);
			}
		};

	private:
		vector<Node> branch(vector<pair<unsigned short, unsigned short>> const &tree, vector<unsigned short> const &degrees, Node &current_node, unsigned short n);
		pair<vector<pair<unsigned short, unsigned short>>, float> DoBranch_and_Bound(vector<vector<float>> const &W);
		bool Held_Karp_bound(vector<vector<float>> const &W, Node &node, vector<unsigned short> &degree, float t, unsigned short const steps);

		bool MST_Prim(vector<pair<unsigned short, unsigned short>> &Tree, vector<vector<unsigned short>> const &omitted, vector<vector<float>> const &Weights, unsigned short const req);

		unsigned short is_incident_to_required(Node &current_node, unsigned short v, unsigned short n);
		unsigned short is_in_tree(vector<pair<unsigned short, unsigned short>> const &tree, unsigned short i, unsigned short j);
		unsigned short is_forbidden(Node const &node, unsigned short i, unsigned short j);
		void insert(vector<Node> &L, Node &new_elem);
		float initial_value(vector<vector<float>> const &W);
		bool check_tour(vector<pair<unsigned short, unsigned short>> const &Tree);

	protected:
		string PrintPath(vector<pair<unsigned short, unsigned short>> &path);

		void Solve(float &opt, string &path);

	public:
		Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D);

	};
}