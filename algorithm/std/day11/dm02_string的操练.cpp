#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "string"
#include "algorithm"

//string�ĳ�ʼ��
void main2015()
{
	string s1 = "aaaa";
	string s2("bbbb");
	string s3 = s2; //ͨ���������캯�� ����ʼ������s3
	string s4(10, 'a');

	cout << "s1:" << s1 << endl;
	cout << "s2:" << s2 << endl;
	cout << "s3:" << s3 << endl;
	cout << "s4:" << s4 << endl;
}

//string�� ����
void main202()
{
	string s1 = "abcdefg";

	//1 ���鷽ʽ
	for (int i=0; i<s1.length(); i++)
	{
		cout << s1[i] << " ";
	}
	cout << endl;

	//2 ������
	for (string::iterator it = s1.begin(); it != s1.end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl;

	//3 at���� �� operator[] ������  -> �Ƿ�����Catch
	try
	{
		for (int i=0; i<s1.length() + 3; i++)
		{
			cout << s1.at(i) << " ";  //�׳��쳣
		}
	}
	catch ( ... )
	{
		cout << "�����쳣\n" ;
	}

	cout << "at֮��" << endl;
	
	//try
	//{
	//	for (int i=0; i<s1.length() + 3; i++)
	//	{
	//		cout << s1[i] << " "; //���ִ��� ���������׳��쳣��ֱ�����������ж� -�� ��Ϊoperator[]����������±������
	//	}
	//}
	//catch ( ... )
	//{
	//	cout << "�����쳣\n" ;
	//}
	
	
}

//�ַ�ָ���string��ת��
void main2013()
{
	string s1 = "aaabbbb";

	printf("s1:%s \n", s1.c_str());        //1 s1===>char *

	string s2("aaabbb");               	//2 char *====>sting 

	//3 s1������ copy buf��
	char buf1[128] = {0};
	s1.copy(buf1, 3, 0);  //��0��ʼ��3���ַ� ������buf1��ָ����ڴ档ע�� ֻ����copy3���ַ� ������C�����ַ���
	cout << "buf1:" << buf1 << endl; 
}

//�ַ����� ����
void main204()
{
	string s1 = "aaa";
	string s2 = "bbb";
	s1 = s1 + s2;           //ƴ�ӷ�1 operator+
	cout << "s1:" << s1 << endl;


	string s3 = "333";
	string s4 = "444";
	s3.append(s4);    	    //ƴ�ӷ�2 append����
	cout << "s3:" << s3 << endl;
}


//�ַ����Ĳ��Һ��滻
void main205()
{
	string s1 = "wbm hello wbm 111  wbm 222  wbm 333 ";
	//			 ��
	//��һ�� ����wbm index

	int index = s1.find("wbm", 0); //λ���±� ��0��ʼ
	cout << "index: " << index << endl;

	//����1 ��wbm���ֵĴ��� ÿһ�γ��ֵ������±�
	int offindex = s1.find("wbm", 0);
	while (offindex != string::npos)
	{
		cout << "offindex:" << offindex << endl;
		offindex = offindex + 1;
		offindex = s1.find("wbm", offindex); 
	}

	//����2  ��Сдwbm===>WBM  replace(pos,size,src)

	string s3 = "aaa  bbb ccc";
	s3.replace(0, 3, "AAA");
	cout << "s3" << s3 << endl;

	offindex = s1.find("wbm", 0);
	while (offindex != string::npos)
	{
		cout << "offindex:" << offindex << endl;
		s1.replace(offindex,3, "WBM");                      //���ҵ���wbm��index��ʼ��3���ֽ� �滻Ϊ WBM
		offindex = offindex + 1;
		offindex = s1.find("wbm", offindex); 
	}

	//����3 assign
	s3.assign(s1);
	cout << "s3�滻��Ľ��: " << s3 << endl;

	s3.assign(3, 'aaa');
	cout << "s3�滻��Ľ��: " << s3 << endl;
}


//erase �ضϣ�����ɾ����   ��   insert ����
void main206()
{
	//����ɾ��
	string s1 = "hello1 hello2 hello1";
	string::iterator it = find(s1.begin(), s1.end(), 'l');
	if (it != s1.end() )
	{
		s1.erase(it);
	}
	cout << "s1ɾ��l�Ժ�Ľ��:" << s1 << endl;

	s1.erase(s1.begin(), s1.end() );
	cout << "s1ȫ��ɾ��:" << s1 << endl;
	cout << "s1���� " << s1.length() << endl;

	//�������
	string s2 = "BBB";
	s2.insert(0, "AAA");             // ͷ�巨
	s2.insert(s2.length(), "CCC");   //  β�巨
	cout << s2 << endl;
}

//���ַ���������
void main207()
{
	string s1 = "AAAbbb";
	transform(s1.begin(), s1.end(),s1.begin(), toupper);      //1��������ڵ�ַ 2�������� 3����ָ ---->>> ��[begin,end)�����ڵ�Ԫ�أ��������������ŵ�to
	cout << "s1" << s1 << endl;

	string s2 = "AAAbbb";
	transform(s2.begin(), s2.end(), s2.begin(), tolower);
	cout << "s2:" << s2 << endl;

}

//�Ƚ�
void main208()
{
	string s1 = "aaaaaa";
	string s2 = "aaaaab";
	const char* c3 = "aaaaab";

	int ret = s1.compare(s2);
	cout << "s1 = " << s1 << " s2 = " << s2 <<"    ret = "<< ret<<endl;

	ret = s1.compare(2,s1.size()-2,s2);    //��2��ʼ��s1.size()-2���ַ�
	cout << "s1 = " << s1 << " s2 = " << s2 << "    ret = " << ret << endl;

	ret = s1.compare(2, s1.size() - 2, s2,2,s2.size()-2);    //��2��ʼ��s1.size()-2���ַ� VS ��2��ʼ��s2 size-2���ַ�
	cout << "s1 = " << s1 << " s2 = " << s2 << "    ret = " << ret << endl;

	ret = s1.compare(c3);
	cout << "s1 = " << s1 << " c3 = " << string(c3) << "    ret = " << ret << endl;
}

//�Ӵ� substr
void main209()
{
	string s1 = "hello world!";

	string sub1 = s1.substr(0, 5);
	cout << "sub1 = " << sub1 << endl;

	string sub2 = s1.substr(1, 2);
	cout << "sub2 = " << sub2 << endl;
}
