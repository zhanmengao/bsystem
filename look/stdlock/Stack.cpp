#include "Define.h"

/*
���ںü����������ʹ�����Ѿ�ʹ���˻����������ݻ��Ǳ��ƻ��ˡ�
�ٽ����������ݱ�¶��������������֮��
�ڱ��������ݵķ��ʽӿڱ���ʹ��ھ�̬����������������
*/
MutexTest mu;
std::stack<char> *pData;
//�ⲿ�õ�����Ҫ���������ݵ�����
void inject(std::stack<char> &data)
{
	pData = &data;
}
// ��ʹprocessû����ʽ���������ǻ��Ǳ�inject����
// processִ�����pData��������������·�������
void Test()
{
	//��������ִ�����û�����ĺ����������������ݴ��ݵ�������������֮��
	mu.process(inject);
	std::stack<char> tmpStack;
	for (int i = 0; pData->size(); ++i)
	{
		auto &data = pData->top();
		data = 'a';
		pData->pop();
		tmpStack.push(data);
	}
	for (int i = 0; tmpStack.size(); ++i)
	{
		auto &data = tmpStack.top();
		pData->push(data);
		tmpStack.pop();
	}
}

//�̰߳�ȫ�ĺ���
std::mutex stackMutex;
std::stack<char> intStack;
void SafeProcess()
{
	//��ʹtop��pop�Ѿ���ԭ�Ӳ�����������Ҫ��������Ϊ���������ݵķ��ʽӿڱ���ʹ��ھ�̬����
	std::lock_guard<std::mutex> guard(stackMutex);       
	if (!intStack.empty())
	{
		const int value = intStack.top();
		intStack.pop();
	}
}
void SafeMain()
{
	std::thread mutexTestThread1(Test);
	std::thread mutexTestThread2(Test);

	std::thread t1(SafeProcess);
	std::thread t2(SafeProcess);
	mutexTestThread1.join();
	mutexTestThread2.join();
	t1.join();
	t2.join();
}