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
		curr_path.resize(numberOfNodes + 1, UINT16_MAX);
		visited.resize(numberOfNodes, false);
	}

	string BranchAndBound::PrintPath()
	{
		string s;

		for (unsigned short i = 0; i <= numberOfNodes; i++)
			s += to_string(curr_path[i]) + " ";

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

	void BranchAndBound::TSPRec(float curr_bound, float curr_weight, unsigned short level)
	{
		if (level == numberOfNodes)
		{
			if (distance[curr_path[level - 1]][curr_path[0]] != 0)
			{
				auto curr_res = curr_weight + distance[curr_path[level - 1]][curr_path[0]];

				if (curr_res < final_res)
					final_res = curr_res;
			}

			return;
		}

		for (unsigned short i = 0; i < numberOfNodes; i++)
			if (distance[curr_path[level - 1]][i] != 0 && visited[i] == false)
			{
				auto temp = curr_bound;
				curr_weight += distance[curr_path[level - 1]][i];

				if (level == 1)
					curr_bound -= ((firstMin(curr_path[level - 1]) + firstMin(i)) / 2);
				else
					curr_bound -= ((secondMin(curr_path[level - 1]) + firstMin(i)) / 2);

				if (curr_bound + curr_weight < final_res)
				{
					curr_path[level] = i;
					visited[i] = true;

					TSPRec(curr_bound, curr_weight, level + 1);
				}

				curr_weight -= distance[curr_path[level - 1]][i];
				curr_bound = temp;

				visited.assign(visited.size(), false);

				for (auto j = 0; j <= level - 1; j++)
					visited[curr_path[j]] = true;
			}
	}

	void BranchAndBound::Solve(float &opt, string &path)
	{
		float curr_bound = 0;

		for (unsigned short i = 0; i < numberOfNodes; i++)
			curr_bound += (firstMin(i) + secondMin(i));

		curr_bound /= 2;

		visited[0] = true;
		curr_path[0] = 0;

		TSPRec(curr_bound, 0, 1);
		curr_path[numberOfNodes] = 0;

		opt = final_res;
		path = PrintPath();

		currentCardinality = numberOfNodes;
	}
}