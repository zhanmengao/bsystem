#pragma once
#include <list>
#include"order.h"
using namespace std;
//描述购物车, 存储了多了产品的订单
class Cart
{
public:
	void add(Order* or);	//增加一个订单
	bool del(int gid);	//如果有商品编号为gid的订单就删除,并返回true,否则返回false
	void print();		//显示所有订单信息
private:
	list<Order*> _goods;//存储了多了产品的订单
};

