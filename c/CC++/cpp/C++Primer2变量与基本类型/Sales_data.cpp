#include<iostream>
#include<string>
#include<vector>
#include "Sales_data.h"
using namespace std;

//���ز�����
inline ostream& operator << (ostream& out, const Sales_data &sa)
{
	cout << "ID=" << sa.bookNo << "  ������" << sa.getdiscount() << " ���ۼۣ�" << sa.getsaleprice() << " ������"<<
		sa.getsellingprice() << " �ۿۣ�"<<sa.getunits_sold()<<endl;
	return out;
}
inline const istream& operator>>(const istream& in, Sales_data &sa)
{
	cout << "������ID"<<endl;
	cin >> sa.bookNo;
	cout<<endl;
	if (!cin.good()){
		cout << "����ʧ�ܣ�"<<endl;
		return in;
	}

	cout << "����������"<<endl;
	cin >> sa.units_sold;
	cout << endl;
	if (!cin.good()){
		cout << "����ʧ�ܣ�" << endl;
		return in;
	}

	cout << "���������ۼ�" << endl;
	cin >> sa.sellingprice;
	cout << endl;
	if (!cin.good()){
		cout << "����ʧ�ܣ�" << endl;
		return in;
	}

	cout << "������������" << endl;
	cin >> sa.saleprice;
	cout << endl;
	if (!cin.good()){
		cout << "���룡" << endl;
		return in;
	}

	cout << "�������ۿ�" << endl;
	cin >> sa.discount;
	if (sa.discount < 0 || sa.discount>10)
	{
		sa.discount = -1;
		cin.setstate(cin.bad());
	}
	cout << endl;
	if (!cin.good()){
		cout << "����ʧ�ܣ�" << endl;
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
