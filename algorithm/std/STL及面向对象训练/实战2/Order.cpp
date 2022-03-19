#include "Order.h"
#include <iostream>
using namespace std;

//构造函数
Order::Order(int gid, int gnum) :goods_id(gid), goods_number(gnum)
{
}
//显示订购的商品编号: 数量
void Order::print()
{
	cout << this->goods_id << "\t数量：" << this->goods_number << endl;
}
//判断当前商品编号是否与参数gid相同,相同返回true,否则返回false
bool Order::operator == (int gid)
{
	return this->goods_id == gid ? true : false;
}