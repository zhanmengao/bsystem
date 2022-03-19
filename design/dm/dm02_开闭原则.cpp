
#include <iostream>
using namespace std;

class BankWorker
{
public:
	void save()
	{
		cout << "���" << endl;
	}

	void moveM()
	{
		cout << "ת��" << endl;
	}

	void jiaofei()
	{
		cout << "����" << endl;
	}
protected:
private:
};


class AbBankWorker
{
public:
	virtual void dothing() = 0;
};

class  SaveBanker : public AbBankWorker
{
public:
	virtual void dothing()
	{
		cout << "���" << endl;
	}
};


class  MoveBanker : public AbBankWorker
{
public:
	virtual void dothing()
	{
		cout << "ת��" << endl;
	}
};

//11:54
class  AdvMoveBanker : public MoveBanker
{
public:
	virtual void dothing()
	{
		cout << "���� ת��" << endl;
	}
};

//14:43
class  AdvAdvMoveBanker : public MoveBanker
{
public:
	virtual void dothing()
	{
		cout << " �Զ��� ���� ת��" << endl;
	}
};

void main02_01()
{
	BankWorker *bw = new BankWorker ;
	bw->jiaofei();
	bw->moveM();
	bw->save();
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}




//��ܺ��� //11:50
void howDo(AbBankWorker *bw)
{
	bw->dothing(); //�ж�̬����
}

void main02_02()
{
	AbBankWorker *bw  = NULL;
	bw = new MoveBanker;
	bw->dothing(); //�ж�̬����
	delete bw;

	bw = new SaveBanker;
	bw->dothing();
	delete bw;
	return ;
}


void main02_03()
{

	AbBankWorker *bw  = NULL;
	bw = new MoveBanker;
	howDo(bw);
	delete bw;

	bw = new SaveBanker;
	howDo(bw);
	delete bw;

	bw = new AdvMoveBanker;
	howDo(bw);
	delete bw;

	bw = new AdvAdvMoveBanker;  
	howDo(bw);  //����չ  ���ú�����д�Ĵ���
	delete bw;
}
void main2222()
{
	//main02_01();
	main02_02();
	main02_03();
	system("pause");
}