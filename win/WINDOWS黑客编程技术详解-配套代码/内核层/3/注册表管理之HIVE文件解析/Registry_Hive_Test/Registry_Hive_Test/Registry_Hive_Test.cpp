// Registry_Hive_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HiveAnalysis.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE; 
//	bRet = AnalysisHiveFile("C:\\Users\\DemonGan\\Desktop\\SOFTWARE");
	bRet = AnalysisHiveFile("C:\\Users\\DemonGan\\Desktop\\SECURITY");
	if (bRet)
	{
		printf("HIVE FILE ANALYSIS SUCCESS.\n");
	}
	else
	{
		printf("HIVE FILE ANALYSIS ERROR.\n");
	}
	system("pause");
	return 0;
}

