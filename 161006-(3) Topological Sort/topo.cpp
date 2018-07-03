#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX 987654321

using namespace std;

class stack
{
public:
	int top;
	int size;
	int first;
	int tail;
	int data[10000];
public:
	void inqueue(int a);
	int dequeue();
	int isEmpty();

	stack()
	{
		top = 0;
		first = 0;
		tail = 0;
	};

};

void stack::inqueue(int a)
{
	data[tail++] = a;
}

int stack::dequeue()
{
	if (tail >= first)
		return data[first++];
	else
	{
		cout << "empty" << endl;
		return -MAX;
	}
}

int stack::isEmpty()
{
	if (first == tail)
		return 1;
	else
		return 0;
}

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
int d[500];
stack s1;

Node* head = NULL;


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

void insertLinked(int add1)
{
	Node* tempHead = head;
	Node* temp = (Node*)malloc(sizeof(Node));

	temp->mynum = add1;
	temp->next = NULL;

	if (head == NULL)
		head = temp;
	else
	{
		temp->next = head;
		head = temp;
	}
}

void printLinked()
{
	Node* find = head;
	while (find != NULL)
	{
		cout << find->mynum << " ";
		find = find->next;
	}
	cout << endl;
}

void DFS_visit(int u)
{
	color[u] = GRAY;

	Node* temp = vertex[u]->next;
	int v = 1;

	while (temp != NULL)
	{
		// temp->mynum : ���� ��� ��ȣ
		// temp->next : ���� ���
		// temp->price : �뾵��

		if (color[temp->mynum] == WHITE)
		{
			p[temp->mynum] = u;
			DFS_visit(temp->mynum);
		}

		temp = temp->next;
	}

	color[u] = BLACK;
	insertLinked(u);
}

void DFS()
{
	for (int u = 1; u <= N; u++)
	{
		color[u] = WHITE;
		p[u] = -1;
	}
	for (int u = 1; u <= N; u++)
	{
		if (color[u] == WHITE)
			DFS_visit(u);
	}
}

void BFS(int s)
{
	for (int u = 1; u < N; u++)
	{
		color[u] = WHITE;
		d[u] = MAX;
		p[u] = -1;
	}
	color[s] = GRAY;
	d[s] = 0;
	p[s] = -1;

	s1.inqueue(1);


	while (!s1.isEmpty())
	{
		int u = s1.dequeue();
		Node* temp = vertex[u]->next;

		while (temp != NULL)
		{
			if (color[temp->mynum] == WHITE)
			{
				color[temp->mynum] = GRAY;
				d[temp->mynum] = d[u] + 1;
				p[temp->mynum] = u;

				s1.inqueue(temp->mynum);
			}
			temp = temp->next;
		}
		u = s1.dequeue();
		color[u] = BLACK;
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

	DFS();


	printList();

	printLinked();

	cin >> p[1];
}