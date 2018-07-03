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

		if (root->front->next == NULL) //마지막 한개노드가 dequeue 될때
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
	int mynum; // 노드 번호
	int capacity; // 용량
	int flow; // 흐르는 량
	node* tween;
	node* next;

}Node;

typedef struct front
{
	Node* next;
	Node* last;
}Front;

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 클래스, 구조체 끝

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 전역

Front** vertex;
int N, M;
int color[5000];
int p[5000];
int d[5000];
Queue s1;
int maxFlow = 0;
int matSize;


int from1[10000];
int to1[10000]; //ecape용 인풋 저장소
int cnt1 = 0; // ecape용 인풋 갯수
int num_esc = 0; // 탈출 해야될 친구 몇명?
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 전역 끝


void insertNode(int from, int to, int capa)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	Node* find = vertex[from]->next;

	temp->mynum = to;
	temp->capacity = capa;
	temp->flow = 0;
	temp->next = NULL;
	temp->tween = NULL;


	if (vertex[from]->next == NULL) // 아직 들어온게 없으면 
	{
		vertex[from]->next = temp;
		vertex[from]->last = temp;
	}
	else
	{
		//		vertex[from]->last->next = temp;

		// 원래는 그냥 last에 넣었지만
		// 이미 존재하는지 확인하고 넣는다.
		while (1)
		{
			if (find == NULL) // 끝까지 없으면 맨 끝에 삽입.
			{
				vertex[from]->last->next = temp;

				vertex[from]->last = temp;
				break;
			}
			if (find->mynum == to) //존재한다면 temp capa값만 바꿈.
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

//vertex 전부 print
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

//s 번째부터 BFS한다.
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

void findFlow()
{
	Node* arr[1000];
	Node* find;
	int residualCapa = 987654321;


	while (1)
	{
		BFS(0);

		int now = N + 1; // 마지막 노드 
		int before = p[N + 1]; // 마지막노드가 갈 곳

		if (p[now] == -1)
			return;

		int temp = N + 1;

		for (int i = d[N + 1] - 1; i >= 0; i--)
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

		//residual 찾기
		for (int i = 0; i <= d[N + 1] - 1; i++)
		{
			if ((arr[i]->capacity - arr[i]->flow) < residualCapa)
				residualCapa = arr[i]->capacity - arr[i]->flow;
		}// 캐파-플로 = 현재 가능한 최대 플로.. 그중 min값 찾는거

		 //flow 값만큼 빼기
		for (int i = 0; i <= d[N + 1] - 1; i++)
		{
			arr[i]->flow = arr[i]->flow + residualCapa;
			arr[i]->tween->flow = 0 - arr[i]->flow;
		}

		//maxFlow값 증가
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

	Data >> matSize;

	for (int i = 0; i < matSize; i++)
	{
		for (int j = 0; j < matSize; j++)
		{
			int left = i * 2 * matSize + (j * 2) + 1;
			int right = i * 2 * matSize + (j * 2) + 2;
			int line = 2 * matSize; // 한 줄의 번호 차이.
			int near = 2;
			int fin = matSize*matSize * 2 + 1;
			char temp;
			Data >> temp;


			//기본적으로 왼쪽은 오른쪽만 가리키고 있지
			from1[cnt1] = left;
			to1[cnt1] = right;
			cnt1++;

			if (temp == '1') // 탈출해야되는 친구면 start와 그 친구를 연결
			{
				from1[cnt1] = 0;
				to1[cnt1] = left;
				cnt1++;
				num_esc++;
			}

			if (i != 0) // 상단이 아니면 위쪽을 가리키면 되겟지
			{
				from1[cnt1] = right;
				to1[cnt1] = left - line;
				cnt1++;
			}
			if (i != matSize - 1) // 하단이 아니면 아래쪽을 가리켜야겟지
			{
				from1[cnt1] = right;
				to1[cnt1] = left + line;
				cnt1++;
			}
			if (j != 0) // 좌측이 아니라면 좌측을 가리켜
			{
				from1[cnt1] = right;
				to1[cnt1] = left - near;
				cnt1++;
			}
			if (j != matSize - 1) // 우측이 아니라면 우측을
			{
				from1[cnt1] = right;
				to1[cnt1] = left + near;
				cnt1++;
			}

			if ((i == 0) || (i == matSize - 1) || (j == 0) || (j == matSize - 1)) // 테두리면 fin 으로 간다
			{
				from1[cnt1] = right;
				to1[cnt1] = fin;
				cnt1++;
			}
		}
	}
	/*
	for (int i = 0; i < cnt1; i++)
	{
		cout << from1[i] << " " << to1[i] <<  " 1" << endl;
	}
	*/
	//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 변환 완료

	//cin >> N;

	//Data >> N >> M;
	N = matSize*matSize * 2;
	M = cnt1;


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
		//Data >> from >> to >> capa;
		from = from1[i];
		to = to1[i];
		capa = 1;

		insertNode(from, to, capa);

		if (!findTo(from, to)) // 존재하지 않으면ㅇㅇ
			insertNode(to, from, 0);

		setTween(from, to);
	}

	//BFS(0);


	printList();

	findFlow();

	cout << endl << "maxFlow 값 : ";
	cout << maxFlow << endl;
	cout << "시작 노드 갯수 : ";
	cout << num_esc << endl;

	if (maxFlow == num_esc)
		cout << "탈출 성공!" << endl;
	else
		cout << "실. 패 " << endl;
	cin >> p[1];
}