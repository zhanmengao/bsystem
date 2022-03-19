#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>

//获取登录用户名
const char* GetUserName()
{
	return getenv("LOGNAME");
}

const char* GetPwd()
{
	return getenv("PWD");
}