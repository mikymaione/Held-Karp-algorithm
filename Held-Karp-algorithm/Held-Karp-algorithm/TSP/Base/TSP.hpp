/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

namespace TSP
{
	namespace Base
	{
		class TSP
		{
		protected:
			// precalculated
			unsigned int POWER2[31] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824 };

			const vector<vector<float>> distance;
			const unsigned short numberOfNodes;

			time_point<system_clock> begin;
			atomic<unsigned short> currentCardinality = 0;
			atomic<unsigned short> maxCardinality = 1;
			atomic<bool> programEnded = false;
			atomic<bool> writingBuffer = false;

		protected:
			unsigned int Powered2Code(const vector<unsigned short> &S);

			unsigned int Powered2Code(const vector<unsigned short> &S, const unsigned short exclude);

			unsigned int Powered2Code(const unsigned int code, const unsigned short exclude);

			void ETL();
			void ETLw();

			template <class T>
			static T generateRandomNumber(const T startRange, const T endRange, const T limit);

			virtual void Solve(float &opt, string &path) = 0;

		public:
			TSP(const vector<vector<float>> &DistanceMatrix2D);

			void Run();
			void SilentSolve(float &opt, string &path);

			static vector<vector<float>> New_RND_Distances(const unsigned short Size_of_RandomDistanceCosts);

		};
	}
}