#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

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
			cout << "(" << Print->mynum  << " , " << Print->price << ")" << " ";
			
			Print = Print->next;
		}
		cout << endl;
	}
}


int main()
{
	ifstream Data;
	string fileName;

	cin >> fileName;

	Data.open(fileName);

	Data >> N >> M;

	vertex = (Front**)malloc( (N+1) * sizeof(Front*));

	for (int i = 0; i <= N; i++)
	{
		vertex[i] = (Front*)malloc(sizeof(Front));
	}

	for (int i = 1; i <= N; i++)
	{
		vertex[i]->last = NULL;
		vertex[i]->next = NULL;
	}

	for (int i = 0; i < M; i++)
	{
		int from, to, price;
		Data >> from >> to >> price;

		insert(from, to, price);
		insert(to, from, price);
	}

	printList();

	for (int i = 0; i <= N; i++)
		free(vertex[i]);
	free(vertex);
	int a;
	cin >> a;
}

//말록에 N+1에 괄호를 안쳐서 오류났었음.