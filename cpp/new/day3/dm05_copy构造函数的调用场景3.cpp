
#include <iostream>
using namespace std;


class Location 
{ 
public:
	Location( int xx = 0 , int yy = 0 ) 
	{ 
		X = xx ;  
		Y = yy ; 
		cout << "Constructor Object.\n" ; 
	}

	//copy构造函数  完成对象的初始化
	Location(const Location & obj) //copy构造函数 
	{
		X = obj.X;
		Y = obj.Y;
	}
	~Location() 
	{ 
		cout << X << "," << Y << " Object destroyed." << endl ; 
	}
	int  GetX () { return X ; }		
	int GetY () { return Y ; }
private :   
	int  X , Y ;
} ;


//业务函数  形参是一个元素
void f(Location p)
{
	cout<<p.GetX()<<endl;
}

void playobj()
{
	Location  a(1, 2);
	Location  b = a;
	cout<<"b对象已经初始化完毕"<<endl;

	f(b); //b实参取初始化形参p,会调用copy构造函数
}

void main501()
{
	playobj();
	system("pause");
	return ;
}