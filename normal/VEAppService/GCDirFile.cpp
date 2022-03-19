#include "stdafx.h"
#include "GCDirFile.h"
#include <iostream>
#include <io.h>
#include< direct.h> 
#include <ShlObj.h>
#include "curl.h"
#include <time.h>
#include "utils.h"

GCDirFile::GCDirFile(void)
{
}


GCDirFile::~GCDirFile(void)
{
}

size_t writeFunc(void *ptr, size_t size, size_t nmemb, FILE *stream);
int progressFunc(char *progress_data, double t, double d, double ultotal, double ulnow);

BOOL GCDirFile::CreateMDirectory( string szPath )
{
	wstring strDir;
	cstringClass.StringToWString(szPath, strDir);

	if (_mkdir(szPath.c_str()) == 0)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL GCDirFile::DeleteDirectory( CString directory_path )
{   
	//
	return FALSE;
}

BOOL GCDirFile::DirectoryExist( CString strPath )
{
	//CString strPath = lpszPath;
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile( strPath, &wfd );
	if ((hFind!=INVALID_HANDLE_VALUE) && ( wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
	{
		rValue = TRUE;
	}
	FindClose(hFind);

	return rValue;
}

BOOL GCDirFile::DirectoryEmpty( CString strPath )
{
	return PathIsDirectoryEmpty( strPath );
}

BOOL GCDirFile::FileDirExist( CString strPath )
{
	//���ܣ�����ļ�/·���Ƿ����
	return PathFileExists( strPath );
		
}

CString GCDirFile::GetPathFileName( CString strPath )
{
	
	//���ܣ����·���е��ļ���
	//���磺 PathFileFileName( ��c:\\Program Files\\File.txt�� ) = ��File.txt����
	//���·���в������ļ������򷵻�ֵ���������Ŀ¼����

	return PathFindFileName( strPath );

}

CString GCDirFile::GetFileExtName( CString strPath )
{
	
	//���ܣ���ȡ·�����ļ����е��ļ���չ��
	//���磺PathFindExtension( ��File.txt�� ) = ��.txt��
	return PathFindExtension( strPath );
}

CString GCDirFile::GetPathNextComponent( CString strPath )
{
	
	//���磺PathFindNextComponent( ��c:\\Program Files\\Directory�� ) = ��Program Files\\Directory��
	//PathFindNextComponent( ��c:\\Program Files�� ) = ��Program Files��
	return PathFindNextComponent( strPath );
}

CString GCDirFile::RemovePathArgs( CString pszPath )
{
	//���ܣ�ɾ��·���д��еĲ���

	CString tmp = pszPath;
	
	PathRemoveArgs( tmp.GetBuffer() );
	tmp.ReleaseBuffer();
	
	return tmp;
	
}

CString GCDirFile::RemovePathBackslash( CString pszPath )
{
	//���ܣ�ɾ��·����β����ġ�\������
	 PathRemoveBackslash( pszPath.GetBuffer() );
	 pszPath.ReleaseBuffer();

	 return pszPath;
}

void GCDirFile::RemovePathBlanks( LPTSTR pszPath )
{
	//���ܣ�ɾ���ַ���ͷβ�Ŀո�
	PathRemoveBlanks( pszPath );
}

void GCDirFile::RemoveFileExt( LPTSTR pszPath )
{
	//���ܣ�ɾ��·��������ļ���չ��
	PathRemoveExtension( pszPath );

}

CString GCDirFile::GetFilePathStr( LPTSTR pszPath )
{
	//���ܣ�ɾ��·��������ļ����͡�\�����š��ú������Է�����һ���ļ���·����
	//����szPath = ��c:\\windows\\system32\\nt.dll�� ;
	//����PathRemoveFileSpec( szPath ) ��szPath = ��c:\\windows\\system32�� 
	 if ( PathRemoveFileSpec( pszPath ) )
	 {
		 return (CString)pszPath;
	 }  
	 else
	 {
	     return _T("Error:Path");
	 }

}

CString GCDirFile::GetPathDriveName( LPTSTR pszPath )
{
	//���ܣ���·���з������̷�

	if( PathStripToRoot(  pszPath ) )
	{
		return (CString)pszPath;
	}
	else
	{
		return _T("Error:Path");
	}    
}

int GCDirFile::ReadDirPathList( string DirPath, vector<Filepath> &filespath)
{
	return GetFiles(DirPath, filespath);
}

int GCDirFile::GetFiles(string path, vector<Filepath> &filespath )
{
	int index = 0;
	intptr_t hFile = 0;
	string p;

	struct _finddata_t fileinfo;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			Filepath tn;
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				string fileName = fileinfo.name;
				if (index < 2 ) {
					size_t position = fileName.find_first_of("$");
					size_t position1 = fileName.find_last_of(".Bin");
					if (position != 0 && position1 != fileName.size()) {
						string one = ".";
						string two = "..";
						if (fileName != one && fileName != two) {
							tn.types = 0;
							tn.path = p.assign(path).append("\\").append(fileName);
							tn.size = fileinfo.size;
							tn.names = fileName;
							tn.time = fileinfo.time_write;
								//cstringClass.FormatTime(fileinfo.time_create);

							filespath.push_back(tn);
						}
					}
					index++;
				}
				else {
					tn.types = 0;
					tn.path = p.assign(path).append("\\").append(fileName);
					tn.size = fileinfo.size;
					tn.names = fileName;
					//printf("�������� %d", fileinfo.time_create);
					tn.time = fileinfo.time_write;
						//cstringClass.FormatTime(fileinfo.time_create);

					filespath.push_back(tn);
				}
			}
			else
			{
				tn.types = 1;
				tn.path = p.assign(path).append("\\").append(fileinfo.name);
				tn.size = fileinfo.size;
				tn.names = fileinfo.name;
				tn.time = fileinfo.time_write;
					//cstringClass.FormatTime(fileinfo.time_create);
				filespath.push_back(tn);
			}
		} while(_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//�ƶ��ļ�
int GCDirFile::moveFile(const char *oldPath, const char *newPath) {

	CFileFind finder;
	char szNewTextPath[MAX_PATH] = "";
	CString csOldPath(oldPath);
	BOOL bWorking = finder.FindFile(csOldPath);
	if (!bWorking) {
		return -1;
	}
	//������ʶ��
	USES_CONVERSION;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) {
			continue;
		}
		else {
			CString str = finder.GetFileName();
			//���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
			char * pFileName = T2A(str);
			sprintf(szNewTextPath, "%s\\%s", newPath, pFileName);
			CString csNewPath = szNewTextPath;
			BOOL boo = MoveFile(finder.GetFilePath(), csNewPath);
			if (!boo) {
				return FALSE;
			}
		}
		
	}
	finder.Close();
	return TRUE;
}

int GCDirFile::copyFile(const char *oldPath, const char *newPath) {
	
	int i = 1;
	CFileFind finder;
	char szNewTextPath[MAX_PATH] = "";
	CString csOldPath(oldPath);
	BOOL bWorking = finder.FindFile(csOldPath);
	if (!bWorking) {
		return -1;
	}
	//������ʶ��
	USES_CONVERSION;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) {
			continue;
		}
		else if (finder.IsDirectory()) {
			CString cs_oldPath = finder.GetFilePath();
			char * ch_oldPath = T2A(cs_oldPath);

			CString cs_newPath(newPath);
			CString nextFilePath = cs_newPath+"\\"+finder.GetFileName();
			if (!PathIsDirectory(nextFilePath))
			{
				::CreateDirectory(nextFilePath, NULL);
			}
			char * ch_nextFilePath = T2A(nextFilePath);

			sprintf(szNewTextPath, "%s\\%s", ch_oldPath, "*.*");
			copyFile(szNewTextPath, ch_nextFilePath);
		}
		else {
			CString str = finder.GetFileName();
			//char * pFileName = CStringToChar(str);
			//���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
			char * pFileName = T2A(str);
			sprintf(szNewTextPath, "%s\\%s", newPath, pFileName);

			CString csNewPath = szNewTextPath;
			BOOL boo = CopyFile(finder.GetFilePath(), csNewPath, FALSE);
		}
	
	}
	finder.Close();
	return TRUE;
}

int GCDirFile::deleteFile(const char *filePath,const char *filePath1) {

	CFileFind finder;
	char szNewTextPath[MAX_PATH] = "";
	CString cs_filePath(filePath);
	CString cs_filePath1(filePath1);
	BOOL bWorking = finder.FindFile(cs_filePath);
	if (!bWorking) {
		return -1;
	}
	//������ʶ��
	USES_CONVERSION;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) {
			continue;
		}
		else if (finder.IsDirectory()) {
			CString cs_oldPath = finder.GetFilePath();
			char * ch_oldPath = T2A(cs_oldPath);

			sprintf(szNewTextPath, "%s\\%s", ch_oldPath, "*.*");
			deleteFile(szNewTextPath, ch_oldPath);
		}
		else {
			CString cs_deletePath = finder.GetFilePath();
			DeleteFile(cs_deletePath);
		}

	}

	//CString str = cs_filePath;

	//int i = str.Find('*.*', str.GetLength() - 2);
	//if (i != -1) {
	//	//CString str2 = cs_filePath.Left(i - 2);
	//	int i1 = str.ReverseFind('\\');
	//	str = str.Left(i1);

	//}
	if (PathIsDirectory(cs_filePath1))
	{
		 RemoveDirectory(cs_filePath1);
	}

	finder.Close();
	return TRUE;
}

//�����ļ�
#define MAX_PACK_SIZE 1024
int GCDirFile::SendFile(SOCKET sock, const char *path)
{
	FILE * fp;
	//char temp[1024];

	fp = fopen(path, "rb");
	
	if (fp == NULL) {
		LOG(INFO) << "SendFile() ���ļ�ʧ��!";
		return FALSE;
	}
	fseek(fp, 0, SEEK_END);
	long long fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	_int64 i = 0;
	char buff[MAX_PACK_SIZE];

	while (i<fileLength)
	{
		int nSize;
		if (i + MAX_PACK_SIZE>fileLength)
		{
			nSize = (int)(fileLength - i);
		}
		else
		{
			nSize = MAX_PACK_SIZE - 1;
		}
		fread(buff, sizeof(char), nSize, fp);
		int nSend;
		nSend = send(sock, buff, nSize, 0);
		if (nSend == SOCKET_ERROR)
		{
			LOG(INFO) << "SendFile() downloadFile error";
			return FALSE;
		}
		i += nSend;
		fseek(fp, -(nSize - nSend), SEEK_CUR);  //��λ��ʵ���ѷ��͵���λ��  
		memset(buff, 0, sizeof(char)*MAX_PACK_SIZE); //��buff���  
	}
	fclose(fp);
	/*int num;
	while (1)
	{
		num = fread(temp, 1, 1024, fp);
		if (num == 0)
			break;
		send(sock, temp, num, 0);

	}*/
	return TRUE;
}

//�ϴ��ļ�
int GCDirFile::RecvFile(const char *url,string diskpath) {

		UtilsTools utilsTools;
		string newStr = diskpath.substr(0, diskpath.find_last_of("/"));
		wstring strDir;
		cstringClass.StringToWString(newStr, strDir);
		
		if (!utilsTools.CreateMultiDirectory(strDir.c_str())) {

			std::cout << "CreateMultiDirectory Failed!" << std::endl;
			return FALSE;

		}
	
		CURL *curl;
		CURLcode res;
		FILE *outfile;
		char *progress_data = "* ";

		curl = curl_easy_init();
		if (curl)
		{
			//LOG(INFO) << "�ļ�url:" << url << " ����·��:" << diskpath.c_str();
			outfile = fopen(diskpath.c_str(), "wb");

			if (outfile == NULL) {
				LOG(INFO) << "RecvFile open file error!";

				std::cout << "RecvFile open file error!";

				return FALSE;
			}

			std::cout << "start upload file! " << std::endl;

			//curl_easy_setopt(curl, CURLOPT_VERBOSE, curllog);
			//curl_easy_setopt(curl, CURLOPT_STDERR, curllog);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
			curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);//�趨Ϊ����֤֤���HOST 
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunc);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
			curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
			res = curl_easy_perform(curl);
		}
		fclose(outfile);
		if (res != CURLE_OK) {
			std::cout << "Not CURLE_OK: " << res << std::endl;
			LOG(INFO) << "Not CURLE_OK: " << res;

			curl_easy_cleanup(curl);
			return FALSE;
		}
	curl_easy_cleanup(curl);
	return TRUE;
}

//�����ļ��У�Ĭ������Ȩ��Ϊ�� administrators 
int GCDirFile::CreateFolder(const char *folderPath) {
	
	char *folderPaths  = const_cast<char*>(folderPath);
	LPWSTR folderPathStr = cstringClass.Str2Unicode(folderPaths);

	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = &sd;
	//CreateDirectory(folderPathStr, &sa);
	_mkdir(folderPaths);

	//if (!FolderAddOrDelGroup(folderPaths, "Administrators", GENERIC_ALL, 1)) {

	//	RemoveDirectory(folderPathStr);
	//	LOG(INFO) << "�����ļ���,����Ĭ��Ȩ��administrators ʧ��!";
	//	return FALSE;
	//}
	return TRUE;
}

#define myHeapAlloc(x) (HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,x))
#define myHeapFree(x) (HeapFree(GetProcessHeap(),0,x))
/*
	Ϊ�ļ���Ŀ¼�����һ���˻����飩��Ȩ��
	folderPath �ļ�
	groupName �˻�
	dwAccessMask Ȩ��
	status �ܾ� 0 ���� ����
*/
int GCDirFile::FolderAddOrDelGroup(char *folderPath, char *groupName, DWORD dwAccessMask, UINT status) {

	LPWSTR lpszFileName = cstringClass.Str2Unicode(folderPath);
	LPWSTR lpszAccountName = cstringClass.Str2Unicode(groupName);

	//����SID����
	SID_NAME_USE snuType;
	//������LookupAccountName��صı���
	TCHAR * szDomain = NULL;
	DWORD cbDomain = 0;
	LPVOID pUserSID = NULL;
	DWORD cdUserSID = 0;
	//�ļ���صİ�ȫ������ sd �ı���
	PSECURITY_DESCRIPTOR pFileSD = NULL;
	DWORD cbFileSD = 0;
	//һ���µ�sd�ı��������ڹ����µ�ACL�������е�acl����Ҫ�¼ӵ�acl����������
	SECURITY_DESCRIPTOR newSD;
	//��acl��صı���
	PACL pACL = NULL;
	BOOL fDaclPresent;
	BOOL fDaclDefaulted;
	ACL_SIZE_INFORMATION aclInfo;
	//һ���µ�acl����
	PACL pNewACL = NULL;
	DWORD cbNewACL = 0;
	// һ����ʱʹ�õ�ace����
	LPVOID pTempACE = NULL;
	UINT currentAceIndex = 0;//ace ��acl�е�λ��
	UINT newAceIndex = 0; //�����ace��acl�е�λ��
						  //api�����ķ���ֵ���������еĺ���������ʧ��
	BOOL fResult = FALSE;
	BOOL fAPISuccess = FALSE;
	SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;

	__try {

		// STEP 1: ͨ���û���ȡ��SID
		//     ����һ����LookupAccountName���������������Σ���һ����ȡ������Ҫ
		// ���ڴ�Ĵ�С��Ȼ�󣬽����ڴ���䡣�ڶ��ε��ò���ȡ�����û����ʻ���Ϣ��
		// LookupAccountNameͬ������ȡ�����û������û������Ϣ������ο�MSDN��
		//
		fAPISuccess = LookupAccountName(NULL, lpszAccountName, pUserSID, &cdUserSID, szDomain, &cbDomain, &snuType);
		// ���ϵ���API��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
		// �����Ǵ�����ڴ治��Ĵ���

		if (fAPISuccess) {
			__leave;
		}
		else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			_tprintf(TEXT("LookupAccountName() failed.Error %d\n"), GetLastError());
			__leave;
		}

		pUserSID = myHeapAlloc(cdUserSID);
		if (!pUserSID)
		{
			_tprintf(TEXT("HeapAlloc() failed.Error %d\n"), GetLastError());
			__leave;
		}

		szDomain = (TCHAR*)myHeapAlloc(cbDomain * sizeof(TCHAR));
		if (!szDomain)
		{
			_tprintf(TEXT("HeapAlloc() failed.Error %d\n"), GetLastError());
			__leave;
		}

		fAPISuccess = LookupAccountName(NULL, lpszAccountName, pUserSID, &cdUserSID, szDomain, &cbDomain, &snuType);
		if (!fAPISuccess) {
			_tprintf(TEXT("LookupAccountName() failed.Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 2: ȡ���ļ���Ŀ¼����صİ�ȫ������SD
		//     ʹ��GetFileSecurity����ȡ��һ���ļ�SD�Ŀ�����ͬ�����������Ҳ
		// �Ǳ��������Σ���һ��ͬ����ȡSD���ڴ泤�ȡ�ע�⣬SD�����ָ�ʽ������ص�
		// ��self-relative���� ��ȫ�ģ�absolute����GetFileSecurityֻ��ȡ������
		// ��صġ�����SetFileSecurity����Ҫ��ȫ�ġ������Ϊʲô��Ҫһ���µ�SD��
		// ������ֱ����GetFileSecurity���ص�SD�Ͻ����޸ġ���Ϊ������صġ���Ϣ
		// �ǲ������ġ�
		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, 0, &cbFileSD);
		if (fAPISuccess) {
			__leave;
		}
		else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			_tprintf(TEXT("GetFileSecurity() failed.Error %d\n"), GetLastError());
			__leave;
		}
		pFileSD = myHeapAlloc(cbFileSD);
		if (!pFileSD) {
			_tprintf(TEXT("HeapAlloc() failed.Error %d\n"), GetLastError());
			__leave;
		}

		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, cbFileSD, &cbFileSD);
		if (!fAPISuccess) {
			_tprintf(TEXT("GetFileSecurity() failed.Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 3: ��ʼ��һ���µ�SD
		// 
		if (!InitializeSecurityDescriptor(&newSD, SECURITY_DESCRIPTOR_REVISION)) {
			_tprintf(TEXT("InitializeSecurityDescriptor() failed.")
				TEXT("Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 4: ��GetFileSecurity ���ص�SD��ȡDACL
		// 
		if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL, &fDaclDefaulted)) {
			_tprintf(TEXT("GetSecurityDescriptorDacl() failed.Error %d\n"), GetLastError());
			__leave;
		}
		// 
		// STEP 5: ȡ DACL���ڴ�size
		//     GetAclInformation�����ṩDACL���ڴ��С��ֻ����һ������Ϊ
		// ACL_SIZE_INFORMATION��structure�Ĳ�����ȡDACL����Ϣ����Ϊ��
		// �������Ǳ������е�ACE��
		aclInfo.AceCount = 0;
		aclInfo.AclBytesFree = 0;
		aclInfo.AclBytesInUse = sizeof(ACL);

		if (pACL == NULL) {
			fDaclPresent = FALSE;
		}

		if (fDaclPresent) {
			if (!GetAclInformation(pACL, &aclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation)) {
				_tprintf(TEXT("GetAclInformation() failed.Error %d\n"), GetLastError());
				__leave;
			}
		}

		// STEP 6: �����µ�ACL��size
		//    ����Ĺ�ʽ�ǣ�ԭ�е�DACL��size������Ҫ��ӵ�һ��ACE��size����
		// ������һ����ACE��ص�SID��size������ȥ�����ֽ��Ի�þ�ȷ�Ĵ�С��
		cbNewACL = aclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(pUserSID) - sizeof(DWORD);

		// 
		// STEP 7: Ϊ�µ�ACL�����ڴ�
		// 
		pNewACL = (PACL)myHeapAlloc(cbNewACL);
		if (!pNewACL) {
			_tprintf(TEXT("myHeapAlloc() failed.Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 8: ��ʼ���µ�ACL�ṹ
		// 
		if (!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2)) {
			_tprintf(TEXT("InitializeAcl() failed.Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 9  ����ļ���Ŀ¼�� DACL �����ݣ��������е�ACE���µ�DACL��
		// 
		//     ����Ĵ���������ȼ��ָ���ļ���Ŀ¼���Ƿ���ڵ�DACL������еĻ���
		// ��ô�Ϳ������е�ACE���µ�DACL�ṹ�У����ǿ��Կ���������ķ����ǲ���
		// ACL_SIZE_INFORMATION�ṹ�е�AceCount��Ա����ɵġ������ѭ���У�
		// �ᰴ��Ĭ�ϵ�ACE��˳�������п�����ACE��ACL�е�˳���Ǻܹؼ��ģ����ڿ�
		// �������У��ȿ����Ǽ̳е�ACE������֪��ACE����ϲ�Ŀ¼�м̳�������
		// 
		newAceIndex = 0;
		if (fDaclPresent && aclInfo.AceCount) {
			for (currentAceIndex = 0; currentAceIndex < aclInfo.AceCount; currentAceIndex++) {
				// 
				// STEP 10: ��DACL��ȡACE
				// 
				if (!GetAce(pACL, currentAceIndex, &pTempACE)) {
					_tprintf(TEXT("GetAce() failed. Error %d\n"),
						GetLastError());
					__leave;
				}
				// 
				// STEP 11: ����Ƿ��ǷǼ̳е�ACE
				//     �����ǰ��ACE��һ���Ӹ�Ŀ¼�̳�����ACE����ô���˳�ѭ����
				// ��Ϊ���̳е�ACE�����ڷǼ̳е�ACE֮�󣬶�������Ҫ��ӵ�ACE
				// Ӧ�������еķǼ̳е�ACE֮�����еļ̳е�ACE֮ǰ���˳�ѭ��
				// ����Ϊ��Ҫ���һ���µ�ACE���µ�DACL�У���������ٰѼ̳е�
				// ACE�������µ�DACL�С�
				//
				if (((ACCESS_ALLOWED_ACE*)pTempACE)->Header.AceFlags & INHERITED_ACE) {
					break;
				}
				// 
				// STEP 12: ���Ҫ������ACE��SID�Ƿ����Ҫ�����ACE��SIDһ����
				// ���һ������ô��Ӧ�÷ϵ��Ѵ��ڵ�ACE��Ҳ����˵��ͬһ���û��Ĵ�ȡ
				// Ȩ�޵����õ�ACE����DACL��Ӧ��Ψһ�������������ͬһ�û�������
				// �˵�ACE�����ǿ��������û���ACE��
				// 
				if (EqualSid(pUserSID, &(((ACCESS_ALLOWED_ACE*)pTempACE)->SidStart))) {
					//��ͬ
					continue;
				}
				// 
				// STEP 13: ��ACE���뵽�µ�DACL��
				//    ����Ĵ����У�ע�� AddAce �����ĵ����������������������˼�� 
				// ACL�е�����ֵ����ΪҪ��ACE�ӵ�ĳ����λ��֮�󣬲���MAXDWORD��
				// ��˼��ȷ����ǰ��ACE�Ǳ����뵽����λ�á�
				//
				if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempACE, ((PACE_HEADER)pTempACE)->AceSize)) {
					_tprintf(TEXT("AddAce() failed. Error %d\n"),
						GetLastError());
					__leave;
				}
				newAceIndex++;
			}
		}

		// 
		// STEP 14: ��һ�� access-allowed ��ACE ���뵽�µ�DACL��
		//     ǰ���ѭ�����������еķǼ̳���SIDΪ�����û���ACE���˳�ѭ���ĵ�һ����
		// ���Ǽ�������ָ����ACE��
		// 
		// Ex��ĺ��������һ������AceFlag�������˲�������������������ǿ���������һ
		// ����ACE_HEADER�Ľṹ���Ա������������õ�ACE���Ա�����Ŀ¼���̳���ȥ���� 
		// AddAccessAllowedAce�������ܶ��������������AddAccessAllowedAce����
		// �У�����ACE_HEADER����ṹ���óɷǼ̳еġ�
		// 

		if (status == 0) {
			/*if (!AddAccessDeniedAceEx(pNewACL, ACL_REVISION2, CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE, dwAccessMask, pUserSID)) {
			_tprintf(TEXT("AddAccessAllowedAceEx() failed. Error %d\n"),
			GetLastError());
			__leave;
			}*/
		}
		else
		{
			if (!AddAccessAllowedAceEx(pNewACL, ACL_REVISION2, CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE, dwAccessMask, pUserSID)) {
				_tprintf(TEXT("AddAccessAllowedAceEx() failed. Error %d\n"),
					GetLastError());
				__leave;
			}
		}
		
		// STEP 15: �����Ѵ��ڵ�ACE��˳�򿽱��Ӹ�Ŀ¼�̳ж�����ACE
		//
		//bitset<32> bit(dwAccessMask);
		if (fDaclPresent && aclInfo.AceCount) {
			for (; currentAceIndex < aclInfo.AceCount; currentAceIndex++)
			{
				// 
				// STEP 16: ���ļ���Ŀ¼����DACL�м���ȡACE
				// 
				if (!GetAce(pACL, currentAceIndex, &pTempACE)) {
					_tprintf(TEXT("GetAce() failed. Error %d\n"),
						GetLastError());
					__leave;
				}

				if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempACE, ((PACE_HEADER)pTempACE)->AceSize)) {
					_tprintf(TEXT("AddAce() failed. Error %d\n"),
						GetLastError());
					__leave;
				}
			}
		}
		// 
		// STEP 18: ���µ�ACL���õ��µ�SD��
		// 
		if (!SetSecurityDescriptorDacl(&newSD, TRUE, pNewACL, FALSE)) {
			_tprintf(TEXT("SetSecurityDescriptorDacl() failed. Error %d\n"),
				GetLastError());
			__leave;
		}
		// 
		// STEP 19: ���ϵ�SD�еĿ��Ʊ���ٿ������µ�SD�У�����ʹ�õ���һ���� 
		// SetSecurityDescriptorControl() ��API����
		// 
		SECURITY_DESCRIPTOR_CONTROL controlBitsOfInterest = 0;
		SECURITY_DESCRIPTOR_CONTROL controlBitsToSet = 0;
		SECURITY_DESCRIPTOR_CONTROL oldControlBits = 0;
		DWORD dwRevision = 0;
		if (!GetSecurityDescriptorControl(pFileSD, &oldControlBits,
			&dwRevision)) {
			_tprintf(TEXT("GetSecurityDescriptorControl() failed. Error %d\n"), GetLastError());
			__leave;
		}
		if (oldControlBits & SE_DACL_AUTO_INHERITED) {
			controlBitsOfInterest =
				SE_DACL_AUTO_INHERIT_REQ |
				SE_DACL_AUTO_INHERITED;
			controlBitsToSet = controlBitsOfInterest;
		}
		else if (oldControlBits & SE_DACL_PROTECTED) {
			controlBitsOfInterest = SE_DACL_PROTECTED;
			controlBitsToSet = controlBitsOfInterest;
		}
		if (controlBitsOfInterest) {
			if (!SetSecurityDescriptorControl(&newSD,controlBitsOfInterest,controlBitsToSet)) {
				_tprintf(TEXT("SetSecurityDescriptorControl() failed.Error %d\n"), GetLastError());
				__leave;
			}
		}
		// STEP 20: ���µ�SD�������õ��ļ��İ�ȫ������
		// 
		if (!SetFileSecurity(lpszFileName, secInfo, &newSD)) {
			_tprintf(TEXT("SetFileSecurity() failed. Error %d\n"),GetLastError());
			__leave;
		}
		fResult = TRUE;
	}
	__finally {
		// 
		// STEP 21: �ͷ��ѷ�����ڴ棬����Memory Leak
		// 
		if (pUserSID)  myHeapFree(pUserSID);
		if (szDomain)  myHeapFree(szDomain);
		if (pFileSD) myHeapFree(pFileSD);
		if (pNewACL) myHeapFree(pNewACL);
	}
	return fResult;
}

int GCDirFile::renameFile(const char *oldName, const char *newName) {

	//CString cNewName(newName);

	//if (!PathFileExists(cNewName) || !PathIsDirectory(cNewName)) {
	int error = rename(oldName, newName);
		if (  error ==  0) 
		{
			return TRUE;
		}
		else 
		{
			return FALSE;
			
		}
//	}
//	else {
	//	return FALSE;
	//}
}

int GCDirFile::SetUserFolderSize(string path,string size) {

	HINSTANCE hd;
	CString commandParameter;

	wstring strDir;
	cstringClass.StringToWString(path, strDir);

	if (!PathIsDirectory(strDir.c_str()))
	{
		return FALSE;
	}
	commandParameter.Format(_T("quota add /path:%S /limit:%S /type:hard"), cstringClass.replace_all(path, "/", "\\").c_str(), size.c_str());
	hd = ShellExecute(NULL, L"open", L"dirquota", commandParameter, NULL, 0);
	long i = (long)hd;
	if (i <= 32) {
		return FALSE;
	}
	return TRUE;
}

int GCDirFile::ModifyUserFolderSize(string path, string size) {

	HINSTANCE hd;
	CString commandParameter;

	wstring strDir;
	cstringClass.StringToWString(path, strDir);

	if (!PathIsDirectory(strDir.c_str()))
	{
		return FALSE;
	}
	commandParameter.Format(_T("quota modify /path:%S /limit:%S /type:hard"), cstringClass.replace_all(path, "/", "\\").c_str(), size.c_str());
	hd = ShellExecute(NULL, L"open", L"dirquota", commandParameter, NULL, 0);
	long i = (long)hd;
	if (i <= 32) {
		return FALSE;
	}
	return TRUE;
}

int GCDirFile::DelUserFolderSize(string path) {

	HINSTANCE hd;
	CString commandParameter;

	wstring strDir;
	cstringClass.StringToWString(path, strDir);

	if (!PathIsDirectory(strDir.c_str()))
	{
		return FALSE;
	}
	commandParameter.Format(_T("quota delete /path:%S /quiet"), cstringClass.replace_all(path, "/", "\\").c_str());
	hd = ShellExecute(NULL, L"open", L"dirquota", commandParameter, NULL, 0);
	long i = (long)hd;
	if (i <= 32) {
		return FALSE;
	}
	return TRUE;
}



size_t writeFunc(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	return fwrite(ptr, size, nmemb, stream);
}

int progressFunc(char *progress_data, double t, /* dltotal */double d, /* dlnow */double ultotal, double ulnow)
{
	//printf("%s %g / %g (%g %%)\n", progress_data, d, t, d*100.0 / t);
	return 0;
}