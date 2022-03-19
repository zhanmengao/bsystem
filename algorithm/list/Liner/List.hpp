#ifndef _LIST_HPP
#define _LIST_HPP
#pragma once
#include"LinerList.h"
#include<stdexcept>
#include<iostream>
namespace stb
{
	template<class T>
    class Node
	{
	public:
        Node<T>* pLeft = nullptr;
        Node<T>* pRight = nullptr;;
        T Data;
	};

	template<class T>
	class List :public LinearList<T>
	{
	public:
		typedef Node<T> node_type,*node_point;
		node_point pHead = nullptr;
		List()
		{
		}
		~List()
		{
			Clear();
		}
		node_point Begin()
		{
			return pHead;
		}
		node_point End()
		{
			if (Empty())
			{
				return nullptr;
			}
			return pHead->pLeft;
		}
		virtual bool Empty() const
		{
			return pHead == nullptr;
		}
		virtual unsigned int Size() const
		{
			if (Empty())
			{
				return 0;
			}
			unsigned int nSize = 0;
			node_point pNode = pHead;
			do
			{
				nSize++;
				pNode = pNode->pRight;
			} while (pNode != pHead);
			return nSize;
		}
		virtual int IndexOf(const T& obj) const
		{
			if (Empty())
			{
				return -1;
			}
			unsigned int nPos = 0;
			node_point pNode = pHead;
            while (pNode->Data != obj)
			{
				nPos++;
				pNode = pNode->pRight;
				if (pNode == pHead)
				{
					return -1;
				}
			}
			return nPos;
		}
		virtual T& operator[](unsigned int index)
		{
			if (Empty())
			{
				throw std::out_of_range("operator[]" + index);
			}
			node_point pNode = pHead;
			for (unsigned i = 0; i < index; i++)
			{
				pNode = pNode->pRight;
			}
            return pNode->Data;
		}
		virtual void PushBack(const T &obj)
		{
			node_point pN = new Node<T>();
            pN->Data = obj;
			if (pHead == nullptr)
			{
				pHead = pN;
				pHead->pRight = pN;
				pHead->pLeft = pN;
			}
			else
			{
				Node<T>* pLastBack = pHead->pLeft;
				pLastBack->pRight = pN;
				pN->pRight = pHead;

				pHead->pLeft = pN;
				pN->pLeft = pLastBack;
			}
		}
		void PushFront(const T& obj)
		{
			node_point pN = new Node<T>();
            pN->Data = obj;
			if (pHead == nullptr)
			{
				pHead = pN;
				pHead->pRight = pN;
				pHead->pLeft = pN;
			}
			else
			{
				Node<T>* pLastBack = pHead->pLeft;
				pLastBack->pRight = pN;
				pN->pRight = pHead;

				pHead->pLeft = pN;
				pN->pLeft = pLastBack;

				pHead = pN;
			}
		}
		void Add(const T& obj, int pos)
		{
			if (pHead == nullptr || pos == 0)
			{
				PushBack(obj);
				return;
			}
			unsigned int absPos = abs(pos);
			Node<T> *pNode = pHead;
			if (pos > 0)
			{
				for (int i = 0; i < absPos - 2; i++)
				{
					pNode = pNode->pRight;
				}
			}
			else if (pos < 0)
			{
				for (int i = 0; i < absPos - 2; i++)
				{
					pNode = pNode->pLeft;
				}
			}
			node_point pN = new Node<T>();
            pN->Data = obj;
			node_point pRight = pNode->pRight;

			pNode->pRight = pN;
			pN->pRight = pRight;

			pRight->pLeft = pN;
			pN->pLeft = pNode;
		}
		virtual void Clear()
		{
			if (Empty())
			{
				return;
			}
			node_point pNode = pHead;
			pHead->pLeft->pRight = nullptr;
			while (pNode != nullptr)
			{
				node_point pRight = pNode->pRight;
				delete pNode;
				pNode = pRight;
			}
			pHead = nullptr;
		}
		virtual void RemoveAt(unsigned int index)
		{
			if (Empty())
			{
				return;
			}
			node_point pNode = pHead;
			{
				for (int i = 0; i < index - 1; i++)
				{
					pNode = pNode->pRight;
				}
			}
			Remove(pNode);
		}
		virtual void Remove(const T &obj)
		{
			if (Empty())
			{
				return;
			}
			node_point pNode = pHead;
			do
			{
                if (pNode->Data == obj)
				{
					Remove(pNode);
					return;
				}
				pNode = pNode->pRight;
			} while (pNode != pHead);
		}
		void Remove(node_point pNode)
		{
			if (pNode != nullptr)
			{
				node_point pRight = pNode->pRight;
				node_point pLeft = pNode->pLeft;
				if (pHead->pRight == pHead)
				{
					pHead = nullptr;
					ClearNode(&pNode);
					return;
				}
				else if (pNode == pHead)
				{
					pHead = pNode->pRight;
				}
				if (pRight != nullptr)
				{
					pRight->pLeft = pLeft;
				}
				if (pLeft != nullptr)
				{
					pLeft->pRight = pRight;
				}
			}
			ClearNode(&pNode);
		}
		void Reverse()
		{
			if (Empty())
			{
				return;
			}
			node_point pLeft = nullptr;
			node_point pRight = nullptr;
			node_point pNode = pHead;
			do
			{
				pRight = pNode->pRight;
				pLeft = pNode->pLeft;

				pNode->pRight = pLeft;
				pNode->pLeft = pRight;
				pNode = pRight;
			} while (pNode != pHead);
			pHead = pNode->pRight;
		}
		void ClearNode(node_point* node)
		{
			if (node != nullptr && *node != nullptr)
			{
				delete (*node);
				*node = nullptr;
			}
		}
	};

	static void ListMain()
	{
		std::cout << "List-----------------------------------------------" << std::endl;
		stb::List<int> mList;
		for (int i = 0; i < 3; i++)
		{
			mList.PushBack(i);
		}
		for (int i = 0; i < mList.Size(); i++)
		{
			std::cout << mList[i] << std::endl;
		}
		Node<int>* pNode = mList.Begin();
		if (pNode != nullptr)
		{
			do
			{
                std::cout << pNode->Data << "	";
				pNode = pNode->pRight;
			} while (pNode != mList.Begin());
			std::cout << std::endl;
		}

		mList.Reverse();
		pNode = mList.Begin();
		if (pNode != nullptr)
		{
			do
			{
                std::cout << pNode->Data << "	";
				pNode = pNode->pRight;
			} while (pNode != mList.Begin());
			std::cout << std::endl;
		}

		mList.RemoveAt(5);
		pNode = mList.Begin();
		if (pNode != nullptr)
		{
			do
			{
                std::cout << pNode->Data << "	";
				pNode = pNode->pRight;
			} while (pNode != mList.Begin());
			std::cout << std::endl;
		}

		mList.Remove(2);
		pNode = mList.Begin();
		if (pNode != nullptr)
		{
			do
			{
                std::cout << pNode->Data << "	";
				pNode = pNode->pRight;
			} while (pNode != mList.Begin());
			std::cout << std::endl;
		}


		mList.PushFront(100);
		mList.PushBack(200);
		mList.Add(90, 5);
		pNode = mList.Begin();
		if (pNode != nullptr)
		{
			do
			{
                std::cout << pNode->Data << "	";
				pNode = pNode->pRight;
			} while (pNode != mList.Begin());
			std::cout << std::endl;
		}
		std::cout << "Index of:" << mList.IndexOf(9) << std::endl;
		std::cout << "Size:" << mList.Size() << std::endl;
		mList.RemoveAt(5);
		pNode = mList.Begin();
		if (pNode != nullptr)
		{
			do
			{
                std::cout << pNode->Data << "	";
				pNode = pNode->pRight;
			} while (pNode != mList.Begin());
			std::cout << std::endl;
		}
		mList.Clear();
		std::cout << "Size:" << mList.Size() << std::endl;
		std::cout << "Index of:" << mList.IndexOf(9) << std::endl;
	}
}

#endif
