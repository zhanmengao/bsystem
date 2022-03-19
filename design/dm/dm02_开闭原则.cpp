
#include <iostream>
using namespace std;

class BankWorker
{
public:
	void save()
	{
		cout << "存款" << endl;
	}

	void moveM()
	{
		cout << "转账" << endl;
	}

	void jiaofei()
	{
		cout << "交费" << endl;
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
		cout << "存款" << endl;
	}
};


class  MoveBanker : public AbBankWorker
{
public:
	virtual void dothing()
	{
		cout << "转账" << endl;
	}
};

//11:54
class  AdvMoveBanker : public MoveBanker
{
public:
	virtual void dothing()
	{
		cout << "批量 转账" << endl;
	}
};

//14:43
class  AdvAdvMoveBanker : public MoveBanker
{
public:
	virtual void dothing()
	{
		cout << " 自动化 批量 转账" << endl;
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




//框架函数 //11:50
void howDo(AbBankWorker *bw)
{
	bw->dothing(); //有多态发生
}

void main02_02()
{
	AbBankWorker *bw  = NULL;
	bw = new MoveBanker;
	bw->dothing(); //有多态发生
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
	howDo(bw);  //可扩展  调用后来人写的代码
	delete bw;
}
void main2222()
{
	//main02_01();
	main02_02();
	main02_03();
	system("pause");
}