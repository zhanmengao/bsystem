#include<iostream>
using namespace std;
void mainStream()
{
	//good:��δ���ڴ��� bad:���ѱ��� fail:һ��Io����ʧ�� eof:�������ļ�����
	cout <<"cin state:"<<cin.good() << cin.bad() << cin.eof() << cin.fail() << endl;
	auto old_state = cin.rdstate();			 //��סcin�ĵ�ǰ״̬
	cin.clear();										 //ʹcin��Ч
	cin.setstate(old_state);						//��cin��λ

	//��׼�⽫cin��cout�����������κδ�cin��ȡ���ݵĲ�������ˢ��cout��
	cin.tie(nullptr);                          //ȡ������
	cin.tie(&cout);                          //���¹���
	//����֮�⣺
	cout << "hi!" << endl;				 //endl������в���ˢ�»�����
	cout << "hi!" << flush;				 //ˢ�»����������κθ����ַ�
	cout << "hi!" << ends;			 //ends����ո�ˢ�»�����
	cout << unitbuf;						 //����cout�޻�����
	cout << nounitbuf;					 //�ص������Ļ��巽ʽ


}