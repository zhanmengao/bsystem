
#include <iostream>
using namespace std;
#include "vector"
//���ߺ���
void printV(vector<int> &v);
typedef struct
{
	unsigned char  bLength;                   //���������ṹ��С
	unsigned char  bDescriptorType;           //�ӿ������������ͱ��(0x04)
	unsigned char  bInterfaceNumber;          //�ӿڵı��
	unsigned char  bAlternateSetting;         //���ñ�ţ��������Ϊ�ӿڵ����úš�һ���ӿ�ֻ��һ��������Ч��
	unsigned char  bNumEndpoints;             //�ýӿ�ʹ�õĶ˵������������˵�0
	unsigned char  bInterfaceClass;           //�ӿ�����
	unsigned char  bInterfaceSubClass;        //�ӿ�������
	unsigned char  bInterfaceProtocol;        //�ӿ���ѭ��Э��
	unsigned char  iInterface;                //�����ýӿڵ��ַ�������ֵ
} USB_INTERFACE_DESCRIPTOR;

//����Ԫ�ص� ��Ӻ�ɾ��
void mainVV3()
{
	vector<int> v1;

	cout << "length:" << v1.size() <<endl;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	cout << "length:" << v1.size() <<endl;

	cout << "ͷ��Ԫ��" << v1.front() <<"    β��Ԫ��" <<v1.back()<<endl;


	//�޸� ͷ��Ԫ��
	//��������ֵ����ֵ Ӧ�÷���һ������
	v1.front() = 11;
	v1.back() = 55;

	while (v1.size() > 0)
	{
		cout <<"β��Ԫ��" << v1.back() ; //��ȡβ��Ԫ��
		v1.pop_back(); //ɾ��β��Ԫ��
	}

	vector<USB_INTERFACE_DESCRIPTOR> interfacevec;
	for (int i = 0; i < 5; i++)
	{
		USB_INTERFACE_DESCRIPTOR info;
		interfacevec.push_back(info);
	}
}

//vector�ĳ�ʼ��
void main3012()
{
	vector<int> v1;              //Ĭ�Ϲ���
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);

	vector<int> v2 = v1;  //��������

	vector<int> v3(v1.begin(), v1.begin()+2 );     //ͨ������������

	vector<int> v4{ 0, 2, 5, 8, 2 };              //ͨ����ֵ�б���

	v4 = { 3, 3, 5, 6, 98 };

	vector<int> v5(10, 1);                      //����һ��size=10������ ���ڲ���ֵ��Ϊ1
}



//vector�ı��� ͨ������ķ�ʽ 
void main3013()
{
	vector<int> v1(10);   //��ǰ���ڴ�׼���ã����ڲ���ֵ��Ϊ���ֵ

	for (int i=0; i<10; i++)
	{
		v1[i] = i + 1;
	}

	printV(v1);

}

//push_back��ǿ������
void main304() 
{
	vector<int> v1(10);   //��ǰ���ڴ�׼����
	v1.push_back(100);    //��β������-> index=10
	v1.push_back(200);    //index = 11
	cout << "size: " << v1.size() << endl;         //12
	printV(v1);
}


//1������ end()����� 
//   1	3	5
//	 ��	
//	          ��
//�� it == v1.end()��ʱ�� ˵����������Ѿ����������...
//end()��λ�� Ӧ���� 5�ĺ���

//2 ������������
/*
typedef iterator pointer;                                                        ���������
typedef const_iterator const_pointer;                                            const������
typedef _STD reverse_iterator<iterator> reverse_iterator;                        ���������
typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;           const���������
*/
void main3015()
{
	vector<int> v1(10);   
	for (int i=0; i<10; i++)
	{
		v1[i] = i + 1;
	}

	//�������
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it ++ )
	{
		cout << *it << " ";
	}

	//�������
	for (vector<int>::reverse_iterator rit = v1.rbegin(); rit!=v1.rend(); rit++ )
	{
		cout << *rit << " ";
	}

}

//vector  ɾ��
void main306()
{
	vector<int> v1(10);   
	for (int i=0; i<10; i++)
	{
		v1[i] = i + 1;
	}

	//����ɾ��
	v1.erase(v1.begin(), v1.begin()+3);
	printV(v1);

	//����Ԫ�ص�λ�� ָ��λ��ɾ��
	v1.erase(v1.begin()); //��ͷ��ɾ��һ��Ԫ��
	printV(v1);

	cout << endl;

	//����Ԫ�ص�ֵ 
	v1[1] = 2;
	v1[3] = 2;
	printV(v1);

	for (vector<int>::iterator it =v1.begin(); it != v1.end();)
	{
		if (*it == 2)
		{
			it = v1.erase(it);  //�� ɾ����������ָ���Ԫ�ص�ʱ��,eraseɾ����������it�Զ����ƶ�
		}
		else
		{
			it ++;
		}
	}
	printV(v1);

	cout << endl;
	v1.insert(v1.begin(), 100);
	v1.insert(v1.end(), 200);
	printV(v1);
}

//reserve �����ڲ������С
void main307()
{
	vector<int> v1;
	v1.reserve(50);
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
		cout << "����" << i << "����ʱ=" << v1.capacity() << "��v1.size()=" << v1.size() << "  v1��ַΪ" <<
			&v1 << endl;
	}
}