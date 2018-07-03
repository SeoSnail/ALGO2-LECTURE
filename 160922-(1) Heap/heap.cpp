#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

int arr[1000];
int temp[1000];
int heapSort[1000];
int N;

void shiftdown(int A)
{
	int shift_key = arr[A];
	int parent = A;
	int smallChild;

	while (2 * parent <= N)
	{
		if ((2 * parent < N) && (arr[2 * parent] < arr[2 * parent + 1]))
			smallChild = 2 * parent;
		else smallChild = 2 * parent + 1;

		if (shift_key > arr[smallChild])
		{
			arr[parent] = arr[smallChild];
			parent = smallChild;
		}
		else break;
	}
	arr[parent] = shift_key;

}

void makeHeap(int n)
{
	for (int i = n/2 ; i >= 1; i--)
	{
		shiftdown(i);
	}
}

int root()
{
	int keyOut = arr[1];
	arr[1] = arr[N];
	N--;
	shiftdown(1);
	return keyOut;
}

void removeKeys()
{
	int cnt = N;

	for (int i = 1; i <= cnt; i++)
	{
		heapSort[i] = root();
	}
}

void insert(int num)
{
	N++;

	int child = N;
	for (int i = N / 2; i >= 1; i = i / 2)
	{
		if (arr[i] > num)
		{
			arr[child] = arr[i];
			child = i;
		}
		else
		{
			arr[child] = num;
			break;
		}
	}
}

int main()
{
	ifstream Data;
	string fileName;
	int cnt;

	cin >> fileName;

	Data.open(fileName);

	Data >> N;
	cnt = N;
	for (int i = 1; i <= N; i++)
		Data >> arr[i];
	for (int i = N + 1; i < 1000; i++)
		arr[i] = 987654321;


	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[input]" << endl;


	makeHeap(N);

	for (int i = 1; i <= N; i++)
		temp[i] = arr[i];

	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[make heap]" << endl;


	removeKeys();
	
	for (int i = 1; i <= cnt; i++)
		cout << heapSort[i] << " ";
	cout << "[heapSort]" << endl;

	N = cnt;
	for (int i = 1; i <= N; i++)
		arr[i] = temp[i]; // º¹¿ø


	insert(50);

	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[insert 50]" << endl;

	root();

	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[delete min]" << endl;


	insert(11);

	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[insert 11]" << endl;

	insert(1);

	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[insert 1]" << endl;
	
	root();

	for (int i = 1; i <= N; i++)
		cout << arr[i] << " ";
	cout << "[delete min]" << endl;

	cin >> fileName;//system break;
}