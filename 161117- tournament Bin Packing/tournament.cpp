#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>

int ttree[1000]; // tournament tree (index)
int leftSize[1000]; // 트리 리프의 값 즉 남은 bin의 사이즈
int M, binN, sizeN;


using namespace std;

int main()
{
	ifstream Data;
	string fileName;
	int amt;

	std::cin >> fileName;

	Data.open(fileName);

	Data >> binN >> M >> sizeN;

	
	/*
	for (int i = 0; i < binN; i++)
	{
		ttree[i + binN] = i + 1;
	}

	for (int i = binN - 1; i >= 1; i--)
	{
		ttree[i] = ttree[i*2];
	}
	*/

	//초기화 시작
	for (int i = 1; i <= binN; i++)
	{
		leftSize[i] = sizeN;
	}

	int cntT = 1;
	for (int i = binN/2; i <= binN; i++)
	{

		ttree[i] = cntT;

		cntT += 2;
	}
	for (int i = binN/2 - 1; i >= 1; i--)
	{
		ttree[i] = ttree[i * 2];
	}

	// 초기화 끝

	for (int i = 0; i < M; i++) // M : data의 수
	{
		Data >> amt;

		int j = 1;
		while (1) // j를 통해 순회, root 부터 큰지 작은지 보면서 말단까지간다.
		{
			// leaf 노드면 처리하고 종료
			if (binN / 2 <= j && j <= binN - 1) 
			{
				if (leftSize[2 * (j - binN / 2) + 1] >= amt)// 왼쪽 bin의 여분이 input data 보다 크면 왼쪽 bin의 여분을 깎는다.
					leftSize[2 * (j - binN / 2) + 1] -= amt;
				else if (leftSize[2 * (j - binN / 2) + 2] >= amt) // 오른쪽  "
					leftSize[2 * (j - binN / 2) + 2] -= amt;
				else // 둘다 input보다 작으면 그 다음 bin을 검사한다.
				{
					j++;
					continue;
				}

				//트리 갱신
				
				if (leftSize[2 * (j - binN / 2) + 1] >= leftSize[2 * (j - binN / 2) + 2]) //좌자식이 크거나 같으면
				{
					ttree[j] = 2 * (j - binN / 2) + 1; // left
				}
				else
				{
					ttree[j] = 2 * (j - binN / 2) + 2; // right
				}
				
				j /= 2;
				while (j != 0)
				{
					//좌자식 우자식의 leftSize 비교 좌자식이 크면 나를 좌자식 값으로 바꿈
					if (leftSize[ttree[j*2]] >= leftSize[ttree[j*2 + 1]])
					{
						ttree[j] = ttree[j * 2]; 
					}
					else
					{
						ttree[j] = ttree[j * 2 + 1];
					}
					j /= 2;
				}

				break;
			}

			if (leftSize[ttree[j]] >= amt)
			{
				j = j * 2 ; // left
			}
			else
			{
				j = j * 2 + 1;
			}

		}
		for (int i = 1; i <= binN; i++)
		{
			cout << leftSize[i] << " ";
		}
		cout << endl;
		
		for (int i = 1; i < binN; i++)
		{
			cout << ttree[i] << " ";
		}
		cout << endl;
		
		
		
		
	}
	for (int i = 1; i <= binN; i++)
	{
		cout << leftSize[i] << " ";
	}
	cout << endl;

	std::cin >> amt;
}