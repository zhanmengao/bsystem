#pragma once

int Auto()
{
	//��ͨ������
	int a = 1, b = 3;
	auto c = a + b;					// cΪint��


	const int i = 5;				 //const����
	auto j = i; // ����i�Ƕ���const, �ᱻ����, ����j��������int
	auto k = &i; // ����i��һ������, �Գ���ȡ��ַ��һ�ֵײ�const, ����b��������const int*
	const auto l = i; //���ϣ���ƶϳ��������Ƕ���const��, ��ô����Ҫ��autoǰ�����cosnt

					  //���ú�ָ������
	int x = 2;
	int& y = x;
	auto z = y; //z��int�Ͳ���int& ��
	auto& p1 = y; //p1��int&��
	auto p2 = &x; //p2��ָ������int*
}


int func() { return 0; };
int Decltype()
{
	//��ͨ����
	decltype(func()) sum = 5; // sum�������Ǻ���func()�ķ���ֵ������int, ������ʱ����ʵ�ʵ��ú���func()
	int a = 0;
	decltype(a) b = 4; // a��������int, ����b������Ҳ��int

					   //�����Ƕ���const���ǵײ�const, decltype���ᱣ��   
	const int c = 3;
	decltype(c) d = c; // d�����ͺ�c��һ����, ���Ƕ���const
	int e = 4;
	const int* f = &e; // f�ǵײ�const
	decltype(f) g = f; // gҲ�ǵײ�const

					   //������ָ������
					   //1. ������ʽ����������, ��ôdecltype������Ҳ������
	const int i = 3, &j = i;
	decltype(j) k = 5; // k�������� const int&

					   //2. ������ʽ����������, ������Ҫ�õ����������ָ�������, ��Ҫ�޸ı��ʽ:
	int i = 3, &r = i;
	decltype(r + 0) t = 5; // ��ʱ��int����

						   //3. ��ָ��Ľ����ò������ص�����������
	//int i = 3, j = 6, *p = &i;
	//decltype(*p) c = j; // c��int&����, c��j����һ��

						//4. ���һ�����ʽ�����Ͳ�������, ����������Ҫ�ƶϳ�����, ��ô���Լ���һ������, �ͱ��������������
	int i = 3;
	decltype((i)) j = i; // ��ʱj��������int&����, j��i������һ��
}