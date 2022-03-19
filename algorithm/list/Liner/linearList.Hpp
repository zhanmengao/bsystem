#include<iostream>
#include <string>
using namespace std;

template<class T>
class linearList
{
public:
	virtual bool isEmpty() = 0;;
	virtual int size() = 0;
	virtual int indexOf(T obj) = 0;
	virtual T get(int index) = 0;
	virtual void add(T &obj) = 0;
	virtual void add(T &obj, int index) = 0;
	virtual T remove() = 0;
	virtual T remove(int index) = 0;
	virtual T remove(T &obj) = 0;
protected:
	int length;
	T *mArr;
	//最后一个元素的位置
	int nowIndex;
};
template<class T>
class ArrayList :public linearList < T >
{
public:
	ArrayList(int length = 10)
	{
		this->length = length;
		mArr = new T[length];
		nowIndex = 0;
	}
	ArrayList(const ArrayList &obj)
	{
		//拷贝
		mArr = new T[obj.size()];
		length = obj.length;
		nowIndex = nowIndex;
		//拷贝值
		for (int i = 0; i <= nowIndex; i++)
		{
			mArr[i] = obj.mArr[i];
		}
	}
public:
	virtual bool isEmpty()
	{
		return nowIndex == 0;
	}
	virtual int size()
	{
		return nowIndex + 1;
	}
	virtual int indexOf(T obj)
	{
		//遍历
		for (int i = 0; i < length; i++)
		{
			if (obj == mArr[i])
			{
				return i;
			}
		}
		return -1;
	}
	virtual T get(int index)
	{
		return mArr[index];
	}
	virtual void add(T &obj)
	{
		//满检测
		if (isOver())
		{
			expansion();
		}
		//插入到尾部
		mArr[++nowIndex] = obj;
	}
	virtual void add(T &obj, int index)
	{
		//满检测
		if (isOver())
		{
			expansion();
		}
		//腾位置  memmove??
		for (int i = nowIndex; i >= index; i++)
		{
			mArr[i + 1] = mArr[i];
		}
		mArr[index] = obj;
	}
	virtual T remove()
	{
		T temp = mArr[nowIndex];
		mArr[nowIndex] = NULL;
		nowIndex--;
		return temp;
	}
	virtual T remove(int index)
	{
		if (index > nowIndex)
		{
			return NULL;
		}
		T temp = mArr[index];
		mArr[index] = NULL;
		for (int i = index; i <= nowIndex; i++)
		{
			mArr[index] = mArr[i + 1];
		}
		nowIndex--;
	}
	virtual T remove(T &obj)
	{
		return NULL;
	}
private:
	bool isOver()
	{
		return nowIndex == length - 1;
	}
	void expansion()
	{
		//扩充
		T *temp = new T[length * 2];
		//拷贝数据
		for (int i = 0; i < length; i++)
		{
			temp[i] = mArr[i];
		}
		delete[] mArr;
		length *= 2;
		mArr = temp;
	}
};