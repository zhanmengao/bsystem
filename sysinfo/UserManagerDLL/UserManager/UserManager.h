#pragma once

#define ULONG unsigned long
#define EXPORT extern "C" __declspec(dllexport)

EXPORT ULONG  AddUserPlus(const char* userName, const char* password,const char* groupName1,const char* groupName2);

EXPORT ULONG  AddUser(const char* userName, const char* password);

EXPORT ULONG DelUser(const char* userName);

EXPORT ULONG AddGroupPlus(const char* groupName, const char* user);
EXPORT ULONG AddGroup(const char* groupName);

EXPORT ULONG DelGroup(const char* groupName);

EXPORT ULONG AddUserToGroup(const char* group, const char* user);

EXPORT ULONG DelUserToGroup(const char* group, const char* user);

EXPORT ULONG GetGroupList(char* GroupList,int& bufSize);

EXPORT ULONG GetGroupUsers(const char* group, char* UserList, int& bufSize);

EXPORT ULONG GetUserGroups(const char* user, char* uGroupList, int& bufSize);

EXPORT ULONG SetUserActive(const char* user, bool bActive);

EXPORT ULONG SetUserPassword(const char* user, const char* oldpassword, const char* newpassword);

EXPORT ULONG SetUserPasswordAdmin(const char* UserName, const char* UserPassword);

EXPORT  unsigned ReturnErrInfo(ULONG info, char* buf, int& len);