﻿/*
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
		vector<vector<bool>> E(numberOfNodes, vector<bool>(numberOfNodes, false));
		unsigned short i = 0, x = 0;

		for (auto const e : path)
		{
			E[e.first][e.second] = true;
			E[e.second][e.first] = true;
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

	//Returns the index of a vertex adjacent to v by a required edge, if it exists, and n otherwise
	unsigned short Branch_and_Bound::is_incident_to_required(Node &current_node, unsigned short v, unsigned short n)
	{
		for (unsigned short i = 0; i < current_node.R.size(); i++)
		{
			if (v == current_node.R[i].first)
				return current_node.R[i].second;

			if (v == current_node.R[i].second)
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
	Checks wether the edge {i,j} is in the list of forbidden edges of the Node node
	returns 1 if it is contained and 0 otherwise
	*/
	unsigned short Branch_and_Bound::is_forbidden(Node const &node, unsigned short i, unsigned short j)
	{
		for (unsigned short k = 0; k < node.F.size(); k++)
			if ((node.F[k].first == i && node.F[k].second == j) || (node.F[k].first == j && node.F[k].second == i))
				return 1;

		return 0;
	}

	/*
	Performs the branching step of the branch-and-bound algorithm on the Node current_node,
	where tree is the one-tree computed for current_node, degrees contains the degree of every vertex within the tree and n is the problem dimension
	the function expects tree not to be a tour, but to contain some vertex of degree at least 3
	returns a vector of new Nodes to be added to the queue
	All choices are being made as recommended in the paper by Volgenant & Jonker
	*/
	vector<Branch_and_Bound::Node> Branch_and_Bound::branch(vector<pair<unsigned short, unsigned short>> const &tree, vector<unsigned short> const &degrees, Node &current_node, unsigned short n)
	{
		vector<Node> result;
		auto min_degree_req = n;	//minimal degree greater than two of a vertex incident to a required edge
		auto min_degree = n;		//minimal degree greater than two of a vertex not incident to a required edge
		auto p_req = n;				//vertex where min_degree_req was attained
		auto p = n;					//vertex where min_degree was attained
		auto req_neighbor = n;		//other endpoint of the required edge incident to p_req

		//Try to find a vertex of degree at least 3 (but degree as small as possible) that is incident to a required edge (this would be p_req).
		//If no such vertex exists, find any vertex of degree at least 3 but as small as possible, this will be p		
		for (unsigned short i = 0; i < n; i++)
			if (degrees[i] > 2)
				if (is_incident_to_required(current_node, i, n) == n)
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

			//Create the Node for S2, where e_1 is required as there is one more required edge incident to p_req, forbid all other edges incident to p_req
			auto F = current_node.F;
			auto R = current_node.R;
			R.push_back(make_pair(i, p_req));

			//forbid all edges incident to p except the two required ones
			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p_req && k != req_neighbor && !is_forbidden(current_node, p_req, k))
					F.push_back(make_pair(p_req, k));

			Node S2(R, F, current_node.λ, n);
			result.push_back(S2);

			//Create the Node for S3, where e_1 is forbidden
			R = current_node.R;
			F = current_node.F;
			F.push_back(make_pair(i, p_req));

			//check if all but two are forbidden, if so, require the two other ones
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

			//Create the Node S1, where e_1 and e_2 are required
			auto F = current_node.F;
			auto R = current_node.R;
			R.push_back(make_pair(i, p));
			R.push_back(make_pair(j, p));

			//forbid all edges incident to p except the two required ones
			for (unsigned short k = 0; k < n; k++)
				if (k != i && k != p && k != j && !is_forbidden(current_node, p, k))
					F.push_back(make_pair(p, k));

			Node S1(R, F, current_node.λ, n);
			result.push_back(S1);

			//Create the Node S2, where e_1 is required and e_2 is forbidden
			R = current_node.R;
			F = current_node.F;
			R.push_back(make_pair(i, p));
			F.push_back(make_pair(j, p));

			//check if all but two are forbidden, if so, require the two other ones
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

			//Create the Node S3, where e_1 is forbidden
			R = current_node.R;
			F = current_node.F;
			F.push_back(make_pair(i, p));

			// if all but two edges incident to p are forbidden, we can require the last two ones
			if (num_forbidden == n - 3)
			{
				//we can reuse forbidden, but we have to be careful because it should be a bit different now
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
	Calculates the Held-Karp bound (with forbidden & required edges as given in Node)
	and completes the information (HK, λ, one-tree) in the given node object
	before the execution, node contains required edges, forbidden edges, empty tree and initial λ value from the parent branching node
	returns wether the algorithm terminated successfully (0) or not (1)
	*/
	bool Branch_and_Bound::Held_Karp_bound(Node &node, vector<unsigned short> &degree, float t, unsigned short const steps)
	{
		vector<vector<float>> Weights(numberOfNodes, vector<float>(numberOfNodes));
		vector<pair<unsigned short, unsigned short>> Tree(numberOfNodes);
		vector<vector<unsigned short>> omitted(numberOfNodes, vector<unsigned short>(numberOfNodes, 0));
		vector<float> OptLambda(numberOfNodes);

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

		//mark all required and forbidden edges
		//if edges incident to 0 are required we store them
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

		//if edges incident to 0 are forbidden we mark them
		for (unsigned short i = 0; i < node.F.size(); i++)
		{
			omitted.at(node.F[i].first).at(node.F[i].second) = 2;
			omitted.at(node.F[i].second).at(node.F[i].first) = 2;

			if (node.F[i].first == 0)
				forbidden.at(node.F[i].second) = 1;

			if (node.F[i].second == 0)
				forbidden.at(node.F[i].first) = 1;
		}

		//compute new weights
		for (unsigned short i = 0; i < numberOfNodes; i++)
			for (unsigned short j = 0; j < numberOfNodes; j++)
				Weights[i][j] = distance[i][j] + node.λ[i] + node.λ[j];

		for (unsigned short k = 0; k < steps; k++)
		{
			//compute MST, if MST does not exist return 1
			if (MST_Prim(Tree, omitted, Weights, req))
				return 1;

			//1-tree
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
				Treeweight -= 2 * node.λ[i];
			}

			//store Tree and λ
			if (node.HK < Treeweight)
			{
				node.HK = Treeweight;

				for (unsigned short i = 0; i < (node.one_tree).size(); i++)
				{
					node.one_tree[i] = Tree[i];
					OptLambda[i] = node.λ[i];
				}
			}

			//update weights
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
			degree[node.one_tree[i].first]++;
			degree[node.one_tree[i].second]++;
		}

		return 0;
	}

	bool Branch_and_Bound::MST_Prim(vector<pair<unsigned short, unsigned short>> &Tree, vector<vector<unsigned short>> const &omitted, vector<vector<float>> const &Weights, unsigned short const req)
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

			Tree[j] = make_pair(vertex, min[vertex].second);
			minimum = FLT_MAX;
		}

		if (req_num < req)
			return true;

		return false;
	}

	/*
	Computes the inital value for t, t_0 as recommended in the paper
	*/
	float Branch_and_Bound::t1()
	{
		vector<pair<unsigned short, unsigned short>> Tree(distance.size());
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

		//one_tree
		Tree[Tree.size() - 2] = make_pair(0, first);
		Tree[Tree.size() - 1] = make_pair(0, second);

		//compute
		for (unsigned short i = 0; i < Tree.size(); i++)
			t += distance[Tree[i].first][Tree[i].second];

		return t / (2.0f * distance.size());
	}

	/*
	Performs the sorted insertion of the branching node new_elem in the queue L wrt the Held-Karp-Bound of new_elem
	*/
	void Branch_and_Bound::insert(vector<Node> &L, Node &new_elem)
	{
		Node *tmp;
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
	pair<vector<pair<unsigned short, unsigned short>>, float> Branch_and_Bound::HKAlgo()
	{
		vector<Node> S;
		vector<unsigned short> degree(numberOfNodes, 0);
		vector<pair<unsigned short, unsigned short>> path(numberOfNodes);

		path[0] = make_pair(numberOfNodes - 1, 0);

		auto UB = distance[numberOfNodes - 1][0];

		// Upper bound
		for (unsigned short i = 0; i < distance.size() - 1; i++)
		{
			UB += distance[i][i + 1];
			path[i + 1] = make_pair(i, i + 1);
		}

		auto t = t1();
		auto N = (numberOfNodes * numberOfNodes / 50.0f) + numberOfNodes + 15;

		// 1. Draw and initialize the root node.
		{
			Node root(numberOfNodes);
			Held_Karp_bound(root, degree, t, N);

			// 3. When a solution has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found, then the best solution found is optimal.
			if (!check_tour(root.one_tree))
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

			auto B = branch(root.one_tree, degree, root, numberOfNodes);

			for (unsigned short i = 0; i < B.size(); i++)
				if (!Held_Karp_bound(B[i], degree, t, N))
					if (B[i].HK < UB)
						insert(S, B[i]);
		}
		// END. 1. Draw and initialize the root node.

		// 2. Repeat the following step until a solution (i.e., a complete circuit, represented by a terminal node) has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found:
		{
			vector<unsigned short> degree1(numberOfNodes, 0);
			auto current = S.size() - 1;

			while (current >= 0)
			{
				auto node = S[current];
				S.pop_back();

				// 3. When a solution has been found and no unexplored non-terminal node has a smaller bound than the length of the best solution found, then the best solution found is optimal.
				if (ceil(node.HK) >= UB)
					return make_pair(path, UB);

				if (node.HK < UB)
				{
					for (unsigned short i = 0; i < numberOfNodes; i++)
					{
						degree1.at(node.one_tree[i].first)++;
						degree1.at(node.one_tree[i].second)++;
					}

					if (!check_tour(node.one_tree))
					{
						UB = node.HK;

						for (unsigned short i = 0; i < numberOfNodes; i++)
							path[i] = node.one_tree[i];
					}
					else
					{
						auto B = branch(node.one_tree, degree1, node, numberOfNodes);

						for (unsigned short i = 0; i < B.size(); i++)
							if (!Held_Karp_bound(B[i], degree1, t, N))
								if (B[i].HK < UB)
								{
									if (!check_tour(B[i].one_tree))
									{
										UB = B[i].HK;

										for (unsigned short k = 0; k < numberOfNodes; k++)
											path[k] = B[i].one_tree[k];
									}

									insert(S, B[i]);
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