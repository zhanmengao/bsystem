
#include <iostream>
using namespace std;


class Test3
{
public:
	void init(int _a, int _b)
	{
		a = _a;
		b = _b;
	}

protected:
private:
	int a;
	int b;
};


void main301()
{

	//��û���ṩ���캯��,c++���������Զ������ṩһ��Ĭ�ϵĹ��캯��
	//��û���ṩ���캯�� copy���칹�캯��, c++���������Զ�������Ա�ṩһ�� Ĭ�ϵ�copy���캯��  = 
	Test3 t1;
	int a = 10;
	int b = 20;
	t1.init(a, b);

	Test3 tArray[3];
	tArray[0].init(1, 2);
	tArray[1].init(1, 2);
	tArray[2].init(1, 2);

	Test3 t21;
	t21.init(1, 2);
	Test3 t22;
	t22.init(1, 2);
	Test3 t23;
	t23.init(1, 2);

	//�����ֳ���֮�� ��ʾ�ĳ�ʼ������ �Եú�����
	Test3 tArray2[3] = { t21, t22, t23 };

	//�����ֳ���֮��,���㲻��,�����Ҫ
	Test3 tArray3[1999] = { t21, t22, t23 };


	cout << "hello..." << endl;
	system("pause");
	return;
}