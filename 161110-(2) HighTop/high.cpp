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

	int height;
	int weight;
	int size;

	node* next;
}Node;

typedef struct front
{
	Node* next;
	Node* last;

	int size;
	int height;
	int weight;
}Front;


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 전역

Front** vertex;
int N, M;
int color[500];
int p[500];
int d[500];
stack s1;
int bottom[1000], height[1000], weight[1000];
int heightSum;

int topo[1000];
int cnt_topo;
int dp[1000];
int blockNum;
int before[1000];
int blockStack[1000];

Node* head = NULL;

// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 전역

void insert(int from, int to, int price)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	Node* find = vertex[from]->next;

	temp->mynum = to;
	temp->next = NULL;
	temp->price = price;
	temp->height = price;
	temp->weight = weight[to];
	temp->size = bottom[to];

	if (from == 0)
	{
		temp->height = 0;
		temp->size = 987654321;
		temp->weight = 987654321;
	}
	if (from == N + 1)
	{
		temp->height = 0;
		temp->size = 0;
		temp->weight = 0;
	}

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
	for (int i = 0; i <= N; i++)
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

void doTopo()
{
	Node* find = head;
	while (find != NULL)
	{
		topo[cnt_topo++] = find->mynum;
		before[find->mynum] = cnt_topo - 1;
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

		// temp->mynum : 현재 노드 번호
		// temp->next : 다음 노드

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
	for (int u = 0; u <= N; u++)
	{
		color[u] = WHITE;
		p[u] = -1;
	}
	for (int u = 0; u <= N; u++)
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

void topologiSort()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			if (bottom[i] > bottom[j] && weight[i] > weight[j])
			{
				insert(i, j, height[j]);
			}
		}
	}
}

int main()
{
	ifstream Data;
	string fileName;

	cin >> fileName;

	Data.open(fileName);

	Data >> N;

	vertex = (Front**)malloc((N + 1 +1) * sizeof(Front*));

	for (int i = 0; i <= N+1; i++)
	{
		vertex[i] = (Front*)malloc(sizeof(Front));
	}

	dp[0] = 0;
	for (int i = 1; i <= N + 1; i++)
	{
		dp[i] = -987654321;
	}

	for (int i = 1; i <= N; i++)
	{
		Data >> bottom[i] >> height[i] >> weight[i];
	}


	for (int i = 0; i <= N + 1; i++)
	{
		vertex[i]->last = NULL;
		vertex[i]->next = NULL;

		vertex[i]->size = bottom[i];
		vertex[i]->height = height[i];
		vertex[i]->weight = weight[i];

		color[i] = WHITE;
		p[i] = -1;
	}

	vertex[0]->size = 987654321;
	vertex[0]->height = 0;
	vertex[0]->weight = 987654321;



	topologiSort();

	for (int i = 1; i <= N; i++)
	{
		insert(0, i, height[i]);

		insert(i, N + 1, 0);

	}

	DFS();


	printLinked();
	cout << endl << endl;
	doTopo();


//	printList();
	//dp로 해야됨. 
	for (int i = 0; i <= N; i++)
	{
		Front* left = vertex[topo[i]];

		for (int j = i+1; j <= N+1; j++)
		{
			Front* right = vertex[topo[j]];

			if ((left->weight > right->weight) && (left->size > right->size))
			{
				if (dp[j] < dp[i] + right->height)
				{
					dp[j] = dp[i] + right->height;
					p[topo[j]] = topo[i];
				}
			}


		}

	}

	int x = N+1;
	/*
	for (int i = 0; i <= N+1; i++)
	{
		cout << p[i] << " ";
	}
	cout << endl;
	*/

	while (p[x] != 0)
	{
		blockNum++;

		x = p[x];
	}

	 x = N + 1;

	cout << blockNum << endl;
	while (p[x] != 0)
	{
		cout << p[x] << endl;

		x = p[x];
	}
	

	for (int i = 0; i <= N + 1; i++)
	{
		cout.width(3);
		cout << dp[i] << " ";
	}
	cout << endl;

	cout << dp[N + 1] << endl;

	cin >> p[1];
}