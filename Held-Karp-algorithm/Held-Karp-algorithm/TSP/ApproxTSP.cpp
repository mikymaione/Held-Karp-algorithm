/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

TSP is an extension of the Hamiltonian circuit problem.
The problem can be described as: find a tour of N cities in a country (assuming all cities to be visited are reachable), the tour should:
1. visit every city just once
2. return to the starting point
3. be of minimum distance.
*/
#pragma once

#include <stack>

#include "ApproxTSP.hpp"
#include "../ADS/Graph.hpp"
#include "../MST/Prim.hpp"

namespace TSP
{
	ApproxTSP::ApproxTSP(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	/*
	Algo. from: Cormen, T. and Leiserson, C. and Rivest, R. and Stein, C., 2010. Introduzione agli algoritmi e strutture dati. McGraw-Hill.
	Algo.:
	00	function algorithm ApproxTSP (G, c) is
	01		Seleziona un vertice r da G.V
	02		Calcola albero di connessione minimo T per G dalla radice r utilizzando MST-Prim(G, c, r)
	03		Sia H una lista di vertici, ordinata in base a quando un vertice viene visitato per primo in un attraversamento anticipato di T
	04		return il ciclo hamiltoniano H
	05	end function
	*/
	void ApproxTSP::Solve(float &opt, string &path) // Θ(V²)
	{
		maxCardinality = 4;

		Graph G(numberOfNodes);
		G.MakeConnected(distance);
		currentCardinality++;

		MST::Prim prim;
		prim.Solve(distance, G, 0); // O(E ㏒ V)
		currentCardinality++;

		stack<size_t> H;
		G.PreVisit(H, 0); // O(V)
		currentCardinality++;

		opt = 0;
		path = "0 ";

		size_t u = 0;
		while (!H.empty()) // O(V)
		{
			auto v = H.top();
			H.pop();

			opt += distance[u][v];
			path += to_string(v) + " ";

			u = v;
		}
		currentCardinality++;
	}
}