#include<iostream>
#include<string>
using namespace std;

class Component
{
	//�ӿ�
public:
	virtual void Operation() = 0;
	virtual ~Component() {}
};

class ConcreteComponent : public Component
{
	//����ʵ�ֶ�����Ҫ��װ�ε���
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
		component = c;				 //����Component
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
	std::string addedState;    //������й���
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
	}    //�������з���
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