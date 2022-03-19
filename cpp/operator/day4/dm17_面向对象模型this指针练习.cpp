
#include <iostream>
using namespace std;
class Test
{
public:
	Test(int a, int b) //---> Test(Test *this, int a, int b)
	{
		this->a = a;
		this-> b = b;	
	}
	void printT()
	{
		cout<<"a: " <<a <<endl;
		cout<< "b: " << this->b <<endl;
	}
	//1 const 写的什么位置 没有关系 
	//2 const修饰的是谁？
	// 2-1const修饰的是形参a 不是
	// 2-2const修饰的是属性this->a  this->b 
	// 2-3 const修饰的是this指针所指向的内存空间， 修饰的是this指针

	 void  OpVar( int a, int b) const   //==>void  OpVar(const Test *this, int a, int b)  
		 //==>void  OpVar( const Test *const this, int a, int b)  
	{
		a = 100;
		//this->a = 100;
		//this->b = 200;
		//this = 0x11;
		
		
	
		//cout<<"a: " <<a <<endl;
		cout<< "b: " << this->b <<endl;
	}
protected:
private:
	int a;
	int b;
};

void main1701()
{

	int *m_space = new int[0];
	if (m_space == NULL)
	{
		return ;
	}
	
	Test t1(1, 2);
	t1.printT();// ===> printT(&t1)
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}