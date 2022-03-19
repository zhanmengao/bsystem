#include"SysDefine.h"


int mainStrspn(int argc, char const *argv[])
{
	char *cs="wangbiyuwudi";
	char *ct="wang";
	char *ct2="yu";
	int ret,ret2;
	ret=strspn(cs,ct);
	ret2=strcspn(cs,ct2);
	printf("cs=%s,ct=%s,ct2=%s,ret=%u,ret2=%u\n",cs,ct,ct2,ret,ret2);
	return 0;
}