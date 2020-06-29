/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once


#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "Base/TSP.hpp"

using namespace std;

namespace TSP
{
	class HeldKarp : public Base::TSP
	{
	protected:
		struct sInfo
		{
			float cost;
			vector<unsigned short> path;
		};

		// <Code, <Node, Data>>
		queue<map<unsigned int, map<unsigned short, sInfo>>> C;

	protected:
		void AddNewToQueue();

		void CalcPath(const unsigned int code, const unsigned short π, float &opt, string &path);

		void Combinations(const unsigned short K, const unsigned short N);
		void Combinations_FreeMem(stack<unsigned short> &Q, vector<unsigned short> &S, const unsigned short K, const unsigned short N, const unsigned short sCur);

		void Solve(float &opt, string &path);

	public:
		HeldKarp(const vector<vector<float>> &DistanceMatrix2D);

	};
}