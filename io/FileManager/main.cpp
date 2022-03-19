#include"FileManager.h"
#include"Command.hpp"
#include"FileAttrHelper.h"
#include"PathHelper.hpp"
#include"SpecialFolderPath.cpp"
#include"../../../ZIP/ZIPUtils/ZIPUtils.h"
void mainLogic()
{
	char command[1024];
	PrintFIlesInfo();
	memset(command, 0, 1024);
	while (cin.getline(command, 1024))
	{
		string buf(command);
		if (buf == "a")
		{
			Back();
		}
		//前进
		else
		{
			To(wstring());
		}
		memset(command, 0, 1024);
	}
}
void main344()
{
	char buf[1024];
	memset(buf, 'a', 1023);
	buf[1023] = '\0';
	FileManager::_CreateFile(string(TEST_ROOT) + "aaa.txt", buf, 1024);

	cout << "Read File" << buf << endl;
	std::vector<FileInfo> mFileList;
	FileManager::GetAllFileInfo((TEST_ROOT), mFileList);
	for (int i = 0; i < mFileList.size(); i++)
	{
		auto &fileData = mFileList[i];
		if (string(fileData.name) == ".." ||
			string(fileData.name) == "3333.txt")
		{
			FileManager::RemoveFile(string(TEST_ROOT) + fileData.name);
		}
		else if (FileAttrHelper::IsFolder(fileData.attrib))
		{
			cout << "Folder:" << fileData.name << endl;
			FileManager::RenameFile(string(TEST_ROOT) + fileData.name, string(TEST_ROOT) + "newName");
		}
		else
		{
			cout << "FileName:" << fileData.name << endl;
		}
	}
	mainLogic();

}

void ACLmain()
{
	TCHAR uName[1024] = { 0 };
	DWORD uSize = 1024;
	GetUserName(uName, &uSize);
	printf("EnableFileAccountPrivilege %u \n", FileAttrHelper::EnableFileAccountPrivilege(L"D:\\tt2", uName));
	printf("EnableFileAccountPrivilege %u \n", FileAttrHelper::EnableFileAccountPrivilege(L"D:\\tt2", uName, DELETE));
	//printf("AltEnableFileAccountPrivilege %u \n", FileAttrHelper::AltEnableFileAccountPrivilege(L"D:\\tt2", uName, GENERIC_READ));
	printf("SetFolderShare %u \n", FileAttrHelper::SetFolderShare(L"D:\\tt2", L"DataShare", uName));
	printf("SetFolderPrivate %u \n", FileAttrHelper::SetFolderPrivate(L"DataShare"));
}

//将目录下所有文件输出到文件
void main()
{
	fstream fs("a.txt");
	if (!fs.is_open())
	{
		fs.open("a.txt", ios::out | ios::trunc);
	}
	//遍历目录
	std::vector<FileInfo> outFileList;
	FileManager::GetAllFileInfo("D:\\Space\\lib\\OpenCV\\BIN32", outFileList);
	for (auto it = outFileList.begin(); it != outFileList.end(); it++)
	{
		fs << it->name << std::endl;
	}
	fs.close();

#define FOLDER "D:\\Space\\Network\\MQ\\rocket-client-cpp\\Release\\"
#define CP "D:\\Space\\Network\\MQ\\rocket-client-cpp\\Cp\\"
	FolderInfo folder;
	FileManager::GetAllFileInfo(FOLDER, folder);

	//FileManager::CopyDirectory(FOLDER, CP);
	FolderInfo cp;
	FileManager::GetAllFileInfo(CP, cp);

#define ZIP_PATH "D:\\Space\\System\\File\\FileManager\\x64\\Debug\\veidp_v0.9.4.zip"
#define VEIDP_NEW "D:\\Space\\System\\File\\FileManager\\x64\\Debug\\newversion"
#define VEIDP_PATH "D:\\Space\\System\\File\\FileManager\\x64\\Debug"
	UnzipFile(ZIP_PATH, VEIDP_NEW);

	//遍历目录，替换文件
	FileManager::CopyDirectory(VEIDP_NEW, VEIDP_PATH);
	cin.get();
}

void Pathmain()
{
	wcout << PathHelper::GetDriveDir(L"D:") << endl;
	GetSpecialFolderQS();
	wcout << PathHelper::GetAppData() << endl;
	wcout << PathHelper::GetStarUp() << endl;
	wcout << PathHelper::GetWindowsFolder() << endl;
	wcout << PathHelper::GetSendTo() << endl;
	wcout << PathHelper::GetDocumentation() << endl;
	wcout << PathHelper::GetDesktop() << endl;
	wcout << PathHelper::GetProgram() << endl;
	wcout << PathHelper::GetCommnAppData() << endl;
	wcout << PathHelper::GetCommnProgram() << endl;
	wcout << PathHelper::GetTemp() << endl;
	wcout << PathHelper::GetUserFolder() << endl;
}
