
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
	//1 const д��ʲôλ�� û�й�ϵ 
	//2 const���ε���˭��
	// 2-1const���ε����β�a ����
	// 2-2const���ε�������this->a  this->b 
	// 2-3 const���ε���thisָ����ָ����ڴ�ռ䣬 ���ε���thisָ��

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