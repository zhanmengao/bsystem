
#include <iostream>
using namespace std;
#include <iomanip>

/*

��׼���������cout
	cout.flush()
	cout.put()
	cout.write()
	cout.width()
	cout.fill()
	cout.setf(���)
	*/

/*
manipulator(�����������Ʒ�)
flush
endl
oct
dec
hex
setbase
setw
setfill
setprecision
��
*/

//put:д��һ���ַ�
//write��д��ָ�����ȵ��ֽ�����
void main801()
{
	cout << "hello" << endl;
	cout.put('h').put('e').put('l').put('\n');
	char *p = "hello itcast";

	cout.write(p, strlen(p)) << endl;
	cout.write(p, strlen(p) - 4) << endl;
	cout.write(p, strlen(p) + 4) << endl;
}

void main802()
{
	//ʹ�����Ա����
	cout << "<start>";
	cout.width(30);           //ָ����
	cout.fill('*');          //��*����������
	cout.setf(ios::showbase);            //��������־λ
	cout << hex << 123 << "<End>\n";

	cout << endl << endl;

	//ʹ�ÿ��Ʒ�
	cout << "<Start>" 
		<< setw(30) 
		<< setfill('*') 
		<< setiosflags(ios::showbase) //����
		<< setiosflags(ios::internal)
		<< hex
		<< 123
		<< "<End>\n"
		<< endl;
}



int main803()
{
	int a;
	cout<<"input a:";
	cin>>a;
	cout<<"dec:"<<dec<<a<<endl;                //��ʮ������ʽ�������
	cout<<"hex:"<<hex<<a<<endl;               //��ʮ��������ʽ�������a
	cout<<"oct:"<<setbase(8)<<a<<endl;        //�԰˽�����ʽ�������a
	char *pt="China";                         //ptָ���ַ���"China"
	cout<<setw(10)<<pt<<endl;                 //ָ�����Ϊ,����ַ���
	cout<<setfill('*')<<setw(10)<<pt<<endl;   //ָ�����,����ַ���,�հ״���'*'���
	double pi=22.0/7.0;                       //����piֵ

	//��ָ����ʽ���,8λС��
	cout<<setiosflags(ios::scientific)<<setprecision(8);
	cout<<"pi="<<pi<<endl;                      //���piֵ
	cout<<"pi="<<setprecision(4)<<pi<<endl;     //��ΪλС��
	cout<<"pi="<<setiosflags(ios::fixed)<<pi<<endl; //��ΪС����ʽ���
	return 0;
}


int main804( )
{
	double a=123.456,b=3.14159,c=-3214.67;
	cout<<setiosflags(ios::fixed)<<setiosflags(ios::right)<<setprecision(2);
	cout<<setw(10)<<a<<endl;
	cout<<setw(10)<<b<<endl;
	cout<<setw(10)<<c<<endl;
	return 0;
}
