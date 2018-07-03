#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#define WHITE 0
#define GRAY 1
#define BLACK 2

using namespace std;


typedef struct node
{
	int mynum;
	int price;
	node* next;

}Node;

typedef struct front
{
	Node* next;
	Node* last;
}Front;

Front** vertex;
int N, M;
int color[500];
int p[500];

void insert(int from, int to, int price)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	Node* find = vertex[from]->next;

	temp->mynum = to;
	temp->next = NULL;
	temp->price = price;

	if (vertex[from]->next == NULL)
		vertex[from]->next = temp;
	else
	{
		/*
		while (find->next != NULL)
		{
		find = find->next;
		}

		find->next = temp;
		*/

		vertex[from]->last->next = temp;
	}
	vertex[from]->last = temp;
}
void printList()
{
	for (int i = 1; i <= N; i++)
	{
		Node* Print = vertex[i]->next;

		cout.width(3);
		cout << i << " : ";

		while (Print != NULL)
		{
			cout << "(" << Print->mynum << " , " << Print->price << ")" << " ";

			Print = Print->next;
		}
		cout << endl;
	}
}

void DFS_visit(int u)
{
	color[u] = GRAY;
	
	Node* temp = vertex[u]->next;
	int v = 1;

	while(temp != NULL)
	{
		// temp->mynum : 현재 노드 번호
		// temp->next : 다음 노드
		// temp->price : 노쓸모

		if (color[temp->mynum] == WHITE)
		{
			p[temp->mynum] = u;
			DFS_visit(temp->mynum);
		}

		temp = temp->next;
	}

	color[u] = BLACK;
}

void DFS()
{
	for (int u = 1; u <= N; u++)
	{
		color[u] = WHITE;
		p[u] = -1;
	}
	for (int u = 1 ; u <= N; u++)
	{
		if (color[u] == WHITE)
			DFS_visit(u);

	}
}


int main()
{
	ifstream Data;
	string fileName;

	cin >> fileName;

	Data.open(fileName);

	Data >> N >> M;

	vertex = (Front**)malloc((N + 1) * sizeof(Front*));

	for (int i = 0; i <= N; i++)
	{
		vertex[i] = (Front*)malloc(sizeof(Front));
	}

	for (int i = 1; i <= N; i++)
	{
		vertex[i]->last = NULL;
		vertex[i]->next = NULL;
		
		color[i] = WHITE;
		p[i] = -1;
	}

	for (int i = 0; i < M; i++)
	{
		int from, to;
		Data >> from >> to;

		insert(from, to, 0);
	}
	DFS_visit(1);

	printList();

	for (int i = 1; i <= N; i++)
		cout << p[i] << " ";
	cout << endl;



	cin >> p[1];
}