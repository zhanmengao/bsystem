/*
1��ʹ��STL map��������ͳ��һƪӢ�������е��ʳ��ֵ�Ƶ�ʲ����ͳ�ƽ����
��ʹ��C++��׼���е�fstream�����ļ���#include<fstream>,��ϸʹ�÷������ĵ���
*/

#include<iostream>
#include "ArtStatistic.h"   //���µ�������ͳ��������ͷ�ļ�
using namespace std;

int main(int argc, char* argv[])
{
	ArtStatistic as;
	/*
	ע�⣺���ʹ�����·��Ҫע��·���Ƿ��������ͬһ·���£�����ʹ�þ���·��
	*/
	if (!as.openFile("G:\\WORK_PROJECTS\\STLѵ��\\Debug\\History of CPP.txt"))
	{
		cout << "Open File Failed!!" << endl;
		system("pause");
		return -1;
	}

	as.statistic();
	as.closeFile();
	as.outDatas();

	system("pause");
	return 0;
}