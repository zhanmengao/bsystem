#pragma once
#include"move.h"

A pppp()
{
	A a;
	return a;
}

A ppmove()
{
	A a;
	return std::move(a);
}


class ABCD {
public:
	ABCD(int a, int b)
	{
		cout << "����" << endl;
	}
	~ABCD()
	{
		cout << "����" << endl;

	}
	ABCD(ABCD &obj)
	{
		cout << "��������" << endl;
	}
private:
	int a;
	int b;
};
ABCD pppp1()
{
	ABCD a(1, 2);
	printf("build in %s \n", __FUNCTION__);
	return a;
}

int mainReturn()
{
	ABCD a1 = pppp1();			//��������һ������ֵ����a1
	printf("\n");
	return 0;
}
int mainReturn3()
{	
	A a = pppp();						//�ƶ����죺��δ���ᴥ��RVO
	printf("\n");
	return 0;
}


int mainReturn1() {
	A &&b = pppp();		//���ᴥ��RVO������������һ����ʱ�Ķ�����ʱ������������ں�rval_ref��
	//������ȼ�
	const A& rval_ref = pppp();
	return 0;
}

int mainReturn2() {
	//��ʾ��1���ƣ�std::moveһ����ʱ������û�б�Ҫ�ģ�Ҳ����Ե�����ֵ�Ż���
	A && c = ppmove();
	return 0;
}