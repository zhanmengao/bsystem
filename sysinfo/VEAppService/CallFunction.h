#pragma once

#include <vector>
#include <atlstr.h>
#include "WindowsUser.h"
#include "GCDirFile.h"
#include "CommonFiles.h"
#include "WindowApp.h"

using namespace std;

class CallFunction {

private:

	CWindowsUser windowsUser;
	GCDirFile    dirfiles;
	WindowApp	 windowAPP;

public:
	CallFunction();
	~CallFunction();
	int WebR_GetUserList(vector<CString> &AddList);
	int WebR_GetUserList(vector<CString> &AddList, CString GroupName);
	int WebR_GetGroupList(vector<CString> &AddList);

	int WebR_AddSysUser(CString UserNameStr, CString PasswordStr);
	int WebR_SetUserStatus(CString UserNameStr, int status);
	int WebR_DelSysUser(CString UserNameStr);

	int WebR_AddUserToGroup(CString GroupNameStr, CString UserNameStr);
	int WebR_DelUserToGroup(CString GroupNameStr, CString UserNameStr);

	int WebR_AddSysGroup(CString UserGroupStr);
	int WebR_DelSysGroup(CString UserGroupStr);
	

	int WebR_SetUserInfo(CString UserName, UserStruct_I1 UserInfo);
	int WebR_GetUserInfo(CString UserName, UserStruct_I1 &UserInfo);

	int WebR_GetUserToGroups(CString UserNameStr, vector<CString> &GroupList);
	int WebR_UserLSGroupListCString(CString UserName);
	int WebR_UserLSGroup(CString UserName, CString GroupName);

	int WebR_ReturnErrInfo(int WebR_info);

	int WebR_SetUserProfile(CString UserName, UserStruct_I1 UserInfo);
	int WebR_SetUserHomeDir(CString UserName, UserStruct_I1 UserInfo);
	
	int WebR_ReadDirPathList( string DirPath, vector<Filepath> &filespath);

	int WebR_DownloadFile(SOCKET socket, const char *path);

	int WebR_MoveFile(const char *newPath,const char *oldPath);
	int WebR_RecvFile(const char *url, string path);
	int WebR_copyFile(const char *oldPath, const char *newPath);
	int WebR_deleteFile(const char *filePath);
	int WebR_CreateFolder(const char *path);
	int WebR_FolderAddOrDelGroup(char *folderPath, char *groupName, DWORD dwAccessMask, UINT status);
	int WebR_renameFile(const char *oldName, const char *newName);
	int WebR_SetUserFolderSize(string path, string size);
	int WebR_ModifyUserFolderSize(string path, string size);
	int WebR_DelUserFolderSize(string path);



	int WebR_GetInstallAppList(Json::Value &Items);
	int WebR_GetVEAppList(Json::Value & Items);
	int WebR_AppendAppList(Json::Value & Items);
	int WebR_DeleteVEAppList(Json::Value & Items);
};