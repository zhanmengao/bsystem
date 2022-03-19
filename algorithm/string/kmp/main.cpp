#include"../stb/array.hpp"
#include"../Queue/Stack.h"
#include"../Queue/Queue.h"
#include"../stb/String.hpp"
using namespace stb;
using namespace std;


void mainArr()
{
	Array<int>  a1(10);

	for (int i = 0; i < a1.Length(); i++)
	{
		a1[i] = i;
	}

	cout << "\n打印数组a1: ";
	for (int i = 0; i < a1.Length(); i++)
	{
		cout << a1[i] << "	";
	}
	cout << endl;

	Array<int> a2 = a1;
	cout << "\n打印数组a2: ";
	for (int i = 0; i < a2.Length(); i++)
	{
		cout << a2[i] << " ";
	}
	cout << endl;

	//3
	Array<int> a3(5);
	{
		a3 = a1;
		a3 = a2 = a1;
		cout << "\n打印数组a3: ";
		for (int i = 0; i < a3.Length(); i++)
		{
			cout << a3[i] << " ";
		}
	}


	//功能4
	if (a3 == a1)
	{
		printf("a3 a1相等\n");
	}
	else
	{
		printf("a3 a1不相等\n");
	}

	Array<String> mStrArr(10);
	for (int i = 0; i < mStrArr.Length(); i++)
	{
		mStrArr[i] = "q";
	}
	Array<String> mStrArr2(mStrArr);
	for (int i = 0; i < mStrArr2.Length(); i++)
	{
		cout << mStrArr2[i] << " ";
	}
}
void mainStack()
{
	stb::Stack<String> stack;
	stack.Push("QQQ");
	stack.Push("WWW");
	stack.Push("EEE");
	while (!stack.Empty())
	{
		//cout << stack.Back() << endl;
		stack.Pop();
	}

	stb::Queue<String> queue;
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
		cout << arrQueue.Front() << endl;
		arrQueue.Pop();
	}

	stb::StackPlus<int> arrStack;
	for (int i = 0; i < 10; i++)
	{
		arrStack.Push(i);
	}
	while (!arrStack.Empty())
	{
		cout << arrStack.Front() << endl;
		arrStack.Pop();
	}
}

int main()
{
	mainStack();

	char str[] = "aa bb cc";
	Trim(str);
	printf("%s \n", str);
	return 0;
}