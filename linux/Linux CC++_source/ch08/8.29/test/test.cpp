
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::yield
#include <atomic>         // std::atomic
using namespace std;

atomic<bool> ready(false);

void thfunc(int id) 
{
	while (!ready) //һֱ�ȴ���֪��main�߳�������ȫ�ֱ���ready            
		 this_thread::yield();
 
	for (volatile int i = 0; i < 1000000; ++i) 
	{}
	 cout << id<<",";//�������������������ȴ�ӡ
}

int main()
{
	 thread threads[10];
	 cout << "race of 10 threads that count to 1 million:\n";
	for (int i = 0; i < 10; ++i) 
		threads[i] = thread(thfunc, i);
	ready = true;                 
	for (auto& th : threads) th.join();
	 cout << '\n';

	return 0;
}