#include<iostream>
#include<vector>
#include<string>
using namespace std;

int main15()
{
	int bdd = 9;
	[bdd](const int& num){return bdd + num; };
	[bdd](const int& num)->int{return bdd + num; };
   return 0;
}