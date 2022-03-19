#include <iostream>
#include <memory>
using namespace std;
//unique_ptr �� C++ 11 �ṩ�����ڷ�ֹ�ڴ�й©������ָ���е�һ��ʵ�֣������������ָ������Ȩ������ָ�롣
/*
unique_ptr����ʼ���ǹ�����ԭʼָ���Ψһ�����ߡ������޷�����unique_ptr������ֻ���ƶ���
����ÿ��unique_ptr������ԭʼָ���Ψһ�����ߣ��������������������ֱ��ɾ��������ָ�룬����Ҫ�κβο�������
*/
#include <iostream>
#include <memory>

struct Task 
{
	int mId;
	Task(int id) :mId(id) 
	{
		std::cout << "Task::Constructor" << std::endl;
	}
	~Task() 
	{
		std::cout << "Task::Destructor" << std::endl;
	}
};

int main2()
{
	// �ն��� unique_ptr
	std::unique_ptr<int> ptr1;

	// ��� ptr1 �Ƿ�Ϊ��
	if (!ptr1)
		std::cout << "ptr1 is empty" << std::endl;

	// ��� ptr1 �Ƿ�Ϊ��
	if (ptr1 == nullptr)
		std::cout << "ptr1 is empty" << std::endl;

	// ����ͨ����ֵ��ʼ��unique_ptr
	// std::unique_ptr<Task> taskPtr2 = new Task(); // Compile Error

	// ͨ��ԭʼָ�봴�� unique_ptr
	std::unique_ptr<Task> taskPtr(new Task(23));

	// ��� taskPtr �Ƿ�Ϊ��
	if (taskPtr != nullptr)
		std::cout << "taskPtr is  not empty" << std::endl;

	// ���� unique_ptr����ָ��ĳ�Ա
	std::cout << taskPtr->mId << std::endl;

	std::cout << "Reset the taskPtr" << std::endl;
	// ���� unique_ptr Ϊ�գ���ɾ��������ԭʼָ��
	taskPtr.reset();

	// ����Ƿ�Ϊ�� / �����û�й�����ԭʼָ��
	if (taskPtr == nullptr)
		std::cout << "taskPtr is  empty" << std::endl;

	// ͨ��ԭʼָ�봴�� unique_ptr
	std::unique_ptr<Task> taskPtr2(new Task(55));

	if (taskPtr2 != nullptr)
		std::cout << "taskPtr2 is  not empty" << std::endl;

	// unique_ptr�����ܸ���
	//taskPtr = taskPtr2;											//compile error
	//std::unique_ptr<Task> taskPtr3 = taskPtr2;

	{
		// ת������Ȩ����unique_ptr�е�ָ��ת�Ƶ���һ��unique_ptr�У�
		std::unique_ptr<Task> taskPtr4 = std::move(taskPtr2);
		// ת�ƺ�Ϊ��
		if (taskPtr2 == nullptr)
			std::cout << "taskPtr2 is  empty" << std::endl;
		// ת������ǿ�
		if (taskPtr4 != nullptr)
			std::cout << "taskPtr4 is not empty" << std::endl;

		std::cout << taskPtr4->mId << std::endl;

		//taskPtr4 ��������������ŵ������ڽ�delete�������ָ��
	}

	std::unique_ptr<Task> taskPtr5(new Task(66));

	if (taskPtr5 != nullptr)
		std::cout << "taskPtr5 is not empty" << std::endl;

	// �ͷ�����Ȩ�������Ҫ�Լ�delete��
	Task * ptr = taskPtr5.release();

	if (taskPtr5 == nullptr)
		std::cout << "taskPtr5 is empty" << std::endl;

	std::cout << ptr->mId << std::endl;

	delete ptr;

	return 0;
}
