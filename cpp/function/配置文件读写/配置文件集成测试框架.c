#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cfg_op.h"

#define CFGNAME "c:/mycfg.ini"
void mymenu()
{
	printf("=============================\n");
	printf("1 测试写配置文件\n");
	printf("2 测试读配置文件\n");
	printf("0 退出\n");
	printf("=============================\n");
}

//获取配置项
int TGetCfg()
{
	int		ret = 0;
	//读配置项 
	char	name[1024] = {0};
	char	valude[1024] = {0};
	int		vlen = 0;

	printf("\n请键入key:");
	scanf("%s", name);

	ret = GetCfgItem(CFGNAME /*in*/, name /*in*/, valude/*in*/, &vlen);
	if (ret != 0)
	{
		printf("func WriteCfgItem err:%d \n", ret);
		return ret;
	}
	printf("valude:%s \n", valude);

}

//写配置项 
int TWriteCfg()
{
	int		ret = 0;
	//写配置项 
	char name[1024] = {0};
	char valude[1024] = {0};


	printf("\n请键入key:");
	scanf("%s", name);

	printf("\n请键入valude:");
	scanf("%s", valude);

	ret = WriteCfgItem(CFGNAME /*in*/, name /*in*/, valude/*in*/,strlen(valude) /*in*/);
	if (ret != 0)
	{
		printf("func WriteCfgItem err:%d \n", ret);
		return ret;
	}
	printf("你的输入是：%s = %s \n", name , valude);
	return ret;
}

void main()
{

	int choice;

	for (;;)
	{
		//显示一个菜单
		mymenu();
		scanf("%d", &choice);
		switch (choice)
		{
			
		case 1:   //写配置项
			TWriteCfg();
			break;
		case 2: 
			TGetCfg(); //读配置项
			break;
		case 0: 
			exit(0);
		default:;
			exit(0);
		}

	}
	printf("hello...\n");
	system("pause");
	return ;
}