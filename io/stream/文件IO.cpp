#include<iostream>
#include <string>
#include <fstream>
using namespace std;

void main93923213()
{
	char *fname = "c:/2.txt";
	ofstream fout(fname);//�����
	fout << "hello Perfect" << endl;
	fout.close();

	//��һ��������ܵ�
	ifstream fin(fname);					//��������������
	char ch;
	while (fin.get(ch))
	{
		cout << ch;
	}
	fin.close();
}