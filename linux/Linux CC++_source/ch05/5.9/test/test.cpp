#include <unistd.h>  
using namespace std;
#include <iostream>

int main(int argc, char* argv[])
{
	execl("mytest", NULL); //�����κβ�����mytest
	cout << "------------------\n";//���execlִ�гɹ�����һ�䲻��ִ�е��ġ�
	
	return 0;
}
