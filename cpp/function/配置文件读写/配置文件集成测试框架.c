#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cfg_op.h"

#define CFGNAME "c:/mycfg.ini"
void mymenu()
{
	printf("=============================\n");
	printf("1 ����д�����ļ�\n");
	printf("2 ���Զ������ļ�\n");
	printf("0 �˳�\n");
	printf("=============================\n");
}

//��ȡ������
int TGetCfg()
{
	int		ret = 0;
	//�������� 
	char	name[1024] = {0};
	char	valude[1024] = {0};
	int		vlen = 0;

	printf("\n�����key:");
	scanf("%s", name);

	ret = GetCfgItem(CFGNAME /*in*/, name /*in*/, valude/*in*/, &vlen);
	if (ret != 0)
	{
		printf("func WriteCfgItem err:%d \n", ret);
		return ret;
	}
	printf("valude:%s \n", valude);

}

//д������ 
int TWriteCfg()
{
	int		ret = 0;
	//д������ 
	char name[1024] = {0};
	char valude[1024] = {0};


	printf("\n�����key:");
	scanf("%s", name);

	printf("\n�����valude:");
	scanf("%s", valude);

	ret = WriteCfgItem(CFGNAME /*in*/, name /*in*/, valude/*in*/,strlen(valude) /*in*/);
	if (ret != 0)
	{
		printf("func WriteCfgItem err:%d \n", ret);
		return ret;
	}
	printf("��������ǣ�%s = %s \n", name , valude);
	return ret;
}

void main()
{

	int choice;

	for (;;)
	{
		//��ʾһ���˵�
		mymenu();
		scanf("%d", &choice);
		switch (choice)
		{
			
		case 1:   //д������
			TWriteCfg();
			break;
		case 2: 
			TGetCfg(); //��������
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