#pragma once

struct Node
{
	Node* pLeft;
	Node* pRight;
	Node* pPerent;
	int* data;
	unsigned size;     //�Ե�ǰ�ڵ�Ϊ���������ڵ�����
};

class BinaryTree
{
public:
	unsigned size(){ return root->size; };
protected:
	Node* root;
};