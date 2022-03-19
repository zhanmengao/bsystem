#include <iostream>
#include<stdlib.h>
/*
����ģʽ��׼��һ���㷨������ÿ���㷨��װ������ʹ�����ǿ��Ի�����

�ⲿ���г�����Ե�ָ�룬���ö�̬����ʵ��ָ����Եļ��ܺ���
*/

/*
����ģʽ �����ڲ����������ϣ� �ٸ�����  ��
���� 1 3 5  ��΢��֧����  ���� 246 ��֧����֧���������춼�����ã������� �������滻֧�ֵ�֧����ʽ��
*/
namespace Pay
{
	class AbstractPay
	{
	public:
		void genrateOrder()
		{
			cout << "��������ҵ�񷽶���" << endl;
		}
		virtual void pay() = 0;
		virtual void handleOrder() = 0;
	};


	class WxPay :public AbstractPay
	{
	public:
		virtual void pay()
		{
			cout << "����΢��֧������֧��" << endl;
		}
		virtual void handleOrder()
		{
			cout << "����΢�Żص�����ѯ�����Ƿ������" << endl;
		}
	};

	class AliPay :public AbstractPay
	{
	public:
		virtual void pay()
		{
			cout << "����֧����API����֧��" << endl;
		}
		virtual void handleOrder()
		{
			cout << "����֧�����ص�����ѯ�����Ƿ������" << endl;
		}
	};
}
int main()
{
	Pay::AbstractPay *abstrapay = NULL;
	int day = rand()/7;
	if (day % 2 == 1)
	{
		abstrapay = new Pay::AliPay();
	}
	else
	{
		abstrapay = new Pay::WxPay();
	}
	abstrapay->genrateOrder();
	abstrapay->pay();
	abstrapay->handleOrder();
	return 0;
}