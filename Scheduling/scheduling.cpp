#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>


using namespace std;

int scheduleArr[1001];
int Empty[1001];
int headArr[1001];
int N;

int find(int i)
//find the root of the tree containing element i.
{
	while (headArr[i] >= 0)
	{
		i = headArr[i];
	}
	return i;
}

int CollapsingFind(int i) // 루트를 직접 지정으로 바꾼다.
{
	int r;

	for (r = i; headArr[r] >= 1; r = headArr[r]);

	while (i != r)
	{
		int s = headArr[i];
		headArr[i] = r;
		i = s;
	}

	return r;

}

void weightedUnion(int num, int dday, int profit)
{
	int root1, root2;
	int empty_slot, temp;

	root1 = CollapsingFind(dday);
	empty_slot = Empty[root1];
	scheduleArr[empty_slot] = num;

	if (empty_slot == 1) temp = N;
	else temp = empty_slot - 1;

	root2 = CollapsingFind(temp);

	if (headArr[root1] < headArr[root2])
	{
		headArr[root1] = headArr[root1] + headArr[root2];
		headArr[root2] = root1;
		Empty[root1] = Empty[root2];
	}
	else
	{
		headArr[root2] = headArr[root1] + headArr[root2];
		headArr[root1] = root2;
	}
}

int main()
{
	ifstream Data;
	memset(headArr, -1, 1000 * sizeof(int));
	headArr[0] = 987654321;
	string fileName;
	

	cin >> fileName;
	Data.open(fileName);

	Data >> N;

	for (int i = 0; i < 1000; i++)
		Empty[i] = i;

	for (int i = 1; i <= N; i++)
	{
		int num, dday, profit;
		int root1, root2;
		int empty_slot, temp;

		Data >> num >> dday >> profit;
		
		weightedUnion(num, dday, profit);

		for (int i = 1; i <= N; i++)
		{
			cout << scheduleArr[i] << " ";
		}
		
		cout << endl;

	}

}