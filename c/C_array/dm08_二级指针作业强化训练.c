#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//有一个字符串符合以下特征（”abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";）


//分清楚赋值指针变量 和 操作逻辑之间的关系
int spitString2(const char *buf1, char c, char **myp /*in*/, int *count)
{
	//strcpy(buf2[0], "aaaaa");
	//strcpy(buf2[1], "bbbbbb");
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
				strncpy(myp[tmpcount], pTmp,  p-pTmp);
				myp[tmpcount][p-pTmp]  = '\0';  //把第一行数据变成 C风格字符串
				tmpcount ++;
				//3重新 让p和ptmp达到下一次检索的条件
				pTmp = p = p + 1;
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

void main881()
{
	int ret = 0, i = 0;
	char *p1 = "abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";
	char cTem= ',';
	int nCount;

	char **p = NULL;  //char buf[10][30]
	p = (char **)malloc(10 * sizeof(char *)); // char * array[10]
	if (p == NULL)
	{
		return;
	}
	for (i=0; i<10; i++)
	{
		p[i] = (char *)malloc(30 * sizeof(char)); 
	}

	ret = spitString2(p1, cTem, p, &nCount);
	if (ret != 0)
	{
		printf("fucn spitString() err: %d \n", ret);
		return ret;
	}

	for (i=0; i<nCount; i++ )
	{
		printf("%s \n", p[i]);
	}


	//释放内存
	for (i=0; i<10; i++)
	{
		free(p[i]);
	}
	free(p);
}