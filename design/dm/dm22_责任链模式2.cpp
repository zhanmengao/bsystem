
#include <iostream>
using namespace std;

/*
struct Teacher
{
	char name;
	int age;
	Teacher *next;
};
*/

//���공 �Ժ�,��Ҫ������ ������ȥ
class CarHandle
{
public:
	virtual void HandleCar() = 0;
	CarHandle *setNextHandle(CarHandle *handle)
	{
		m_handle = handle;
		return m_handle;
	}
	
protected:
	CarHandle *m_handle;  //��һ������Ԫ
};

class HeadCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "�쳵ͷ" << endl;
		//�� �泵ͷ �Ժ� ������ݽ��� ��һ��������
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}
	}
};

class BodyCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "�� ����" << endl;
		//�� �� ���� �Ժ� ������ݽ��� ��һ��������
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}

	}
};

class TailCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "�쳵β" << endl;
		//�� �쳵β �Ժ� ������ݽ��� ��һ��������
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}
	}
};


void main()
{
	CarHandle *headHandle = new HeadCarHandle;
	CarHandle *bodyHandle = new BodyCarHandle;
	CarHandle *tailHandle = new TailCarHandle;

	//����Ĵ����ϵ
	/*
	headHandle->setNextHandle(bodyHandle);
	bodyHandle->setNextHandle(tailHandle);
	tailHandle->setNextHandle(NULL);
	*/

		
	headHandle->setNextHandle(tailHandle);
	tailHandle->setNextHandle(bodyHandle);
	bodyHandle->setNextHandle(NULL);
	
	headHandle->HandleCar();

	/*
	//ҵ���߼� д���ڿͻ�����..
	headHandle->HandleCar();
	bodyHandle->HandleCar();
	tailHandle->HandleCar();
	*/

	delete headHandle;
	delete bodyHandle;
	delete tailHandle;

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}