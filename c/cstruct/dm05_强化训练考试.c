

#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//排序myp1和myp2的集合，排序结果放到myp3
int sort(char **myp1 /*in*/, int num1, char (*myp2)[30], int num2, char ***myp3, int *num3)   //myp1:指向字符串的指针  myp2:指向char[30]的指针
{
	int i = 0, j = 0, k= 0;
	int  tmplen = 0;
	char **p3 = NULL;
	char *tmpP = NULL;
	p3 = (char **)malloc( (num1 + num2) * sizeof(char *)  ); //里面装的是指针
	if (p3 == NULL) 
	{
		return -1;
	}
	//根据

	//把myp1的数据拷贝到p3
	for (i=0; i<num1; i++)
	{
		tmplen= strlen(myp1[i]) + 1;
		p3[i] = (char *)malloc( tmplen * sizeof(char)) ;
		if (p3[i] == NULL)
		{
			return -2;
		}
		strcpy(p3[i], myp1[i]);
	}

	//把myp2的数据也拷贝到p3
	for (j=0; j<num2; j++, i++)
	{
		tmplen = strlen(myp2[j]) + 1;
		p3[i] = (char *)malloc (tmplen * sizeof(char));
		if (p3[i] == NULL)
		{
			return -3;
		}
		strcpy(p3[i], myp2[j]);
	}

	tmplen = num1 + num2;
	//*num3 = num1 + num2;

	//排序
	for (i=0; i<tmplen; i++)
	{
		for (j=i+1; j<tmplen; j++)
		{
			if ( strcmp(p3[i], p3[j]) > 0 )
			{
				tmpP = p3[i];
				p3[i] = p3[j];
				p3[j] = tmpP;
			}
		}
	}

	//间接赋值
	*num3 = tmplen;
	*myp3 = p3;
	return 0;
}


void sortFree01(char **p, int len)
{
	int i = 0;
	if (p == NULL)
	{
		return ;
	}

	for (i=0; i<len ; i++)
	{
		free(p[i]);
	}
	free(p);
}

void sortFree02(char ***myp, int len) //把二级指针指向二维内存释放掉,,同时间接的修改了实参的值  myp指向二维数组
{
	int i = 0;
	char **p = NULL;
	if (myp == NULL)
	{
		return ;
	}

	p  = *myp; //还原成二级指针
	if (p == NULL)
	{
		return ;
	}

	for (i=0; i<len ; i++)
	{
		free(p[i]);
	}
	free(p);
	//myp 是实参的地址
	*myp = NULL; //间接赋值是指针存在的最大意义
}

int  main001()
{
	int ret = 0;
	char *p1[] = {"aa", "ccccccc", "bbbbbb"};
	char buf2[10][30] = {"111111", "3333333", "222222"};
	char **p3 = NULL;
	int len1, len2, len3, i = 0;

	len1 = sizeof(p1)/sizeof(*p1);
	len2 = 3;

	ret = sort(p1, len1, buf2, len2, &p3, &len3);
	if (ret != 0)
	{
		printf("func sort() err:%d \n", ret);
		return ret;
	}

	for (i=0; i<len3; i++)
	{
		printf("%s\n", p3[i]);
	}
	return 0;
}