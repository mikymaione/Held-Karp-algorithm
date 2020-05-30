/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <list>
#include <set>
#include <vector>

#include "TSP.hpp"

using namespace std;

namespace TSP
{
	class BranchAndBound : public TSP
	{
	private:
		struct Edge
		{
			unsigned short u, v;

			Edge() {}

			Edge(unsigned short u_, unsigned short v_) :
				u(u_), v(v_) {}
		};

	private:
		template<class T, class Z>
		vector<vector<T>> MatrixMultiplication(const vector<vector<T>> &Mat1, const  vector<vector<Z>> &Mat2);

		template<class T>
		vector<T> MatrixSum(const vector<vector<T>> &A);

		template<class T>
		T MatrixSum(const vector<T> &A);

		void MST_Prim(list<Edge> &adjacency_list, vector<vector<bool>> &adjacency);
		void Subgradient(unsigned short alpha, unsigned short initial_step, unsigned short max_iter);

	protected:
		string PrintPath();

		void Solve(float &opt, string &path);

	public:
		BranchAndBound(const vector<vector<float>> &DistanceMatrix2D);

	};
}