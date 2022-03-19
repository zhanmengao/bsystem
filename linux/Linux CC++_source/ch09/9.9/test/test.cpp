#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex

volatile int counter(0);     //����һ��ȫ�ֱ��������������������ۼ�
std::mutex mtx;           // ���ڱ���counter�Ļ�����

void thrfunc() 
{
	for (int i = 0; i < 10000; ++i) 
	{
		if (mtx.try_lock())// ����������
		{
			++counter; //�������ۼ�
			mtx.unlock(); //����������
		}
		else std::cout << "try_lock false\n"  ;
	}
}

int main(int argc, const char* argv[]) 
{
	std::thread threads[10];
	
	for (int i = 0; i < 10; ++i)
		threads[i] = std::thread(thrfunc); //����10���߳�

	for (auto& th : threads) th.join(); //�ȴ�10���߳̽���
	std::cout << "count to " << counter << " successfully \n";

	return 0;
}
