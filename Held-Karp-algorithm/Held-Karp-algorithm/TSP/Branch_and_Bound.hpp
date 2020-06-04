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
		struct QNode
		{
			//required edges
			vector<pair<unsigned int, unsigned int>> R;

			//forbidden edges
			vector<pair<unsigned int, unsigned int>> F;

			//initial λ vector
			vector<float> λ;

			//one_tree
			vector<pair<unsigned int, unsigned int>> one_tree;

			//Held-Karp-Bound
			float HK = 0;

			QNode(vector<pair<unsigned int, unsigned int>> R, vector<pair<unsigned int, unsigned int>> F, vector<float> λ, unsigned int size) :
				R(R),
				F(F),
				λ(λ)
			{
				one_tree.resize(size);
			}
		};

	private:
		pair<vector<pair<unsigned int, unsigned int>>, float> DoBranch_and_Bound(vector<vector<float>> const &W);
		bool Held_Karp_bound(vector<vector<float>> const &W, QNode &QNode, vector<int> &degree, float t, unsigned int const steps);
		vector<QNode> branch(vector<pair<unsigned int, unsigned int>> const &tree, vector<int> const &degrees, QNode &current_node, unsigned int n);

		bool minimum_spanning_tree(vector<pair<unsigned int, unsigned int>> &Tree, vector<vector<int>> const &omitted, vector<vector<float>> const &Weights, unsigned int const req);

		unsigned int is_incident_to_required(QNode &current_node, unsigned int v, unsigned int n);
		unsigned int is_in_tree(vector<pair<unsigned int, unsigned int>> const &tree, unsigned int i, unsigned int j);
		unsigned int is_forbidden(QNode const &node, unsigned int i, unsigned int j);
		void insert(vector<QNode> &L, QNode &new_elem);
		float initial_value(vector<vector<float>> const &W);
		bool check_tour(vector<pair<unsigned int, unsigned int>> const &Tree);

	protected:
		string PrintPath(vector<pair<unsigned int, unsigned int>> &path);

		void Solve(float &opt, string &path);

	public:
		Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D);

	};
}