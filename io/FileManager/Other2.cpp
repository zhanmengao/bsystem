#include <stdio.h> 
#include<iostream> 
#include<vector> 
#include <Windows.h> 
#include <fstream>  
#include <iterator> 
#include <string> 
using namespace std;


/*----------------------------
* ���� : �ݹ�����ļ��У��ҵ����а����������ļ�
*----------------------------
* ���� : find
* ���� : public
*
* ���� : lpPath [in]   ��������ļ���Ŀ¼
* ���� : fileList [in]  ���ļ����Ƶ���ʽ�洢��������ļ�
*/
void find(WCHAR* lpPath, std::vector<std::wstring> &fileList)
{
	WCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	lstrcpy(szFind, lpPath);
	lstrcat(szFind, L"\\*.*");

	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)  return;

	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				WCHAR szFile[MAX_PATH];
				lstrcpy(szFile, lpPath);
				lstrcat(szFile, L"\\");
				lstrcat(szFile, (WCHAR*)(FindFileData.cFileName));
				find(szFile, fileList);
			}
		}
		else
		{
			//std::cout << FindFileData.cFileName << std::endl; 
			fileList.push_back(FindFileData.cFileName);
		}
		if (!FindNextFile(hFind, &FindFileData))  break;
	}
	FindClose(hFind);
}


int main2()
{
	std::vector<std::wstring> fileList;//����һ����Ž���ļ����Ƶ����� 

											//����һ�ν���������ļ�,��ȡ�ļ����б� 
	find(L"XXXX�����ļ���Ŀ¼", fileList);//֮��ɶ��ļ��б��е��ļ�������Ӧ�Ĳ��� 

								  //����ļ����������ļ������� 
	for (int i = 0; i < fileList.size(); i++)
	{
		//cout << fileList[i] << endl;
	}
	cout << "�ļ���Ŀ��" << fileList.size() << endl;
	return 0;
}