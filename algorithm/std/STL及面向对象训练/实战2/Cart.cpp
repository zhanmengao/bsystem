#include "Cart.h"

//增加一个订单
void Cart::add(Order* or)
{
	this->_goods.push_back(or);
}
//如果有商品编号为gid的订单就删除,并返回true,否则返回false
bool Cart::del(int gid)
{
	list<Order*>::iterator it = _goods.begin();
	while (it!=_goods.end() )
	{
		if ( **it == gid)
		{
			this->_goods.remove(*it);
			return true;
		}
		it++;
	}
	return false;
	
}
//显示所有订单信息
void Cart::print()
{
	for each (Order* or in this->_goods)
	{
		or->print();
	}
}