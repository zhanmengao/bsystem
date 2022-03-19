#include<iostream>
#include<string>
#include<vector>
using namespace std;
//��ʾ��
class Sales_data{
public:
	Sales_data(string bookNo = "", unsigned units_sold = 0 , double sellingprice=0.0, double saleprice=0.0, double discount = 0)
		:bookNo(bookNo), units_sold(units_sold), sellingprice(sellingprice), saleprice(saleprice), discount(discount)
	{
	}
	Sales_data(const Sales_data& book) :bookNo(book.bookNo), units_sold(book.units_sold), sellingprice(book.sellingprice), saleprice(book.saleprice), discount(book.discount)
	{
	}
	Sales_data(const istream& in)
	{
		in >> *this;
	}
public:
	//���ز�����
	inline friend ostream& operator<<(ostream& out, const Sales_data &sa);
	inline friend const istream& operator>>(const istream& in, Sales_data &sa);
	Sales_data& operator=(const Sales_data& book);
	bool operator==(const Sales_data& sa) const;
	bool operator!=(const Sales_data& sa) const;
	bool operator>(const Sales_data& sa) const;
	bool operator>=(const Sales_data& sa) const;
	bool operator<(const Sales_data& sa) const;
	bool operator<=(const Sales_data& sa) const;
	
public:
	//get set
	void setBookNo(const string& BookNo){
		this->bookNo = BookNo;
	}
	const string& getBookNo() const{
		return bookNo;
	}
	void setunits_sold(const unsigned& units_sold){
		this->units_sold = units_sold;
	}
	const unsigned& getunits_sold() const{
		return units_sold;
	}
	void setsellingprice(const double& sellingprice){
		this->sellingprice = sellingprice;
	}
	const double& getsellingprice() const{
		return sellingprice;
	}
	void setsaleprice(const double& saleprice){
		this->saleprice = saleprice;
	}
	const double& getsaleprice() const{
		return this->saleprice;
	}
	void setdiscount(const double& discount){
		this->discount = discount;
	}
	const double& getdiscount() const{
		return discount;
	}
private:
	string bookNo;//���ID
	unsigned units_sold=0;//����
	double sellingprice=0;//���ۼ�
	double saleprice=0;//�ۼ�
	double discount = 0.0;//�ۿ�
protected:
};