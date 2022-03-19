#pragma once
#include <iostream>
#include <vector>
#include <initializer_list>

template <class T>
struct S {
	std::vector<T> v;
	S(std::initializer_list<T> l) : v(l)
	{
		std::cout << "constructed with a " << l.size() << "-element list\n";
	}
	void append(std::initializer_list<T> l) 
	{
		v.insert(v.end(), l.begin(), l.end());
	}
	std::pair<const T*, std::size_t> c_arr() const 
	{
		return{ &v[0], v.size() };  // �� return ����и����б��ʼ��
									// �ⲻʹ�� std::initializer_list
	}
};

template <typename T>
void templated_fn(T) {}

int maininitializer_list()
{
	S<int> s = { 1, 2, 3, 4, 5 }; // ���Ƴ�ʼ��
	s.append({ 6, 7, 8 });      // ���������е��б��ʼ��

	std::cout << "The vector size is now " << s.c_arr().second << " ints:\n";

	for (auto n : s.v)
		std::cout << n << ' ';
	std::cout << '\n';

	std::cout << "Range-for over brace-init-list: \n";

	for (int x : {-1, -2, -3}) // auto �Ĺ�����˴���Χ for ����
		std::cout << x << ' ';
	std::cout << '\n';

	auto al = { 10, 11, 12 };   // auto ���������

	std::cout << "The list bound to auto has size() = " << al.size() << '\n';

	//    templated_fn({1, 2, 3}); // ������󣡡� {1, 2, 3} �����Ǳ��ʽ��
	// �������ͣ��� T �޷��Ƶ�
	templated_fn<std::initializer_list<int>>({ 1, 2, 3 }); // OK
	templated_fn<std::vector<int>>({ 1, 2, 3 });           // Ҳ OK
}