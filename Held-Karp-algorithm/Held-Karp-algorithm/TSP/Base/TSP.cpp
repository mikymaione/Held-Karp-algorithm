/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Held–Karp algorithm
The Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).

T(n) = O(n²)

TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.
*/
#pragma once

#include "TSP.hpp"

namespace TSP
{
	namespace Base
	{

		TSP::TSP(const vector<vector<float>> &DistanceMatrix2D) :
			numberOfNodes(DistanceMatrix2D.size()),
			distance(DistanceMatrix2D) {}

		unsigned int TSP::Powered2Code(const vector<unsigned short> &S)
		{
			return Powered2Code(S, UINT16_MAX);
		}

		unsigned int TSP::Powered2Code(const vector<unsigned short> &S, const unsigned short exclude)
		{
			unsigned int code = 0;

			for (const auto e : S)
				if (e != exclude)
					code += POWER2[e]; //code += 1 << e;

			return code;
		}

		unsigned int TSP::Powered2Code(const unsigned int code, const unsigned short exclude)
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
			const auto m = maxCardinality._My_val;

			if (s > 0)
			{
				stringstream ss;
				ss
					<< " "
					<< 100 * s / m
					<< "% - ET: "
					<< T
					<< "s ETL: "
					<< (m - s) * T / s
					<< "s\r";

				cout << ss.str();
				fflush(stdin);
			}

			writingBuffer = false;
		}

		void TSP::ETL()
		{
			while (!programEnded)
			{
				ETLw();
				this_thread::sleep_for(seconds(1));
			}
		}

		vector<vector<float>> TSP::New_RND_Distances(const unsigned short Size_of_RandomDistanceCosts)
		{
			vector<vector<float>> A(Size_of_RandomDistanceCosts, vector<float>(Size_of_RandomDistanceCosts, 0));

			for (auto x = 0; x < Size_of_RandomDistanceCosts; x++)
				for (auto y = 0; y < Size_of_RandomDistanceCosts; y++)
					A[x][y] = (x == y ? 0 : generateRandomNumber<unsigned char>(1, 100, UCHAR_MAX));

			return A;
		}

		void TSP::SilentSolve(float &opt, string &path)
		{
			Solve(opt, path);
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
				float opt;
				string path;

				Solve(opt, path);

				programEnded = true;

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
}