

#include <iostream>
using namespace std;

//HeroFighter  AdvHeroFighter EnemyFighter


class HeroFighter
{
public:
	virtual int power()  //C++会对这个函数特殊处理
	{
		return 10;
	}
};

class EnemyFighter
{
public:
	int attack()
	{
		return 15;
	}
};


class AdvHeroFighter : public HeroFighter
{
public:
	virtual int power()
	{
		return 20;
	}
};

class AdvAdvHeroFighter : public HeroFighter
{
public:
	virtual int power()
	{
		return 30;
	}
};

//多态威力
//1 PlayObj给对象搭建舞台  看成一个框架
//15:20
void PlayObj(HeroFighter *hf, EnemyFighter *ef)
{
	//不写virtual关键字 是静态联编 C++编译器根据HeroFighter类型,去执行 这个类型的power函数 在编译器编译阶段就已经决定了函数的调用
	//动态联编: 迟绑定:  //在运行的时候,根据具体对象(具体的类型),执行不同对象的函数 ,表现成多态.
	if (hf->power() > ef->attack())  //hf->power()函数调用会有多态发生
	{
		printf("主角win\n");
	}
	else
	{
		printf("主角挂掉\n");
	}
}


//多态的思想
//面向对象3大概念
//封装: 突破c函数的概念....用类做函数参数的时候,可以使用对象的属性 和对象的方法 
//继承: A B 代码复用
//多态 : 可以使用未来...


//多态很重要
//实现多态的三个条件
//C语言 间接赋值 是指针存在的最大意义
//是c语言的特有的现象 (1 定义两个变量  2 建立关联  3 *p在被调用函数中去间接的修改实参的值)

//实现多态的三个条件
//1 要有继承 
//2 要有虚函数重写
//3 用父类指针(父类引用)指向子类对象....

void main1401()
{
	HeroFighter		hf;
	AdvHeroFighter	Advhf;
	EnemyFighter	ef;

	AdvAdvHeroFighter advadvhf;

	PlayObj(&hf, &ef);
	PlayObj(&Advhf, &ef);

	PlayObj(&advadvhf, &ef) ; //这个框架 能把我们后来人写的代码,给调用起来

	cout<<"hello..."<<endl;
	system("pause");

}
void main1402()
{
	
	HeroFighter		hf;
	AdvHeroFighter	Advhf;
	EnemyFighter	ef;
	
	if (hf.power() > ef.attack())
	{
		printf("主角win\n");
	}
	else
	{
		printf("主角挂掉\n");
	}

	if (Advhf.power() > ef.attack())
	{
		printf("Adv 主角win\n");
	}
	else
	{
		printf("Adv 主角挂掉\n");
	}

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}