#pragma once
//C������
#ifndef __cplusplus
typedef struct Student
{
	int age;
} S;
//�ȼ���

// c
struct Student
{
	int age;
};
#endif
//C++��
#ifdef __cplusplus
//����Ϊ����ʹ�� Student ��Ҳ����ʹ�� struct Student�����£�
// cpp
#if 1
{
	struct Student
	{
		int age;
	};
	void f(Student me);       // ��ȷ��"struct" �ؼ��ֿ�ʡ��
}
#endif

//.��������ʶ���ռ䶨���� struct Student {...};��ʹ�� Student me; ʱ��
//������������ȫ�ֱ�ʶ����Student δ�ҵ����������ʶ����������
//���������� Student ͬ������֮���� Student ֻ��������������ṹ��
typedef struct Student
{
	int age;
} S;

void Student() {}           // ��ȷ������� "Student" ֻ����˺���

//void S() {}               // ���󣬷��� "S" �Ѿ�������Ϊһ�� "struct Student" �ı���

int maintypedef()
{
	Student();
	struct Student me;      // ���� "S me";
	return 0;
}
#endif