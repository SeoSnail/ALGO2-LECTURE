#include <iostream>
#include <fstream>

using namespace std;

int schedule[500];
int parent[500];
int Empty[500];

void simpleUnion(int i, int j)
//Replace the disjoint sets with roots i and j, i! = j with //their union
{
	parent[i] = j;
}

int simpleFind(int i)
//find the root of the tree containing element i.
{
	while (parent[i] >= 0) i = parent[i];
	return i;
}

void weightedUnion(int i, int j)
//Union sets with roots i and j, i ¡Á j, using the weighting rule.
//parent[ i ] = - count[ i ] and parent[ j ] = - count[ j ].
{
	int temp = parent[i] + parent[j];
	if (parent[i] > parent[j]) { // i has fewer nodes
		parent[i] = j;
		parent[j] = temp;
	}
	else { // j has fewer nodes (or i and j have the same number of nodes)
		parent[j] = i;
		parent[i] = temp;
	}
}

int main(void)
{
	int n;

	ifstream inFile("s1.txt");
	inFile >> n;

	for (int i = 1; i <= n; i++)
	{
		parent[i] = -1;
		Empty[i] = i;
	}

	int jobNo;
	int deadline;
	int profit;
	int p;
	int add_cnt = 0;

	int root1, root2;
	int Empty_slot;
	int temp;

	for (int i = 0; i < n; i++)
	{
		inFile >> jobNo;
		inFile >> deadline;
		inFile >> profit;
		root1 = simpleFind(deadline);
		Empty_slot = Empty[root1];
		schedule[Empty_slot] = jobNo;
		if (Empty_slot == 1) temp = n;
		else temp = Empty_slot - 1;

		root2 = simpleFind(temp);
		if (parent[root1] < parent[root2])
		{
			parent[root1] = parent[root1] + parent[root2];
			parent[root2] = root1;
			Empty[root1] = Empty[root2];
		}
		else {
			parent[root2] = parent[root1] + parent[root2];
			parent[root1] = root2;
		}

		cout << "Empty :  ";
		for (int i = 1; i <= n; i++)
		{
			cout << Empty[i] << " ";
		}
		cout << endl;

		cout << "parent :  ";
		for (int i = 1; i <= n; i++)
		{
			cout << parent[i] << " ";
		}
		cout << endl;

		cout << "schedule : ";
		for (int i = 1; i <= n; i++)
		{
			cout << schedule[i] << "  ";
		}
		cout << endl;

	}
	int a;
	cin >> a;
}
//let next job be (x,y,z);
//root1 = find(y);
//Empty_slot = Empty[root1];
//schedule[Empty_slot] = x;
//if (Empty_slot == 1) temp = num_jobs;
//	 else temp = Empty_slot -1;
//root2 = find(temp);
//if (parent[root1] < parent[root2] )
//	   { parent[root1] = parent[root1]+ parent[root2];
//		 parent[root2] = root1;
//		 Empty[root1] = Empty[root2];}
//else {  parent[root2] = parent[root1]+ parent[root2];
//		parent[root1] = root2; }


