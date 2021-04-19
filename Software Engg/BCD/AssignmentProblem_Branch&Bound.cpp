/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGINEERING LAB. (UG2, Sem2)
 * ASSIGNMENT: ASSIGNMENT PROBLEM OPTIMIZATION (QUESTION 2)
 */

/************************************************************************
* COMPILATION: g++ 2.cpp -o 2
* EXECUTION: ./2.exe
------------------------------------------------------------------------
* Sample input for testing code: 

Enter dimension of cost matrix (N):
4

Enter the cost matrix:
9 2 7 8
6 4 3 7
5 8 1 8
7 6 9 4
-------------------------------------------------------------------------
* Sample Output: 

Allotment of Jobs:
------------------------------------
Assign Worker A to Job 1
Assign Worker B to Job 0
Assign Worker C to Job 2
Assign Worker D to Job 3

Final Optimized cost = 13
***********************************************************************/

#include <bits/stdc++.h>
using namespace std;

/* type definitions to shorten syntax */
typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<vector<int>> vvi;

/* macros for ease of syntax */
#define pb push_back

/* state space tree node */
struct Node
{
	Node *parent; // stores parent node of current node [helps in tracing path when answer is found]
	int pathCost; // contains cost for ancestors nodes including current node
	int cost;	  // contains least promising cost
	int workerID; // contain worker number
	int jobID;	  // contains Job ID
	vb assigned;  // Boolean array assigned will contains info about available jobs
};

/* Function to allocate a new search tree node [Here Person x is assigned to job y] */
Node *newNode(int x, int y, vb assigned, int N, Node *parent)
{
	Node *node = new Node;

	for (int j = 0; j < N; j++)
		(node->assigned).push_back(assigned[j]);

	if (y >= 0)
		(node->assigned)[y] = true;

	node->parent = parent;
	node->workerID = x;
	node->jobID = y;

	return node;
}

/* Comparison object to be used to order the heap */
struct comp
{
	bool operator()(const Node *lhs, const Node *rhs) const
	{
		return lhs->cost > rhs->cost;
	}
};

/* Function to calculate the least promising cost of node after worker x is assigned to job y. */
int calculateCost(vvi costMatrix, int N, int x, int y, vb assigned)
{
	int cost = 0;

	// to store unavailable jobs
	vb available(N, true);

	// start from next worker
	for (int i = x + 1; i < N; i++)
	{
		int min = INT_MAX, minIndex = -1;

		// do for each job
		for (int j = 0; j < N; j++)
		{
			// if job is unassigned
			if (!assigned[j] && available[j] && costMatrix[i][j] < min)
			{
				// store job number
				minIndex = j;

				// store cost
				min = costMatrix[i][j];
			}
		}

		// add cost of next worker
		cost += min;

		// job becomes unavailable
		available[minIndex] = false;
	}

	return cost;
}

/* print Assignments */
void printAssignments(Node *min)
{
	if (min->parent == NULL)
		return;

	printAssignments(min->parent);
	cout << "Assign Worker " << char(min->workerID + 'A')
		 << " to Job " << min->jobID << endl;
}

/* utility function to find minimum cost using Branch and Bound */
int findMinCost(vvi costMatrix, int N)
{
	// Create a priority queue to store live nodes of search tree;
	priority_queue<Node *, std::vector<Node *>, comp> pq;

	// initialize heap to dummy node with cost 0
	vb assigned(N, false);
	Node *root = newNode(-1, -1, assigned, N, NULL);
	root->pathCost = root->cost = 0;
	root->workerID = -1;

	// Add dummy node to list of live nodes;
	pq.push(root);

	cout << "\n-----------------------------------\n";
	cout << "Allotment of Jobs:\n";
	cout << "------------------------------------\n";

	/* Finds a live node with least cost, add its childrens to list of live nodes and finally deletes it from the list. */
	while (!pq.empty())
	{
		// Find a live node with least estimated cost
		Node *min = pq.top();

		// The found node is deleted from the list of
		// live nodes
		pq.pop();

		// i stores next worker
		int i = min->workerID + 1;

		// if all workers are assigned a job
		if (i == N)
		{
			printAssignments(min);
			return min->cost;
		}

		// do for each job
		for (int j = 0; j < N; j++)
		{
			// If unassigned
			if (!min->assigned[j])
			{
				// create a new tree node
				Node *child = newNode(i, j, min->assigned, N, min);

				// cost for ancestors nodes including current node
				child->pathCost = min->pathCost + costMatrix[i][j];

				// calculate its lower bound
				child->cost = child->pathCost + calculateCost(costMatrix, N, i, j, child->assigned);

				// Add child to list of live nodes;
				pq.push(child);
			}
		}
	}
}

int main()
{
	// take cost matrix dimension as input
	int N;
	cout << "Enter dimension of cost matrix (N):\n";
	cin >> N;

	// declaring matrix
	vvi costMatrix(N, vi(N));

	// taking cost matrix dimension as input
	cout << "\nEnter the cost matrix:\n";
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cin >> costMatrix[i][j];
		}
	}

	// final output of optimization
	int min_cost = findMinCost(costMatrix, N);
	cout << "\nFinal Optimized cost = " << min_cost << "\n";

	return 0;
}
