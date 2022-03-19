#include<iostream>
#include<string>
using namespace std;

class Component
{
	//接口
public:
	virtual void Operation() = 0;
	virtual ~Component() {}
};

class ConcreteComponent : public Component
{
	//具体实现对象，需要被装饰的类
public:
	void Operation()
	{
		cout << "ConcreteComponent" << endl;
	}
};

class Decorator : public Component
{
private:
	Component component;
public:
	void SetComponent(Component c)
	{
		component = c;				 //设置Component
	}
	virtual void Operation()
	{
		if (component)
			component->Operation();
	}
};

class ConcreteDecoratorA : public Decorator
{
private:
	std::string addedState;    //本类独有功能
public:
	void Operation()
	{
		Decorator::Operation();
		addedState = "New state";
		cout << "ConcreteDecoratorA" << addedState << endl;
	}
};

class ConcreteDecoratorB : public Decorator
{
private:
	void AddedBehavior()
	{
		cout << "ConcreteDecoratorB" << AddedBehavior << endl;
	}    //本类特有方法
public:
	void Operation()
	{
		Decorator::Operation();
		AddedBehavior();
	}
};

int main()
{
	Component c = new ConcreteComponent();

	Decorator* d1 = new ConcreteDecoratorA();
	d1->SetComponent(c);
	d1->Operation();					//ConcreteComponent
												//ConcreteDecoratorA New_state
	Decorator* d2 = new ConcreteDecoratorB();
	d2->SetComponent(c);
	d2->Operation();					//ConcreteComponent
												//ConcreteDecoratorB AddedBehavior
	delete d1;
	delete d2;
	delete c;
	return 0;
}