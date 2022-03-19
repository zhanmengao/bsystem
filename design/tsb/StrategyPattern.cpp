#include <iostream>
#include<stdlib.h>
/*
策略模式：准备一组算法，并将每个算法封装起来，使得它们可以互换。

外部持有抽象策略的指针，利用多态调用实际指向策略的加密函数
*/

/*
策略模式 体现在策略两个字上， 举个例子  ，
星期 1 3 5  用微信支付，  星期 246 用支付宝支付，星期天都可以用，调用者 可随意替换支持的支付方式。
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