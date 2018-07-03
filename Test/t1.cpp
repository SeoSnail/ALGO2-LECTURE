#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void init(int tree[], int player[], int bin_num, int bin_capacity); //tree,player 초기화
void insertToBin(int player[], int tree[], int bin_num, int &search_idx, int insert_num); //bin에 삽입
void update(int player[], int tree[], int bin_num, int &search_idx); //tree를 update
void printTreePlayer(int tree[], int plyaer[], int bin_num); //tree,flare 출력

double Log2(double n) //밑이 2인 로그값 구하기
{
	// log(n)/log(2) is log2.  
	return log(n) / log(double(2));
}

int main(void)
{
	ifstream fin;
	int bin_num, n, bin_capacity;
	int * tree;
	int * player;

	fin.open("b6.txt");
	fin >> bin_num >> n >> bin_capacity;

	tree = new int[bin_num]; //tree 1~bin_num-1 까지 공간 할당
	player = new int[bin_num + 1]; //player 1~bin_num 까지 공간 할당

	init(tree, player, bin_num, bin_capacity); //tree, flare 초기화

	int insert_num;
	int search_idx;
	
	printTreePlayer(tree, player, bin_num);

	for (int i = 0; i <n; i++) //input 개수 만큼 반복
	{
		fin >> insert_num;
		search_idx = 1;
		insertToBin(player, tree, bin_num, search_idx, insert_num); //insert_num을 Bin에 넣기(update과정 포함)
		printTreePlayer(tree, player, bin_num); //tree, flare 배열 출력
	}

	cin >> insert_num;
}

void init(int tree[], int player[], int bin_num, int bin_capacity)
{
	int p;
	int s = pow(2, ceil(Log2(double(bin_num)) - 1.0));
	int offset = 2 * s - 1;
	int LowExt = 2 * ((bin_num - 1) - (s - 1));

	//player 초기화
	for (int i = 1; i <= bin_num; i++)
	{
		player[i] = bin_capacity;
	}

	//tree 초기화
	if (bin_num % 2 == 0) //n이 짝수
	{
		for (int k = 1; k <= bin_num; k = k + 2) {
			if (k <= LowExt)
				p = (k + offset) / 2;
			else
				p = (k - LowExt + bin_num - 1) / 2;
			tree[p] = k;
		}
	}
	else { // n 이 홀수
		for (int k = 1; k <= LowExt; k = k + 2) {
			p = (k + offset) / 2;
			tree[p] = k;
		}
		for (int k = LowExt + 2; k <= bin_num; k = k + 2)
		{
			p = (k - LowExt + bin_num - 1) / 2;
			tree[p] = k;
		}
	}

	for (int k = (bin_num - 1) / 2; k > 0; k--) //나머지 tree 초기화
		tree[k] = tree[k * 2];
}

void insertToBin(int player[], int tree[], int bin_num, int &search_idx, int insert_num)
{
	int s = pow(2, (ceil(Log2(double(bin_num)) - 1.0)));
	int offset = 2 * s - 1;
	int LowExt = 2 * ((bin_num - 1) - (s - 1));
	int start_bin_idx = (2 * s - 1 + LowExt) / 2 + 1; //leaf node가 나타나기 시작하는 index번호

	while (1)
	{
		if (player[tree[search_idx]] >= insert_num) //현재 가리키는 위치에있는 bin의 용량이 insert_num보다 크거나 같으면
		{
			search_idx *= 2; //child노드 로 이동
			if (search_idx >= start_bin_idx  && search_idx <= (2 * s - 1) + LowExt)//leaf 노드에 도달하면
			{
				if (search_idx <= offset) //index번호가 offset 보다 작거나 같으면
				{
					if (player[search_idx + 1 - start_bin_idx + LowExt] >= insert_num) //왼쪽노드의 bin의 용량이 insert_num 보다 크거나 같으면
						player[search_idx + 1 - start_bin_idx + LowExt] = player[search_idx + 1 - start_bin_idx + LowExt] - insert_num; //왼쪽노드에 해당하는 bin의 용량을 감소
					else //왼쪽노드의 bin 의 용량이 insert_num 보다 작으면
					{
						player[search_idx + 1 - start_bin_idx + LowExt + 1] = player[search_idx + 1 - start_bin_idx + LowExt + 1] - insert_num; //오른쪽 노드에 해당하는 bin의 용량을 감소
						search_idx += 1; //오른쪽 노드로 index 이동
					}
				}
				else //offset 보다 크면
				{
					if (player[search_idx - offset] >= insert_num) //왼쪽노드의 bin의 용량이 insert_num 보다 크거나 같으면
						player[search_idx - offset] = player[search_idx - offset] - insert_num; //왼쪽노드에 해당하는 bin의 용량을 감소
					else //왼쪽노드의 bin 의 용량이 insert_num 보다 작으면
					{
						player[search_idx - offset + 1] = player[search_idx - offset + 1] - insert_num; //오른쪽 노드에 해당하는 bin의 용량을 감소
						search_idx += 1; //오른쪽 노드로 index 이동
					}
				}
				//tree를 업데이트
				update(player, tree, bin_num, search_idx);
				break; //update가 완료되었으니 while문을 탈출
			}
		}
		else //현재 가리키는 위치에 있는 bin의 용량이 insert_num보다 작으면
		{
			search_idx += 1; //인접노드로 이동
			if (search_idx == start_bin_idx)
			{
				player[search_idx - start_bin_idx + LowExt + 1] = player[search_idx - start_bin_idx + LowExt + 1] - insert_num;
				update(player, tree, bin_num, search_idx);
				break; //update가 완료되었으니 while문을 탈출
			}
		}

	}
}

void update(int player[], int tree[], int bin_num, int &search_idx)
{
	int s = pow(2, (ceil(Log2(double(bin_num)) - 1.0)));
	int offset = 2 * s - 1;
	int LowExt = 2 * ((bin_num - 1) - (s - 1));
	int start_bin_idx = (2 * s - 1 + LowExt) / 2 + 1;

	while (search_idx != 1) //root노드에 도달할때까지 반복
	{
		if (search_idx % 2 == 0) //index번호가 짝수이면
		{
			if (search_idx >= start_bin_idx) //leaf node이면서
			{
				if (search_idx > offset) //index번호가 offset(2s-1)보다 클때
				{
					if (player[search_idx - offset] >= player[search_idx - offset + 1]) //왼쪽노드의 용량이 크거나 같으면
					{
						tree[search_idx / 2] = search_idx - offset; //용량이 더 큰 빈의 번호를 저장
					}
					else //오른쪽노드의 용량이 더 크면
					{
						tree[search_idx / 2] = search_idx - offset + 1;
					}
				}
				else //index번호가 offset(2s-1)보다 작거나같을때
				{
					if (player[search_idx + 1 - start_bin_idx + LowExt] >= player[search_idx + 1 - start_bin_idx + LowExt + 1]) //왼쪽노드의 용량이 크거나 같으면
					{
						tree[search_idx / 2] = search_idx + 1 - start_bin_idx + LowExt; //용량이 더 큰 빈의 번호를 저장
					}
					else //오른쪽노드의 용량이 더 크면
					{
						tree[search_idx / 2] = search_idx + 1 - start_bin_idx + LowExt + 1;
					}
				}
			}
			else //start_bin_index보다 작을때
			{
				if (search_idx == start_bin_idx - 1) //start_bin_idx-1 인 경우에는
				{
					if (player[tree[search_idx]] >= player[search_idx + 2 - start_bin_idx + LowExt]) //왼쪽노드의 용량이 크거나 같으면
					{
						tree[search_idx / 2] = tree[search_idx];
					}
					else //오른쪽노드의용량이 더 크면
					{
						tree[search_idx / 2] = search_idx + 2 - start_bin_idx + LowExt;
					}
				}
				else
				{
					if (player[tree[search_idx]] >= player[tree[search_idx + 1]]) //왼쪽노드의 용량이 크거나 같으면
					{
						tree[search_idx / 2] = tree[search_idx];
					}
					else //오른쪽노드의 용량이 더 크면
					{
						tree[search_idx / 2] = tree[search_idx + 1];
					}
				}
			}
		}
		else //index번호가 홀수이면
		{
			if (search_idx >= start_bin_idx) //leaf node이면서
			{
				if (search_idx > offset) //index번호가 offset보다 클때
				{
					if (player[search_idx - offset - 1] >= player[search_idx - offset]) //왼쪽 노드가 오른쪽노드보다 크거나 같으면
					{
						tree[search_idx / 2] = search_idx - offset - 1;
					}
					else //오른쪽 노드가 더 클때
					{
						tree[search_idx / 2] = search_idx - offset;
					}
				}
				else //offset보다 작거나같을때
				{
					if (search_idx == start_bin_idx) //start_bin_idx 인 경우에는
					{
						if (player[tree[search_idx - start_bin_idx + LowExt]] >= player[search_idx + 1 - start_bin_idx + LowExt]) //왼쪽노드의 용량이 크거나 같으면
						{
							tree[search_idx / 2] = tree[search_idx - 1 - start_bin_idx + LowExt];
						}
						else //오른쪽 노드가 더 클때
						{
							tree[search_idx / 2] = search_idx + 1 - start_bin_idx + LowExt;

						}
					}
					else
					{
						if (player[search_idx + 1 - start_bin_idx + LowExt - 1] >= player[search_idx + 1 - start_bin_idx + LowExt]) //왼쪽 노드가 오른쪽노드보다 크거나 같으면
						{
							tree[search_idx / 2] = search_idx + 1 - start_bin_idx + LowExt - 1;
						}
						else //오른쪽 노드가 더클때
						{
							tree[search_idx / 2] = search_idx + 1 - start_bin_idx + LowExt;
						}
					}
				}
			}
			else //search_idx가 start_bin_idx보다 작을때
			{
				if (player[tree[search_idx - 1]] >= player[tree[search_idx]]) //왼쪽노드의 용량이 크거나 같으면
				{
					tree[search_idx / 2] = tree[search_idx - 1];
				}
				else
				{
					tree[search_idx / 2] = tree[search_idx];
				}
			}
		}
		search_idx /= 2; //부모노드로 이동
	}
}

void printTreePlayer(int tree[], int player[], int bin_num)
{
	//tree 출력
	cout << "tree : ";
	for (int i = 1; i <= bin_num - 1; i++)
	{
		cout << tree[i] << " ";
	}
	cout << endl;
	//flare 출력
	cout << "player : ";

	for (int i = 1; i <= bin_num; i++)
	{
		cout << player[i] << " ";
	}
	cout << endl << endl;;
}
