/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

#include "HeldKarp.hpp"
#include "Christofides.hpp"

using namespace TSP;
using namespace std;
using namespace std::experimental;

void Run(string tipo, const uint_least8_t NumberOfNodes)
{
	string z, e;
	auto delim = ',';
	size_t current, previous;
	uint_least8_t x, y;

	vector<vector<uint_least8_t>> DistanceMatrix2D(NumberOfNodes, vector<uint_least8_t>(NumberOfNodes, 0));

	auto curP = filesystem::current_path();
	curP.append("TSP_Instances\\" + tipo + to_string(NumberOfNodes) + ".txt");

	ifstream input(curP);

	for (y = 0; y < NumberOfNodes; y++)
	{
		input >> z;

		previous = 0;
		current = z.find(delim);
		x = 0;

		while (current != string::npos)
		{
			e = z.substr(previous, current - previous);
			previous = current + 1;
			current = z.find(delim, previous);

			DistanceMatrix2D[x][y] = stoi(e);
			x++;
		}
	}

	HeldKarp hk(DistanceMatrix2D);
	hk.Run();
}

void StartElaboration_aTSP(const string graphToSolve)
{
	if (graphToSolve == "all" || graphToSolve == "4")
		Run("asym", 4);

	if (graphToSolve == "all" || graphToSolve == "10")
		Run("asym", 10);

	if (graphToSolve == "all" || graphToSolve == "15")
		Run("asym", 15);

	if (graphToSolve == "all" || graphToSolve == "20")
		Run("asym", 20);

	if (graphToSolve == "all" || graphToSolve == "25")
		Run("asym", 25);
}

void StartElaboration_sTSP(const string graphToSolve)
{

	if (graphToSolve == "all" || graphToSolve == "4")
		Run("sym", 4);

	if (graphToSolve == "all" || graphToSolve == "10")
		Run("sym", 10);

	if (graphToSolve == "all" || graphToSolve == "15")
		Run("sym", 15);

	if (graphToSolve == "all" || graphToSolve == "20")
		Run("sym", 20);

	if (graphToSolve == "all" || graphToSolve == "25")
		Run("sym", 25);

	if (graphToSolve == "all")
	{
		Run("sym", 100);
		Run("sym", 500);
		Run("sym", 1000);
		Run("sym", 5000);
	}
}

void WriteTitle()
{
	cout
		<< "Held-Karp algorithm to solve the Traveling Salesman Problem" << endl
		<< endl
		<< "Held-Karp-algorithm parameters: " << endl
		<< " [graph to solve = {3, 4, 5, 6, 15, 20, 25, 30, all}]" << endl
		<< endl
		<< endl
		<< "Copyright 2020 (c) [MAIONE MIKY]. All rights reserved." << endl
		<< "Licensed under the MIT License." << endl
		<< endl
		<< endl;
}

int main(int argc, char **argv)
{
	WriteTitle();

	try
	{
		if (argc > 1)
		{
			const string graphToSolve = argv[1];

			StartElaboration_aTSP(graphToSolve);
		}
	}
	catch (const exception &e)
	{
		cout << endl << "Exception occurred: " << e.what() << endl;
	}

	cout << endl << "End." << endl;
	system("pause");
}