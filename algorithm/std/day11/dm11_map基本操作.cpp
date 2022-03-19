
#include <iostream>
using namespace std;
#include "map"
#include "string"


//mapԪ�ص����/����/ɾ����������
void main1101()
{
	map<int, string> map1;

	//����1
	map1.insert(pair<int, string>(1,"teacher01") );
	map1.insert(pair<int, string>(2,"teacher02") );
	
	

	//����2 
	map1.insert(make_pair(3, "teacher04") );
	map1.insert(make_pair(4, "teacher05") );

	//����3 
	map1.insert(map<int, string>::value_type(5, "teacher05") );
	map1.insert(map<int, string>::value_type(6, "teacher06") );

	//����4
	map1[7] = "teacher07";
	map1[8] = "teacher08";

	//��ĩβ���
	if (map1.find(3) == map1.end())
	{
		map1[3] = "aaa";
	}
	
	//map1['z'] = "teacher08";

	//�����ı���
	for (map<int, string>::iterator it = map1.begin(); it!=map1.end(); it++ )
	{
		cout << it->first << "\t" << it->second << endl;
	}
	cout << "��������" << endl;


	//����Ԫ�ص�ɾ��
	while (!map1.empty())
	{
		map<int, string>::iterator it = map1.begin();
		cout << it->first << "\t" << it->second << endl;
		map1.erase(it);
	}
	for (map<int, string>::iterator it = map1.begin(); it != map1.end();)
	{
		cout << it->first << "\t" << it->second << endl;
		it = map1.erase(it);
	}
}

//��������ַ��� ��ͬ
//ǰ���ַ��� ����ֵΪpair<iterator,bool>	��key�Ѿ����� �򱨴�
//	������									��key�Ѿ�����,���޸�									
void main1102()
{
	map<int, string> map1;

	//��鷵��ֵ

	//����1
	pair<map<int, string>::iterator, bool>  mypair1 =  map1.insert(pair<int, string>(1,"teacher01") );
	map1.insert(pair<int, string>(2,"teacher02") );

	//����2 
	pair<map<int, string>::iterator, bool>  mypair3 = map1.insert(make_pair(3, "teacher04") );
	map1.insert(make_pair(4, "teacher05") );

	//����3 
	pair<map<int, string>::iterator, bool>  mypair5 = map1.insert(map<int, string>::value_type(5, "teacher05") );
	if (mypair5.second != true)
	{
		cout << "key 5 ����ʧ��" << endl;
	}
	else
	{
		cout << mypair5.first->first << "\t" <<  mypair5.first->second <<endl;
	}

	
	pair<map<int, string>::iterator, bool>  mypair6 =  map1.insert(map<int, string>::value_type(5, "teacher55") );
	if (mypair6.second != true)
	{
		cout << "key 6 ����ʧ��" << endl;
	}
	else
	{
		cout << mypair6.first->first << "\t" <<  mypair6.first->second <<endl;
	}

	//����4
	map1[7] = "teacher07";
	map1[7] = "teacher77";

	//�����ı���
	for (map<int, string>::iterator it = map1.begin(); it!=map1.end(); it++ )
	{
		cout << it->first << "\t" << it->second << endl;
	}
	cout << "��������" << endl;

}

void main1103()
{
	map<int, string> map1;
	map1.insert(pair<int, string>(1,"teacher01") );
	map1.insert(pair<int, string>(2,"teacher02") );
	map1.insert(make_pair(3, "teacher04") );
	map1.insert(make_pair(4, "teacher05") );
	map1.insert(map<int, string>::value_type(5, "teacher05") );
	map1.insert(map<int, string>::value_type(6, "teacher06") );
	map1[7] = "teacher07";
	map1[8] = "teacher08";

	//map�Ĳ��� //�쳣����
	map<int, string>::iterator it2 = map1.find(100);
	if (it2 == map1.end())
	{
		cout << "key 100 ��ֵ ������" << endl;
	}
	else
	{
		cout << it2->first << "\t" << it2->second << endl;
	}

	//equal_range //�쳣����
	pair<map<int, string>::iterator , map<int, string>::iterator> mypair = map1.equal_range(5); //�������������� �γ�һ�� pair
	//��һ�������� >= 5�� λ�� 
	//��һ�������� = 5�� λ�� 
	if (mypair.first == map1.end() )
	{
		cout << "��һ�������� >= 5�� λ�� ������" << endl;
	}
	else
	{
		cout << mypair.first->first << "\t" << mypair.first->second << endl;
	}
	//ʹ�õڶ���������
	if (mypair.second == map1.end() )
	{
		cout << "�ڶ��������� > 5�� λ�� ������" << endl;
	}
	else
	{
		cout << mypair.second->first << "\t" << mypair.second->second << endl;
	}
}