
#include <iostream>
using namespace std;
#include "list"

class Doctor
{
public:
	void treat_eye()
	{
		cout << "ҽ�� ���� �ۿƲ�" << endl;
	}

	void treat_nose()
	{
		cout << "ҽ�� ���� �ǿƲ�" << endl;
	}
};

class Command
{
public:
	virtual void treat() = 0;
};
class CommandTreatEye : public Command
{
public:
	CommandTreatEye(Doctor *doctor)
	{
		m_doctor = doctor;
	}
	void treat()
	{
		m_doctor->treat_eye();
	}
private:
	Doctor *m_doctor;
};


class CommandTreatNose : public Command
{
public:
	CommandTreatNose(Doctor *doctor)
	{
		m_doctor = doctor;
	}
	void treat()
	{
		m_doctor->treat_nose();
	}
private:
	Doctor *m_doctor;
};


class BeautyNurse
{
public:
	BeautyNurse(Command *command)
	{
		this->command = command;
	}
public:
	void SubmittedCase() //�ύ���� �µ�����
	{
		command->treat();
	}
protected:
private:
	Command *command;
};

//��ʿ��
class HeadNurse 
{
public:
	HeadNurse()
	{
		m_list.clear();
	}
	
public:
	void setCommand(Command *command)
	{
		m_list.push_back(command);
	}
	void SubmittedCase() //�ύ���� �µ�����
	{
		for (list<Command *>::iterator it=m_list.begin(); it!=m_list.end(); it++)
		{
			(*it)->treat();
		}
	}
private:
	list<Command *> m_list;
};



void main21_1()
{
	//1 ҽ��ֱ�ӿ���
	/*
	Doctor *doctor = new Doctor ;
	doctor->treat_eye();
	delete doctor;
	*/

	//2 ͨ��һ������ ҽ��ͨ�� ���� ���� �β�
	Doctor *doctor = new Doctor ;
	Command * command = new CommandTreatEye(doctor); //shift +u //תСд shift+ctl + uת��д
	command->treat();
	delete command;
	delete doctor;
	return ;
}


void main21_2()
{
	//3 ��ʿ�ύ���� �����Ͽ���
	BeautyNurse		*beautynurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command  = NULL;

	doctor = new Doctor ;

	//command = new CommandTreatEye(doctor); //shift +u //תСд shift+ctl + uת��д
	command = new CommandTreatNose(doctor); //shift +u //תСд shift+ctl + uת��д
	beautynurse = new BeautyNurse(command);
	beautynurse->SubmittedCase();
	
	delete doctor;
	delete command;
	delete beautynurse;
	return ;
}


//4 ͨ����ʿ�� �������µ�����
void main21_3()
{
	//��ʿ�ύ���� �����Ͽ���
	HeadNurse		*headnurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command1  = NULL;
	Command			*command2  = NULL;

	doctor = new Doctor ;

	command1 = new CommandTreatEye(doctor); //shift +u //תСд shift+ctl + uת��д
	command2 = new CommandTreatNose(doctor); //shift +u //תСд shift+ctl + uת��д

	headnurse = new HeadNurse(); //new ��ʿ��
	headnurse->setCommand(command1);
	headnurse->setCommand(command2);

	headnurse->SubmittedCase(); // ��ʿ�� �����µ�����

	delete doctor;
	delete command1;
	delete command2;
	delete headnurse;
	return ;
}


void main()
{
	//main21_1();
	//main21_2();
	main21_3();
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}