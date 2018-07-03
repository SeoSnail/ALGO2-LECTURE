#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;


int p[1000];
int d[1000];
int length[1000];
int w[1000];
int maxLen;
int cnt;
int n;

void bellmanFord()
{
	int localLength = 0;

	for (int i = 0; i <= n; i++)
	{
		for (int j = i+1; j <= n; j++)
		{
			localLength += length[j];

			if (localLength > maxLen)
				break;

			if (d[j] > d[i] + w[j])
			{
				d[j] = d[i] + w[j];
				p[j] = i;
			}
		}
		localLength = 0;

	}
}

void print_order(int now)
{
	if (now == 0 | now == -1)
		return;

	print_order(p[now]);

	cout << now << " ";
}

int main()
{
	fstream Data;
	string fileName;

	cin >> fileName;

	Data.open(fileName);

	Data >> maxLen >> n;

	d[0] = 0;
	w[0] = 0;
	p[0] = -1;

	for (int i = 1; i <= n; i++)
		d[i] = 987654321;
	for (int i = 1; i <= n+1; i++)
		Data >> length[i];
	for (int i = 1; i <= n+1; i++)
		Data >> w[i];

	bellmanFord();


	for (int x = 0; x <= n; x++)
		cout << d[x] << " ";
	cout << endl << endl;

	cout << d[n] << endl;

	int now = n;
	while (1)
	{
		now = p[now];
		cnt++;

		if (now == 0 || now == -1)
			break;
	}

	cout << cnt << endl;
	print_order(n);


	cin >> d[0];
}