
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"
#include "iterator"  //�������������ͷ�ļ�
#include<numeric>
#include "map"
#include "deque"

class Speaker
{
public:
	string	m_name;
	int		m_score[3];
};

//����ѡ��
int GenSpeaker(map<int, Speaker> &mapSpeaker, vector<int> &v)
{
	string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	random_shuffle(str.begin(), str.end());

	for (int i=0; i<24; i++)
	{
		Speaker tmp;
		tmp.m_name = "ѡ��";
		tmp.m_name = tmp.m_name + str[i];
		mapSpeaker.insert(pair<int, Speaker>(100+i, tmp) );
	}

	for (int i=0; i<24; i++)
	{
		v.push_back(100+i); //�μӱ�������Ա
	}
	return 0;
}

//ѡ�ֳ�ǩ
int speech_contest_draw(vector<int> &v)
{
	random_shuffle(v.begin(), v.end());
	return 0;
}


//ѡ�ֱ���
int speech_contest(int index, vector<int> &v1, map<int, Speaker> &mapSpeaker, vector<int> &v2 )
{
	//С��ı����÷� ��¼����;���ǰ���� ��3��
	multimap<int, int, greater<int>> multmapGroup; //С��ɼ�
	int tmpCount = 0;

	for (vector<int>::iterator it=v1.begin(); it!=v1.end(); it++ )
	{
		tmpCount ++;
		//���
		{
			deque<int> dscore;
			for (int j=0; j<10; j++) //10����ί���
			{
				int score = 50+rand()%50;
				dscore.push_back(score);
			}
			sort(dscore.begin(), dscore.end());
			dscore.pop_back();
			dscore.pop_front(); //ȥ����ͷ� ��߷�

			//��ƽ����
			int scoresum = accumulate(dscore.begin(), dscore.end(), 0);
			int scoreavg = scoresum/dscore.size();
			mapSpeaker[*it].m_score[index] = scoreavg; //ѡ�ֵ÷� ����������
			multmapGroup.insert(pair<int ,int>(scoreavg, *it ) );
		}

		//�������
		if (tmpCount % 6 == 0)
		{
			cout << "С��ı����ɼ�" << endl;
			for (multimap<int, int, greater<int>>::iterator mit=multmapGroup.begin(); mit!=multmapGroup.end(); mit++ )
			{
				//���  ����  �÷�
				cout << mit->second << "\t" << mapSpeaker[mit->second].m_name << "\t" << mit->first << endl; 
			}

			//ǰ��������
			while (multmapGroup.size() > 3)
			{
				multimap<int, int, greater<int>>::iterator it1 = multmapGroup.begin();
				v2.push_back(it1->second); //��ǰ���� �ŵ�v2 �������� ��
				multmapGroup.erase(it1);
			}

			multmapGroup.clear(); //�����С�� �����ɼ�
		}
	
	}
	return 0;
};

//�鿴�������
int speech_contest_print(int index, vector<int> &v, map<int, Speaker> &mapSpeaker)
{
	printf("��%d�� ��������\n", index+1);
	for (vector<int>::iterator it=v.begin(); it!=v.end(); it++)
	{
		cout << "�������: " << *it << "\t" << mapSpeaker[*it].m_name << "\t" << mapSpeaker[*it].m_score[index] << endl;
	}
	return 0;
};

void main1()
{
	//��������� 
	map<int, Speaker>	mapSpeaker; //�μӱ�����ѡ��
	vector<int>			v1; //��1�� �ݽ����� ����
	vector<int>			v2; //��2�� �ݽ����� ����
	vector<int>			v3; //��3�� �ݽ����� ����
	vector<int>			v4; //���ǰ���� �ݽ����� ����

	//����ѡ�� �õ���һ��ѡ�ֵı�������
	GenSpeaker(mapSpeaker, v1);

	//��1�� ѡ�ֳ�ǩ ѡ�ֱ��� �鿴�������
	cout << "\n\n\n�����,��ʼ��1�ֱ���" << endl;
	cin.get();
	speech_contest_draw(v1);
	speech_contest(0, v1, mapSpeaker, v2);
	speech_contest_print(0, v2, mapSpeaker);

	//��2�� ѡ�ֳ�ǩ ѡ�ֱ��� �鿴�������
	cout << "\n\n\n�����,��ʼ��2�ֱ���" << endl;
	cin.get();
	speech_contest_draw(v2);
	speech_contest(1, v2, mapSpeaker, v3);
	speech_contest_print(1, v3, mapSpeaker);

	//��3�� ѡ�ֳ�ǩ ѡ�ֱ��� �鿴�������
	cout << "\n\n\n�����,��ʼ��3�ֱ���" << endl;
	cin.get();
	speech_contest_draw(v3);
	speech_contest(2, v3, mapSpeaker, v4);
	speech_contest_print(2, v4, mapSpeaker);
}