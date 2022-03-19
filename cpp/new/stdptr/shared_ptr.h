#pragma once
namespace stb
{
	template<typename T>
	class SharedPtr
	{
	public:
		SharedPtr(T* ptr = NULL) :_ptr(ptr), _pcount(new int(1))
		{}

		SharedPtr(const SharedPtr& s) :_ptr(s._ptr), _pcount(s._pcount)
		{
			*(_pcount)++;
		}

		SharedPtr<T>& operator=(const SharedPtr& s)
		{
			if (this != &s)
			{
				if (--(*(this->_pcount)) == 0)
				{
					delete this->_ptr;
					delete this->_pcount;
				}
				_ptr = s._ptr;
				_pcount = s._pcount;
				*(_pcount)++;
			}
			return *this;
		}
		T& operator*()
		{
			return *(this->_ptr);
		}
		T* operator->()
		{
			return this->_ptr;
		}
		~SharedPtr()
		{
			--(*(this->_pcount));
			if (this->_pcount == 0)
			{
				delete _ptr;
				_ptr = NULL;
				delete _pcount;
				_pcount = NULL;
			}
		}
	private:
		T* _ptr;
		int* _pcount;//指向引用计数的指针
	};
}