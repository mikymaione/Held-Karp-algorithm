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

#include <algorithm>
#include <set>

#include "Branch_and_Bound.hpp"
#include "../MST/Prim.hpp"

namespace TSP
{
	Branch_and_Bound::Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string Branch_and_Bound::PrintPath(vector<sEdge> &path)
	{
		string s;
		set<unsigned short> S;
		vector<vector<bool>> E(numberOfNodes, vector<bool>(numberOfNodes, false));
		unsigned short i = 0, x = 0;

		for (auto const e : path)
		{
			E[e.from][e.to] = true;
			E[e.to][e.from] = true;
		}

		while (x < numberOfNodes)
		{
			s += to_string(i) + " ";
			S.insert(i);

			for (unsigned short y = 0; y < numberOfNodes; y++)
				if (E[i][y] && S.count(y) == 0)
				{
					i = y;
					break;
				}

			x++;
		}

		return s + "0";
	}

	vector<sNode> Branch_and_Bound::Branch(sTree &T, vector<unsigned short> &δ, sNode &current_node, unsigned short n)
	{
		vector<sNode> result;
		auto min_δ_req = n;
		auto min_δ = n;
		auto p_req = n;
		auto p = n;
		auto req_neighbor = n;

		for (unsigned short i = 0; i < n; i++)
			if (δ[i] > 2)
				if (current_node.IncidentToRequired(i, n) == n)
				{
					if (δ[i] < min_δ)
					{
						min_δ = δ[i];
						p = i;
					}
				}
				else
				{
					if (δ[i] < min_δ_req)
					{
						min_δ_req = δ[i];
						p_req = i;
						req_neighbor = current_node.IncidentToRequired(i, n);
					}
				}

		if (min_δ_req < n)
		{
			unsigned short i;

			for (i = 0; i < n; i++)
				if (i != p_req && i != req_neighbor)
					if (!T.Contains(i, p_req) && !current_node.Forbidden(i, p_req))
						break;

			if (i == n)
				for (i = 0; i < n; i++)
					if (i != p_req && i != req_neighbor)
						if (!current_node.Forbidden(i, p_req))
							break;

			auto F = current_node.F;
			auto R = current_node.R;
			R.push_back(sEdge(i, p_req));

			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p_req && k != req_neighbor && !current_node.Forbidden(p_req, k))
					F.push_back(sEdge(p_req, k));

			sNode S2(R, F, current_node.λ, n);
			result.push_back(S2);

			R = current_node.R;
			F = current_node.F;
			F.push_back(sEdge(i, p_req));

			vector<unsigned short> forbidden(n, 0);
			forbidden[p_req] = 1;

			auto num_forbidden = CountElements(F, forbidden, p_req);

			if (num_forbidden == n - 3)
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != req_neighbor)
							R.push_back(sEdge(p_req, k));

			sNode S3(R, F, current_node.λ, n);
			result.push_back(S3);
		}
		else
		{
			unsigned short i, j;

			for (i = 0; i < n; i++)
				if (i != p && !T.Contains(i, p) && !current_node.Forbidden(i, p))
					break;

			if (i == n)
				for (i = 0; i < n; i++)
					if (i != p && !current_node.Forbidden(i, p))
						break;

			for (j = 0; j < n; j++)
				if (j != p && j != i)
					if (!current_node.Forbidden(j, p))
						break;

			auto F = current_node.F;
			auto R = current_node.R;
			R.push_back(sEdge(i, p));
			R.push_back(sEdge(j, p));

			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p && k != j && !current_node.Forbidden(p, k))
					F.push_back(sEdge(p, k));

			sNode S1(R, F, current_node.λ, n);
			result.push_back(S1);

			R = current_node.R;
			F = current_node.F;
			R.push_back(sEdge(i, p));
			F.push_back(sEdge(j, p));

			vector<unsigned short> forbidden(n, 0);
			forbidden[p] = 1;

			auto num_forbidden = CountElements(F, forbidden, p);

			if (num_forbidden == n - 3)
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != i)
							R.push_back(sEdge(p, k));

			sNode S2(R, F, current_node.λ, n);
			result.push_back(S2);

			R = current_node.R;
			F = current_node.F;
			F.push_back(sEdge(i, p));

			if (num_forbidden == n - 3)
			{
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != i)
							R.push_back(sEdge(p, k));

				R.push_back(sEdge(p, j));
			}

			sNode S3(R, F, current_node.λ, n);
			result.push_back(S3);
		}

		return result;
	}

	bool Branch_and_Bound::Bound(sNode &node, vector<unsigned short> &δ, float t, unsigned short const steps)
	{
		MST::Prim prim;

		vector<vector<float>> w(numberOfNodes, vector<float>(numberOfNodes));
		vector<vector<unsigned short>> omitted(numberOfNodes, vector<unsigned short>(numberOfNodes, 0));
		vector<float> Λ(numberOfNodes);
		vector<bool> forbidden(numberOfNodes, 0);
		sTree T(numberOfNodes);

		float W = 0;

		float Δ = 3.0f * t / (2.0f * steps);
		float dΔ = t / (steps * steps - steps);

		float min1 = FLT_MAX;
		float min2 = FLT_MAX;

		unsigned short id1, id2;

		unsigned short req1 = 0;
		unsigned short req2 = 0;
		unsigned short req = node.R.size();

		for (unsigned short i = 0; i < numberOfNodes; i++)
			δ[i] = 0;

		for (unsigned short i = 0; i < node.R.size(); i++)
		{
			omitted[node.R[i].from][node.R[i].to] = 1;
			omitted[node.R[i].to][node.R[i].from] = 1;

			if (node.R[i].from == 0)
			{
				req--;

				if (req < node.R.size() - 1)
					req2 = node.R[i].to;
				else
					req1 = node.R[i].to;
			}

			if (node.R[i].to == 0)
			{
				req--;

				if (req < node.R.size() - 1)
					req2 = node.R[i].from;
				else
					req1 = node.R[i].from;
			}
		}

		for (unsigned short i = 0; i < node.F.size(); i++)
		{
			omitted[node.F[i].from][node.F[i].to] = 2;
			omitted[node.F[i].to][node.F[i].from] = 2;

			if (node.F[i].from == 0)
				forbidden[node.F[i].to] = 1;

			if (node.F[i].to == 0)
				forbidden[node.F[i].from] = 1;
		}

		for (unsigned short i = 0; i < numberOfNodes; i++)
			for (unsigned short j = 0; j < numberOfNodes; j++)
				w[i][j] = (i == j ? FLT_MAX : distance[i][j] + node.λ[i] + node.λ[j]);

		for (unsigned short k = 0; k < steps; k++)
		{
			if (prim.Solve(T, omitted, w, req, numberOfNodes))
				return true;

			for (unsigned short i = 1; i < numberOfNodes; i++)
				if (w[0][i] < min1 && !forbidden[i])
				{
					min1 = w[0][i];
					id1 = i;
				}

			for (unsigned short i = 1; i < numberOfNodes; i++)
				if (i != id1)
					if (w[0][i] < min2 && !forbidden[i])
					{
						min2 = w[0][i];
						id2 = i;
					}

			if (req1 == 0)
			{
				T[T.size() - 2] = sEdge(0, id1);
				T[T.size() - 1] = sEdge(0, id2);
			}
			else if (req2 == 0)
			{
				T[T.size() - 2] = sEdge(0, id1);
				T[T.size() - 1] = sEdge(0, req1);
			}
			else
			{
				T[T.size() - 2] = sEdge(0, req1);
				T[T.size() - 1] = sEdge(0, req2);
			}

			for (unsigned short i = 0; i < T.size(); i++)
			{
				δ[T[i].from]++;
				δ[T[i].to]++;
			}

			for (unsigned short i = 0; i < T.size(); i++)
			{
				W += w[T[i].from][T[i].to];
				W -= 2 * node.λ[i];
			}

			if (node.bound < W)
			{
				node.bound = W;

				for (unsigned short i = 0; i < node.oneTree.size(); i++)
				{
					node.oneTree[i] = T[i];
					Λ[i] = node.λ[i];
				}
			}

			for (unsigned short i = 0; i < numberOfNodes; i++)
				for (unsigned short j = 0; j < numberOfNodes; j++)
					if (i != j)
						w[i][j] += (δ[i] - 2) * t + (δ[j] - 2) * t;

			for (unsigned short i = 0; i < numberOfNodes; i++)
			{
				node.λ[i] += (δ[i] - 2) * t;
				δ[i] = 0;
			}

			t -= Δ;
			Δ -= dΔ;
			W = 0;

			min1 = FLT_MAX;
			min2 = FLT_MAX;
		}

		for (unsigned short i = 0; i < numberOfNodes; i++)
		{
			node.λ[i] = Λ[i];

			δ[node.oneTree[i].from]++;
			δ[node.oneTree[i].to]++;
		}

		return false;
	}

	// calc T1
	float Branch_and_Bound::t1()
	{
		float t = 0;
		unsigned short id1, id2;

		sTree T(numberOfNodes);
		vector<vector<unsigned short>> omitted(numberOfNodes, vector<unsigned short>(numberOfNodes, 0));

		MST::Prim prim;
		prim.Solve(T, omitted, distance, 0, numberOfNodes);

		T.GetMin0Nodes(distance, id1, id2);

		T[T.size() - 2] = sEdge(0, id1);
		T[T.size() - 1] = sEdge(0, id2);

		for (unsigned short i = 0; i < T.size(); i++)
			t += distance[T[i].from][T[i].to];

		return t / (2.0f * numberOfNodes);
	}

	// Add to priority queue
	void Branch_and_Bound::PQ_Add(vector<sNode> &PQ, sNode &new_elem)
	{
		PQ.push_back(new_elem);

		sort(PQ.begin(), PQ.end(), [](sNode &l, sNode &r) {
			return l.bound > r.bound;
		});
	}

	void Branch_and_Bound::Solve(float &opt, string &path)
	{
		vector<sEdge> tour(numberOfNodes);

		// Upper bound on tour 0-1-2-.....-n
		float UB = distance[numberOfNodes - 1][0];
		tour[0] = sEdge(numberOfNodes - 1, 0);

		for (unsigned short i = 0; i < distance.size() - 1; i++)
		{
			UB += distance[i][i + 1];
			tour[i + 1] = sEdge(i, i + 1);
		}


		auto t = t1();
		unsigned short M = numberOfNodes * numberOfNodes / 50 + numberOfNodes + 15;

		vector<sNode> Q;

		// Calculate first Bound
		{
			vector<unsigned short> δ(numberOfNodes, 0);

			sNode root(numberOfNodes);
			Bound(root, δ, t, M);

			if (!root.oneTree.CheckTour())
			{
				for (unsigned short i = 0; i < numberOfNodes; i++)
					tour[i] = root.oneTree[i];

				opt = root.bound;
				path = PrintPath(tour);

				return;
			}

			M = ceil(numberOfNodes / 4.0f + 5);
			t = 0;

			for (unsigned short i = 0; i < numberOfNodes; i++)
				t += abs(root.λ[i]);

			t /= 2.0f * numberOfNodes;

			auto B = Branch(root.oneTree, δ, root, numberOfNodes);

			for (unsigned short i = 0; i < B.size(); i++)
				if (!Bound(B[i], δ, t, M))
					if (B[i].bound < UB)
						PQ_Add(Q, B[i]);
		}

		{
			vector<unsigned short> δ1(numberOfNodes, 0);

			while (!Q.empty())
			{
				auto node = Q.back();
				Q.pop_back();

				if (ceil(node.bound) >= UB)
				{
					// exceeded UB
					opt = UB;
					path = PrintPath(tour);

					return;
				}

				if (node.bound < UB)
				{
					for (unsigned short i = 0; i < numberOfNodes; i++)
					{
						δ1[node.oneTree[i].from]++;
						δ1[node.oneTree[i].to]++;
					}

					if (!node.oneTree.CheckTour())
					{
						UB = node.bound;

						for (unsigned short i = 0; i < numberOfNodes; i++)
							tour[i] = node.oneTree[i];
					}
					else
					{
						auto B = Branch(node.oneTree, δ1, node, numberOfNodes);

						for (unsigned short i = 0; i < B.size(); i++)
							if (!Bound(B[i], δ1, t, M))
								if (B[i].bound < UB)
								{
									if (!B[i].oneTree.CheckTour())
									{
										UB = B[i].bound;

										for (unsigned short k = 0; k < numberOfNodes; k++)
											tour[k] = B[i].oneTree[k];
									}

									PQ_Add(Q, B[i]);
								}
					}
				}

				for (unsigned short k = 0; k < numberOfNodes; k++)
					δ1[k] = 0;
			}
		}

		opt = UB;
		path = PrintPath(tour);
	}

}