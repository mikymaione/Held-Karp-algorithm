/*
MIT License
Copyright (c) 2019: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "pch.h"
#include "Graph.h"

#include <iostream>
#include <unordered_set>

Graph::Graph(unsigned short NumeroNodi)
{
	N = NumeroNodi;
	Adj = new vector<unsigned short>[N];

	// DFS
	start_visit = new unsigned int[N];
	end_visit = new unsigned int[N];
	// DFS

	// BFS & DFS
	color = new eColor[N];
	predecessor = new unsigned short[N];
	distance = new unsigned short[N];

	for (unsigned short i = 0; i < N; i++)
	{
		color[i] = eColor::white;
		predecessor[i] = USHRT_MAX;
	}
	// BFS & DFS
}

Graph::~Graph()
{
	delete[] distance;
	delete[] start_visit;
	delete[] end_visit;

	delete[] color;
	delete[] predecessor;

	delete[] Adj;
}

void Graph::RandomGraph(unsigned char NumeroArchiPerNodoMin, unsigned char NumeroArchiPerNodoMax)
{
	if (NumeroArchiPerNodoMax > N)
		throw invalid_argument("NumeroArchiPerNodoMax > N");

	unsigned short a, y, zero = 0, N_max_ = N - 1, USHRT_MAX_ = USHRT_MAX;
	unsigned char NumeroArchiPerNodo, x, UCHAR_MAX_ = UCHAR_MAX;

	unordered_set<unsigned short> NumeriUsati;

	for (y = 0; y < N; y++)
	{
		NumeroArchiPerNodo = generateRandomNumber(NumeroArchiPerNodoMin, NumeroArchiPerNodoMax, UCHAR_MAX_);

		for (x = 0; x < NumeroArchiPerNodo; x++)
		{
			do
			{
				a = generateRandomNumber(zero, N_max_, USHRT_MAX_);
			} while (NumeriUsati.find(a) != NumeriUsati.end());

			NumeriUsati.insert(a);
			AddEdge(y, a);
		}

		NumeriUsati.clear();
	}
}

void Graph::AddEdge(unsigned short from, unsigned short to)
{
	Adj[from].push_back(to);
}

void Graph::PrintEdges()
{
	cout << "Edges :" << endl;

	for (unsigned int i = 0; i < N; i++)
	{
		cout << i;

		for (auto to : Adj[i])
			cout << " > " << to;

		cout << endl;
	}
}

void Graph::BFS(unsigned short s)
{
	queue<unsigned short> Q;

	color[s] = eColor::gray;
	distance[s] = 0;

	Q.push(s);

	while (!Q.empty())
	{
		auto v = Q.front();
		Q.pop();

		for (auto w : Adj[v])
			if (color[w] == eColor::white)
			{
				color[w] = eColor::gray;
				distance[w] = distance[v] + 1;
				predecessor[w] = v;

				Q.push(w);
			}

		color[v] = eColor::black;
	}
}

void Graph::BFS_ShortestPath_Print(unsigned short from, unsigned short to)
{
	if (from == to)
		cout << from << " ";
	else if (predecessor[to] == UINT_MAX)
		cout << "Non esiste nessun cammino tra " << from << "e" << to;
	else
	{
		BFS_ShortestPath_Print(from, predecessor[to]);
		cout << to << " ";
	}
}

void Graph::BFS_ShortestPath(unsigned short from, unsigned short to)
{
	cout << "Shortest path from " << from << " to " << to << ": ";

	BFS_ShortestPath_Print(from, to);

	cout << endl;
}

void Graph::DFS()
{
	time = 0;

	for (unsigned short i = 0; i < N; i++)
		if (color[i] == eColor::white)
			DFS_Visit(i);
}

void Graph::DFS_Visit(unsigned short u)
{
	time++;
	color[u] = eColor::gray;
	start_visit[u] = time;

	for (auto v : Adj[u])
		if (color[v] == eColor::white)
		{
			predecessor[v] = u;
			DFS_Visit(v);
		}

	time++;
	end_visit[u] = time;
	color[u] = eColor::black;
}

template <class T>
T Graph::generateRandomNumber(T startRange, T endRange, T limit)
{
	T r = rand();

	T range = endRange - startRange;
	range++;

	T num = r % range + startRange;

	return num;
}