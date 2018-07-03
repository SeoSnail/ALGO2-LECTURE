#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX 987654321

using namespace std;


typedef struct queue_list {
	struct QueueNode *front;
	struct QueueNode *tail;
} queueList;

typedef struct QueueNode
{
	int vertex;
	struct QueueNode * next;
}QueueNode;

class Queue {
private:
	queueList * root;
public:
	Queue()
	{
		root = new queueList();
		root->front = NULL;
		root->tail = NULL;
	}
	void enqueue(int input)
	{
		QueueNode * newNode = new QueueNode();
		newNode->vertex = input;
		newNode->next = NULL;
		if (root->front == NULL && root->tail == NULL)
		{
			root->front = root->tail = newNode;
			return;
		}
		root->tail->next = newNode;
		root->tail = newNode;
	}
	void dequeue()
	{
		QueueNode * dequeue_node = root->front;

		if (root->front == NULL && root->tail == NULL)
		{
			cout << "Queue is Empty" << endl;
			return;
		}
		int output = root->front->vertex;

		if (root->front->next == NULL) //������ �Ѱ���尡 dequeue �ɶ�
		{
			root->front = NULL;
			root->tail = NULL;
			delete dequeue_node;
			return;
		}
		root->front = dequeue_node->next;
		delete dequeue_node;
	}
	int getFront()
	{
		return root->front->vertex;
	}
	bool isEmpty()
	{
		if (root->front == NULL && root->tail == NULL)
			return true;
		return false;
	}
};



typedef struct node
{
	int mynum; // ��� ��ȣ
	int capacity; // �뷮
	int flow; // �帣�� ��
	node* tween;
	node* next;

}Node;

typedef struct front
{
	Node* next;
	Node* last;
}Front;

//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� Ŭ����, ����ü ��

//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� ����

Front** vertex;
int N, M;
int color[500];
int p[500];
int d[500];
Queue s1;
int maxFlow =0;

//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� ���� ��


void insertNode(int from, int to, int capa)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	Node* find = vertex[from]->next;

	temp->mynum = to;
	temp->capacity = capa;
	temp->flow = 0;
	temp->next = NULL;
	temp->tween = NULL;


	if (vertex[from]->next == NULL) // ���� ���°� ������ 
	{
		vertex[from]->next = temp;
		vertex[from]->last = temp;
	}
	else
	{
		//		vertex[from]->last->next = temp;

				// ������ �׳� last�� �־�����
				// �̹� �����ϴ��� Ȯ���ϰ� �ִ´�.
		while (1)
		{
			if (find == NULL) // ������ ������ �� ���� ����.
			{
				vertex[from]->last->next = temp;

				vertex[from]->last = temp;
				break;
			}
			if (find->mynum == to) //�����Ѵٸ� temp capa���� �ٲ�.
			{
				find->capacity = temp->capacity;
				break;
			}
			find = find->next;
		}
	}
}


void setTween(int from, int to)
{
	Node* findFrom = vertex[from]->next;
	Node* findTo = vertex[to]->next;

	while (1)
	{
		if (findFrom->mynum == to)
			break;

		findFrom = findFrom->next;
	}

	while (findTo->mynum != from)
	{
		findTo = findTo->next;
	}

	findFrom->tween = findTo;
	findTo->tween = findFrom;
}

//vertex ���� print
void printList()
{
	for (int i = 0; i <= N + 1; i++)
	{
		Node* Print = vertex[i]->next;

		cout.width(3);
		cout << i << " : ";

		while (Print != NULL)
		{
			cout << "(" << Print->mynum << " , " << Print->capacity << ")" << " ";

			Print = Print->next;
		}
		cout << endl;
	}
}

//s ��°���� BFS�Ѵ�.
void BFS(int s)
{
	for (int u = 0; u <= N + 1; u++)
	{
		color[u] = WHITE;
		d[u] = MAX;
		p[u] = -1;
	}
	color[s] = GRAY;
	d[s] = 0;
	p[s] = -1;

	s1.enqueue(s);


	while (!s1.isEmpty())
	{
		int u = s1.getFront();
		s1.dequeue();
		Node* temp = vertex[u]->next;

		while (temp != NULL)
		{
			if ((color[temp->mynum] == WHITE) && (temp->capacity - temp->flow > 0))
			{
				color[temp->mynum] = GRAY;
				d[temp->mynum] = d[u] + 1;
				p[temp->mynum] = u;

				s1.enqueue(temp->mynum);
			}
			temp = temp->next;
		}
		color[u] = BLACK;
	}
}

// setFlow���� minFlow ���� ������ �־��� ���⼭ arr�� ������ �ְ� �����ؾߵȴ�.
// �׷��� minFlow�� ��ŭ flow�� �ø� �� �ִ�. 
void findFlow()
{
	Node* arr[1000];
	Node* find;
	int residualCapa = 987654321;


	while (1)
	{
		BFS(0);

		int now = N + 1; // ������ ��� 
		int before = p[N + 1]; // ��������尡 �� ��

		if (p[now] == -1)
			return;

		int temp = N + 1;
		
		for (int i = d[N+1] -1 ; i >= 0; i--)
		{
			Node* find = vertex[p[temp]]->next;

			while (1)
			{
				if (find->mynum == temp)
					break;
				find = find->next;
			}
			arr[i] = find;

			temp = p[temp];
		}

		//residual ã��
		for (int i = 0; i <= d[N+1] - 1; i++)
		{
			if ((arr[i]->capacity - arr[i]->flow) < residualCapa)
				residualCapa = arr[i]->capacity - arr[i]->flow;
		}// ĳ��-�÷� = ���� ������ �ִ� �÷�.. ���� min�� ã�°�

		//flow ����ŭ ����
		for (int i = 0; i <= d[N+1] - 1; i++)
		{
			arr[i]->flow = arr[i]->flow + residualCapa;
			arr[i]->tween->flow = 0 - arr[i]->flow;
		}

		//maxFlow�� ����
		maxFlow = maxFlow + residualCapa;
	}

}

int findTo(int from, int to)
{
	Node* find = vertex[to]->next;

	while (find != NULL)
	{
		if (find->mynum == from)
			return 1;
		find = find->next;
	}

	return 0;
}

int main()
{
	ifstream Data;
	string fileName;

	cin >> fileName;

	Data.open(fileName);

	Data >> N >> M;

	vertex = (Front**)malloc((N + 1 + 1) * sizeof(Front*));

	for (int i = 0; i <= N + 1; i++)
	{
		vertex[i] = (Front*)malloc(sizeof(Front));
	}

	for (int i = 0; i <= N + 1; i++)
	{
		vertex[i]->last = NULL;
		vertex[i]->next = NULL;

		color[i] = WHITE;
		p[i] = -1;
	}

	for (int i = 0; i < M; i++)
	{
		int from, to, capa;
		Data >> from >> to >> capa;
		
		
		insertNode(from, to, capa);
		
		if (!findTo(from, to)) // �������� �����餷��
			insertNode(to, from, 0);
		
		setTween(from, to);
	}

	//BFS(0);


	printList();

	/*
	for (int i = 0; i <= N + 1; i++)
		cout << p[i] << " ";
	cout << endl;
	*/
	findFlow();

	cout << endl << "�� : ";
	cout << maxFlow << endl;

	cin >> p[1];
}