#pragma once
#include<vector>
#include<memory>
#include <stdexcept>
#include<string>
#include<iostream>
using namespace std;
class MyClass
{
public:
	MyClass()
	{
		cout << str << "		���캯������" << endl;
	}
	MyClass(const string& str) :str(str)
	{
		cout << str << "	���캯������" << endl;
	}
	MyClass(const MyClass& other) :str(other.str)
	{
		cout << str << "	��������" << endl;
	}
	void operator=(const MyClass& other)
	{
		str = other.str;
		cout << str << "	��ֵ������" << endl;
	}
	void operator=(const string& str)
	{
		this->str = str;
		cout << str << "	��ֵ������" << endl;
	}
	~MyClass()
	{
		cout << str << "		������������" << endl;
	}
	bool empty() const
	{
		return str.empty();
	}
	string& operator+=(const string& rStr)
	{
		str += rStr;
		return str;
	}
	friend ostream& operator<<(ostream& os, const MyClass& obj)
	{
		os << obj.str;
		return cout;
	}
	//�õ���ƣ�����ģʽ����������ָ�룬����ȷ��������Ķ������ǡ����ʱ�̱��ͷ�
	static shared_ptr<MyClass> factory(const string& str)
	{
		return make_shared<MyClass>(str);
	}
	//error:��֧���Զ�ת��
	//static shared_ptr<MyClass> factory(const string& str)
	//{
	//	return new MyClass(str);
	//}
	static shared_ptr<MyClass> Clone(const MyClass& obj)
	{
		return make_shared<MyClass>(obj.str);
	}
	//����shared_ptr�Ŀ�����������++
	static void ProcessShared(shared_ptr<MyClass> ptr)
	{
		//ִ�н�����������--;
		cout << "Process	 shared		" << ptr->str << endl;
		return;
	}
	static void ProcessPtr(MyClass* ptr)
	{
		cout << "Process	 ptr	" << ptr->str << endl;
	}
	//����һ���µķ��أ����
	static unique_ptr<MyClass> UClone(const MyClass& obj)
	{
		return unique_ptr<MyClass>(new MyClass(obj));
	}
private:
	string str;
};
template<class T>
class Blob
{
public:
	typedef std::vector<T> point_type;
	Blob() :rData(std::make_shared<point_type>())
	{

	}
    Blob(std::initializer_list<T> il) :rData(std::make_shared<point_type>(il))
	{

	}
	unsigned size() const
	{
		return rData->size();
	}
	bool empty() const
	{
		return rData->empty();
	}
	void push_back(const T& obj)
	{
		rData->push_back(obj);
	}
	void pop_back()
	{
		check(0, "pop_back() on empty blod!");
		return rData->pop_back();
	}
	T& front()
	{
		check(0, "front() on empty blod!");
		return rData->front();
	}
	T& back()
	{
		check(0, "back() on empty blod!");
		return rData->back();
	}
	const T& front() const
	{
		check(0, "front() on empty blod!");
		return rData->front();
	}
	const T& back() const
	{
		check(0, "back() on empty blod!");
		return rData->back();
	}
	T& operator[](unsigned index)
	{
		check(index, "operator[] error!");
		return rData->at(index);
	}
	unsigned use_count() const
	{
		return rData.use_count();
	}
private:
	void check(unsigned index, const std::string& str) const
	{
		if (index < 0 || index >= rData->size())
		{
			throw std::out_of_range(str);
		}
	}
private:
	//ָ��vector������ָ�룬ʵ��vector����
	std::shared_ptr<point_type> rData;
};


void mainBlob()
{
	Blob<string> mBlob;
	mBlob.push_back("sssss");
	Blob<string> mBlob2(mBlob);						//�������죬���ü���++
	cout << mBlob.use_count() << endl;
	mBlob.push_back("bbbbb");
	for (int i = 0; i < mBlob2.size(); i++)
	{
		cout << mBlob2[i] << endl;
	}
}
