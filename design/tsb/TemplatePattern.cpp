#pragma once
#include <iostream>
using namespace std;

/*
模板方法模式：定义 一系列算法， 子类延伸实现。着重点在于：子类去处理不同的方法实现。

模板方法主要体现在子类的分别实现。
*/

namespace Pay
{
	class AbstractPay
	{
	public:
		void genrateOrder()
		{
			cout << "正在生成业务方订单" << endl;
		}
		virtual void pay() = 0;
		virtual void handleOrder() = 0;
	};


	class WxPay :public AbstractPay
	{
	public:
		virtual void pay()
		{
			cout << "调用微信支付发起支付" << endl;
		}
		virtual void handleOrder()
		{
			cout << "处理微信回调，查询订单是否处理完成" << endl;
		}
	};

	class AliPay :public AbstractPay
	{
	public:
		virtual void pay()
		{
			cout << "调用支付宝API发起支付" << endl;
		}
		virtual void handleOrder()
		{
			cout << "处理支付宝回调，查询订单是否处理完成" << endl;
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