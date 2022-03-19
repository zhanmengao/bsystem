#include "stdafx.h"
#include "CallFunction.h"


CallFunction::CallFunction()
{
}


CallFunction::~CallFunction()
{
}

int CallFunction::WebR_GetUserList(vector<CString> &AddList)
{
	return windowsUser.GetUserList(AddList);
}

int CallFunction::WebR_GetUserList(vector<CString> &AddList, CString GroupName)
{

	return 0;
}

int CallFunction::WebR_GetGroupList(vector<CString> &AddList)
{
	//CWindowsUser windowsUser;
	windowsUser.GetGroupList( AddList );
	return 0;
}

int CallFunction::WebR_AddSysUser(CString UserNameStr, CString PasswordStr)
{	
	return windowsUser.AddSysUser(UserNameStr, PasswordStr);
}

int CallFunction::WebR_SetUserStatus(CString UserNameStr, int status )
{
	return windowsUser.SetUserStatus(UserNameStr, status);
}

int CallFunction::WebR_AddSysGroup(CString UserGroupStr)
{
	return windowsUser.AddUserGroup(UserGroupStr);
}

int CallFunction::WebR_DelSysGroup(CString UserGroupStr)
{
	return windowsUser.DelUserGroup(UserGroupStr);
}


int CallFunction::WebR_DelSysUser(CString UserNameStr)
{
	return windowsUser.DelSysUser(UserNameStr);
}

int CallFunction::WebR_AddUserToGroup(CString GroupNameStr, CString UserNameStr)
{
	return windowsUser.AddUserToGroup(GroupNameStr, UserNameStr);
}

int CallFunction::WebR_DelUserToGroup(CString GroupNameStr, CString UserNameStr)
{
	return windowsUser.DelUserToGroup(GroupNameStr, UserNameStr);
}

int CallFunction::WebR_SetUserInfo(CString UserName, UserStruct_I1 UserInfo)
{
	return 0;
}

int CallFunction::WebR_GetUserInfo(CString UserName, UserStruct_I1 &UserInfo)
{
	return 0;
}

int CallFunction::WebR_GetUserToGroups(CString UserNameStr, vector<CString> &GroupList)
{
	return 0;
}

int CallFunction::WebR_UserLSGroupListCString(CString UserName)
{
	return 0;
}

int CallFunction::WebR_UserLSGroup(CString UserName, CString GroupName)
{
	
	return 0;
}

int CallFunction::WebR_ReturnErrInfo(int WebR_info)
{
	return 0;
}

int CallFunction::WebR_SetUserProfile(CString UserName, UserStruct_I1 UserInfo)
{
	return 0;
}

int CallFunction::WebR_SetUserHomeDir(CString UserName, UserStruct_I1 UserInfo)
{
	return windowsUser.SetUserHomeDir(UserName, UserInfo);
}

/////////////////////////////////////////////////////////////////////////////////////////////
int CallFunction::WebR_ReadDirPathList( string DirPath, vector<Filepath> &filespath )
{
	return dirfiles.ReadDirPathList(DirPath, filespath);
}

int CallFunction::WebR_DownloadFile(SOCKET socket, const char *path)
{
	return dirfiles.SendFile(socket, path);
}

int CallFunction::WebR_MoveFile(const char *newPath, const char *oldPath) {

	return dirfiles.moveFile( oldPath, newPath);
}

int CallFunction::WebR_copyFile(const char *oldPath, const char *newPath) {

	return dirfiles.copyFile(oldPath, newPath);
}

int CallFunction::WebR_deleteFile(const char *filePath) {
	return dirfiles.deleteFile(filePath,"");
}


int CallFunction::WebR_RecvFile( const char *url, string path)
{
	return dirfiles.RecvFile(url,path);
}

int CallFunction::WebR_CreateFolder(const char *path)
{
	return dirfiles.CreateFolder(path);
}

int CallFunction::WebR_FolderAddOrDelGroup(char *folderPath, char *groupName, DWORD dwAccessMask,UINT status)
{
	return dirfiles.FolderAddOrDelGroup(folderPath,groupName, dwAccessMask, status);
}

int CallFunction::WebR_renameFile(const char *oldName, const char *newName)
{
	return dirfiles.renameFile(oldName, newName);
}

int CallFunction::WebR_SetUserFolderSize(string path,string size) 
{
	return dirfiles.SetUserFolderSize(path,size);
}

int CallFunction::WebR_ModifyUserFolderSize(string path, string size) 
{
	return dirfiles.ModifyUserFolderSize(path, size);
}

int CallFunction::WebR_DelUserFolderSize(string path) 
{
	return dirfiles.DelUserFolderSize(path);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int CallFunction::WebR_GetInstallAppList(Json::Value &Items) 
{

	return  windowAPP.GetInstallAppList(Items);
}

int CallFunction::WebR_GetVEAppList(Json::Value &Items)
{

	return  windowAPP.GetVEAppList(Items);
}

int CallFunction::WebR_AppendAppList(Json::Value &Items)
{

	return  windowAPP.AppendVEApp(Items);
}

int CallFunction::WebR_DeleteVEAppList(Json::Value &Items)
{

	return  windowAPP.DeleteVEApp(Items);
}

