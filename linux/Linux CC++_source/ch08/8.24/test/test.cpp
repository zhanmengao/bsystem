#include <iostream>  
#include <thread>  
using namespace std;  
   
typedef struct  //����ṹ�������
{
	int n;
	const char *str;
}MYSTRUCT;

void thfunc(void *arg)  //�̺߳���
{  
	MYSTRUCT *p = (MYSTRUCT*)arg;
	cout << "in thfunc:n=" << p->n<<",str="<< p->str <<endl; //��ӡ�ṹ�������
}  
      
int main(int argc, char *argv[])  
{  
	MYSTRUCT mystruct; //����ṹ��
	//��ʼ���ṹ��
	mystruct.n = 110;
	mystruct.str = "hello world";

	thread t(thfunc, &mystruct);  //�����̶߳���t�������̺߳���ָ����̺߳�����������
	t.join();  //�ȴ��̶߳���t����
      
	return 0;  
}  
