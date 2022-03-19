#include"Stack.h"
#include"Queue.h"
#include<string>
#include<iostream>

void mainStack()
{
	stb::Stack<std::string> stack;
	stack.Push("QQQ");
	stack.Push("WWW");
	stack.Push("EEE");
	while (!stack.Empty())
	{
		//cout << stack.Back() << endl;
		stack.Pop();
	}

	stb::Queue<std::string> queue;
	queue.Push("QQQ");
	queue.Push("WWW");
	queue.Push("EEE");
	while (!queue.Empty())
	{
		//cout << queue.Front() << endl;
		queue.Pop();
	}

	stb::QueuePlus<int> arrQueue;
	for (int i = 0; i < 10; i++)
	{
		arrQueue.Push(i);
	}
	while (!arrQueue.Empty())
	{
		std::cout << arrQueue.Front() << std::endl;
		arrQueue.Pop();
	}

	stb::StackPlus<int> arrStack;
	for (int i = 0; i < 10; i++)
	{
		arrStack.Push(i);
	}
	while (!arrStack.Empty())
	{
		std::cout << arrStack.Front() << std::endl;
		arrStack.Pop();
	}
}