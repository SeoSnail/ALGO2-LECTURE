#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>

int tree[1000]; // tournament tree (index)
int player[1000]; // Ʈ�� ������ �� �� ���� bin�� ������
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

//2^n �� ������ �ƴ��� �Ǵ�.
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

		for( int k = LowExt +2 ; k <= binN ; k += 2) // LowExt +2 ���ʹ� 
		{
			//S-1 + S-(binN-LowExt) + i; i=1����.
			//2S-1 -(binN-LowExt) +2 = ������. 
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
	while (j != 1) //��Ʈ����
	{
		if (j % 2 == 0) //¦��
		{
			if (j >= turningPoint) // ����
			{
				if (j > offset) // ��� ���� 
				{	
					// ū���� ����
					if (player[j - offset] >= player[j - offset + 1]) 
					{
						tree[j / 2] = j - offset; 
					}
					else 
					{
						tree[j / 2] = j - offset + 1;
					}
				}
				else // �ϴ� ����
				{
					//ū���� ����
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
			else // ������ �ƴҶ�
			{
				if (j == turningPoint - 1) //turningPoint-1 �� ��쿡�� �� 
				{
					if (player[tree[j]] >= player[j + 2 - turningPoint + LowExt]) //���ʳ���� �뷮�� ũ�ų� ������
					{
						tree[j / 2] = tree[j];
					}
					else //�����ʳ���ǿ뷮�� �� ũ��
					{
						tree[j / 2] = j + 2 - turningPoint + LowExt;
					}
				}
				else
				{
					if (player[tree[j]] >= player[tree[j + 1]]) //���ʳ���� �뷮�� ũ�ų� ������
					{
						tree[j / 2] = tree[j];
					}
					else //�����ʳ���� �뷮�� �� ũ��
					{
						tree[j / 2] = tree[j + 1];
					}
				}
			}
		}
		else // Ȧ��
		{
			if (j >= turningPoint) //leaf node�̸鼭
			{
				if (j > offset) //index��ȣ�� offset���� Ŭ��
				{
					if (player[j - offset - 1] >= player[j - offset]) //���� ��尡 �����ʳ�庸�� ũ�ų� ������
					{
						tree[j / 2] = j - offset - 1;
					}
					else //������ ��尡 �� Ŭ��
					{
						tree[j / 2] = j - offset;
					}
				}
				else //offset���� �۰ų�������
				{
					if (j == turningPoint) //turningPoint �� ��쿡��
					{
						if (player[tree[j - turningPoint + LowExt]] >= player[j + 1 - turningPoint + LowExt]) //���ʳ���� �뷮�� ũ�ų� ������
						{
							tree[j / 2] = tree[j - 1 - turningPoint + LowExt];
						}
						else //������ ��尡 �� Ŭ��
						{
							tree[j / 2] = j + 1 - turningPoint + LowExt;

						}
					}
					else
					{
						if (player[j + 1 - turningPoint + LowExt - 1] >= player[j + 1 - turningPoint + LowExt]) //���� ��尡 �����ʳ�庸�� ũ�ų� ������
						{
							tree[j / 2] = j + 1 - turningPoint + LowExt - 1;
						}
						else //������ ��尡 ��Ŭ��
						{
							tree[j / 2] = j + 1 - turningPoint + LowExt;
						}
					}
				}
			}
			else //j�� turningPoint���� ������
			{
				if (player[tree[j - 1]] >= player[tree[j]]) //���ʳ���� �뷮�� ũ�ų� ������
				{
					tree[j / 2] = tree[j - 1];
				}
				else
				{
					tree[j / 2] = tree[j];
				}
			}
		}
		j /= 2; //�θ���� �̵�
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

	

	height = ceil(log2(binN)) -1 ; // ceil : �Ҽ��� �ø� ��ȯ.
	S = pow(2.0, height);
	offset = 2*S-1;
	LowExt = 2*( binN-S );
	branch = binN - 1; // ���׶�� ����
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

	for (int i = 0; i < M; i++) // M : data�� ��&
	{
		Data >> amt;

		int j = 1;
		while (1) // j�� ���� ��ȸ, root ���� ū�� ������ ���鼭 ���ܱ�������.
		{
			
			if (player[tree[j]] >= amt) // ���� tree���� �ش��ϴ� bin�� leftsize >= amt
			{
				j = j * 2; // �ڽ�����
				if (turningPoint <= j && j <= offset + LowExt) // ���ܿ� �����ϸ�
				{
					if (j <= offset) // offset ���� �۴ٴ°� LowExt�� �ִ� ���� �ƴ϶�°�
					{
						if (player[j - turningPoint + LowExt + 1] >= amt)// �� �ڽ��� leftSize�� ��ǲ���� ŭ 
							player[j - turningPoint + LowExt + 1] -= amt;
						else
						{
							player[j - turningPoint + LowExt + 2] -= amt;
							j++; // j�� ���ڽ����� �ٲ۴�.
						}
					}
					else // LowExt ���̸�
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
				j++; // �������� �Ѿ.

				if (j == turningPoint) //�Ѿ�µ� �������̶�� 
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