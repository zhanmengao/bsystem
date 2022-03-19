#pragma once
#include<iostream>
#include<memory>
using namespace std;

class A {
public:
	A(int size) : size_(size) {
		data_ = new int[size];
	}
	A() {
		cout << "build " << endl;
	}
	A(const A& a) {
		size_ = a.size_;
		data_ = new int[size_];
		cout << "copy " << endl;
	}
	A(A&& a) {
		this->data_ = a.data_;
		a.data_ = nullptr;
		cout << "move " << endl;
	}
	~A() {
		if (data_ != nullptr) {
			delete[] data_;
		}
		cout << "delete" << endl;
	}
	int *data_ = nullptr;
	int size_;
};





int mainMove() {
	A a(10);
	A b = a;
	A c = std::move(a);				// 调用移动构造函数
	return 0;
}