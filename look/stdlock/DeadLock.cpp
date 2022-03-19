#include "Define.h"

//��������ͼ��һ���������������ڸò����ѱ��Ͳ����ʱ�������ǲ�����ġ�
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex middle_level_mutex(7000);
hierarchical_mutex low_level_mutex(5000);

int do_low_level_stuff()
{
	return 5000;
}
int low_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
	cout << "low_level_func" << endl;
	return do_low_level_stuff();
}

int middle_level_stuff(int some_param)
{
	return some_param;
}
int middle_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(middle_level_mutex);
	cout << "middle_level_func" << endl;
	return middle_level_stuff(low_level_func());
}

void high_level_stuff(int some_param){}
void high_level_func()
{
	cout << "high_level_func" << endl;
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
	high_level_stuff(middle_level_func());
}


void mainLock()
{
	high_level_func();
}

//��������𣬿��ܷ����
void hierarchical_mutex::check_for_hierarchy_violation()
{
	if (this_thread_hierarchy_value <= hierarchy_value)
	{
		throw std::logic_error("mutex hierarchy violated");
	}
}
//�����ɹ��󣬸��µ�ǰ������
void hierarchical_mutex::update_hierarchy_value()
{
	previous_hierarchy_value = this_thread_hierarchy_value;
	this_thread_hierarchy_value = hierarchy_value;
}

void hierarchical_mutex::lock()
{
	check_for_hierarchy_violation();
	internal_mutex.lock();
	update_hierarchy_value();
}
//��������ԭ������ǰ��������
void hierarchical_mutex::unlock()
{
	this_thread_hierarchy_value = previous_hierarchy_value;
	internal_mutex.unlock();
}
//���Լ���
bool hierarchical_mutex::try_lock()
{
	check_for_hierarchy_violation();
	if (!internal_mutex.try_lock())
		return false;
	update_hierarchy_value();
	return true;
}
unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);