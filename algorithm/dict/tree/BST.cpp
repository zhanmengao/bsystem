#include "BST.h"

BinarySearchTree::BinarySearchTree()
{

}
BinarySearchTree::~BinarySearchTree()
{

}
//加入树
void BinarySearchTree::Put(const int &data)
{
	Put(data, root);
}

void BinarySearchTree::Put(const int &data, Node* root)
{
	if (root == nullptr)
	{
		root = new Node;
		root->data = new int;
		*(root->data) = data;
	}
	else if(*(root->data)>=data)         //root大，往左走
	{              
		Put(data, root->pLeft);
	}
	else          //root小，往右走
	{
		Put(data, root->pRight);
	}
}


Node* BinarySearchTree::Get(const int &data) const
{
	return Get(data, root);
}


Node* BinarySearchTree::Get(const int &data, Node* root) const
{
	if (root == nullptr || *(root->data) == data)
	{
		return root;
	}
	else if (*(root->data) > data)      //root大，往左找
	{
		return Get(data, root->pLeft);
	}
	else
	{
		return Get(data, root->pRight);
	}
}

Node& BinarySearchTree::GetMin() const
{
	return GetMin(root);
}
Node& BinarySearchTree::GetMax() const
{
	return GetMax(root);
}
Node& BinarySearchTree::GetMin(Node* root) const
{
	Node* pCur = root;
	while (pCur->pLeft != nullptr)
	{
		pCur = pCur->pLeft;
	}
	return *pCur;
}

Node& BinarySearchTree::GetMax(Node* root) const
{
	Node* pCur = root;
	while (pCur->pRight != nullptr)
	{
		pCur = pCur->pRight;
	}
	return *pCur;
}
/*功能描述：用二叉搜索树的右孩子替代它
参数：
返回值：*/
void BinarySearchTree::DeleteMin()
{
	//找到它的最小节点
	Node& target = GetMin(this->root);
	//让它的右孩子代替它
	target.data = target.pRight->data;
	if (target.pRight != nullptr)
	{
		delete target.pRight;
	}
}

void BinarySearchTree::DeleteMax()
{
	//找到它的最大节点
	Node& target = GetMin(this->root);
	//让它的左孩子代替它
	target.data = target.pLeft->data;
	if (target.pLeft != nullptr)
	{
		delete target.pLeft;
	}
};


Node* BinarySearchTree::Delete(const int& data)      //拿要删除节点的右子树最小节点 代替要删除的节点
{
	return nullptr;
}



void BinarySearchTree::Clear()
{

}