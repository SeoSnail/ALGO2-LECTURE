#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define MAX_MATRIX 100

using namespace std;

fstream Data;
int M[MAX_MATRIX][MAX_MATRIX];
int d[MAX_MATRIX];
int p[MAX_MATRIX][MAX_MATRIX];
int m;
int i, j, diagonal;
int min;
string fileName;

void print_order(int i, int j)
{
	int k;
	if (i == j)
		cout << i;
	else
	{
		k = p[i][j];
		cout << "(";
		print_order(i, k);
		print_order(k + 1, j);
		cout << ")";
	}
}

int main()
{

	cin >> fileName;

	Data.open(fileName);
	Data >> m;
	m--;
	for (i = 0; i <= m; i++)
	{
		Data >> d[i];
	}

	for (i = 1; i <= m; i++)
	{
		M[i][i] = 0;
		//p[i][i] = i;
	}


	for (diagonal = 1; diagonal < m; diagonal++)
	{
		for (i = 1; i <= m - diagonal; i++)
		{
			j = i + diagonal;
			min = M[i][i] + M[i + 1][j] + d[i - 1] * d[i] * d[j];
			p[i][j] = i;

			if (min > M[i][i] + M[i][j - 1] + d[i - 1] * d[j - 1] * d[j])
			{
				min = M[i][i] + M[i][j - 1] + d[i - 1] * d[j - 1] * d[j];
				p[i][j] = j - 1;
			}
			M[i][j] = min;
		}
	}
	/*
	for (i = 1; i <= m; i++)
	{
	for (j = 1; j <= m; j++)
	printf("%5d", M[i][j]);
	printf("\n");
	}
	*/
	for (i = 1; i <= m; i++)
	{
		for (j = 1; j <= m; j++)
			printf("%5d", p[i][j]);
		printf("\n");
	}

	print_order(1, m);

	cin >> M[0][0];
	return 0;
}