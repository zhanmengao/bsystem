#include<iostream>
#include<string>
#include<vector>
#include<typeinfo>
using namespace std;

int main234()
{
	int i = 0, &r = i;
	auto a = r;//a��int����r��ֵ�����г�ʼ����r��i�ı���
	a = 42;
	cout << a<<typeid(a).name()<<endl;
	
	const int ci = i, &cr = ci;//ci��const int��cr��ci�ı���
	auto b = ci;//b��int����ci��ֵ���г�ʼ��
	b = 42;
	cout << b<<typeid(b).name() << endl;
	auto c = cr;//c��int����cr��ֵ���г�ʼ����cr��ci�ı���
	c = 42;
	cout << typeid(c).name() << endl;
	auto d = &i;//d��int *��ָ��i�ĵ�ַ
	d = const_cast<int *>(&cr);
	cout << typeid(d).name() << endl;
	auto e = &ci;//e��int const * 
	cout << typeid(e).name() << endl;

	//����
	auto &g = ci;//g�ǳ����� const int &
	cout << typeid(g).name() << endl;
	const auto &j = 42;//j�ǳ����� const int &
	cout << typeid(j).name() << endl;
	return 0;
}
