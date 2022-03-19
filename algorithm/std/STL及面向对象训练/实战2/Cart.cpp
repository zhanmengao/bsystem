#include "Cart.h"

//����һ������
void Cart::add(Order* or)
{
	this->_goods.push_back(or);
}
//�������Ʒ���Ϊgid�Ķ�����ɾ��,������true,���򷵻�false
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
//��ʾ���ж�����Ϣ
void Cart::print()
{
	for each (Order* or in this->_goods)
	{
		or->print();
	}
}