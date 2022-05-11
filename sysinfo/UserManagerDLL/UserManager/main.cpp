#include"UserManager.h"
#include<stdlib.h>
#include<stdio.h>
#define USER_NAME  "qaq"
#define OLD_PASS  "P@ssword233"
#define NEW_PASS  "qaq1234567"
#define GROUP "gQAQ"
#define _ADD 1
void main()
{
#if _ADD
	printf("%s\n",ReturnErrInfo(AddUser(USER_NAME, OLD_PASS)));
	printf("%s\n", ReturnErrInfo(SetUserPasswordAdmin(USER_NAME, NEW_PASS)));
	printf("%s\n", ReturnErrInfo(AddGroup(GROUP)));
	printf("%s\n", ReturnErrInfo(AddUserToGroup(GROUP,USER_NAME)));
	printf("%s\n", ReturnErrInfo(DelUserToGroup(GROUP, USER_NAME)));
	printf("%s\n", ReturnErrInfo(AddUserToGroup(GROUP, USER_NAME)));
	printf("%s\n", ReturnErrInfo(SetUserActive(USER_NAME, false)));
#endif
	//≤‚ ‘¡–æŸ
	char *gName[30] = { 0 };
	for (int i = 0; i < 30; i++)
	{
		gName[i] = (char*)malloc(sizeof(char) * 260);
	}

	char *uName[30] = { 0 };
	for (int i = 0; i < 30; i++)
	{
		uName[i] = (char*)malloc(sizeof(char) * 260);
	}

	char *ugName[30] = { 0 };
	for (int i = 0; i < 30; i++)
	{
		ugName[i] = (char*)malloc(sizeof(char) * 260);
	}
	unsigned gSize = GetGroupList(gName);
	for (unsigned i = 0; i < gSize; i++)
	{
		printf("Group[%u]:%s \n", i, gName[i]);
		unsigned uSize = GetGroupUsers(gName[i],uName);
		for (unsigned j = 0; j < uSize; j++)
		{
			printf("	User[%u]:%s \n", j, uName[j]);
			unsigned gs = GetUserGroups(uName[j], ugName);
			for (unsigned k = 0; k < gs; k++)
			{
				printf("		Groups[%u]:%s \n", k, ugName[k]);
			}
		}
	}
	for (int i = 0; i < 30; i++)
	{
		free(ugName[i]);
		ugName[i] = NULL;
	}
	for (int i = 0; i < 30; i++)
	{
		free(uName[i]);
		uName[i] = NULL;
	}
	for (int i = 0; i < 30; i++)
	{
		free(gName[i]);
		gName[i] = NULL;
	}
#if _ADD
	printf("%s\n", ReturnErrInfo(SetUserActive(USER_NAME, true)));
	printf("%s\n", ReturnErrInfo(DelGroup(GROUP)));
	printf("%s\n", ReturnErrInfo(SetUserPassword(USER_NAME, NEW_PASS, OLD_PASS)));
	printf("%s\n", ReturnErrInfo(DelUser(USER_NAME)));
#endif
	system("pause");
}