#include <iostream>       // std::cout
#include<mutex>
/*
ֻҪ����������Ϊ�������ݸ�std::lock()��std::lock()���ܹ���ס�����������std::lock()��δָ��������������˳�����ܹ���֤��

std::lock()ִ�гɹ�ʱ�����л��������Ѿ�������������û����������
std::lock()ִ��ʧ��ʱ���ѱ��������Ļ��������ᱻ����
*/
/*
lock_guard��
��������֤���̼߳��ͬ��������ȴ�����в�������˴��в�������������кܴ��Ӱ�죬��������Ҫ�����ܵļ�С����������Ҳ����ʹ��ϸ��������

��һ��lock_guard���Ĳ��ã�������lock_guardֻ�ܱ�֤��������ʱ��ִ�н���������lock_guard����û���ṩ�����ͽ����Ľӿڣ�
������Щʱ�������������
*/
namespace Lock
{
	class some_big_object
	{
	public:
		some_big_object(int a) :x(a) {}
		void Print(){ std::cout << x << std::endl; }
	private:
		int x;
	};
	class TBigObj
	{
	private:
		some_big_object& some_detail;
		std::mutex m;
	public:
		TBigObj(some_big_object & sd) :some_detail(sd){}
		//std::lock()������ס������������std::lock_guard����unlock���������������������std::lock_guard()����Ҫ�ֶ�unlock()��
		//std::adopt_lock������ʾ�������Ѿ���������������ǲ����ظ�������
		friend void swap(TBigObj& lhs, TBigObj& rhs)
		{
			if (&lhs == &rhs)
				return;
			std::lock(lhs.m, rhs.m);             //ԭ�Ӳ�����ͳͳ����
			std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);         //��lock_guard�����������Զ�����
			std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
			std::swap(lhs.some_detail, rhs.some_detail);
		}
	};

	template<class T>
	void swap(T& lhs, T& rhs);

	template<>
	void swap<some_big_object>(some_big_object &x, some_big_object &y)
	{
		TBigObj a(x), b(y);
		swap(a, b);       //����TObj��swap
	}
}


int mainStdLock()
{
	Lock::some_big_object a(1), b(2);
	a.Print(), b.Print();
	Lock::swap<Lock::some_big_object>(a, b);       //����someBigObj��swap
	a.Print(), b.Print();
	return 0;
}


