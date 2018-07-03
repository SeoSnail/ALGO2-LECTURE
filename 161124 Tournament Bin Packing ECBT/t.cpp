#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>

int tree[1000]; // tournament tree (index)
int player[1000]; // 트리 리프의 값 즉 남은 bin의 사이즈
int M, binN, sizeN;
int LowExt, S, height;
int offset, p;
int branch, turningPoint;

using namespace std;

// log() = log base e
// log10() = log base 10
// logx / log2 = log base 2

double log2(double n)
{
	return log10(n)/log10(2.0);
}

//2^n 인 수인지 아닌지 판단.
bool is2(int n)
{
	if( n == 1 || n == 0)
		return true;
	if(n%2 != 0)
		return false;
	
	return is2(n/2);
}

int max2n(int n)
{
	if (is2(n))
		return n;

	return max2n(--n);
}

void initTree()
{
	if(binN%2 == 0)
	{
		for(int i=1; i <= binN; i+=2)
		{
			if( i <= LowExt)
				p = (i+offset)/2;
			else
				p = (i-LowExt + binN -1)/2;

			tree[p] = i;
		}


	}
	else
	{
		for(int k = 1; k <= LowExt; k += 2) // 
		{
			p = (k+offset)/2;

			tree[p] = k;
		}

		for( int k = LowExt +2 ; k <= binN ; k += 2) // LowExt +2 부터는 
		{
			//S-1 + S-(binN-LowExt) + i; i=1부터.
			//2S-1 -(binN-LowExt) +2 = 시작점. 
			//offset - (binN-LowExt) + k -LowExt;
			//= offset - binN +k;
			p = (offset - binN + k)/2;

			tree[p] = k;
		}
	}

	for(int k = (binN-1)/2 ; k > 0 ; k--)
		tree[k] = tree[k*2];
}

void rebuildTree(int j)
{
	while (j != 1) //루트까지
	{
		if (j % 2 == 0) //짝수
		{
			if (j >= turningPoint) // 말단
			{
				if (j > offset) // 상단 말단 
				{	
					// 큰쪽을 저장
					if (player[j - offset] >= player[j - offset + 1]) 
					{
						tree[j / 2] = j - offset; 
					}
					else 
					{
						tree[j / 2] = j - offset + 1;
					}
				}
				else // 하단 말단
				{
					//큰쪽을 저장
					if (player[j + 1 - turningPoint + LowExt] >= player[j + 1 - turningPoint + LowExt + 1]) 
					{
						tree[j / 2] = j + 1 - turningPoint + LowExt; 
					}
					else
					{
						tree[j / 2] = j + 1 - turningPoint + LowExt + 1;
					}
				}
			}
			else // 말단이 아닐때
			{
				if (j == turningPoint - 1) //turningPoint-1 인 경우에는 즉 
				{
					if (player[tree[j]] >= player[j + 2 - turningPoint + LowExt]) //왼쪽노드의 용량이 크거나 같으면
					{
						tree[j / 2] = tree[j];
					}
					else //오른쪽노드의용량이 더 크면
					{
						tree[j / 2] = j + 2 - turningPoint + LowExt;
					}
				}
				else
				{
					if (player[tree[j]] >= player[tree[j + 1]]) //왼쪽노드의 용량이 크거나 같으면
					{
						tree[j / 2] = tree[j];
					}
					else //오른쪽노드의 용량이 더 크면
					{
						tree[j / 2] = tree[j + 1];
					}
				}
			}
		}
		else // 홀수
		{
			if (j >= turningPoint) //leaf node이면서
			{
				if (j > offset) //index번호가 offset보다 클때
				{
					if (player[j - offset - 1] >= player[j - offset]) //왼쪽 노드가 오른쪽노드보다 크거나 같으면
					{
						tree[j / 2] = j - offset - 1;
					}
					else //오른쪽 노드가 더 클때
					{
						tree[j / 2] = j - offset;
					}
				}
				else //offset보다 작거나같을때
				{
					if (j == turningPoint) //turningPoint 인 경우에는
					{
						if (player[tree[j - turningPoint + LowExt]] >= player[j + 1 - turningPoint + LowExt]) //왼쪽노드의 용량이 크거나 같으면
						{
							tree[j / 2] = tree[j - 1 - turningPoint + LowExt];
						}
						else //오른쪽 노드가 더 클때
						{
							tree[j / 2] = j + 1 - turningPoint + LowExt;

						}
					}
					else
					{
						if (player[j + 1 - turningPoint + LowExt - 1] >= player[j + 1 - turningPoint + LowExt]) //왼쪽 노드가 오른쪽노드보다 크거나 같으면
						{
							tree[j / 2] = j + 1 - turningPoint + LowExt - 1;
						}
						else //오른쪽 노드가 더클때
						{
							tree[j / 2] = j + 1 - turningPoint + LowExt;
						}
					}
				}
			}
			else //j가 turningPoint보다 작을때
			{
				if (player[tree[j - 1]] >= player[tree[j]]) //왼쪽노드의 용량이 크거나 같으면
				{
					tree[j / 2] = tree[j - 1];
				}
				else
				{
					tree[j / 2] = tree[j];
				}
			}
		}
		j /= 2; //부모노드로 이동
	}
}

int main()
{
	ifstream Data;
	string fileName;
	int amt;

	std::cin >> fileName;

	Data.open(fileName);

	Data >> binN >> M >> sizeN; //

	

	height = ceil(log2(binN)) -1 ; // ceil : 소수점 올림 반환.
	S = pow(2.0, height);
	offset = 2*S-1;
	LowExt = 2*( binN-S );
	branch = binN - 1; // 동그라미 갯수
	turningPoint = (offset + LowExt) / 2 + 1;

//	cout << offset << endl;
//	cout << height << " "<< S << " " <<  LowExt << endl;

	for (int i = 1; i <= binN; i++)
	{
		player[i] = sizeN;
	}

	initTree();

	/*
	for(int i=0; i<= offset; i++)
	{
		cout << tree[i] << " ";
	}
	*/

	for (int i = 0; i < M; i++) // M : data의 수&
	{
		Data >> amt;

		int j = 1;
		while (1) // j를 통해 순회, root 부터 큰지 작은지 보면서 말단까지간다.
		{
			
			if (player[tree[j]] >= amt) // 현재 tree값에 해당하는 bin의 leftsize >= amt
			{
				j = j * 2; // 자식으로
				if (turningPoint <= j && j <= offset + LowExt) // 말단에 도착하면
				{
					if (j <= offset) // offset 보다 작다는건 LowExt가 있는 쪽이 아니라는거
					{
						if (player[j - turningPoint + LowExt + 1] >= amt)// 좌 자식의 leftSize가 인풋보다 큼 
							player[j - turningPoint + LowExt + 1] -= amt;
						else
						{
							player[j - turningPoint + LowExt + 2] -= amt;
							j++; // j를 우자식으로 바꾼다.
						}
					}
					else // LowExt 쪽이면
					{
						if (player[j - offset] >= amt)
							player[j - offset] -= amt;
						else
						{
							player[j - offset + 1] -= amt;
							j++;
						}
					}
					rebuildTree(j);

					break;
				}

			}
			else
			{
				j++; // 우측으로 넘어감.

				if (j == turningPoint) //넘어갔는데 시작점이라면 
				{
					player[j - turningPoint + LowExt + 1] -= amt;
					
					rebuildTree(j);
					break; 
				}
			}


		}
		/*
		for (int i = 1; i < binN; i++)
		{
			cout << tree[i] << " ";
		}
		cout << endl;
		*/
		for (int i = 1; i <= binN; i++)
		{
			cout << player[i] << " ";
		}
		cout << endl;
	
		
	}
	for (int i = 1; i <= binN; i++)
	{
		cout << player[i] << " ";
	}
	cout << endl;

	cin >> player[0];
}