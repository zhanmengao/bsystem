#ifndef _HIVE_ANALYSIS_H_
#define _HIVE_ANALYSIS_H_


#include <Windows.h>


BOOL AnalysisHiveFile(char *pszHiveFileName);

// ����HIVE�ļ�ͷ
BOOL AnalysisHiveHeader(PUCHAR pMemory);
// ����NK
BOOL HiveNK(PUCHAR pHBIN, PUCHAR pNode);
// ����VK
BOOL HiveVK(PUCHAR pHBIN, PUCHAR pValue);
// ����LIST
BOOL HiveList(PUCHAR pHBIN, PUCHAR pList);

#endif