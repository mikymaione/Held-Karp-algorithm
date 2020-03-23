/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
#include "sqlite_modern_cpp.h"
#include "HeldKarp.hpp"

using namespace concurrency;
using namespace sqlite;
using namespace std;

class HeldKarpSQLite : public HeldKarp
{
private:
	database *db;

	// TSP ========================================================
	concurrent_unordered_map<unsigned long, concurrent_unordered_map<unsigned char, unsigned short>> *Ccur, *Cprev;
	concurrent_unordered_map<unsigned long, concurrent_unordered_map<unsigned char, unsigned char>> *Pcur, *Pprev;
	// TSP ========================================================

	bool DiskMode = false;

private:
	template <class tC, class tK, class tV>
	void WriteMapToDisk(const string query, concurrent_unordered_map<tC, concurrent_unordered_map<tK, tV>> *Mcur, concurrent_unordered_map<tC, concurrent_unordered_map<tK, tV>> *Mprev)
	{
		if (Mcur != NULL)
		{
			*db << "begin;";
			auto ps = *db << query;

			for each (auto x in *Mcur)
				for each (auto y in x.second)
				{
					ps
						<< x.first
						<< y.first
						<< y.second;

					ps++;
				}

			*db << "commit;";
		}
	}

protected:
	void CSave()
	{
		WriteMapToDisk("INSERT INTO C (code, k, val) VALUES (?, ?, ?);", Ccur, Cprev);
		WriteMapToDisk("INSERT INTO P (code, k, val) VALUES (?, ?, ?);", Pcur, Pprev);

		delete Cprev;
		delete Pprev;

		Cprev = Ccur;
		Pprev = Pcur;

		Ccur = NULL;
		Pcur = NULL;
	}

	void CLoadAll()
	{
		DiskMode = true;
	}

	void CSet(const unsigned long code, const unsigned char key, const unsigned short val)
	{
		if (Ccur == NULL)
			Ccur = new concurrent_unordered_map<unsigned long, concurrent_unordered_map<unsigned char, unsigned short>>();

		(*Ccur)[code][key] = val;
	}

	void PSet(const unsigned long code, const unsigned char key, const unsigned char val)
	{
		if (Pcur == NULL)
			Pcur = new concurrent_unordered_map<unsigned long, concurrent_unordered_map<unsigned char, unsigned char>>();

		(*Pcur)[code][key] = val;
	}

	unsigned short CGet(const unsigned long code, const unsigned char key)
	{
		if (DiskMode)
		{
			unsigned short r;

			*db << "SELECT val FROM C WHERE code = ? AND k = ?;"
				<< code
				<< key
				>> r;

			return r;
		}
		else
		{
			return (*Cprev)[code][key];
		}
	}

	unsigned char PGet(const unsigned long code, const unsigned char key)
	{
		if (DiskMode)
		{
			unsigned char r;

			*db << "SELECT val FROM P WHERE code = ? AND k = ?;"
				<< code
				<< key
				>> r;

			return r;
		}
		else
		{
			return (*Pprev)[code][key];
		}
	}

public:
	HeldKarpSQLite(vector<vector<unsigned char>> &DistanceMatrix2D, const int numThreads) : HeldKarp::HeldKarp(DistanceMatrix2D, numThreads)
	{
		sqlite_config config;
		config.flags = (numThreads > 0 ? OpenFlags::READWRITE | OpenFlags::FULLMUTEX : OpenFlags::READWRITE);

		db = new database("DB.sqlite", config);

		*db << "DELETE FROM P;";
		*db << "DELETE FROM C;";

		Ccur = NULL;
		Pcur = NULL;
		Cprev = NULL;
		Pprev = NULL;

		CSave();
	}

	~HeldKarpSQLite()
	{
		delete db;

		delete Ccur;
		delete Pcur;

		delete Cprev;
		delete Pprev;
	}

};