#include<iostream>
#include <string>
#include <fstream>
using namespace std;

void main93923213()
{
	char *fname = "c:/2.txt";
	ofstream fout(fname);//输出流
	fout << "hello Perfect" << endl;
	fout.close();

	//建一个输出流管道
	ifstream fin(fname);					//建立输入流对象
	char ch;
	while (fin.get(ch))
	{
		cout << ch;
	}
	fin.close();
}