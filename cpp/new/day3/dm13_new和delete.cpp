
#include <iostream>
using namespace std;

// 1
//		malloc	free				c���Եĺ���
//		new		delete 			������	c++���﷨

//2		new �������ͱ���  �����������  ���������

//3		

////�����������
void main1301()
{
	// 
	int *p = (int *)malloc(sizeof(int));
	*p = 10;
	free(p);

	int *p2 = new int; //�����������
	*p2 = 20;
	free(p2);
	//
	int *p3 = new int(30);
	printf("*p3:%d \n", *p3);
	delete p3;

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//�����������
void main1302()
{
	//c���Է�������
	int *p = (int *)malloc(sizeof(int) * 10);  //int array[10];
	p[0] = 1;
	free(p);

	//c++�������� 
	int *pArray = new int[10] ;
	pArray[1] = 2;
	delete [] pArray; //���鲻Ҫ��[] ����

	char *pArray2 = new char[25] ; //char buf[25]
	delete [] pArray2;
	
	return ;
}

class Test13
{
public:
	Test13(int _a)
	{
		a = _a;
		cout<<"���캯��ִ��" <<endl;
	}

	~Test13()
	{
		cout<<"��������ִ��" <<endl;
	}

protected:
private:
	int a;
};

//�������new delete
//��ͬ �� ��ͬ�ĵط� new��ִ�����͹��캯��   delete������ ��ִ�������������
void main1303()
{
	//c 
	Test13 *pT1 = (Test13 *)malloc(sizeof(Test13));
	free(pT1);

	//c++
	Test13 *pT2 = new Test13(10);
	delete pT2;

}