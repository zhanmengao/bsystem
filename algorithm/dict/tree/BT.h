#pragma once

struct Node
{
	Node* pLeft;
	Node* pRight;
	Node* pPerent;
	int* data;
	unsigned size;     //以当前节点为根的子树节点数量
};

class BinaryTree
{
public:
	unsigned size(){ return root->size; };
protected:
	Node* root;
};