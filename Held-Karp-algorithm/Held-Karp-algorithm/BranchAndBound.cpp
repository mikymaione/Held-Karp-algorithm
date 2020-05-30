/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "BranchAndBound.hpp"

namespace TSP
{
	BranchAndBound::BranchAndBound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D)
	{
		current_path.resize(numberOfNodes + 1, UINT16_MAX);
		candidate_queue.resize(numberOfNodes, false);
	}

	string BranchAndBound::PrintPath()
	{
		string s;

		for (unsigned short i = 0; i <= numberOfNodes; i++)
			s += to_string(current_path[i]) + " ";

		return s;
	}


	float BranchAndBound::firstMin(unsigned short i)
	{
		auto min = FLT_MAX;

		for (unsigned short k = 0; k < numberOfNodes; k++)
			if (i != k)
				if (distance[i][k] < min)
					min = distance[i][k];

		return min;
	}

	float BranchAndBound::secondMin(unsigned short i)
	{
		auto first = FLT_MAX;
		auto second = FLT_MAX;

		for (unsigned short j = 0; j < numberOfNodes; j++)
			if (i != j)
				if (distance[i][j] <= first)
				{
					second = first;
					first = distance[i][j];
				}
				else if (distance[i][j] <= second && distance[i][j] != first)
				{
					second = distance[i][j];
				}

		return second;
	}

	void BranchAndBound::Elaborate(float problem_upper_bound, float W, unsigned short L)
	{
		//currentCardinality = L;

		if (L == numberOfNodes)
		{
			if (distance[current_path[L - 1]][current_path[0]] != 0)
			{
				auto min = W + distance[current_path[L - 1]][current_path[0]];

				if (min < current_optimum)
					current_optimum = min;
			}

			return;
		}

		for (unsigned short i = 0; i < numberOfNodes; i++)
			if (distance[current_path[L - 1]][i] != 0 && candidate_queue[i] == false)
			{
				auto temp = problem_upper_bound;
				W += distance[current_path[L - 1]][i];

				if (L == 1)
					problem_upper_bound -= ((firstMin(current_path[L - 1]) + firstMin(i)) / 2);
				else
					problem_upper_bound -= ((secondMin(current_path[L - 1]) + firstMin(i)) / 2);

				if (problem_upper_bound + W < current_optimum)
				{
					current_path[L] = i;
					candidate_queue[i] = true;

					Elaborate(problem_upper_bound, W, L + 1);
				}

				W -= distance[current_path[L - 1]][i];
				problem_upper_bound = temp;

				candidate_queue.assign(candidate_queue.size(), false);

				for (auto j = 0; j <= L - 1; j++)
					candidate_queue[current_path[j]] = true;
			}
	}

	void BranchAndBound::Solve(float &opt, string &path)
	{
		float problem_upper_bound = 0;

		for (unsigned short i = 0; i < numberOfNodes; i++)
			problem_upper_bound += (firstMin(i) + secondMin(i));

		problem_upper_bound /= 2;

		candidate_queue[0] = true;
		current_path[0] = 0;

		Elaborate(problem_upper_bound, 0, 1);
		current_path[numberOfNodes] = 0;

		opt = current_optimum;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}