#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>

int ttree[1000]; // tournament tree (index)
int leftSize[1000]; // Ʈ�� ������ �� �� ���� bin�� ������
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

	//�ʱ�ȭ ����
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

	// �ʱ�ȭ ��

	for (int i = 0; i < M; i++) // M : data�� ��
	{
		Data >> amt;

		int j = 1;
		while (1) // j�� ���� ��ȸ, root ���� ū�� ������ ���鼭 ���ܱ�������.
		{
			// leaf ���� ó���ϰ� ����
			if (binN / 2 <= j && j <= binN - 1) 
			{
				if (leftSize[2 * (j - binN / 2) + 1] >= amt)// ���� bin�� ������ input data ���� ũ�� ���� bin�� ������ ��´�.
					leftSize[2 * (j - binN / 2) + 1] -= amt;
				else if (leftSize[2 * (j - binN / 2) + 2] >= amt) // ������  "
					leftSize[2 * (j - binN / 2) + 2] -= amt;
				else // �Ѵ� input���� ������ �� ���� bin�� �˻��Ѵ�.
				{
					j++;
					continue;
				}

				//Ʈ�� ����
				
				if (leftSize[2 * (j - binN / 2) + 1] >= leftSize[2 * (j - binN / 2) + 2]) //���ڽ��� ũ�ų� ������
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
					//���ڽ� ���ڽ��� leftSize �� ���ڽ��� ũ�� ���� ���ڽ� ������ �ٲ�
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