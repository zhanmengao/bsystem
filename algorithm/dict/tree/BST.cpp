#include "BST.h"

BinarySearchTree::BinarySearchTree()
{

}
BinarySearchTree::~BinarySearchTree()
{

}
//������
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
	else if(*(root->data)>=data)         //root��������
	{              
		Put(data, root->pLeft);
	}
	else          //rootС��������
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
	else if (*(root->data) > data)      //root��������
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
/*�����������ö������������Һ��������
������
����ֵ��*/
void BinarySearchTree::DeleteMin()
{
	//�ҵ�������С�ڵ�
	Node& target = GetMin(this->root);
	//�������Һ��Ӵ�����
	target.data = target.pRight->data;
	if (target.pRight != nullptr)
	{
		delete target.pRight;
	}
}

void BinarySearchTree::DeleteMax()
{
	//�ҵ��������ڵ�
	Node& target = GetMin(this->root);
	//���������Ӵ�����
	target.data = target.pLeft->data;
	if (target.pLeft != nullptr)
	{
		delete target.pLeft;
	}
};


Node* BinarySearchTree::Delete(const int& data)      //��Ҫɾ���ڵ����������С�ڵ� ����Ҫɾ���Ľڵ�
{
	return nullptr;
}



void BinarySearchTree::Clear()
{

}