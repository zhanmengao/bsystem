
#include <iostream>
using namespace std;

class Subject
{
public:
	virtual void sailbook() = 0;
};

class RealSubjectBook : public Subject
{
public:
	virtual void sailbook()
	{
		cout << "����" << endl;
	}
};

//a�а���b�ࣻa��b��ʵ��Э����protocol 
class dangdangProxy : public Subject
{
public:
	virtual void sailbook()
	{
		RealSubjectBook *rsb = new RealSubjectBook;
		dazhe();
		rsb->sailbook();
		dazhe();
	}
public:
	void dazhe()
	{
		cout << "˫ʮһ����" << endl;
	}
private:
	Subject *m_subject;
};

void main()
{
	Subject *s = new dangdangProxy;
	s->sailbook();
	delete s;
	system("pause");
	return ;
}