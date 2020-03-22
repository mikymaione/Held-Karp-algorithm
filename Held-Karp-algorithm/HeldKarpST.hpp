﻿/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <map>
#include "HeldKarp.hpp"

using namespace std;

class HeldKarpST : public HeldKarp
{
protected:
	// TSP ========================================================
	map<unsigned long, map<unsigned char, unsigned short>> C;
	map<unsigned long, map<unsigned char, unsigned char>> P;
	// TSP ========================================================

	void CSet(const unsigned long code, const unsigned char key, const unsigned short val)
	{
		C[code][key] = val;
	}

	void PSet(const unsigned long code, const unsigned char key, const unsigned char val)
	{
		P[code][key] = val;
	}

	unsigned short CGet(const unsigned long code, const unsigned char key)
	{
		return C[code][key];
	}

	unsigned char PGet(const unsigned long code, const unsigned char key)
	{
		return P[code][key];
	}

public:
	HeldKarpST(vector<vector<unsigned char>> & DistanceMatrix2D, const int numThreads) : HeldKarp::HeldKarp(DistanceMatrix2D, numThreads) {}

};