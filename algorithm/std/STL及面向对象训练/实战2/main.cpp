#include<iostream>
#include "cart.h"
#include "Order.h"
using namespace std;

int main(int argc, char* argv[])
{
	Cart c;
	int n, i;
	int gid, gnum;
	cout << "你要订购多少商品" << endl;
	cin >> n;
	//测试 Cart::add方法 
	cout << "测试 Cart::add方法---------------" << endl;
	for (i = 0; i < n; i++)
	{
		cout << "请输入第" << i + 1 << "个订单信息" << endl;
		cin >> gid >> gnum;
		c.add(new Order(gid, gnum));
	}
	//测试Cart::print方法 
	cout << "你的订单信息如下" << endl;
	c.print();
	//测试Cart::del方法 
	cout << "测试Cart::del方法,请输入要删除的订单的产品编号" << endl;
	cin >> gid;
	c.del(gid);
	cout << "现在你的订单信息如下" << endl;
	c.print();
	cout << "Hello world!" << endl;
	system("pause");
	return 0;
}