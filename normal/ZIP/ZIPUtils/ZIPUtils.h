#pragma once
#include"../ZLIBUtils/ZLIBUtils.h"
#ifdef __cplusplus
#include <string>
#include <iostream>
#include <vector>
#include <Shlwapi.h> 
#include <zip.h>
#include <unzip.h>
#include "zlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include"../../File/FileManager/FileManager/FileManager.h"
#include"../../../C++space/StringTools/StringTools/StrUtils.h"
using namespace std;
#pragma comment(lib, "Shlwapi.lib")
#endif

static void UnzipFile(const std::string& strFilePath, const std::string& strTempPath)
{
	int nReturnValue;
	string tempFilePath;
	string srcFilePath(strFilePath);
	string destFilePath;

	std::cout << "Start unpacking the package... " << endl;

	//��zip�ļ�
	unzFile unzfile = unzOpen(srcFilePath.c_str());
	if (unzfile == NULL)
	{
		return;
	}

	//��ȡzip�ļ�����Ϣ
	unz_global_info* pGlobalInfo = new unz_global_info;
	nReturnValue = unzGetGlobalInfo(unzfile, pGlobalInfo);
	if (nReturnValue != UNZ_OK)
	{
		std::cout << "The number of compressed package files is  " << pGlobalInfo->number_entry << endl;
		return;
	}


	//����zip�ļ�
	unz_file_info* pFileInfo = new unz_file_info;
	char szZipFName[MAX_PATH] = { 0 };
	char szExtraName[MAX_PATH] = { 0 };
	char szCommName[MAX_PATH] = { 0 };
	//��Ŵ�zip�н����������ڲ��ļ���
	for (int i = 0; i < pGlobalInfo->number_entry; i++)
	{
		//�����õ�zip�е��ļ���Ϣ
		nReturnValue = unzGetCurrentFileInfo(unzfile, pFileInfo, szZipFName, MAX_PATH, szExtraName, MAX_PATH, szCommName, MAX_PATH);
		if (nReturnValue != UNZ_OK)
			return;
		std::cout << "ZipName: " << szZipFName << "Extra: " << szExtraName << "Comm: " << szCommName << endl;

		string strZipFName = szZipFName;
		if (pFileInfo->external_fa == FILE_ATTRIBUTE_DIRECTORY || (strZipFName.rfind('/') == strZipFName.length() - 1))
		{
			destFilePath = strTempPath + "//" + szZipFName;
			CreateDirectoryA(destFilePath.c_str(), NULL);
		}
		else
		{
			//�����ļ�
			string strFullFilePath;
			tempFilePath = strTempPath + "/" + szZipFName;
			strFullFilePath = tempFilePath;//��������·��

			int nPos = tempFilePath.rfind("/");
			int nPosRev = tempFilePath.rfind("\\");
			if (nPosRev == string::npos && nPos == string::npos)
				continue;

			size_t nSplitPos = nPos > nPosRev ? nPos : nPosRev;
			destFilePath = tempFilePath.substr(0, nSplitPos + 1);

			if (!PathIsDirectoryA(destFilePath.c_str()))
			{
				//��·����ʽͳһ
				destFilePath = StrUtils::ReplaceAllA(destFilePath, "/", "\\");
				//�����༶Ŀ¼
				int bRet = FileManager::CreatedMultipleDirectory(destFilePath);
			}
			strFullFilePath = StrUtils::ReplaceAllA(strFullFilePath, "/", "\\");

			HANDLE hFile = CreateFileA(strFullFilePath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return;
			}

			//���ļ�
			nReturnValue = unzOpenCurrentFile(unzfile);
			if (nReturnValue != UNZ_OK)
			{
				CloseHandle(hFile);
				return;
			}

			//��ȡ�ļ�
			uLong BUFFER_SIZE = pFileInfo->uncompressed_size;;
			void* szReadBuffer = NULL;
			szReadBuffer = (char*)malloc(BUFFER_SIZE);
			if (NULL == szReadBuffer)
			{
				break;
			}

			while (TRUE)
			{
				memset(szReadBuffer, 0, BUFFER_SIZE);
				int nReadFileSize = 0;

				nReadFileSize = unzReadCurrentFile(unzfile, szReadBuffer, BUFFER_SIZE);

				if (nReadFileSize < 0)					 //��ȡ�ļ�ʧ��
				{
					unzCloseCurrentFile(unzfile);
					CloseHandle(hFile);
					return;
				}
				else if (nReadFileSize == 0)           //��ȡ�ļ����
				{
					unzCloseCurrentFile(unzfile);
					CloseHandle(hFile);
					break;
				}
				else									//д���ȡ������
				{
					DWORD dWrite = 0;
					BOOL bWriteSuccessed = WriteFile(hFile, szReadBuffer, BUFFER_SIZE, &dWrite, NULL);
					if (!bWriteSuccessed)
					{
						unzCloseCurrentFile(unzfile);
						CloseHandle(hFile);
						return;
					}
				}
			}

			free(szReadBuffer);
		}
		unzGoToNextFile(unzfile);
	}

	delete pFileInfo;

	delete pGlobalInfo;

	//�ر�
	if (unzfile)
	{
		unzClose(unzfile);
	}
	std::cout << "End unpacking the package... " << endl;
}