#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef vector<bool>::iterator BoolIterator;

class IGate
{
public:
	virtual void input(bool in) = 0; //�������룬֧�ֶ�����
	virtual void reset() = 0;   //��������
	virtual bool output() = 0;//�������
};

class GatesBase : public IGate
{
public:
	GatesBase(){};
	~GatesBase(){};
	virtual void input(bool in) //����
	{
		this->_input.push_back(in);
	}
	virtual void reset()
	{
		this->_input.clear();
	}
	virtual bool output() = 0;  //Ŀǰ�޷�ʵ��

protected:
	bool checkInput(int min)  //�������߼��Ŵ������С�������
	{
		if (this->_input.size() < min)
		{
			cout << _name << "������Ҫ"<< min << "�����룡" << endl;
			return false;
		}
		return true;
	}
	bool checkInput(int min,int max)  //�������߼��Ŵ������С���������������
	{
		if (this->_input.size() < min )
		{
			cout << _name << "������Ҫ" << min << "�����룬�����Ҫ" << max <<"�����룡" << endl;
			return false;
		}
		return true;
	}
	vector<bool> _input;
	string _name;
};

//����ģ����
class AndGate :public GatesBase
{
public:
	AndGate(){ _name = "ADN Gate"; };
	virtual bool output()
	{
		if (checkInput(2))
		{
			return false;
		}
		bool out;
		BoolIterator it = this->_input.begin();
		out = *it;
		while( it != this->_input.end())
		{
			out = *it  && out;
			it++;
		}
		return out;
	}
};

class OrGate : public GatesBase
{
public:
	OrGate() { _name = "OR Gate"; }
	~OrGate();
	virtual bool output()
	{
		if (checkInput(2))
		{
			return false;
		}
		bool out;
		BoolIterator it = this->_input.begin();
		out = *it;
		while (it != this->_input.end())
		{
			out = *it  || out;
			it++;
		}
		return out;
	}
};

class NotGate : public GatesBase
{
public:
	NotGate(){ _name = "NOT Gate"; };
	~NotGate(){};
	virtual bool output()
	{
		if (checkInput(1,1))
		{ 
			return false;
		}
		return !this->_input[0];
	}
};

class NandGate :public GatesBase
{
public:
	NandGate(){ _name = "NAND Gate"; };
	~NandGate(){};
	virtual bool output()
	{
		if (checkInput(1))
		{
			return false;
		}
		bool out;
		BoolIterator it = this->_input.begin();
		out = *it;
		while (it != this->_input.end())
		{
			out = *it || out;
			it++;
		}
		return !out;
	}

};

class XnorGate :public GatesBase
{
public:
	XnorGate(){ _name = "XNOR Gate"; }
	virtual bool output()
	{
		if (checkInput(2,2))
		{
			return false;
		}
		bool ia = this->_input[0];
		bool ib = this->_input[1];
		//���ĸ���������ʵ��
		NandGate na1;
		//��һ��
		for (int i = 0; i < this->_input.size(); i++)
		{
			na1.input(this->_input[i]);
		}
		bool out1 = na1.output();

		//�ڶ���
		na1.reset();
		na1.input(ia);
		na1.input(out1);
		out1 = na1.output();

		na1.reset();
		na1.input(ib);
		na1.input(out1);
		bool out2 = na1.output();
		//������
		na1.reset();
		na1.input(out1);
		na1.input(out2);
		return na1.output();
	}
};