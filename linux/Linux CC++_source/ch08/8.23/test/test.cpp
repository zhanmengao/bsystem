#include <iostream>  
#include <thread>  
using namespace std;  
      
void thfunc(char *s)  //�̺߳���
{  
	cout << "thfunc: " <<s << "\n";   //����s����boy and girl
}  
      
int main(int argc, char *argv[])  
{  
	char s[] = "boy and girl"; //����һ���ַ���
	thread t(thfunc,s);  //�����̶߳��󣬲������ַ���s
	t.join();   //�ȴ�tִ�н���
      
	return 0;  
}  
