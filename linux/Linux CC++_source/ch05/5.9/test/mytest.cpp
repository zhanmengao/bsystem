#include <string.h>
using namespace std;
#include <iostream>
 
int main(int argc, char* argv[])
{
	int i;
	cout <<"argc=" << argc << endl; //��ӡ�´������Ĳ�������
	
	for(i=0;i<argc;i++)   //��ӡ��������
		cout<<argv[i]<<endl;
}
