#pragma once
#include<string>
#include"Blob.hpp"
using namespace std;
/*
ʲôʱ����shared_ptr��
1������֪���Լ���Ҫʹ�ö��ٶ���������
2������֪����������׼ȷ����
3��������Ҫ�ڶ������乲������
*/
shared_ptr<MyClass> c1;					//������Ĭ�Ϲ��캯��������һ����ָ��
shared_ptr<MyClass> c2(new MyClass("Shader c2"));
void CreateSharedMain()
{
	//shared_ptr<string> p2 = new string();	//error��shared_ptr���캯��Ϊexplicit
	if (c1 == nullptr)
	{
		c1.reset(new MyClass("c1"));
	}
	//���p1��Ϊ�գ�������Ƿ�ָ��һ����string
	else if (c1 && c1->empty())
	{
		*c1 = "c1";
	}

	new MyClass("c3");									//!!!!!�����Զ��������ڴ�й©

	shared_ptr<int> p3 = make_shared<int>(42);
	cout << "make_shared<int>(42)	" << *p3 << endl;

	shared_ptr<string> p4 = make_shared<string>(10, '9');
	cout << "make_shared<string>(10, '9')	" << *p4 << endl;

	shared_ptr<string> p5 = make_shared<string>();				//ʹ��ֵ��ʼ��
	cout << "make_shared<string>()	" << *p5 << endl;
	{
		auto c5 = MyClass::factory("c5");
	}
	//�뿪������c5������
}

void CopySharedMain()
{
	shared_ptr<MyClass> c4(new MyClass("c4"));
	c2 = c4;															//c2������Ϊ0��������

	shared_ptr<MyClass> pCpy(c2);				//����һ������ָ�룬������++
	cout << "pCpy.use_count() " << pCpy.use_count() << endl;
	cout << "*pCpy " << *pCpy << endl;
	//����Ƿ���Ψһָ������ָ��
	if (!c2.unique())
	{
		c2.reset(new MyClass("reset"));
	}
	*c2 += "c2";

	auto c6 = MyClass::Clone(*c4);			//���һ��c4
	*c6 = ("c6");
	//�ض���
	c6.reset(new MyClass("c66"));							//c6������

	//shared_ptr<MyClass> c(c6.get());		//���󣡣�����ֱ�Ӱ�����ָ��get�������ڴ�

	swap(c6, c2);					//�������ǵ�ָ��
}

void ParameterShaderMain()
{
	auto c = MyClass::factory("c9");
	MyClass::ProcessShared(MyClass::factory("c10"));			//ִ�н�����c10���ͷ�
	MyClass::ProcessShared(shared_ptr<MyClass>(c));
	MyClass::ProcessPtr(c.get());					//����ԭ��ָ��

}