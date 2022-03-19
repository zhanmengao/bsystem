#pragma once
#define __DEBUG 1
namespace stb
{
	template<class T>
	class LinearList
	{
	public:
		typedef T value_type;
		typedef T* TP;
		virtual bool Empty() const = 0;
		virtual unsigned int Size() const  = 0 ;
		virtual int IndexOf(const T& obj) const  = 0;
		virtual T& operator[](unsigned int index) = 0;
		virtual void PushBack(const T &obj) = 0;
		virtual void Clear() = 0;
		virtual void RemoveAt(unsigned int index) = 0;
		virtual void Remove(const T &obj) = 0;
	};
}