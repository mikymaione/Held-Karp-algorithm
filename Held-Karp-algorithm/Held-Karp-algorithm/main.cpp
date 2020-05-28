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

#include "ApproxTSP.hpp"
#include "Christofides.hpp"
#include "HeldKarp.hpp"
#include "Righini.hpp"

using namespace TSP;
using namespace std;
using namespace std::experimental;

vector<vector<float>> ReadFile(string tipo, const unsigned short NumberOfNodes)
{
	auto delim = '|';
	string z, e;
	size_t current, previous;
	unsigned short x, y;

	vector<vector<float>> DistanceMatrix2D(NumberOfNodes, vector<float>(NumberOfNodes, 0));

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

			DistanceMatrix2D[x][y] = stof(e);
			x++;
		}
	}

	return DistanceMatrix2D;
}

void Run(string algo, string tipo, const unsigned short NumberOfNodes)
{
	auto type = (tipo == "E" ? "sym" : "asym");
	auto DistanceMatrix2D = ReadFile(type, NumberOfNodes);

	if (algo == "H")
	{
		HeldKarp A(DistanceMatrix2D);
		A.Run();
	}
	else if (algo == "A")
	{
		ApproxTSP A(DistanceMatrix2D);
		A.Run();
	}
	else if (algo == "R")
	{
		Righini A(DistanceMatrix2D);
		A.Run();
	}
	else
	{
		Christofides A(DistanceMatrix2D);
		A.Run();
	}
}

void StartElaboration_TSP(string algo, string tipo, const string graphToSolve)
{
	if (graphToSolve == "all" || graphToSolve == "4")
		Run(algo, tipo, 4);

	if (graphToSolve == "all" || graphToSolve == "6")
		Run(algo, tipo, 6);

	if (graphToSolve == "all" || graphToSolve == "10")
		Run(algo, tipo, 10);

	if (graphToSolve == "all" || graphToSolve == "15")
		Run(algo, tipo, 15);

	if (graphToSolve == "all" || graphToSolve == "20")
		Run(algo, tipo, 20);

	if (graphToSolve == "all" || graphToSolve == "25")
		Run(algo, tipo, 25);

	if (graphToSolve == "all" || graphToSolve == "100")
		Run(algo, tipo, 100);

	if (graphToSolve == "all" || graphToSolve == "500")
		Run(algo, tipo, 500);

	if (graphToSolve == "all" || graphToSolve == "1000")
		Run(algo, tipo, 1000);
}

void WriteTitle()
{
	cout
		<< "Held-Karp algorithm to solve the Asymmetric Traveling Salesman Problem" << endl
		<< "Christofides algorithm to solve the Euclidean Traveling Salesman Problem" << endl
		<< endl
		<< "Program parameters:" << endl
		<< " algorithm = {H, C}" << endl
		<< " type = {E, A}" << endl
		<< " [graph to solve = {4, 10, 15, 20, 25, all}]" << endl
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
		if (argc > 3)
		{
			const string algo = argv[1];
			const string type = argv[2];
			const string graphToSolve = argv[3];

			cout << "Solving using ";

			if (algo == "H")
				cout << "Held-Karp algorithm on ";
			else if (algo == "A")
				cout << "Approx-TSP algorithm on ";
			else if (algo == "R")
				cout << "Righini algorithm on ";
			else
				cout << "Christofides algorithm on ";

			if (type == "E")
				cout << "euclidean graph";
			else
				cout << "asymmetric graph";

			cout << endl << endl;

			StartElaboration_TSP(algo, type, graphToSolve);
		}
	}
	catch (const exception &e)
	{
		cout << endl << "Exception occurred: " << e.what() << endl;
	}

	cout << endl << "End." << endl;
	system("pause");
}