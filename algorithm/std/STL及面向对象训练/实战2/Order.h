#pragma once


//描述订单 
class Order
{
public:
	Order(int gid, int gnum);	//构造函数;  
	void print();	//显示订购的商品编号: 数量 
	bool operator == (int gid);	//判断当前商品编号是否与参数gid相同,相同返回true,否则返回false 
private:
	int goods_id;	//商品编号  
	int goods_number;	//商品数量 
};