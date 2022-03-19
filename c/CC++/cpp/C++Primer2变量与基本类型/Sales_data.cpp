#include<iostream>
#include<string>
#include<vector>
#include "Sales_data.h"
using namespace std;

//重载操作符
inline ostream& operator << (ostream& out, const Sales_data &sa)
{
	cout << "ID=" << sa.bookNo << "  销量：" << sa.getdiscount() << " 零售价：" << sa.getsaleprice() << " 批发价"<<
		sa.getsellingprice() << " 折扣："<<sa.getunits_sold()<<endl;
	return out;
}
inline const istream& operator>>(const istream& in, Sales_data &sa)
{
	cout << "请输入ID"<<endl;
	cin >> sa.bookNo;
	cout<<endl;
	if (!cin.good()){
		cout << "输入失败！"<<endl;
		return in;
	}

	cout << "请输入销量"<<endl;
	cin >> sa.units_sold;
	cout << endl;
	if (!cin.good()){
		cout << "输入失败！" << endl;
		return in;
	}

	cout << "请输入零售价" << endl;
	cin >> sa.sellingprice;
	cout << endl;
	if (!cin.good()){
		cout << "输入失败！" << endl;
		return in;
	}

	cout << "请输入批发价" << endl;
	cin >> sa.saleprice;
	cout << endl;
	if (!cin.good()){
		cout << "输入！" << endl;
		return in;
	}

	cout << "请输入折扣" << endl;
	cin >> sa.discount;
	if (sa.discount < 0 || sa.discount>10)
	{
		sa.discount = -1;
		cin.setstate(cin.bad());
	}
	cout << endl;
	if (!cin.good()){
		cout << "输入失败！" << endl;
		return in;
	}
	return in;
}
Sales_data& Sales_data::operator = (const Sales_data& book)
{
	return *this;
}
bool Sales_data::operator == (const Sales_data& sa) const
{
	if (this->bookNo == sa.bookNo)
	{
		return 1;
	}
	else{
		return 0;
	}
}
bool Sales_data::operator != (const Sales_data& sa) const
{
	if (this->bookNo == sa.bookNo)
	{
		return 0;
	}
	else{
		return 1;
	}
}
bool Sales_data::operator>(const Sales_data& sa) const
{
	if (this->bookNo > sa.bookNo)
	{
		return 1;
	}
	else{
		return 0;
	}
}
bool Sales_data::operator >= (const Sales_data& sa) const
{
	if (*this > sa)
	{
		return 1;
	}
	if (*this == sa)
	{
		return 1;
	}
	return 0;
}
bool Sales_data::operator<(const Sales_data& sa) const
{
	if (this->bookNo < sa.bookNo)
	{
		return 1;
	}
	else{
		return 0;
	}
}
bool Sales_data::operator <= (const Sales_data& sa) const
{
	if (*this < sa)
	{
		return 1;
	}
	if (*this == sa)
	{
		return 1;
	}
	return 0;
}
