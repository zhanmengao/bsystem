#pragma once
#include<iostream>
#include<memory>
using namespace std;


void PrintV(int &t) {
	cout << "lvalue" << endl;
}

void PrintV(int &&t) {
	cout << "rvalue" << endl;
}

template<typename T>
void Test(T &&t) {
	PrintV(t);
	PrintV(std::forward<T>(t));

	PrintV(std::move(t));
}

int mainForward() {
	Test(1); // lvalue rvalue rvalue
	int a = 1;
	Test(a); // lvalue lvalue rvalue
	Test(std::forward<int>(a)); // lvalue rvalue rvalue
	Test(std::forward<int&>(a)); // lvalue lvalue rvalue
	Test(std::forward<int&&>(a)); // lvalue rvalue rvalue
	return 0;
}
/*
Test(1)：1是右值，模板中T &&t这种为万能引用，右值1传到Test函数中变成了右值引用，但是调用PrintV()时候，t变成了左值，因为它变成了一个拥有名字的变量，
所以打印lvalue，而PrintV(std::forward<T>(t))时候，会进行完美转发，按照原来的类型转发，所以打印rvalue，PrintV(std::move(t))毫无疑问会打印rvalue。

Test(a)：a是左值，模板中T &&这种为万能引用，左值a传到Test函数中变成了左值引用，所以有代码中打印。

Test(std::forward<T>(a))：转发为左值还是右值，依赖于T，T是左值那就转发为左值，T是右值那就转发为右值。
*/