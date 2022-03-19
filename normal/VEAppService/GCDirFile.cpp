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
	//功能：检查文件/路径是否存在
	return PathFileExists( strPath );
		
}

CString GCDirFile::GetPathFileName( CString strPath )
{
	
	//功能：获得路径中的文件名
	//例如： PathFileFileName( “c:\\Program Files\\File.txt” ) = “File.txt”。
	//如果路径中不包含文件名，则返回值是最下面的目录名。

	return PathFindFileName( strPath );

}

CString GCDirFile::GetFileExtName( CString strPath )
{
	
	//功能：获取路径或文件名中的文件扩展名
	//例如：PathFindExtension( “File.txt” ) = “.txt”
	return PathFindExtension( strPath );
}

CString GCDirFile::GetPathNextComponent( CString strPath )
{
	
	//例如：PathFindNextComponent( “c:\\Program Files\\Directory” ) = “Program Files\\Directory”
	//PathFindNextComponent( “c:\\Program Files” ) = “Program Files”
	return PathFindNextComponent( strPath );
}

CString GCDirFile::RemovePathArgs( CString pszPath )
{
	//功能：删除路径中带有的参数

	CString tmp = pszPath;
	
	PathRemoveArgs( tmp.GetBuffer() );
	tmp.ReleaseBuffer();
	
	return tmp;
	
}

CString GCDirFile::RemovePathBackslash( CString pszPath )
{
	//功能：删除路径结尾多余的’\’符号
	 PathRemoveBackslash( pszPath.GetBuffer() );
	 pszPath.ReleaseBuffer();

	 return pszPath;
}

void GCDirFile::RemovePathBlanks( LPTSTR pszPath )
{
	//功能：删除字符串头尾的空格
	PathRemoveBlanks( pszPath );
}

void GCDirFile::RemoveFileExt( LPTSTR pszPath )
{
	//功能：删除路径后面的文件扩展名
	PathRemoveExtension( pszPath );

}

CString GCDirFile::GetFilePathStr( LPTSTR pszPath )
{
	//功能：删除路径后面的文件名和’\’符号。该函数可以分析出一个文件的路径。
	//例：szPath = “c:\\windows\\system32\\nt.dll” ;
	//调用PathRemoveFileSpec( szPath ) 后，szPath = “c:\\windows\\system32” 
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
	//功能：从路径中分析出盘符

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
					//printf("毫秒数： %d", fileinfo.time_create);
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

//移动文件
int GCDirFile::moveFile(const char *oldPath, const char *newPath) {

	CFileFind finder;
	char szNewTextPath[MAX_PATH] = "";
	CString csOldPath(oldPath);
	BOOL bWorking = finder.FindFile(csOldPath);
	if (!bWorking) {
		return -1;
	}
	//声明标识符
	USES_CONVERSION;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) {
			continue;
		}
		else {
			CString str = finder.GetFileName();
			//调用函数，T2A和W2A均支持ATL和MFC中的字符转换
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
	//声明标识符
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
			//调用函数，T2A和W2A均支持ATL和MFC中的字符转换
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
	//声明标识符
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

//下载文件
#define MAX_PACK_SIZE 1024
int GCDirFile::SendFile(SOCKET sock, const char *path)
{
	FILE * fp;
	//char temp[1024];

	fp = fopen(path, "rb");
	
	if (fp == NULL) {
		LOG(INFO) << "SendFile() 打开文件失败!";
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
		fseek(fp, -(nSize - nSend), SEEK_CUR);  //定位到实际已发送到的位置  
		memset(buff, 0, sizeof(char)*MAX_PACK_SIZE); //将buff清空  
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

//上传文件
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
			//LOG(INFO) << "文件url:" << url << " 磁盘路劲:" << diskpath.c_str();
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
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);//设定为不验证证书和HOST 
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

//创建文件夹，默认设置权限为组 administrators 
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
	//	LOG(INFO) << "创建文件夹,设置默认权限administrators 失败!";
	//	return FALSE;
	//}
	return TRUE;
}

#define myHeapAlloc(x) (HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,x))
#define myHeapFree(x) (HeapFree(GetProcessHeap(),0,x))
/*
	为文件（目录）添加一个账户（组）的权限
	folderPath 文件
	groupName 账户
	dwAccessMask 权限
	status 拒绝 0 允许 其它
*/
int GCDirFile::FolderAddOrDelGroup(char *folderPath, char *groupName, DWORD dwAccessMask, UINT status) {

	LPWSTR lpszFileName = cstringClass.Str2Unicode(folderPath);
	LPWSTR lpszAccountName = cstringClass.Str2Unicode(groupName);

	//声明SID变量
	SID_NAME_USE snuType;
	//声明和LookupAccountName相关的变量
	TCHAR * szDomain = NULL;
	DWORD cbDomain = 0;
	LPVOID pUserSID = NULL;
	DWORD cdUserSID = 0;
	//文件相关的安全描述符 sd 的变量
	PSECURITY_DESCRIPTOR pFileSD = NULL;
	DWORD cbFileSD = 0;
	//一个新的sd的变量，用于构造新的ACL（把已有的acl和需要新加的acl整合起来）
	SECURITY_DESCRIPTOR newSD;
	//和acl相关的变量
	PACL pACL = NULL;
	BOOL fDaclPresent;
	BOOL fDaclDefaulted;
	ACL_SIZE_INFORMATION aclInfo;
	//一个新的acl变量
	PACL pNewACL = NULL;
	DWORD cbNewACL = 0;
	// 一个临时使用的ace变量
	LPVOID pTempACE = NULL;
	UINT currentAceIndex = 0;//ace 在acl中的位置
	UINT newAceIndex = 0; //新添的ace在acl中的位置
						  //api函数的返回值，假设所有的函数都返回失败
	BOOL fResult = FALSE;
	BOOL fAPISuccess = FALSE;
	SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;

	__try {

		// STEP 1: 通过用户名取得SID
		//     在这一步中LookupAccountName函数被调用了两次，第一次是取出所需要
		// 的内存的大小，然后，进行内存分配。第二次调用才是取得了用户的帐户信息。
		// LookupAccountName同样可以取得域用户或是用户组的信息。（请参看MSDN）
		//
		fAPISuccess = LookupAccountName(NULL, lpszAccountName, pUserSID, &cdUserSID, szDomain, &cbDomain, &snuType);
		// 以上调用API会失败，失败原因是内存不足。并把所需要的内存大小传出。
		// 下面是处理非内存不足的错误。

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
		// STEP 2: 取得文件（目录）相关的安全描述符SD
		//     使用GetFileSecurity函数取得一份文件SD的拷贝，同样，这个函数也
		// 是被调用两次，第一次同样是取SD的内存长度。注意，SD有两种格式：自相关的
		// （self-relative）和 完全的（absolute），GetFileSecurity只能取到“自
		// 相关的”，而SetFileSecurity则需要完全的。这就是为什么需要一个新的SD，
		// 而不是直接在GetFileSecurity返回的SD上进行修改。因为“自相关的”信息
		// 是不完整的。
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
		// STEP 3: 初始化一个新的SD
		// 
		if (!InitializeSecurityDescriptor(&newSD, SECURITY_DESCRIPTOR_REVISION)) {
			_tprintf(TEXT("InitializeSecurityDescriptor() failed.")
				TEXT("Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 4: 从GetFileSecurity 返回的SD中取DACL
		// 
		if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL, &fDaclDefaulted)) {
			_tprintf(TEXT("GetSecurityDescriptorDacl() failed.Error %d\n"), GetLastError());
			__leave;
		}
		// 
		// STEP 5: 取 DACL的内存size
		//     GetAclInformation可以提供DACL的内存大小。只传入一个类型为
		// ACL_SIZE_INFORMATION的structure的参数，取DACL的信息，是为了
		// 方便我们遍历其中的ACE。
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

		// STEP 6: 计算新的ACL的size
		//    计算的公式是：原有的DACL的size加上需要添加的一个ACE的size，以
		// 及加上一个和ACE相关的SID的size，最后减去两个字节以获得精确的大小。
		cbNewACL = aclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(pUserSID) - sizeof(DWORD);

		// 
		// STEP 7: 为新的ACL分配内存
		// 
		pNewACL = (PACL)myHeapAlloc(cbNewACL);
		if (!pNewACL) {
			_tprintf(TEXT("myHeapAlloc() failed.Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 8: 初始化新的ACL结构
		// 
		if (!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2)) {
			_tprintf(TEXT("InitializeAcl() failed.Error %d\n"), GetLastError());
			__leave;
		}

		// 
		// STEP 9  如果文件（目录） DACL 有数据，拷贝其中的ACE到新的DACL中
		// 
		//     下面的代码假设首先检查指定文件（目录）是否存在的DACL，如果有的话，
		// 那么就拷贝所有的ACE到新的DACL结构中，我们可以看到其遍历的方法是采用
		// ACL_SIZE_INFORMATION结构中的AceCount成员来完成的。在这个循环中，
		// 会按照默认的ACE的顺序来进行拷贝（ACE在ACL中的顺序是很关键的），在拷
		// 贝过程中，先拷贝非继承的ACE（我们知道ACE会从上层目录中继承下来）
		// 
		newAceIndex = 0;
		if (fDaclPresent && aclInfo.AceCount) {
			for (currentAceIndex = 0; currentAceIndex < aclInfo.AceCount; currentAceIndex++) {
				// 
				// STEP 10: 从DACL中取ACE
				// 
				if (!GetAce(pACL, currentAceIndex, &pTempACE)) {
					_tprintf(TEXT("GetAce() failed. Error %d\n"),
						GetLastError());
					__leave;
				}
				// 
				// STEP 11: 检查是否是非继承的ACE
				//     如果当前的ACE是一个从父目录继承来的ACE，那么就退出循环。
				// 因为，继承的ACE总是在非继承的ACE之后，而我们所要添加的ACE
				// 应该在已有的非继承的ACE之后，所有的继承的ACE之前。退出循环
				// 正是为了要添加一个新的ACE到新的DACL中，这后，我们再把继承的
				// ACE拷贝到新的DACL中。
				//
				if (((ACCESS_ALLOWED_ACE*)pTempACE)->Header.AceFlags & INHERITED_ACE) {
					break;
				}
				// 
				// STEP 12: 检查要拷贝的ACE的SID是否和需要加入的ACE的SID一样，
				// 如果一样，那么就应该废掉已存在的ACE，也就是说，同一个用户的存取
				// 权限的设置的ACE，在DACL中应该唯一。这在里，跳过对同一用户已设置
				// 了的ACE，仅是拷贝其它用户的ACE。
				// 
				if (EqualSid(pUserSID, &(((ACCESS_ALLOWED_ACE*)pTempACE)->SidStart))) {
					//相同
					continue;
				}
				// 
				// STEP 13: 把ACE加入到新的DACL中
				//    下面的代码中，注意 AddAce 函数的第三个参数，这个参数的意思是 
				// ACL中的索引值，意为要把ACE加到某索引位置之后，参数MAXDWORD的
				// 意思是确保当前的ACE是被加入到最后的位置。
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
		// STEP 14: 把一个 access-allowed 的ACE 加入到新的DACL中
		//     前面的循环拷贝了所有的非继承且SID为其它用户的ACE，退出循环的第一件事
		// 就是加入我们指定的ACE。
		// 
		// Ex版的函数多出了一个参数AceFlag（第三人参数），用这个参数我们可以来设置一
		// 个叫ACE_HEADER的结构，以便让我们所设置的ACE可以被其子目录所继承下去，而 
		// AddAccessAllowedAce函数不能定制这个参数，在AddAccessAllowedAce函数
		// 中，其会把ACE_HEADER这个结构设置成非继承的。
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
		
		// STEP 15: 按照已存在的ACE的顺序拷贝从父目录继承而来的ACE
		//
		//bitset<32> bit(dwAccessMask);
		if (fDaclPresent && aclInfo.AceCount) {
			for (; currentAceIndex < aclInfo.AceCount; currentAceIndex++)
			{
				// 
				// STEP 16: 从文件（目录）的DACL中继续取ACE
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
		// STEP 18: 把新的ACL设置到新的SD中
		// 
		if (!SetSecurityDescriptorDacl(&newSD, TRUE, pNewACL, FALSE)) {
			_tprintf(TEXT("SetSecurityDescriptorDacl() failed. Error %d\n"),
				GetLastError());
			__leave;
		}
		// 
		// STEP 19: 把老的SD中的控制标记再拷贝到新的SD中，我们使用的是一个叫 
		// SetSecurityDescriptorControl() 的API函数
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
		// STEP 20: 把新的SD设置设置到文件的安全属性中
		// 
		if (!SetFileSecurity(lpszFileName, secInfo, &newSD)) {
			_tprintf(TEXT("SetFileSecurity() failed. Error %d\n"),GetLastError());
			__leave;
		}
		fResult = TRUE;
	}
	__finally {
		// 
		// STEP 21: 释放已分配的内存，以免Memory Leak
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