﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <set>
#include <vector>

#include "TSP.hpp"

using namespace std;

namespace TSP
{
	class Christofides : public TSP
	{
	private:

		template<class A, class B>
		struct e2
		{
			A a;
			B b;
		};

		vector<vector<uint_least8_t>> Adj;
		vector<uint_least8_t> circuit;

	private:
		set<uint_least8_t> findOdds();

		uint_least16_t make_hamiltonian(vector<uint_least8_t> &path);
		uint_least16_t findBestPath(uint_least8_t start);

		vector<uint_least8_t> euler_tour(uint_least8_t start);

		void perfectMatching();
		void findMST();

	protected:
		string PrintPath(const uint_least32_t code, const uint_least8_t π);

		void Solve(uint_least16_t &opt, string &path);

	public:
		Christofides(const vector<vector<uint_least8_t>> &DistanceMatrix2D);

	};
}