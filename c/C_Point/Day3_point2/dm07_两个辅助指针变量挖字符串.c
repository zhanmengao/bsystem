#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//��һ���ַ�������������������abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";��


//�������ֵָ����� �� �����߼�֮��Ĺ�ϵ
int spitString(const char *buf1, char c, char buf2[10][30], int *count)
{
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
				strncpy(buf2[tmpcount], pTmp,  p-pTmp);
				buf2[tmpcount][p-pTmp]  = '\0';  //�ѵ�һ�����ݱ�� C����ַ���
				tmpcount ++;
				//3���� ��p��ptmp�ﵽ��һ�μ���������
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



//��ҵ �õ������ڴ�ģ���������
/*
int spitString2(const char *buf1, char c, char ***pp, int *count)
{
	
}


char ** spitString3(const char *buf1, char c, int *count)
{

}
*/