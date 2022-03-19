#include<iostream>
#include <string>
using namespace std;

int dianbing(int a, int b, int c)
{
	int x = 10;
	while (x <= 100)
	{
		if (x % 3 == a && x % 5 == b && x % 7 == c)
		{
			return x;
		}
		x++;
	}
	cout << "No answer"<<endl;
	return -1;
}
void main31212313()
{
	cout<<dianbing(2, 1, 3)<<endl;

}
