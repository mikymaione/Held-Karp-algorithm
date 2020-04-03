/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <iostream>
#include "HeldKarpSQLite.hpp"

void HeldKarpSQLite::CSet(const unsigned char cardinality, const unsigned long code, const unsigned char key, const unsigned short val)
{
	*db << "INSERT INTO C (cardinality, code, k, val) VALUES (?, ?, ?, ?);"
		<< cardinality
		<< code
		<< key
		<< val;
}

unsigned short HeldKarpSQLite::CGet(const unsigned char cardinality, const unsigned long code, const unsigned char key)
{
	unsigned short r;

	*db << "SELECT val FROM C WHERE cardinality = ? AND code = ? AND k = ?;"
		<< cardinality
		<< code
		<< key
		>> r;

	return r;
}

HeldKarpSQLite::HeldKarpSQLite(vector<vector<unsigned char>> &DistanceMatrix2D, const unsigned int numThreads) : HeldKarp::HeldKarp(DistanceMatrix2D, numThreads)
{
	sqlite_config config;
	config.flags = (numThreads > 0 ? OpenFlags::READWRITE | OpenFlags::FULLMUTEX : OpenFlags::READWRITE);

	db = new database("DB.sqlite", config);

	*db << "DELETE FROM P;";
	*db << "DELETE FROM C;";
}

HeldKarpSQLite::~HeldKarpSQLite()
{
	delete db;
}