#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//��һ���ַ�������������������abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";��


//�������ֵָ����� �� �����߼�֮��Ĺ�ϵ
int spitString2(const char *buf1, char c, char **myp /*in*/, int *count)
{
	//strcpy(buf2[0], "aaaaa");
	//strcpy(buf2[1], "bbbbbb");
	char *p=NULL, *pTmp = NULL;
	int	tmpcount = 0;

	//1 p��ptmp��ʼ��
	p = buf1;
	pTmp = buf1;

	do 
	{
		//2 ��������������λ�� p����  �γɲ�ֵ ���ַ���
		p = strchr(p, c);
		if (p != NULL)
		{
			if (p-pTmp > 0)
			{
				strncpy(myp[tmpcount], pTmp,  p-pTmp);
				myp[tmpcount][p-pTmp]  = '\0';  //�ѵ�һ�����ݱ�� C����ַ���
				tmpcount ++;
				//3���� ��p��ptmp�ﵽ��һ�μ���������
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


	//�ͷ��ڴ�
	for (i=0; i<10; i++)
	{
		free(p[i]);
	}
	free(p);
}