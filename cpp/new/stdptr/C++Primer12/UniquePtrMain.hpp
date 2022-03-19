#pragma once
#include<memory>
#include"Blob.hpp"

typedef void(*pDeleteMyClass)(MyClass* pClass);
//ɾ����
void DeleteMyClass(MyClass* pClass)
{
	printf("Call DeleteMyClass\n");
	delete pClass;
}
unique_ptr<MyClass> p1(new MyClass("Unique p1"));
void CreateUniqueMain()
{
	unique_ptr<double> d1;					//����ָ��һ��double����
	unique_ptr<double> d2(new double(42));

	//�ṩһ��ɾ����
	unique_ptr<MyClass, pDeleteMyClass> p(new MyClass("pDelete"), DeleteMyClass);
}
void CopyUniqueMain()
{
	//unique_ptr<MyClass> p2(p1);			//error����֧�ֿ�������
	unique_ptr<MyClass> p3;
	//p3 = p1;					//error����֧�ָ�ֵ

	//ת��
	//release()��������ָ��Ŀ���Ȩ������ָ�룬����u�ÿ�
	unique_ptr<MyClass> p4(p1.release());
	p3.reset(p4.release());
	p3 = nullptr;				//�ͷ�p3ָ��Ķ��󣬲���p3�ÿ�

	p4.reset(new MyClass("p4"));
	p4.reset(nullptr);				//�ȼ��� = nullptr

	p4.reset(new MyClass("p5"));
	auto pMyP5 = p4.release();					//����ָ�벻�ٹ���p5����Ҫ��������
	if (pMyP5 != nullptr)
	{
		delete pMyP5;
		pMyP5 = nullptr;
	}
}

void ParameterUniqueMain()
{
	auto pClone = MyClass::UClone(*p1);				//���һ��p

}
