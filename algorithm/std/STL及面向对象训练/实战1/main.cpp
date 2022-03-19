/*
1、使用STL map容器来来统计一篇英文文章中单词出现的频率并输出统计结果。
（使用C++标准库中的fstream操作文件，#include<fstream>,详细使用方法见文档）
*/

#include<iostream>
#include "ArtStatistic.h"   //文章单词数据统计类所在头文件
using namespace std;

int main(int argc, char* argv[])
{
	ArtStatistic as;
	/*
	注意：如果使用相对路径要注意路径是否与程序在同一路径下，否则使用绝对路径
	*/
	if (!as.openFile("G:\\WORK_PROJECTS\\STL训练\\Debug\\History of CPP.txt"))
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