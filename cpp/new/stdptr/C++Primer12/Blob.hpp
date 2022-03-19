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
		cout << str << "		构造函数调用" << endl;
	}
	MyClass(const string& str) :str(str)
	{
		cout << str << "	构造函数调用" << endl;
	}
	MyClass(const MyClass& other) :str(other.str)
	{
		cout << str << "	拷贝构造" << endl;
	}
	void operator=(const MyClass& other)
	{
		str = other.str;
		cout << str << "	赋值操作符" << endl;
	}
	void operator=(const string& str)
	{
		this->str = str;
		cout << str << "	赋值操作符" << endl;
	}
	~MyClass()
	{
		cout << str << "		析构函数调用" << endl;
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
	//好的设计：工厂模式返回其智能指针，可以确保它分配的对象会在恰当的时刻被释放
	static shared_ptr<MyClass> factory(const string& str)
	{
		return make_shared<MyClass>(str);
	}
	//error:不支持自动转换
	//static shared_ptr<MyClass> factory(const string& str)
	//{
	//	return new MyClass(str);
	//}
	static shared_ptr<MyClass> Clone(const MyClass& obj)
	{
		return make_shared<MyClass>(obj.str);
	}
	//发生shared_ptr的拷贝，引用数++
	static void ProcessShared(shared_ptr<MyClass> ptr)
	{
		//执行结束，引用数--;
		cout << "Process	 shared		" << ptr->str << endl;
		return;
	}
	static void ProcessPtr(MyClass* ptr)
	{
		cout << "Process	 ptr	" << ptr->str << endl;
	}
	//创建一个新的返回，深拷贝
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
	//指向vector的智能指针，实现vector共享
	std::shared_ptr<point_type> rData;
};


void mainBlob()
{
	Blob<string> mBlob;
	mBlob.push_back("sssss");
	Blob<string> mBlob2(mBlob);						//拷贝构造，引用计数++
	cout << mBlob.use_count() << endl;
	mBlob.push_back("bbbbb");
	for (int i = 0; i < mBlob2.size(); i++)
	{
		cout << mBlob2[i] << endl;
	}
}
