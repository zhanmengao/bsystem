#include <iostream>       // std::cout
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
using namespace std;

thread::id main_thread_id =  this_thread::get_id(); //��ȡ���߳�id

void is_main_thread() 
{
	if (main_thread_id == this_thread::get_id())  //�ж��Ƿ�����߳�id��ͬ
		std::cout << "This is the main thread.\n";
	else
		std::cout << "This is not the main thread.\n";
}

int main() 
{
	is_main_thread(); // is_main_thread��Ϊmain�̵߳���ͨ��������
	thread th(is_main_thread); // is_main_thread��Ϊ�̺߳���ʹ��
	th.join(); //�ȴ�th����
	return 0;
}
