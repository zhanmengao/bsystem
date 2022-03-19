#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//有一个字符串符合以下特征（”abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";）


//分清楚赋值指针变量 和 操作逻辑之间的关系
int spitString(const char *buf1, char c, char buf2[10][30], int *count)
{
	char *p=NULL, *pTmp = NULL;
	int	tmpcount = 0;

	//1 p和ptmp初始化
	p = buf1;
	pTmp = buf1;

	do 
	{
		//2 检索符合条件的位置 p后移  形成差值 挖字符串
		p = strchr(p, c);
		if (p != NULL)
		{
			if (p-pTmp > 0)
			{
				strncpy(buf2[tmpcount], pTmp,  p-pTmp);
				buf2[tmpcount][p-pTmp]  = '\0';  //把第一行数据变成 C风格字符串
				tmpcount ++;
				//3重新 让p和ptmp达到下一次检索的条件
				p = p + 1;
				pTmp = p;
			}
		}
		else
		{
			break;
		}
	} while (*p!='\0');
	
	*count = tmpcount;
	return 0;
}

void main377S()
{
	int ret = 0, i = 0;
	char *p1 = "abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";
	char cTem= ',';
	int nCount;

	char myArray[10][30];

	ret = spitString(p1, cTem, myArray, &nCount);
	if (ret != 0)
	{
		printf("fucn spitString() err: %d \n", ret);
		return ret;
	}

	for (i=0; i<nCount; i++ )
	{
		printf("%s \n", myArray[i]);
	}
}



//作业 用第三种内存模型求解问题
/*
int spitString2(const char *buf1, char c, char ***pp, int *count)
{
	
}


char ** spitString3(const char *buf1, char c, int *count)
{

}
*/