/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <queue>

#include <Eigen/Core>

#include "BranchAndBound.hpp"

namespace TSP
{
	BranchAndBound::BranchAndBound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D)
	{

	}

	string BranchAndBound::PrintPath()
	{
		string s;

		return s;
	}

	void BranchAndBound::MST_Prim(list<Edge> &adjacency_list, vector<vector<bool>> &adjacency)
	{
		auto n = numberOfNodes;
		auto N = numberOfNodes - 1;
		vector<unsigned short> T(1, 0);

		vector<unsigned short> S(numberOfNodes);
		iota(S.begin(), S.end(), 1);

		vector<vector<float>> cost(N, vector<float>(N, 0));

		for (auto x = 0; x < N; x++)
			for (auto y = 0; y < N; y++)
				cost[x][y] = distance[x + 1][y + 1];

		for (auto i = 0; i < N; i++)
			cost[i][i] = FLT_MAX;

		auto DISTANCE = distance; // clone		

		adjacency_list.clear();

		while (T.size() < N)
		{
			vector<float> list_distance;
			vector<Edge> list_position;

			auto t = 0;

			for (const auto i : T)
			{
				auto j_it = find(cost[i].begin(), cost[i].end(), *min_element(cost[i].begin(), cost[i].end()));
				auto j = std::distance(cost[i].begin(), j_it);

				list_distance.push_back(cost[i][j]);
				list_position.push_back(Edge(i, j));

				t++;
			}

			auto smallest_distance_it = find(list_distance.begin(), list_distance.end(), *min_element(list_distance.begin(), list_distance.end()));
			auto smallest_distance = std::distance(list_distance.begin(), smallest_distance_it);

			auto edge = list_position[smallest_distance];

			DISTANCE[edge.u][edge.v] = FLT_MAX;
			DISTANCE[edge.v][edge.u] = FLT_MAX;

			for (const auto i : T)
			{
				cost[i][edge.v] = FLT_MAX;
				cost[edge.v][i] = FLT_MAX;
			}

			adjacency_list.push_back(Edge(edge.u + 1, edge.v + 1));

			adjacency[edge.u + 1][edge.v + 1] = 0;
			adjacency[edge.v + 1][edge.u + 1] = 0;

			T.push_back(edge.v);
		}

		auto j0_it = find(DISTANCE[0].begin(), DISTANCE[0].end(), *min_element(DISTANCE[0].begin(), DISTANCE[0].end()));
		auto j0 = std::distance(DISTANCE[0].begin(), j0_it);
		DISTANCE[0][j0] = FLT_MAX;

		auto j1_it = find(DISTANCE[0].begin(), DISTANCE[0].end(), *min_element(DISTANCE[0].begin(), DISTANCE[0].end()));
		auto j1 = std::distance(DISTANCE[0].begin(), j1_it);

		adjacency_list.push_back(Edge(0, j0));
		adjacency_list.push_back(Edge(0, j1));

		adjacency[0][j0] = 0;
		adjacency[j0][0] = 0;
		adjacency[0][j1] = 0;
		adjacency[j1][0] = 0;
	}

	void BranchAndBound::Subgradient(unsigned short alpha, unsigned short initial_step, unsigned short max_iter)
	{
		vector<unsigned short> lambda(numberOfNodes, 0);
		vector<unsigned short> e(numberOfNodes, 1);

		auto iteration = 0;
		auto max = max_iter;
		auto step = initial_step;
		alpha = 0.9;

		auto NewCosts = distance; // clone

		while (iteration < max)
		{
			iteration++;

			vector<vector<bool>> X(numberOfNodes, vector<bool>(numberOfNodes, 0));
			list<Edge> adjacency_list;

			MST_Prim(adjacency_list, X);

			auto MXM = MatrixMultiplication(distance, X);
			auto M2 = MatrixSum(MXM);
			auto Langrangian = MatrixSum(M2) / 2;
			//Langrangian -= lambda* (e * 2 - MatrixSum(X));

			//accumulate(v.begin(), v.end(), 0);
		}
	}

	template<class T>
	T BranchAndBound::MatrixSum(const vector<T> &A)
	{
		T R;

		for (auto x = 0; x < A.size(); x++)
			R += A[x];

		return R;
	}

	template<class T>
	vector<T> BranchAndBound::MatrixSum(const vector<vector<T>> &A)
	{
		vector<T> R(A.size(), 0);

		for (auto x = 0; x < A.size(); x++)
			for (auto y = 0; y < A[x].size(); y++)
				R[x] += A[x][y];

		return R;
	}

	template<class T, class Z>
	vector<vector<T>> BranchAndBound::MatrixMultiplication(const vector<vector<T>> &Mat1, const vector<vector<Z>> &Mat2)
	{
		auto a = Mat1.size();
		auto b = Mat1[0].size();
		auto c = Mat2.size();
		auto d = Mat2[0].size();

		vector<vector<T>> Mat3(a, vector<T>(d, 0));

		for (auto i = 0; i < a; i++)
			for (auto j = 0; j < d; j++)
				for (auto k = 0; k < c; k++)
					Mat3[i][j] += Mat1[i][k] * Mat2[k][j];
	}

	void BranchAndBound::Solve(float &opt, string &path)
	{

		opt = 0;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}