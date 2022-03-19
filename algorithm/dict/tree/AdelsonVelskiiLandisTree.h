#pragma once
#include"BinarySearchTree.h"
#define MAX(a,b) (a>b?a:b);

namespace stb
{
    template<typename T>
    class AdelsonVelskiiLandisTree
	{
	public:
        struct AVLNode :public BinaryTree<T>::node_type
		{
			unsigned high;
		};
	public:
		typedef AVLNode avlnode_type, *avlnode_point;
		virtual void Insert(const value_type& val)
		{
			if (pRoot == nullptr)
			{
				pRoot = CreateNode(val);
			}
			else
			{
				Insert(AVLNODE(pRoot), val);
			}
		}
	private:
		virtual avlnode_point Insert(avlnode_point node, const value_type& val)
		{
			if (node == nullptr)
			{
				node = CreateNode(val);
			}
			if (val < node->pData)
			{
				node->pLeft = Insert(Left(node), val);
				if (High(node->pLeft) - High(node->pRight) >= 2)
				{
					if (val < node->pLeft->pData)
					{
						node = SingleLeft(node);
					}
					else
					{
						node = DoubleLeft(node);
					}
				}
			}
			else if (val > node->pData)
			{
				node->pRight = Insert(Right(node), val);
				if (High(node->pRight) - High(node->pLeft) >= 2)
				{
					if (val < node->pRight->pData)
					{
						node = SingleRight(node);
					}
					else
					{
						node = DoubleRight(node);
					}
				}
			}
			UpdateHigh(node);
			return node;
		}
		//左旋：n是o的左孩子：将n的右孩子给o的左孩子，并把o视为自己的右孩子
		avlnode_point SingleLeft(avlnode_point oldR)
		{
			if (oldR == nullptr)
			{
				return nullptr;
			}
			avlnode_point newR = Left(oldR);
			oldR->pLeft = newR->pRight;
			newR->pRight = oldR;

			UpdateHigh(oldR);
			UpdateHigh(newR);
			return newR;
		}
		//右旋：n是o的右孩子：将n的左孩子给o的右孩子，并把o视为自己的右孩子
		avlnode_point SingleRight(avlnode_point oldR)
		{
			if (oldR == nullptr)
			{
				return nullptr;
			}
			avlnode_point newR = Right(oldR);
			oldR->pLeft = newR->pRight;
			newR->pRight = oldR;

			UpdateHigh(oldR);
			UpdateHigh(newR);
			return newR;
		}
		avlnode_point DoubleLeft(avlnode_point oldR)
		{
			if (oldR == nullptr)
			{
				return nullptr;
			}
			oldR->pLeft = SingleRight(Left(oldR));
			return SingleLeft(oldR);
		}
		avlnode_point DoubleRight(avlnode_point oldR)
		{
			if (oldR == nullptr)
			{
				return nullptr;
			}
			oldR->pLeft = SingleLeft(Right(oldR));
			return SingleRight(oldR);
		}
		void UpdateHigh(avlnode_point node)
		{
			if (!node->pLeft && !node->pRight)
			{
				node->high = 1;
			}
			else if (node->pLeft == nullptr)
			{
				node->high = High(node->pRight) + 1;
			}
			else if (node->pRight == nullptr)
			{
				node->high = High(node->pLeft) + 1;
			}
			else
			{
				node->high = MAX(High(node->pRight), High(node->pLeft)) + 1;
			}
		}
		virtual avlnode_point CreateNode(const value_type& val)
		{
			avlnode_point pNode = new avlnode_type();
			pNode->pData = val;
			pNode->high = 0;
			return pNode;
		}
		avlnode_point AVLNODE(node_point node)
		{
			return static_cast<avlnode_point>(node);
		}
		unsigned High(node_point node)
		{
			return static_cast<avlnode_point>(node)->high;
		}
		avlnode_point Left(node_point node)
		{
			return static_cast<avlnode_point>(node->pLeft);
		}
		avlnode_point Right(node_point node)
		{
			return static_cast<avlnode_point>(node->pRight);
		}
    protected:
        node_point pRoot = nullptr;
	};
}
