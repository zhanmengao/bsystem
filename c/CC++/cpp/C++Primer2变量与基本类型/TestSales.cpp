#include<iostream>
#include<string>
#include<vector>
#include "Sales_data.h"
using namespace std;

int main201()
{
	Sales_data sa;
	sa.setBookNo("BK111");
	sa.setunits_sold(100);
	sa.setsellingprice(99);
	sa.setsellingprice(58);
	sa.setdiscount(5.1);

	Sales_data sb("AK111");

	return 0;
}
