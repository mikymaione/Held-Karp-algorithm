/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "TSP.hpp"

namespace TSP
{
	TSP::TSP(const vector<vector<uint_least16_t>> &DistanceMatrix2D) :
		numberOfNodes(DistanceMatrix2D.size()),
		distance(DistanceMatrix2D) {}

	uint_least32_t TSP::Powered2Code(const vector<uint_least16_t> &S)
	{
		return Powered2Code(S, UINT_LEAST16_MAX);
	}

	uint_least32_t TSP::Powered2Code(const vector<uint_least16_t> &S, const uint_least16_t exclude)
	{
		uint_least32_t code = 0;

		for (const auto e : S)
			if (e != exclude)
				code += POWER2[e]; //code += 1 << e;

		return code;
	}

	uint_least32_t TSP::Powered2Code(const uint_least32_t code, const uint_least16_t exclude)
	{
		return code - POWER2[exclude];
		//return code - (1 << exclude);
	}

	template <class T>
	T TSP::generateRandomNumber(const T startRange, const T endRange, const T limit)
	{
		const T r = rand();
		const T range = 1 + endRange - startRange;
		const T num = r % range + startRange;

		return num;
	}

	void TSP::ETLw()
	{
		while (writingBuffer)
			this_thread::sleep_for(milliseconds(100));

		writingBuffer = true;

		const auto T = duration_cast<seconds>(system_clock::now() - begin).count();
		const auto s = currentCardinality._My_val;

		stringstream ss;
		ss
			<< " "
			<< 100 * s / numberOfNodes
			<< "% - ET: "
			<< T
			<< "s ETL: "
			<< (numberOfNodes - s) * T / s
			<< "s\r";

		cout << ss.str();
		fflush(stdin);

		writingBuffer = false;
	}

	void TSP::ETL()
	{
		while (currentCardinality < numberOfNodes - 1)
		{
			ETLw();
			this_thread::sleep_for(seconds(1));
		}
	}

	vector<vector<uint_least16_t>> TSP::New_RND_Distances(const uint_least16_t Size_of_RandomDistanceCosts)
	{
		vector<vector<uint_least16_t>> A(Size_of_RandomDistanceCosts, vector<uint_least16_t>(Size_of_RandomDistanceCosts, 0));

		for (auto x = 0; x < Size_of_RandomDistanceCosts; x++)
			for (auto y = 0; y < Size_of_RandomDistanceCosts; y++)
				A[x][y] = (x == y ? 0 : generateRandomNumber<uint_least16_t>(1, 100, UINT_LEAST16_MAX));

		return A;
	}

	void TSP::Run()
	{
		cout
			<< "Solving a graph of "
			<< to_string(numberOfNodes)
			<< " nodes:"
			<< endl;

		begin = system_clock::now();
		thread tETL(&TSP::ETL, this);

		try
		{
			uint_least16_t opt;
			string path;

			Solve(opt, path);

			cout
				<< "-Cost: "
				<< to_string(opt)
				<< " time: "
				<< duration_cast<milliseconds>(system_clock::now() - begin).count()
				<< "ms, path: "
				<< path
				<< endl;
		}
		catch (const exception &e)
		{
			if (tETL.joinable())
				tETL.join();

			throw e;
		}

		if (tETL.joinable())
			tETL.join();
	}
}