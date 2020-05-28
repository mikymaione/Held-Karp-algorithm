﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once


#include <set>
#include <stack>
#include <string>
#include <vector>

#include "TSP.hpp"

using namespace std;

namespace TSP
{
	class Christofides : public TSP
	{
	private:
		vector<vector<unsigned short>> out_star;

	private:
<<<<<<< HEAD
		float ToHamiltonianPath(vector<unsigned short> &path);
		float findBestPath(unsigned short start);
=======
		unsigned short ToHamiltonianPath(vector<unsigned short> &path);
		unsigned short findBestPath(unsigned short start);
>>>>>>> 0932d920e28ca0de3277e8a7b4fdee6cf85fd2bd

		vector<unsigned short> FindEulerCircuit(unsigned short start);

		void WeightedPerfectMatching();
		void MinimumSpanningTree_Prim();

	protected:
		string PrintPath(vector<unsigned short> circuit);

		void Solve(float &opt, string &path);

	public:
		Christofides(const vector<vector<float>> &DistanceMatrix2D);

	};
}