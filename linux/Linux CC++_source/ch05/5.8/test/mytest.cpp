#include <string.h>
using namespace std;
#include <iostream>
 
int main(int argc, char* argv[])
{
	int i;
	cout <<"argc=" << argc << endl; //��ӡ�´������Ĳ�������
	
	for(i=0;i<argc;i++)   //��ӡ��������
		cout<<argv[i]<<endl;
	 
	if (argc == 2&&strcmp(argv[1], "-p")==0)  //�ж��Ƿ�������˲���-p
		cout << "will print all" << endl;
	else 
		cout << "will print little" << endl;
	
	cout << "my program over" << endl;
	return 0;
}
