/*
MIT License
Copyright (c) 2019: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
unsigned short
16 bit
0 to 65.535
%hu

unsigned int
32 bit
0 to 4.294.967.295
%u
*/

#pragma once

#include <map>
#include <queue>
#include <vector>

using namespace std;

class Graph
{
private:
	enum eColor
	{
		white,
		gray,
		black,
	};

	unsigned short  N;
	vector<unsigned short> *Adj;

	// BFS & DFS
	eColor *color;
	unsigned short *predecessor, *distance;
	// BFS & DFS

	// DFS
	unsigned int *start_visit, *end_visit;
	unsigned int time;
	// DFS

private:
	void DFS_Visit(unsigned short u);

	void BFS_ShortestPath_Print(unsigned short from, unsigned short to);

	template <class T>
	T generateRandomNumber(T startRange, T endRange, T limit);

public:
	Graph(unsigned short NumeroNodi);
	~Graph();

	void RandomGraph(unsigned char NumeroArchiPerNodoMin, unsigned char NumeroArchiPerNodoMax);

	void AddEdge(unsigned short from, unsigned short to);
	void PrintEdges();

	void BFS(unsigned short s);
	void DFS();

	void BFS_ShortestPath(unsigned short from, unsigned short to);
};