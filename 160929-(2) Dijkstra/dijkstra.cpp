#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <fstream>

#define MAX 987654321

using namespace std;


int listN, M; // N : ��� ���� M : ���� ����
int heapN;
int p[1000]; // �θ�� ������
int d[1000]; // index ���� �ش��ϴ� ���� ����� �ٸ� �ּҰ�
bool flag[1000];
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� ����Ʈ ����

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

void insertList(int from, int to, int price)
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
	for (int i = 1; i <= listN; i++)
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

//// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѸ���Ʈ ��


//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� �� ����


Node arr[1000];
Node heapSort[1000];
// ����ȣ, price �ؽ�Ʈ�� ������ Node


// A��° �ε����� shift down
void shiftdown(int A)
{
	Node shift_key = arr[A];
	int parent = A;
	int smallChild;

	while (2 * parent <= heapN)
	{
		if ((2 * parent < heapN) && (arr[2 * parent].price < arr[2 * parent + 1].price))
			smallChild = 2 * parent;
		else smallChild = 2 * parent + 1;

		if (shift_key.price > arr[smallChild].price)
		{
			arr[parent] = arr[smallChild];
			parent = smallChild;
		}
		else break;
	}
	arr[parent] = shift_key;

}

// n���� ��� �� �����. ��Ʈ�� �� �ʿ� �����ϱ� �߰����� �ڷ�
void makeHeap(int n)
{
	for (int i = n / 2; i >= 1; i--)
	{
		shiftdown(i);
	}
}

// ��Ʈ pop
Node root()
{
	Node keyOut = arr[1];
	arr[1] = arr[heapN];
	heapN--;
	shiftdown(1);
	return keyOut;
}

//���� ������� heapSort[i]�� ����
void removeKeys()
{
	int cnt = heapN;

	for (int i = 1; i <= cnt; i++)
	{
		heapSort[i] = root();
	}
}


//int addNode�� ��ǲ
void insertHeap(Node addNode)
{
	heapN++;

	int child = heapN;
	if (heapN == 1)
		arr[heapN] = addNode;
	else
	{
		for (int i = heapN / 2; i >= 1; i = i / 2)
		{
			if (arr[i].price > addNode.price)
			{
				arr[child] = arr[i];
				child = i;
			}


		}

		arr[child] = addNode;
	}
}

//��,�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� �� ��


void MST_Prim(int r, int w) // r : ����ȣ  W: ����Ʈ
{
	d[1] = 0;
	p[1] = -1;
	Node temp;
	temp.mynum = 1;
	temp.price = 0;
	insertHeap(temp);

	while (heapN > 0)
	{
		Node u = root();

		if (flag[u.mynum] == 0)
		{
			Node* find = vertex[u.mynum]->next;

			int v;

			while (find != NULL)// next
			{
				v = find->mynum;
				if (flag[v] == 0 && find->price + d[u.mynum] < d[v]) // w(u, v) = u���� v���� weight
				{
					p[v] = u.mynum;
					d[v] = find->price + d[u.mynum];

					Node temp;
					temp.mynum = v;
					temp.price = d[v];

					insertHeap(temp);
				}

				find = find->next;
			}

			flag[u.mynum] = 1;


			for (int i = 1; i <= heapN; i++)
				cout << arr[i].price << " ";
			cout << endl;
		}
	}

}

int main()
{
	ifstream Data;
	string fileName;

	cin >> fileName;

	Data.open(fileName);

	Data >> listN >> M;

	vertex = (Front**)malloc((listN + 1) * sizeof(Front*));

	for (int i = 0; i <= listN; i++)
	{
		vertex[i] = (Front*)malloc(sizeof(Front));
	}

	for (int i = 1; i <= listN; i++)
	{
		vertex[i]->last = NULL;
		vertex[i]->next = NULL;
	}

	for (int i = 0; i < M; i++)
	{
		int from, to, price;
		Data >> from >> to >> price;

		insertList(from, to, price);
	}

	//printList();


	/*
	for (int i = 0; i <= N; i++)
	free(vertex[i]);
	free(vertex);
	*/

	for (int i = 0; i < 1000; i++)
		d[i] = MAX;
	p[1] = -1;


	int cnt = 0;

	MST_Prim(1, 0);


	// �Ʒ� ���

	for (int i = 1; i <= listN; i++)
	{
		cout << p[i] << " ";
	}
	cout << endl;

	for (int i = 1; i <= listN; i++)
	{
		cout << d[i] << " ";
	}
	cout << endl;


	cin >> p[1];

	/*
	while (1)
	{
	Node tempHeap2;
	tempHeap2 = root();
	int nowV = tempHeap2.mynum;

	if (cnt >= listN - 1)
	break;


	Node* findNode = vertex[nowV]->next;

	while (findNode != NULL)
	{
	Node temp = *(findNode);

	if (flag[temp.mynum] == true)
	{
	findNode = findNode->next;
	continue;
	}

	if()
	insertHeap(temp);

	findNode = findNode->next;
	cnt++;
	}
	flag[nowV] = true;

	for (int i = 1; arr[i].mynum != NULL; i++)
	{
	cout << arr[i].price << " ";
	}
	cout << endl;
	}
	*/



	int a;
	cin >> a;
}