
#include <iostream>
using namespace std;

/*
C����ǿ������ת��(Type Cast)�ܼ򵥣�����ʲô���͵�ת��ͳͳ�ǣ�
	TYPE b = (TYPE)a   
	C++��������ת���ṩ��4������ת����������Ӧ�Բ�ͬ���ϵ�Ӧ�á�
	����static_cast		    ��̬����ת������intת����char
		reinterpreter_cast	���½������� 
		dynamic_cast		����������Ƕ�̬����ת����������͸���֮��Ķ�̬����ת����
		const_cast��		������������ȥconst���ԡ�
		4������ת���ĸ�ʽ��
			TYPE B = static_cast<TYPE> (a)  
*/

//const char *p ��const���� ��pָ����ڴ�ռ� ���ֻ������
void printBuf(const char *  p)
{
	//p[0] = 'Z';
	char *p1 = NULL;
	//����Ա Ҫ�����֪�� ����:ת��֮ǰ��ʲô���� ,ת��֮����ʲô����

	p1 = const_cast<char *>(p);                  //const char * ===> char * //��ֻ������ ȥ��

	p1[0] = 'Z' ;  //ͨ��p1 ȥ�޸����ڴ�ռ�
	cout << p << endl;
}

void mainE1()
{
	char buf[] = "aaaaaaaaafffffddd";

	char *myp = "aaaaaaaaafffffddd";

	//����Ա Ҫȷ�� p��ָ����ڴ�ռ� ȷʵ���޸� ;��������޸Ļ���������Ժ��
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
		cout << "����" << endl;
	}
	void doHome()
	{
		cout << "����" << endl;
	}
};

class Cat : public Animal
{
public:
	virtual void cry()
	{
		cout << "����" << endl;
	}
	void doThing()
	{
		cout << "ץ����" << endl;
	}
};

void playObj(Animal *base)
{
	base->cry(); // 1�м̳� 2�麯����д 3 ����ָ�� ָ���������  ==>��̬
	//��ʶ���������
	// dynamic_cast ����ʱ����ʶ��  RIIT
	
	Dog *pDog = dynamic_cast<Dog *>(base);
	if (pDog != NULL)
	{
		pDog->doHome(); //�ù� ���Լ� ���еĹ��� 
	}

	Cat *pCat = dynamic_cast<Cat *>(base);	//������� ===> �������		����ת��  ������ ת�� С�� 
	if (pCat != NULL)
	{
		pCat->doThing();  //�ù� ���Լ� ���еĹ��� 
	}
}
void playObj(Animal& base)
{
	Dog &dog = dynamic_cast<Dog &>(base);
	dog.doHome();												//�ù� ���Լ� ���еĹ��� 
	
	Cat &cat = dynamic_cast<Cat &>(base);				//������� ===> ������� 
	cat.doThing();  //�ù� ���Լ� ���еĹ��� 
}
void main333()
{
	Dog d1;
	Cat c1;

	Animal *pBase = NULL;

	pBase = &d1;

	pBase = static_cast<Animal *>(&d1);                  //��C++�����ڱ����ʱ����� ���ͼ�� 

	//ǿ������ת�� 
	pBase = reinterpret_cast<Animal *>(&d1); 
	
	{
		Tree t1;
		//pBase = static_cast<Animal *>(&t1);					//err: C++�������������ͼ�飬�������򱨴�
		pBase = reinterpret_cast<Animal *>(&t1);       //reinterpret_cast ���½��� ....ǿ����ת����ζ����ǿ����Animal*���ڴ�ģ�ͽ���&t1
	}

	playObj(&d1);
	playObj(&c1);

	//playObj(d1);  //����ᷢ��һ��ת��ʧ�� ֱ�ӱ���
}

void main4()
{
	double dpi = 3.1415926;

	int num1 = (int)dpi; //C����ת��
	int num2 = static_cast<int>(dpi); //��̬����ת��  �����ʱc++�������������ͼ��
	int num3 = dpi; //c������ ��ʽ����ת���ĵط� ����ʹ�� static_cast<>() ��������ת��

	//char* ===> int *
	char *p1 = "hello...itcast ";
	int *p2 = NULL;
	//p2 = static_cast<int*>(p1); // ʹ��static_cast, ����������ʱ,�������ͼ�� ���д��� ��ʾ����

	p2 = reinterpret_cast<int *>(p1); //����ͬ����֮�䣬����ǿ������ת������reinterpret_cast<>() �������½���

	cout << "p1:" << p1 << endl; //%s
	cout <<"p2" << p2 << endl; //%d

	//�ܽ�:ͨ�� reinterpret_cast<>() �� static_cast<>()��C���Ե�ǿ������ת�� ��������..
}