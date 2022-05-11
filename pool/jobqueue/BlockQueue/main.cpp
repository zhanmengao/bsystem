#include <iostream>
#include <thread>
#include <stdio.h>
#include "BlockQueue.hpp"
using namespace std;

typedef BlockQueue<int> TQueue;

void produce(TQueue &queue)
{
	const int num = 9;
	for (int i = 0; i < num; ++i)
	{
		queue.push_back(i);
	}
}

void consume(TQueue &queue)
{
	while (false == queue.empty())
	{
		int tmp = queue.pop();
		printf("%d\n", tmp);
		std::this_thread::sleep_for(chrono::seconds(1));
	}
}

int main()
{
	TQueue queue(2);
	std::thread t1(produce, std::ref(queue));
	std::thread t2(consume, std::ref(queue));
	std::thread t3(consume, std::ref(queue));
	t3.join();
	t2.join();
	t1.join();
	return 0;
}