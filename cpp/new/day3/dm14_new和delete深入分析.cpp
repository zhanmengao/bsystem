
#include <iostream>
using namespace std;

// 1
//		malloc	free				c���Եĺ���
//		new		delete ������	c++���﷨

//2		new �������ͱ���  �����������  ���������

//3		

////�����������
void main1401()
{
	// 
	int *p = (int *)malloc(sizeof(int));
	*p = 10;
	//free(p);
	delete p;

	int *p2 = new int; //�����������
	*p2 = 20;
	free(p2);
	//
	int *p3 = new int(30);
	printf("*p3:%d \n", *p3);
	//delete p3;
	free(p3);

	cout << "hello..." << endl;
	system("pause");
	return;
}

//�����������
void main1402()
{
	//c���Է�������
	int *p = (int *)malloc(sizeof(int) * 10);  //int array[10];
	p[0] = 1;
	//free(p);
	delete[] p;

	//c++�������� 
	int *pArray = new int[10];
	pArray[1] = 2;
	//delete [] pArray; //���鲻Ҫ��[] ����
	free(pArray);

	char *pArray2 = new char[25]; //char buf[25]
	delete[] pArray2;


	cout << "hello..." << endl;
	system("pause");
	return;
}

class Test14
{
public:
	Test14(int _a)
	{
		a = _a;
		cout << "���캯��ִ��" << endl;
	}

	~Test14()
	{
		cout << "��������ִ��" << endl;
	}

protected:
private:
	int a;
};

//�������new delete
//��ͬ �� ��ͬ�ĵط� new��ִ�����͹��캯��   delete������ ��ִ�������������

//	malloc 	free���� C
//1 new 	delete �������� c++�Ĺؼ���
//����
void main1403()
{
	//c 
	Test14 *pT1 = (Test14 *)malloc(sizeof(Test14));
	free(pT1);//��ִ����������
	//delete pT1; 

	//c++
	Test14 *pT2 = new Test14(10);
	delete pT2;
	//free(pT2);
}