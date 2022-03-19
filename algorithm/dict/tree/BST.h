#pragma once
#include "BT.h"

class BinarySearchTree :public BinaryTree{
public :
	BinarySearchTree();
	virtual ~BinarySearchTree();
public:
	void Put(const int &data);
	Node* Get(const int &data) const;
	Node& GetMin() const;
	Node& GetMax() const;
	Node* Delete(const int& data);
	void DeleteMin();
	void DeleteMax();
	void Clear();
	void Print();
private:
	void Put(const int &data,Node* root);
	Node* Get(const int &data,Node* root) const;
	Node& GetMin(Node* root) const;
	Node& GetMax(Node* root) const;
};