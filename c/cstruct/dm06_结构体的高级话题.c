#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//һ���ṹ�嶨������,��ṹ���еĳ�Ա..�ڴ沼�� �Ͷ�����
// ����ͨ�� age��ַ .ȥ �� ��Ľṹ����ڴ��ַ
typedef struct AdvTeacher
{
	char name[64]; //64
	int age ;  //4
	int p; //4
	char *pname2; //4

}AdvTeacher;


void main61()
{
	int i = 0;
	AdvTeacher  t1;
	AdvTeacher *p = NULL;
	//strcpy(0, "dddd");
	p - 1 ; //����ͨ�� ����ͨ�� ��仰 ��cpu�м���
	p - 2;
	p - 3;

	p - p;

	{
		int offsize = (int)&(p->age);
		printf("%d \n", offsize);
	}
}


void mainOffset()
{
	int i = 0;
	AdvTeacher  t1; // & (t1.age)
	AdvTeacher *p = NULL;
	p = &t1;
	//p = p + 100;
	//strcpy(0, "dddd");
	//p - 1 ; //����ͨ�� ����ͨ�� ��仰 ��cpu�м���
	//p - 2;
	//p - 3;

	
	{
		int offsize1 =   (int)&(p->age) - (int)p; // & (t1.age)
		int offsize2 = (int )&(((AdvTeacher *)0)->age );
		printf("offsize1:%d \n", offsize1);
		printf("offsize2:%d \n", offsize2);

	}
}