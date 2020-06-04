/*
MIT License
Copyright (c) 2020: Michele Maione
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Held–Karp algorithm
The Held–Karp algorithm, is a dynamic programming algorithm proposed in 1962 independently by Bellman and by Held and Karp to solve the Traveling Salesman Problem (TSP).

T(n) = O(n²)

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

namespace TSP
{
	Branch_and_Bound::Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string Branch_and_Bound::PrintPath(vector<Edge> &path)
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

	vector<Branch_and_Bound::Node> Branch_and_Bound::Branch(Tree &tree, vector<unsigned short> &degrees, Node &current_node, unsigned short n)
	{
		vector<Node> result;
		auto min_degree_req = n;
		auto min_degree = n;
		auto p_req = n;
		auto p = n;
		auto req_neighbor = n;

		for (unsigned short i = 0; i < n; i++)
			if (degrees[i] > 2)
				if (current_node.IncidentToRequired(i, n) == n)
				{
					if (degrees[i] < min_degree)
					{
						min_degree = degrees[i];
						p = i;
					}
				}
				else
				{
					if (degrees[i] < min_degree_req)
					{
						min_degree_req = degrees[i];
						p_req = i;
						req_neighbor = current_node.IncidentToRequired(i, n);
					}
				}

		if (min_degree_req < n)
		{
			unsigned short i;

			for (i = 0; i < n; i++)
				if (i != p_req && i != req_neighbor)
					if (!tree.Contains(i, p_req) && !current_node.Forbidden(i, p_req))
						break;

			if (i == n)
				for (i = 0; i < n; i++)
					if (i != p_req && i != req_neighbor)
						if (!current_node.Forbidden(i, p_req))
							break;

			auto F = current_node.F;
			auto R = current_node.R;
			R.push_back(make_pair(i, p_req));

			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p_req && k != req_neighbor && !current_node.Forbidden(p_req, k))
					F.push_back(make_pair(p_req, k));

			Node S2(R, F, current_node.λ, n);
			result.push_back(S2);

			R = current_node.R;
			F = current_node.F;
			F.push_back(make_pair(i, p_req));

			unsigned short num_forbidden = 0;
			vector<unsigned short> forbidden(n, 0);
			forbidden[p_req] = 1;

			for (unsigned short k = 0; k < F.size(); k++)
			{
				if (F[k].first == p_req)
				{
					forbidden[F[k].second] = 1;
					num_forbidden++;
				}

				if (F[k].second == p_req)
				{
					forbidden[F[k].first] = 1;
					num_forbidden++;
				}
			}

			if (num_forbidden == n - 3)
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != req_neighbor)
							R.push_back(make_pair(p_req, k));

			Node S3(R, F, current_node.λ, n);
			result.push_back(S3);
		}
		else
		{
			unsigned short i, j;

			for (i = 0; i < n; i++)
				if (i != p && !tree.Contains(i, p) && !current_node.Forbidden(i, p))
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
			R.push_back(make_pair(i, p));
			R.push_back(make_pair(j, p));

			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p && k != j && !current_node.Forbidden(p, k))
					F.push_back(make_pair(p, k));

			Node S1(R, F, current_node.λ, n);
			result.push_back(S1);

			R = current_node.R;
			F = current_node.F;
			R.push_back(make_pair(i, p));
			F.push_back(make_pair(j, p));

			unsigned short num_forbidden = 0;
			vector<unsigned short> forbidden(n, 0);
			forbidden[p] = 1;

			for (unsigned short k = 0; k < F.size(); k++)
			{
				if (F[k].first == p)
				{
					forbidden[F[k].second] = 1;
					num_forbidden++;
				}

				if (F[k].second == p)
				{
					forbidden[F[k].first] = 1;
					num_forbidden++;
				}
			}

			if (num_forbidden == n - 3)
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != i)
							R.push_back(make_pair(p, k));

			Node S2(R, F, current_node.λ, n);
			result.push_back(S2);

			R = current_node.R;
			F = current_node.F;
			F.push_back(make_pair(i, p));

			if (num_forbidden == n - 3)
			{
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != i)
							R.push_back(make_pair(p, k));

				R.push_back(make_pair(p, j));
			}

			Node S3(R, F, current_node.λ, n);
			result.push_back(S3);
		}

		return result;
	}

	bool Branch_and_Bound::Bound(Node &node, vector<unsigned short> &degree, float t, unsigned short const steps)
	{
		vector<vector<float>> Weights(numberOfNodes, vector<float>(numberOfNodes));
		vector<vector<unsigned short>> omitted(numberOfNodes, vector<unsigned short>(numberOfNodes, 0));
		vector<float> OptLambda(numberOfNodes);
		Tree Tree(numberOfNodes);

		unsigned short req = node.R.size();
		float Treeweight = 0;
		float delta = 3.0f * t / (2.0f * steps);
		float ddelta = t / (steps * steps - steps);
		float firstmin = FLT_MAX;
		float secondmin = FLT_MAX;
		unsigned short first, second;
		unsigned short req1 = 0;
		unsigned short req2 = 0;
		vector<bool> forbidden(numberOfNodes, 0);

		for (unsigned short i = 0; i < numberOfNodes; i++)
			degree[i] = 0;

		for (unsigned short i = 0; i < node.R.size(); i++)
		{
			omitted[node.R[i].first][node.R[i].second] = 1;
			omitted[node.R[i].second][node.R[i].first] = 1;

			if (node.R[i].first == 0)
			{
				req--;

				if (req < node.R.size() - 1)
					req2 = node.R[i].second;
				else
					req1 = node.R[i].second;
			}

			if (node.R[i].second == 0)
			{
				req--;

				if (req < node.R.size() - 1)
					req2 = node.R[i].first;
				else
					req1 = node.R[i].first;
			}
		}

		for (unsigned short i = 0; i < node.F.size(); i++)
		{
			omitted.at(node.F[i].first).at(node.F[i].second) = 2;
			omitted.at(node.F[i].second).at(node.F[i].first) = 2;

			if (node.F[i].first == 0)
				forbidden.at(node.F[i].second) = 1;

			if (node.F[i].second == 0)
				forbidden.at(node.F[i].first) = 1;
		}

		for (unsigned short i = 0; i < numberOfNodes; i++)
			for (unsigned short j = 0; j < numberOfNodes; j++)
				Weights[i][j] = distance[i][j] + node.λ[i] + node.λ[j];

		for (unsigned short k = 0; k < steps; k++)
		{
			if (MST_Prim(Tree, omitted, Weights, req))
				return 1;

			for (unsigned short i = 1; i < numberOfNodes; i++)
				if (Weights[0][i] < firstmin && !forbidden[i])
				{
					firstmin = Weights[0][i];
					first = i;
				}

			for (unsigned short i = 1; i < numberOfNodes; i++)
				if (i != first)
					if (Weights[0][i] < secondmin && !forbidden[i])
					{
						secondmin = Weights[0][i];
						second = i;
					}

			if (req1 == 0)
			{
				Tree[Tree.size() - 2] = Edge(0, first);
				Tree[Tree.size() - 1] = Edge(0, second);
			}
			else if (req2 == 0)
			{
				Tree[Tree.size() - 2] = Edge(0, first);
				Tree[Tree.size() - 1] = Edge(0, req1);
			}
			else
			{
				Tree[Tree.size() - 2] = Edge(0, req1);
				Tree[Tree.size() - 1] = Edge(0, req2);
			}

			for (unsigned short i = 0; i < Tree.size(); i++)
			{
				degree[Tree[i].from]++;
				degree[Tree[i].to]++;
			}

			for (unsigned short i = 0; i < Tree.size(); i++)
			{
				Treeweight += Weights[Tree[i].from][Tree[i].to];
				Treeweight -= 2 * node.λ[i];
			}

			if (node.HK < Treeweight)
			{
				node.HK = Treeweight;

				for (unsigned short i = 0; i < (node.one_tree).size(); i++)
				{
					node.one_tree[i] = Tree[i];
					OptLambda[i] = node.λ[i];
				}
			}

			for (unsigned short i = 0; i < numberOfNodes; i++)
				for (unsigned short j = 0; j < numberOfNodes; j++)
					Weights[i][j] += (degree[i] - 2) * t + (degree[j] - 2) * t;

			for (unsigned short i = 0; i < numberOfNodes; i++)
			{
				node.λ[i] += (degree[i] - 2) * t;
				degree[i] = 0;
			}

			t -= delta;
			delta -= ddelta;
			Treeweight = 0;
			firstmin = FLT_MAX;
			secondmin = FLT_MAX;
		}

		for (unsigned short i = 0; i < numberOfNodes; i++)
		{
			node.λ[i] = OptLambda[i];
			degree[node.one_tree[i].from]++;
			degree[node.one_tree[i].to]++;
		}

		return 0;
	}

	bool Branch_and_Bound::MST_Prim(Tree &Tree, vector<vector<unsigned short>> &omitted, const vector<vector<float>> &Weights, const unsigned short req)
	{
		vector<bool> visited(numberOfNodes, 0);
		vector<pair<float, unsigned short>> min(numberOfNodes, make_pair(FLT_MAX, 0));

		unsigned short req_num = 0;
		unsigned short vertex = 1;
		unsigned short new_vertex = 1;
		float minimum = FLT_MAX;

		visited[1] = 1;

		for (unsigned short j = 0; j < numberOfNodes - 2; j++)
		{
			for (unsigned short i = 2; i < numberOfNodes; i++)
				if (i != vertex && !visited[i])
					if (omitted[i][vertex] == 1)
					{
						min[i].first = 0;
						min[i].second = vertex;
					}
					else if (omitted[i][vertex] == 0 && Weights[i][vertex] < min[i].first)
					{
						min[i].first = Weights[i][vertex];
						min[i].second = vertex;
					}

			for (unsigned short i = 2; i < numberOfNodes; i++)
				if (!visited[i] && min[i].first < minimum)
				{
					minimum = min[i].first;
					new_vertex = i;
				}

			if (new_vertex == vertex)
				return true;

			if (omitted[new_vertex].at(min[new_vertex].second))
				req_num++;

			vertex = new_vertex;
			visited[vertex] = 1;

			Tree[j] = Edge(vertex, min[vertex].second);
			minimum = FLT_MAX;
		}

		if (req_num < req)
			return true;

		return false;
	}

	float Branch_and_Bound::t1()
	{
		Tree Tree(distance.size());
		vector<vector<unsigned short>> omitted(distance.size(), vector<unsigned short>(distance.size(), 0));

		float t = 0;
		float firstmin = FLT_MAX;
		float secondmin = FLT_MAX;

		unsigned short first, second;

		MST_Prim(Tree, omitted, distance, 0);

		for (unsigned short i = 1; i < Tree.size(); i++)
			if (distance[0][i] < firstmin)
			{
				firstmin = distance[0][i];
				first = i;
			}

		for (unsigned short i = 1; i < Tree.size(); i++)
			if (i != first)
				if (distance[0][i] < secondmin)
				{
					secondmin = distance[0][i];
					second = i;
				}

		Tree[Tree.size() - 2] = Edge(0, first);
		Tree[Tree.size() - 1] = Edge(0, second);

		for (unsigned short i = 0; i < Tree.size(); i++)
			t += distance[Tree[i].from][Tree[i].to];

		return t / (2.0f * distance.size());
	}

	void Branch_and_Bound::PQ_Add(vector<Node> &PQ, Node &new_elem)
	{
		PQ.push_back(new_elem);

		sort(PQ.begin(), PQ.end(), [](Node &l, Node &r) {
			return l.HK > r.HK;
		});
	}

	/*
	Top-level outline of the algorithm:
		1. Draw and initialize the root node:
		2. Repeat the following step until a solution (i.e., a complete circuit, represented by a terminal node) has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found:
			- Choose an unexplored non-terminal node with the smallest bound, and process it.
		3. When a solution has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found, then the best solution found is optimal.

		Initialization of the root node
			- The node number of the root node is 0 (or 1 if you like; it doesn't really matter where you start counting).
			- The root node doesn't need a label.
			- The bound of the root node is 0.
			- The incoming matrix of the root node is the original matrix of distances, with M's along the diagonal; the L-value of this matrix is L = 0.

		Steps to process a node:
			1. Compute the opportunity matrix for the node. Beginning with the node's incoming matrix:
				(a) Set L = the incoming matrix's L-value.
				(b) Ensure that every row and every column has exactly one M. [The matrix might contain N's too, from step 6(b)|these don't count as M's.] If necessary, replace one entry of the matrix with an M to meet this condition. [The M's enforce acyclicity until a full circuit has been completed.]
				(c) Ensure that every column contains at least one zero. If necessary, subtract the smallest number in a column from every number in that column, and add the number that was subtracted to L.
				(d) Ensure that every row contains at least one zero. If necessary, subtract the smallest number in a row from every number in that row, and add the number that was subtracted to L. The resulting matrix is this node's opportunity matrix, and the L-value of this opportunity matrix is the value that L has after possibly being increased in steps (c) and (d) above.
			2. [First special case.] Is the opportunity matrix 2 × 2? If so:
				(a) A 2 × 2 opportunity matrix will always look like either |M 0|	or	|0 M|
																			|0 M|		|M 0|
				(b) Create a single child node, and assign it the next available node number.
					- The label of the child node consists of the two links that correspond to the zeroes in the 2 × 2 opportunity matrix.
					- The bound of the child node is the L-value of the 2 × 2 opportunity matrix.
					- The child node has no incoming matrix.
				(c) The child node is a terminal node that represents a solution (i.e., a complete circuit) given by the links specified by the labels of the nodes along the path from the root node to the child node, and the length of this circuit is equal to the bound of the child node.
				(d) Check that the length of this circuit equals the bound of the child node by adding up the distances from the original distance matrix. (If not, something is wrong. Check the arithmetic in your work.)
				(e) You are done processing this node. Do not continue to the following steps. Go back to the top-level outline.
			3. [Second special case.] Check to see whether the opportunity matrix has one or more zeroes that are the only number in their row or the only number in their column. [This can happen if the opportunity matrix contains N's from step 6(b).] If so:
				(a) Choose any such zero in the opportunity matrix. Because this zero is the only number in its row (or in its column), it represents a link that must be taken.
				(b) Create a single child node, and assign it the next available node number.
					- The label of the child node is the link that corresponds to the zero chosen in step (a).
					- The bound of the child node is the L-value of the opportunity matrix of the node being processed.
					- The incoming matrix of the child node is formed from the opportunity matrix of the node being processed by deleting the row and column of the chosen zero, and the L-value of this incoming matrix is the L-value of the opportunity matrix of the node being processed.
				(c) You are done processing this node. Do not continue to the following steps. Go back to the top-level outline.
			4. Compute regrets. Every zero in the opportunity matrix has a corresponding regret, which is the sum of the smallest other number in that row (possibly zero) and the smallest other number in that column (possibly zero).
			5. Choose the largest regret, and call it Rmax.
				(a) If there is a tie: Choose the one that corresponds to the smaller distance in the original distance matrix.
				(b) If there is still a tie: Choose arbitrarily.
			6. Create two child nodes, and assign them the next available node numbers.
				(a) Right child:
					- The label of the right child is the link that corresponds to the regret Rmax.
					- The bound of the right child is the L-value of the opportunity matrix of the node being processed.
					- The incoming matrix of the right child is formed from the opportunity matrix of the node being processed by deleting the row and column of the regret Rmax, and the L-value of this incoming matrix equals the L-value of the opportunity matrix of the node being processed.
				(b) Left child:
					- The label of the left child is the \negation" of the label of the right child.
					- The bound of the left child is the L-value of the opportunity matrix of the node being processed, plus Rmax.
					- The incoming matrix of the left child is formed from the opportunity matrix of the node being processed by replacing the zero corresponding to Rmax with an N (not an M!), and the L-value of this incoming matrix equals the L-value of the opportunity matrix of the node being processed. [The N enforces the decision not to take that link.]
			Note that the bound of the left child does not equal the L-value of its incoming matrix!
			7. You are done processing this node. Go back to the top-level outline.
	*/
	pair<vector<Branch_and_Bound::Edge>, float> Branch_and_Bound::HKAlgo()
	{
		vector<Node> PQ;
		vector<Edge> path(numberOfNodes);
		vector<unsigned short> degree(numberOfNodes, 0);

		path[0] = Edge(numberOfNodes - 1, 0);

		auto UB = distance[numberOfNodes - 1][0];

		// Upper bound
		for (unsigned short i = 0; i < distance.size() - 1; i++)
		{
			UB += distance[i][i + 1];
			path[i + 1] = Edge(i, i + 1);
		}

		auto t = t1();
		auto N = (numberOfNodes * numberOfNodes / 50.0f) + numberOfNodes + 15;

		// 1. Draw and initialize the root node.
		{
			Node root(numberOfNodes);
			Bound(root, degree, t, N);

			// 3. When a solution has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found, then the best solution found is optimal.
			if (!root.one_tree.CheckTour())
			{
				for (unsigned short i = 0; i < numberOfNodes; i++)
					path[i] = root.one_tree[i];

				return make_pair(path, root.HK);
			}

			N = ceil(numberOfNodes / 4.0f) + 5;
			t = 0;

			for (unsigned short i = 0; i < numberOfNodes; i++)
				t += abs((root.λ)[i]);

			t /= 2.0f * numberOfNodes;

			auto B = Branch(root.one_tree, degree, root, numberOfNodes);

			for (unsigned short i = 0; i < B.size(); i++)
				if (!Bound(B[i], degree, t, N))
					if (B[i].HK < UB)
						PQ_Add(PQ, B[i]);
		}
		// END. 1. Draw and initialize the root node.

		// 2. Repeat the following step until a solution (i.e., a complete circuit, represented by a terminal node) has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found:
		{
			vector<unsigned short> degree1(numberOfNodes, 0);
			auto current = PQ.size() - 1;

			while (current >= 0)
			{
				auto node = PQ[current];
				PQ.pop_back();

				// 3. When a solution has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found, then the best solution found is optimal.
				if (ceil(node.HK) >= UB)
					return make_pair(path, UB);

				if (node.HK < UB)
				{
					for (unsigned short i = 0; i < numberOfNodes; i++)
					{
						degree1.at(node.one_tree[i].from)++;
						degree1.at(node.one_tree[i].to)++;
					}

					if (!node.one_tree.CheckTour())
					{
						UB = node.HK;

						for (unsigned short i = 0; i < numberOfNodes; i++)
							path[i] = node.one_tree[i];
					}
					else
					{
						auto B = Branch(node.one_tree, degree1, node, numberOfNodes);

						for (unsigned short i = 0; i < B.size(); i++)
							if (!Bound(B[i], degree1, t, N))
								if (B[i].HK < UB)
								{
									if (!B[i].one_tree.CheckTour())
									{
										UB = B[i].HK;

										for (unsigned short k = 0; k < numberOfNodes; k++)
											path[k] = B[i].one_tree[k];
									}

									PQ_Add(PQ, B[i]);
									current++;
								}
					}
				}

				for (unsigned short k = 0; k < numberOfNodes; k++)
					degree1[k] = 0;

				current--;
			}
		}
		// END 2. Repeat the following step until a solution (i.e., a complete circuit, represented by a terminal node) has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found:

		return make_pair(path, UB);
	}

	void Branch_and_Bound::Solve(float &opt, string &path)
	{
		auto R = HKAlgo();

		opt = R.second;
		path = PrintPath(R.first);
	}

}