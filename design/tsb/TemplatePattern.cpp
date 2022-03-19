#pragma once
#include <iostream>
using namespace std;

/*
ģ�巽��ģʽ������ һϵ���㷨�� ��������ʵ�֡����ص����ڣ�����ȥ����ͬ�ķ���ʵ�֡�

ģ�巽����Ҫ����������ķֱ�ʵ�֡�
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
	Pay::AliPay alipay;
	alipay.genrateOrder();
	alipay.pay();
	alipay.handleOrder();

	Pay::WxPay wxpay;
	wxpay.genrateOrder();
	wxpay.pay();
	wxpay.handleOrder();
	return 0;
}