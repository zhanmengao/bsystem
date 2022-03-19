#include<iostream>
#include<memory>
using namespace std;

//[��׽�б�](����) mutable -> ����ֵ���� {������}
void mainLambda()
{
	int a = 5, b = 7;
	auto total = [](int x, int y)->int {return x + y; };			//������������
	cout << total(a, b) << endl;										//12

	auto fun1 = [=] {return a + b; };									//ֵ���ݲ�׽�����������
	cout << fun1() << endl;												//12

	auto fun2 = [&](int c) {b = a + c; a = 1; };					//ʡ���˷���ֵ���ͣ����ò�������
	fun2(3);    
	cout << a << " " << b << endl;
	a = 5; b = 7;																		//���޸ĺ����¸�ֵ

	auto fun3 = [=, &b](int c) mutable {b = a + c; a = 1; };    //��ֵ���ݲ�׽�ı������ں����������Ҫ�޸ģ�Ҫ��mutaple,��ΪĬ��const����
	fun3(3);
	cout << a << " " << b << endl;
	a = 5; b = 7;																		 //���޸ĺ����¸�ֵ

	auto fun4 = [=](int x, int y) mutable->int {a += x; b += y; return a + b; };
	int t = fun4(10, 20);
	cout << t << endl;
	cout << a << " " << b << endl; 
}

/*
Lambda������º����Ĺ�ϵ
��C++11֮ǰ��STL�е�һЩ�㷨��Ҫʹ��һ�ֺ�������-- - �º���(functor)��
�䱾�������¶���ͳ�Ա����operator()��ʹ��ʹ���Ϻ�����ͨ������
��ʵ��ϸ�ĵ������Ѿ����֣�Lambda������º����ƺ���һЩĬ����
�º�����rate��ʼ��,Lambda��׽rate���������������ϣ�����һ�¡�
��ʵ�ϣ��º�������ʵ��Lambda����һ�ַ�ʽ��
������ͨ�����Lambda����ת��Ϊһ���ź������󣬵��Ƿº������﷨ȴ�����Ǵ����˺ܴ�ı�ݡ�
��C++11�У�Lambda�������㷺ʹ�ã��ܶ�º�����ȡ����
*/
class Price
{
private:
	float _rate;
public:
	Price(float rate) :_rate(rate) {}
	float operator()(float price)
	{
		return price*(1 - _rate / 100);
	}
};

void mainLa2()
{
	float rate = 5.5f;

	Price c1(rate);
	auto c2 = [rate](float price)->float {return price*(1 - rate / 100); };

	float p1 = c1(3699);    //�º���
	float p2 = c2(3699);    //Lambda����
}
/*
Lambda��static inline����
Lambda��������ʡ���ⲿ������static inline���������൱��һ���ֲ��������ֲ����������ڸ�������
�����ⲿ��static inline�������������Զ���ĺ꣬Lambda������û��ʵ������ʱ����������(��Ҳ�����)
����Lambda�����ɶ��Ը��á�
�����������󣬸�Lambda�����Ͳ��ٿ����ˣ�������Ⱦ�κ����ֿռ�.

����ֵ���ݲ�׽��mutable
 �����ᵽ��mutable ����ȡ��Lambda�ĳ������ԣ����ֵ������Ҫ�ں��������޸ľ�Ҫ��mutable
*/
void mainLa3()
{
	int j = 12;
	auto by_val = [=] {return j + 1; };
	auto by_ref = [&] {return j + 1; };
	cout << by_val() << endl;   //13
	cout << by_ref() << endl;   //13
	j++;
	cout << by_val() << endl;   //13
	cout << by_ref() << endl;   //14
}
/*
��������ӣ�j++��֮�����ֵ���ݽ��������12��ԭ���ǣ�ֵ����j����Ϊһ��������һ����ʼ����
�Ͳ������޸ģ�������Ϊ��һ���͸���������jͬ���ĳ��������������ò�׽�У�j��Ȼ�Ǹ��������е�ֵ��
��ʵһ��ֵ���ݵĵ�Lambdaת��Ϊ�ź����󣬻��Ϊһ��class�ĳ�����Ա������
*/
class const_val_lambda
{
public:
	const_val_lambda(int v) :val(v) {}
public:
	//void operator()()const { val = 3; } //����
private:
	int val;
};
/*
׼ȷ�ؽ�������C++11��׼�е�lambda�ȼ۵����г���operatorO�ķº�����
�����ʹ�ò�׽�б��ʱ�����ע�⣬��ֵ���ݷ�ʽ��׽�ı�����lambda�����в��ɸ��ĵĳ�����
��׼��ô��ƿ�����Դ������STL�㷨һЩ����ϵ�ȱ�ݣ��Էº���û�������ƣ��Ӷ�����һЩ��Ʋ����ر����õ��㷨������
����һ��ؽ��������������������ԣ��ı���������п�����������ʱ����ͨ���������κ����塣
�������ʱ����ʱ����ֻ������lambda���������룬�����Ҫ�������������ģ�
���ǿ���ʹ�����ã�����ͨ����lambda��������ֵ��ʵ�֡�
���⣬lambda������mutable���η����������䳣���ԣ�������ʵ����ֻ���ṩ��һ���﷨�ϵĿ����ԣ�
��ʵ��Ӧ��û�ж�����Ҫʹ��mutable��lambda�����ĵط���
�����ʱ������ʹ��Ĭ�ϰ汾�ģ���mutable����lambda����Ҳ���㹻�ˡ�
*/

/*
Lambda�����뺯��ָ��
Lambda���������Ǽ򵥵ĺ���ָ�����ͣ������Զ������ͣ�ÿ��Lambda���������һ���հ����͵���ʱ����(��ֵ)������C++11����Lambda��������ָ���ת����ǰ���ǣ�
Lambdaû�в�׽�κα���
����ָ����ʾ�ĺ���ԭ�ͣ������Lambda����ͬ�ĵ��÷�ʽ
*/
#include <memory>
#include <functional>

class A
{

};

void(*testDeleter1)(A *) { [](A *) {} };							//����ָ����ʽ
auto testDeleter2 = [](A *) {};										//lambda��ʽ
std::function<void(A*)> testDeleter3 = [](A *) {};		//std::function��ʽ

void main333()
{
	int a = 3, b = 4;

	auto total = [](int x, int y)->int {return x + y; };
	typedef int(*all)(int x, int y);
	typedef int(*one)(int x);

	all p;
	p = total;
	one q;
	//q = total;  //����,������һ��

	decltype(total) all_1 = total;
	//decltype(total) all_2 = p;  //����ָ���޷�ת��ΪLambda

	A *pa1 = new A(), *pa2 = new A(), *pa3 = new A(), *pa4 = new A();


	std::unique_ptr< A, decltype(testDeleter1) >  uptr1(pa1, testDeleter1);
	std::unique_ptr< A, decltype(testDeleter2) >  uptr2(pa2, testDeleter2);
	std::unique_ptr< A, decltype(testDeleter3) >  uptr3(pa3, testDeleter3);
	std::unique_ptr< A, std::function<void(A*)> > uptr4(pa4, testDeleter3);
}
/*
[]��lambda������������׽�б��ܹ���׽�������еı���������lambda����ʹ�ã�
[var] ��ʾ��ֵ���ݷ�ʽ��׽����var
[=] ��ʾֵ���ݲ�׽���и����������
[&var] ��ʾ�����ô��ݷ�ʽ��׽����var
[&] ��ʾ���ô��ݲ�׽���и����������
[this] ��ʾֵ���ݷ�ʽ��׽��ǰ��thisָ��
����һЩ��ϣ�
[=,&a] ��ʾ�����ô��ݷ�ʽ��׽a,ֵ���ݷ�ʽ��׽��������
ע�⣺
��׽�б���������ظ����ݣ��磺[=,a]��[&,&this]�����������ʱ�ڵĴ���
�����б�����ͨ�����Ĳ����б�һ�¡��������Ҫ���ݲ�������������ͬ()һͬ��ʡ�ԡ���
mutable ����ȡ��Lambda�ĳ������ԣ���ΪLambdaĬ����const���ԣ�multable��������Lamdba�������޸�ֵ���ݵı����������޸ĺ󲢲���Ӱ���ⲿ�ı�����
->�������������voidʱ��������->һ��ʡ�ԡ�������������ͺ���ȷ������ʡ�ԣ��ñ������Զ��Ƶ����͡�
 ���������ͨ����һ�������˿���ʹ�ò���֮�⣬������ʹ�ò���ı���
*/

