#pragma once
#include <iostream>
using namespace std;

void fun(char* p)
{
	cout << "char*" << endl;
}
void fun(int* p)
{
	cout << "int* p" << endl;
}
void fun(int p)
{
	cout << "int" << endl;
}

int main()
{
	fun(NULL);						//µ÷ÓÃint
	fun((char*)nullptr);			//char*
	fun((int*)nullptr);				//int*
	return 0;
}