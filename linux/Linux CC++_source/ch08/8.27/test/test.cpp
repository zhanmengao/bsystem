#include <iostream>  
#include <thread>  
 
using namespace std;  
   
void fun(int & n)  //�̺߳���
{  
	cout << "fun: " << n << "\n";  
	n += 20;  
	this_thread::sleep_for(chrono::milliseconds(10));   //�ȴ�10����
}  
int main()  
{  
	int n = 0;  
	
	cout << "n=" << n << '\n';  
	n = 10;  
 thread t1(fun, ref(n));   //ref(n)��ȡn������
	 thread t2(move(t1));     //t2ִ��fun��t1����thread����
	 t2.join();  //�ȴ�t2ִ�����
	cout << "n=" << n << '\n';  
	return 0;  
}  
