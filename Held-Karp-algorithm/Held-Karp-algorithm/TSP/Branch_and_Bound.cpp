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

#include <set>

#include "Branch_and_Bound.hpp"

namespace TSP
{
	Branch_and_Bound::Branch_and_Bound(const vector<vector<float>> &DistanceMatrix2D) : TSP(DistanceMatrix2D) {}

	string Branch_and_Bound::PrintPath(vector<pair<unsigned short, unsigned short>> &path)
	{
		string s;
		set<unsigned short> S;
		unsigned short i = 0, x = 0;
		vector<vector<bool>> E(numberOfNodes, vector<bool>(numberOfNodes, false));

		for (auto const e : path)
		{
			E[e.first][e.second] = true;
			E[e.second][e.first] = true;
		}

		while (x < numberOfNodes)
		{
			s += to_string(i) + " ";
			S.insert(i);

			for (size_t y = 0; y < numberOfNodes; y++)
				if (E[i][y] && S.count(y) == 0)
				{
					i = y;
					break;
				}

			x++;
		}

		return s + "0";
	}

	//Returns the index of a vertex adjacent to v by a required edge, if it exists, and n otherwise
	unsigned short Branch_and_Bound::is_incident_to_required(QNode &current_node, unsigned short v, unsigned short n)
	{
		for (unsigned short i = 0; i < current_node.R.size(); i++)
		{
			if (current_node.R[i].first == v)
				return current_node.R[i].second;

			if (current_node.R[i].second == v)
				return current_node.R[i].first;
		}
		return n;
	}

	/*
	Checks wether the edge {i,j} is contained in the tree given by the edge list tree
	returns 1 if it is contained and 0 otherwise
	*/
	unsigned short Branch_and_Bound::is_in_tree(vector<pair<unsigned short, unsigned short>> const &tree, unsigned short i, unsigned short j)
	{
		for (unsigned short k = 0; k < tree.size(); k++)
			if ((tree[k].first == i && tree[k].second == j) || (tree[k].first == j && tree[k].second == i))
				return 1;

		return 0;
	}

	/*
	Checks wether the edge {i,j} is in the list of forbidden edges of the QNode node
	returns 1 if it is contained and 0 otherwise
	*/
	unsigned short Branch_and_Bound::is_forbidden(QNode const &node, unsigned short i, unsigned short j)
	{
		for (unsigned short k = 0; k < node.F.size(); k++)
			if ((node.F[k].first == i && node.F[k].second == j) || (node.F[k].first == j && node.F[k].second == i))
				return 1;

		return 0;
	}

	/*
	Performs the branching step of the branch-and-bound algorithm on the QNode current_node,
	where tree is the one-tree computed for current_node, degrees contains the degree of every vertex within the tree and n is the problem dimension
	the function expects tree not to be a tour, but to contain some vertex of degree at least 3
	returns a vector of new QNodes to be added to the queue
	All choices are being made as recommended in the paper by Volgenant & Jonker
	*/
	vector<Branch_and_Bound::QNode> Branch_and_Bound::branch(vector<pair<unsigned short, unsigned short>> const &tree, vector<int> const &degrees, QNode &current_node, unsigned short n)
	{
		vector<QNode> result;
		unsigned short min_degree_req = n; //minimal degree greater than two of a vertex incident to a required edge
		unsigned short min_degree = n;	 //minimal degree greater than two of a vertex not incident to a required edge
		unsigned short p_req = n;			 //vertex where min_degree_req was attained
		unsigned short p = n;				 //vertex where min_degree was attained
		unsigned short req_neighbor = n;	 //other endpoint of the required edge incident to p_req

											 /* Try to find a vertex of degree at least 3 (but degree as small as possible)
											 that is incident to a required edge (this would be p_req)
											 If no such vertex exists, find any vertex of degree at least 3 but as small as possible, this will be p
											 */
		for (unsigned short i = 0; i < n; i++)
			if (degrees[i] > 2)
				if (is_incident_to_required(current_node, i, n) == n)
				{
					if ((unsigned short)degrees[i] < min_degree)
					{
						min_degree = degrees[i];
						p = i;
					}
				}
				else
				{
					if ((unsigned short)degrees[i] < min_degree_req)
					{
						min_degree_req = degrees[i];
						p_req = i;
						req_neighbor = is_incident_to_required(current_node, i, n);
					}
				}

		if (min_degree_req < n) //p_req existed; leads to branching with two new vertices
		{
			//choose e_1 that is not in the tree
			unsigned short i;

			for (i = 0; i < n; i++)
				if (i != p_req && i != req_neighbor)
					if (!is_in_tree(tree, i, p_req) && !is_forbidden(current_node, i, p_req))
						break; //we may take e_1 = {i, p}

			if (i == n) //all non-tree edges incident to p are forbidden, choose a tree edge as e_1			
				for (i = 0; i < n; i++)
					if (i != p_req && i != req_neighbor)
						if (!is_forbidden(current_node, i, p_req))
							break; //we may take e_1 = {i, p}

								   //Create the QNode for S2, where e_1 is required
								   //as there is one more required edge incident to p_req, forbid all other edges incident to p_req
			vector<pair<unsigned short, unsigned short>> R = current_node.R;
			R.push_back(pair<unsigned short, unsigned short>(i, p_req));
			vector<pair<unsigned short, unsigned short>> F = current_node.F;

			//forbid all edges incident to p except the two required ones
			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p_req && k != req_neighbor && !is_forbidden(current_node, p_req, k))
					F.push_back(pair<unsigned short, unsigned short>(p_req, k));

			QNode S2 = QNode(R, F, current_node.λ, n);
			result.push_back(S2);

			//Create the QNode for S3, where e_1 is forbidden
			R = current_node.R;
			F = current_node.F;
			F.push_back(pair<unsigned short, unsigned short>(i, p_req));

			//check if all but two are forbidden, if so, require the two other ones
			vector<unsigned short> forbidden(n, 0);
			forbidden[p_req] = 1;
			unsigned short num_forbidden = 0;
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
							R.push_back(pair<unsigned short, unsigned short>(p_req, k));

			QNode S3 = QNode(R, F, current_node.λ, n);
			result.push_back(S3);
		}
		else //p_req doesn't exist, use p. leads to branching with three new vertices
		{
			//choose e_1 that is (preferably) not in the tree and some e_2
			unsigned short i, j;
			for (i = 0; i < n; i++)
				if (i != p && !is_in_tree(tree, i, p) && !is_forbidden(current_node, i, p))
					break; //we may take e_1 = {i, p}

			if (i == n) //all non-tree edges incident to p are forbidden, choose a tree edge as e_1			
				for (i = 0; i < n; i++)
					if (i != p && !is_forbidden(current_node, i, p))
						break; //we may take e_1 = {i, p}

			for (j = 0; j < n; j++)
				if (j != p && j != i)
					if (!is_forbidden(current_node, j, p))
						break; //we may take e_2 = {j, p}

							   //Create the QNode S1, where e_1 and e_2 are required
			vector<pair<unsigned short, unsigned short>> R = current_node.R;
			R.push_back(pair<unsigned short, unsigned short>(i, p));
			R.push_back(pair<unsigned short, unsigned short>(j, p));
			vector<pair<unsigned short, unsigned short>> F = current_node.F;

			//forbid all edges incident to p except the two required ones
			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p && k != j && !is_forbidden(current_node, p, k))
					F.push_back(pair<unsigned short, unsigned short>(p, k));

			QNode S1 = QNode(R, F, current_node.λ, n);
			result.push_back(S1);

			//Create the QNode S2, where e_1 is required and e_2 is forbidden
			R = current_node.R;
			F = current_node.F;
			R.push_back(pair<unsigned short, unsigned short>(i, p));
			F.push_back(pair<unsigned short, unsigned short>(j, p));
			//check if all but two are forbidden, if so, require the two other ones
			vector<unsigned short> forbidden(n, 0);
			forbidden[p] = 1;
			unsigned short num_forbidden = 0;

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
							R.push_back(pair<unsigned short, unsigned short>(p, k));

			QNode S2 = QNode(R, F, current_node.λ, n);
			result.push_back(S2);

			//Create the QNode S3, where e_1 is forbidden
			R = current_node.R;
			F = current_node.F;
			F.push_back(pair<unsigned short, unsigned short>(i, p));

			// if all but two edges incident to p are forbidden, we can require the last two ones
			if (num_forbidden == n - 3)
			{
				//we can reuse forbidden, but we have to be careful because it should be a bit different now
				for (unsigned short k = 0; k < n; k++)
					if (!forbidden[k])
						if (k != i)
							R.push_back(pair<unsigned short, unsigned short>(p, k));

				R.push_back(pair<unsigned short, unsigned short>(p, j));
			}

			QNode S3 = QNode(R, F, current_node.λ, n);
			result.push_back(S3);
		}

		return result;
	}

	/*
	Checks wether the one-tree given by the edge list tree is actually a tour
	returns 1 if it is, and 0 otherwise
	*/
	bool Branch_and_Bound::check_tour(vector<pair<unsigned short, unsigned short>> const &Tree)
	{
		vector<unsigned short> degree(Tree.size(), 0);
		for (unsigned short i = 0; i < Tree.size(); i++)
		{
			degree[Tree[i].first]++;
			degree[Tree[i].second]++;
		}

		//check whether the degree of each vertex is two
		for (unsigned short i = 0; i < Tree.size(); i++)
			if (degree[i] != 2)
				return 1;

		return 0;
	}

	/*
	Calculates the Held-Karp bound (with forbidden & required edges as given in QNode)
	and completes the information (HK, λ, one-tree) in the given node object
	before the execution, node contains required edges, forbidden edges, empty tree and initial λ value from the parent branching node
	returns wether the algorithm terminated successfully (0) or not (1)
	*/
	bool Branch_and_Bound::Held_Karp_bound(vector<vector<float>> const &W, QNode &QNode, vector<int> &degree, float t, unsigned short const steps)
	{
		unsigned short size = W.size();

		vector<vector<float>> Weights(size, vector<float>(size));
		vector<pair<unsigned short, unsigned short>> Tree(size);
		vector<vector<int>> omitted(size, vector<int>(size, 0));
		vector<float> OptLambda(size);

		unsigned short req = QNode.R.size();
		float Treeweight = 0;
		float delta = 3.0f * t / (2.0f * steps);
		float ddelta = t / (steps * steps - steps);
		float firstmin = numeric_limits<float>::infinity();
		float secondmin = numeric_limits<float>::infinity();
		unsigned short first, second;
		unsigned short req1 = 0;
		unsigned short req2 = 0;
		vector<bool> forbidden(size, 0);

		for (unsigned short i = 0; i < size; i++)
			degree[i] = 0;

		//mark all required and forbidden edges
		//if edges incident to 0 are required we store them
		for (unsigned short i = 0; i < QNode.R.size(); i++)
		{
			omitted[QNode.R[i].first][QNode.R[i].second] = 1;
			omitted[QNode.R[i].second][QNode.R[i].first] = 1;

			if (QNode.R[i].first == 0)
			{
				req--;

				if (req < QNode.R.size() - 1)
					req2 = QNode.R[i].second;
				else
					req1 = QNode.R[i].second;
			}

			if (QNode.R[i].second == 0)
			{
				req--;

				if (req < QNode.R.size() - 1)
					req2 = QNode.R[i].first;
				else
					req1 = QNode.R[i].first;
			}
		}

		//if edges incident to 0 are forbidden we mark them
		for (unsigned short i = 0; i < QNode.F.size(); i++)
		{
			omitted.at(QNode.F[i].first).at(QNode.F[i].second) = 2;
			omitted.at(QNode.F[i].second).at(QNode.F[i].first) = 2;

			if (QNode.F[i].first == 0)
				forbidden.at(QNode.F[i].second) = 1;

			if (QNode.F[i].second == 0)
				forbidden.at(QNode.F[i].first) = 1;
		}

		//compute new weights
		for (unsigned short i = 0; i < size; i++)
			for (unsigned short j = 0; j < size; j++)
				Weights[i][j] = W[i][j] + QNode.λ[i] + QNode.λ[j];

		for (unsigned short k = 0; k < steps; k++)
		{
			//compute MST, if MST does not exist return 1
			if (minimum_spanning_tree(Tree, omitted, Weights, req))
				return 1;

			//1-tree
			for (unsigned short i = 1; i < size; i++)
				if (Weights[0][i] < firstmin && !forbidden[i])
				{
					firstmin = Weights[0][i];
					first = i;
				}

			for (unsigned short i = 1; i < size; i++)
				if (i != first)
					if (Weights[0][i] < secondmin && !forbidden[i])
					{
						secondmin = Weights[0][i];
						second = i;
					}

			if (req1 == 0)
			{
				Tree[Tree.size() - 2] = make_pair(0, first);
				Tree[Tree.size() - 1] = make_pair(0, second);
			}
			else if (req2 == 0)
			{
				Tree[Tree.size() - 2] = make_pair(0, first);
				Tree[Tree.size() - 1] = make_pair(0, req1);
			}
			else
			{
				Tree[Tree.size() - 2] = make_pair(0, req1);
				Tree[Tree.size() - 1] = make_pair(0, req2);
			}

			//compute degrees
			for (unsigned short i = 0; i < Tree.size(); i++)
			{
				degree[Tree[i].first]++;
				degree[Tree[i].second]++;
			}

			//update HK
			for (unsigned short i = 0; i < Tree.size(); i++)
			{
				Treeweight += Weights[Tree[i].first][Tree[i].second];
				Treeweight -= 2 * QNode.λ[i];
			}

			//store Tree and λ
			if (QNode.HK < Treeweight)
			{
				QNode.HK = Treeweight;

				for (unsigned short i = 0; i < (QNode.one_tree).size(); i++)
				{
					(QNode.one_tree)[i] = Tree[i];
					OptLambda[i] = QNode.λ[i];
				}
			}

			//update weights
			for (unsigned short i = 0; i < size; i++)
				for (unsigned short j = 0; j < size; j++)
					Weights[i][j] += (degree[i] - 2) * t + (degree[j] - 2) * t;

			//update λ
			for (unsigned short i = 0; i < size; i++)
			{
				QNode.λ[i] += (degree[i] - 2) * t;
				degree[i] = 0;
			}

			//update t, delta
			t -= delta;
			delta -= ddelta;
			Treeweight = 0;
			firstmin = numeric_limits<float>::infinity();
			secondmin = numeric_limits<float>::infinity();
		}

		for (unsigned short i = 0; i < size; i++)
		{
			QNode.λ[i] = OptLambda[i];
			degree[QNode.one_tree[i].first]++;
			degree[QNode.one_tree[i].second]++;
		}

		return 0;
	}

	/*
	Implementation of Prim's algorithm in order to compute an MST
	of the subgraph induced by the vertices 1, ..., n-1
	of the graph given by the cost matrix Weights
	that takes into account that some edges are forbidden (omitted)
	and some edges are required (req)
	Saves an edge list of the resulting tree in Tree
	returns wether the algorithm terminated successfully (0) or not (1).
	*/
	bool Branch_and_Bound::minimum_spanning_tree(vector<pair<unsigned short, unsigned short>> &Tree, vector<vector<int>> const &omitted, vector<vector<float>> const &Weights, unsigned short const req)
	{
		unsigned short size = Weights.size();
		vector<bool> visited(size, 0);

		//min stores for each vertex an edge of minimum weight incident to the set of visited verticies
		vector<pair<float, unsigned short>> min(size, make_pair(numeric_limits<float>::infinity(), 0));

		unsigned short vertex = 1;
		unsigned short new_vertex = 1;
		float minimum = numeric_limits<float>::infinity();

		//number of required edges in Tree
		unsigned short req_num = 0;

		visited[1] = 1;

		//build MST
		for (unsigned short j = 0; j < size - 2; j++)
		{
			//do not consider vertex 0 and 1
			for (unsigned short i = 2; i < size; i++)
			{
				if (i != vertex && !visited[i])
				{
					//update the minimum weight of a cennection to visited verticies
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
				}
			}

			//find an edge of minimum weight between visited and not visited verticies
			for (unsigned short i = 2; i < size; i++)
			{
				if (!visited[i] && min[i].first < minimum)
				{
					minimum = min[i].first;
					new_vertex = i;
				}
			}
			//too many forbidden edges
			if (new_vertex == vertex)
				return 1;

			//update number of required edges in Tree
			if (omitted[new_vertex].at(min[new_vertex].second))
				req_num++;

			//update vertex
			vertex = new_vertex;
			visited[vertex] = 1;

			//insert corresponding edge in Tree
			Tree[j] = make_pair(vertex, min[vertex].second);
			minimum = numeric_limits<float>::infinity();
		}
		//too many required edges
		if (req_num < req)
			return 1;

		return 0;
	}

	/*
	Computes the inital value for t, t_0 as recommended in the paper
	*/
	float Branch_and_Bound::initial_value(vector<vector<float>> const &W)
	{
		vector<pair<unsigned short, unsigned short>> Tree(W.size());
		vector<vector<int>> omitted(W.size(), vector<int>(W.size(), 0));

		float t = 0;
		float firstmin = FLT_MAX;
		float secondmin = FLT_MAX;

		unsigned short first, second;

		//compute MST
		minimum_spanning_tree(Tree, omitted, W, 0);

		for (unsigned short i = 1; i < Tree.size(); i++)
			if (W[0][i] < firstmin)
			{
				firstmin = W[0][i];
				first = i;
			}

		for (unsigned short i = 1; i < Tree.size(); i++)
			if (i != first)
				if (W[0][i] < secondmin)
				{
					secondmin = W[0][i];
					second = i;
				}

		//one_tree
		Tree[Tree.size() - 2] = make_pair(0, first);
		Tree[Tree.size() - 1] = make_pair(0, second);

		//compute
		for (unsigned short i = 0; i < Tree.size(); i++)
			t += W[Tree[i].first][Tree[i].second];

		return t / (2.0f * W.size());
	}

	/*
	Performs the sorted insertion of the branching node new_elem in the queue L wrt the Held-Karp-Bound of new_elem
	*/
	void Branch_and_Bound::insert(vector<QNode> &L, QNode &new_elem)
	{
		QNode *tmp;
		unsigned short position = L.size();

		L.push_back(new_elem);

		while (position > 0 && L[position - 1].HK < L[position].HK)
		{
			tmp = &L[position];
			L[position] = L[position - 1];
			L[position - 1] = *tmp;
			position--;
		}
	}

	pair<vector<pair<unsigned short, unsigned short>>, float> Branch_and_Bound::DoBranch_and_Bound(vector<vector<float>> const &W)
	{
		unsigned short size = W.size();
		vector<int> degree(size, 0);
		float U = W[size - 1][0];
		float t;
		unsigned short N;

		vector<pair<unsigned short, unsigned short>> Opt(size);

		Opt[0] = make_pair(size - 1, 0);

		//compute upper bound
		for (unsigned short i = 0; i < W.size() - 1; i++)
		{
			U += W[i][i + 1];
			Opt[i + 1] = make_pair(i, i + 1);
		}

		//initialization of N and t
		//for root:
		t = initial_value(W);
		N = ceil(size * size / 50.0f) + size + 15;

		//computing lower bound
		QNode root(vector<pair<unsigned short, unsigned short>>(), vector<pair<unsigned short, unsigned short>>(), vector<float>(size), size);
		Held_Karp_bound(W, root, degree, t, N);

		//if a tour is already found, it is optimum
		if (!check_tour(root.one_tree))
		{
			for (unsigned short i = 0; i < size; i++)
				Opt[i] = root.one_tree[i];

			return make_pair(Opt, root.HK);
		}

		//initialization of N and t for other verticies
		N = ceil(size / 4.0f) + 5;
		t = 0;

		for (unsigned short i = 0; i < size; i++)
			t += abs((root.λ)[i]);

		t /= 2.0f * size;

		//begin of branching
		vector<QNode> B = branch(root.one_tree, degree, root, size);

		//queue
		vector<QNode> S;
		for (unsigned short i = 0; i < B.size(); i++)
			if (!Held_Karp_bound(W, B[i], degree, t, N))
				if (B[i].HK < U)
					insert(S, B[i]);

		vector<int> degree1(size, 0);
		int current = S.size() - 1;

		//Branch and Bound using best bound
		while (current >= 0)
		{
			QNode node = S.at(current);
			S.pop_back();

			//since we use best bound, node.HK is a lower bound for the optimum
			if (ceil(node.HK) >= U)
				return make_pair(Opt, U);

			//consider node only if its HK bound is smaller than U
			if (node.HK < U)
			{
				for (unsigned short i = 0; i < size; i++)
				{
					degree1.at(node.one_tree[i].first)++;
					degree1.at(node.one_tree[i].second)++;
				}

				//check whether the current one_tree is a tour
				if (!check_tour(node.one_tree))
				{
					//update U
					U = node.HK;

					//update Opt
					for (unsigned short i = 0; i < size; i++)
						Opt[i] = node.one_tree[i];
				}
				else
				{
					//branch with node
					vector<QNode> B = branch(node.one_tree, degree1, node, size);
					for (unsigned short i = 0; i < B.size(); i++)
						if (!Held_Karp_bound(W, B[i], degree1, t, N))
							//consider B[i] only if its HK bound is smaller than U
							if (B[i].HK < U)
							{
								//check whether we found a tour
								if (!check_tour(B[i].one_tree))
								{
									U = B[i].HK;

									for (unsigned short k = 0; k < size; k++)
										Opt[k] = B[i].one_tree[k];
								}

								//insert B[i] wrt Held-Karp-Bound
								insert(S, B[i]);
								current++;
							}
				}
			}

			for (unsigned short k = 0; k < size; k++)
				degree1[k] = 0;

			current--;
		}

		return make_pair(Opt, U);
	}

	void Branch_and_Bound::Solve(float &opt, string &path)
	{
		auto R = DoBranch_and_Bound(distance);

		opt = R.second;
		path = PrintPath(R.first);
	}

}