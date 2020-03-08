/*
MIT License
Copyright (c) 2019: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "pch.h"
#include "Graph.h"

#include <iostream>
#include <time.h>

void mainGrafoRandom()
{
	Graph G(15);
	G.RandomGraph(1, 4);
	G.PrintEdges();
}

void mainGrafoPrestabilito()
{
	Graph G(6);

	G.AddEdge(0, 1);
	G.AddEdge(0, 2);
	G.AddEdge(1, 2);
	G.AddEdge(1, 3);
	G.AddEdge(2, 3);
	G.AddEdge(3, 4);
	G.AddEdge(4, 5);

	G.BFS(0);
	G.BFS_ShortestPath(0, 5);
	G.PrintEdges();
}

int main()
{
	srand(time(NULL));

	mainGrafoRandom();
}