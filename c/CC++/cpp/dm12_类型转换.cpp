
#include <iostream>
using namespace std;

/*
C风格的强制类型转换(Type Cast)很简单，不管什么类型的转换统统是：
	TYPE b = (TYPE)a   
	C++风格的类型转换提供了4种类型转换操作符来应对不同场合的应用。
	　　static_cast		    静态类型转换。如int转换成char
		reinterpreter_cast	重新解释类型 
		dynamic_cast		命名上理解是动态类型转换。如子类和父类之间的多态类型转换。
		const_cast，		字面上理解就是去const属性。
		4种类型转换的格式：
			TYPE B = static_cast<TYPE> (a)  
*/

//const char *p 的const修饰 让p指向的内存空间 变成只读属性
void printBuf(const char *  p)
{
	//p[0] = 'Z';
	char *p1 = NULL;
	//程序员 要清楚的知道 变量:转换之前是什么类型 ,转换之后是什么类型

	p1 = const_cast<char *>(p);                  //const char * ===> char * //把只读属性 去掉

	p1[0] = 'Z' ;  //通过p1 去修改了内存空间
	cout << p << endl;
}

void mainE1()
{
	char buf[] = "aaaaaaaaafffffddd";

	char *myp = "aaaaaaaaafffffddd";

	//程序员 要确保 p所指向的内存空间 确实能修改 ;如果不能修改会带来灾难性后果
	printBuf (buf);

	//printBuf (myp);

}


class Tree {};

class Animal
{
public:
	virtual void cry() = 0;
};

class Dog : public Animal
{
public:
	virtual void cry()
	{
		cout << "汪汪" << endl;
	}
	void doHome()
	{
		cout << "看家" << endl;
	}
};

class Cat : public Animal
{
public:
	virtual void cry()
	{
		cout << "喵喵" << endl;
	}
	void doThing()
	{
		cout << "抓老鼠" << endl;
	}
};

void playObj(Animal *base)
{
	base->cry(); // 1有继承 2虚函数重写 3 父类指针 指向子类对象  ==>多态
	//能识别子类对象
	// dynamic_cast 运行时类型识别  RIIT
	
	Dog *pDog = dynamic_cast<Dog *>(base);
	if (pDog != NULL)
	{
		pDog->doHome(); //让够 做自己 特有的工作 
	}

	Cat *pCat = dynamic_cast<Cat *>(base);	//父类对象 ===> 子类对象		向下转型  把老子 转成 小子 
	if (pCat != NULL)
	{
		pCat->doThing();  //让够 做自己 特有的工作 
	}
}
void playObj(Animal& base)
{
	Dog &dog = dynamic_cast<Dog &>(base);
	dog.doHome();												//让够 做自己 特有的工作 
	
	Cat &cat = dynamic_cast<Cat &>(base);				//父类对象 ===> 子类对象 
	cat.doThing();  //让够 做自己 特有的工作 
}
void main333()
{
	Dog d1;
	Cat c1;

	Animal *pBase = NULL;

	pBase = &d1;

	pBase = static_cast<Animal *>(&d1);                  //让C++编译在编译的时候进行 类型检查 

	//强制类型转换 
	pBase = reinterpret_cast<Animal *>(&d1); 
	
	{
		Tree t1;
		//pBase = static_cast<Animal *>(&t1);					//err: C++编译器会做类型检查，不符合则报错
		pBase = reinterpret_cast<Animal *>(&t1);       //reinterpret_cast 重新解释 ....强制类转换的味道，强行用Animal*的内存模型解释&t1
	}

	playObj(&d1);
	playObj(&c1);

	//playObj(d1);  //这里会发生一个转换失败 直接报错
}

void main4()
{
	double dpi = 3.1415926;

	int num1 = (int)dpi; //C类型转换
	int num2 = static_cast<int>(dpi); //静态类型转换  编译的时c++编译器会做类型检查
	int num3 = dpi; //c语言中 隐式类型转换的地方 均可使用 static_cast<>() 进行类型转换

	//char* ===> int *
	char *p1 = "hello...itcast ";
	int *p2 = NULL;
	//p2 = static_cast<int*>(p1); // 使用static_cast, 编译器编译时,会做类型检查 若有错误 提示错误

	p2 = reinterpret_cast<int *>(p1); //若不同类型之间，进行强制类型转换，用reinterpret_cast<>() 进行重新解释

	cout << "p1:" << p1 << endl; //%s
	cout <<"p2" << p2 << endl; //%d

	//总结:通过 reinterpret_cast<>() 和 static_cast<>()把C语言的强制类型转换 都覆盖了..
}