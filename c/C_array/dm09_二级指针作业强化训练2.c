#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//��һ���ַ�������������������abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";��


//�������ֵָ����� �� �����߼�֮��Ĺ�ϵ
char ** spitString3(const char *buf1, char c, int *count)
{
	char *p=NULL, *pTmp = NULL;
	int	tmpcount = 0;
	char **myp = NULL;

	//1 p��ptmp��ʼ��
	p = buf1;
	pTmp = buf1;


	//��һ�����count
	do 
	{
		//2 ��������������λ�� p����  �γɲ�ֵ ���ַ���
		p = strchr(p, c);
		if (p != NULL)
		{
			if (p-pTmp > 0)
			{
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

	//���ݶ����� ��ȷ�ķ����ڴ�
	myp = (char **)malloc(tmpcount * sizeof(char *) );
	if (myp == NULL)
	{
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	tmpcount = 0;
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
				int len = p - pTmp + 1;
				myp[tmpcount] = (char *)malloc(len * sizeof(char));
				if (myp[tmpcount] == NULL)
				{
					return NULL;
				}
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
	
	//*count = tmpcount;
	return myp;
}

void FreeMem2(char **myp, int count)
{
	int i =0; 
	if (myp == NULL)
	{
		return;
	}
	for (i=0; i<count; i++)
	{
		if (myp[i] != NULL)
		{
			free(myp[i]);
		}
	}
	if (myp != NULL)
	{
		free(myp);
	}
}

void FreeMem3(char ***p, int count)
{
	int i =0; 
	char **myp = NULL;
	if (p == NULL)
	{
		return;
	}
	myp = *p;

	if (myp == NULL) 
	{
		return ;
	}

	for (i=0; i<count; i++)
	{
		if (myp[i] != NULL)
		{
			free(myp[i]);
		}
	}
	if (myp != NULL)
	{
		free(myp);
	}
	*p = NULL; //��ʵ�ζ���ָ�� ���޸ĳ�NULL
}

//�������ֵָ����� �� �����߼�֮��Ĺ�ϵ
int  spitString4(const char *buf1, char c, char ***myp3, int *count)
{
	int ret = 0;
	char *p=NULL, *pTmp = NULL;
	int	tmpcount = 0;
	char **myp = NULL;

	//1 p��ptmp��ʼ��
	p = buf1;
	pTmp = buf1;


	//��һ�����count
	do 
	{
		//2 ��������������λ�� p����  �γɲ�ֵ ���ַ���
		p = strchr(p, c);
		if (p != NULL)
		{
			if (p-pTmp > 0)
			{
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

	//���ݶ����� ��ȷ�ķ����ڴ�
	myp = (char **)malloc(tmpcount * sizeof(char *) );
	if (myp == NULL)
	{
		ret = -1;
		printf("func spitString4() err:%d  (tmpcount * sizeof(char *) )", ret);
		goto END;
		//return -1;
	}
	memset(myp, 0, tmpcount * sizeof(char *));

	//////////////////////////////////////////////////////////////////////////

	tmpcount = 0;
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
				int len = p - pTmp + 1;
				myp[tmpcount] = (char *)malloc(len * sizeof(char));
				if (myp[tmpcount] == NULL)
				{
					ret = -2;
					printf("func spitString4() err:%d  malloc(len * sizeof(char) )", ret);
					goto END;
				}
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

END:	
	if (ret != 0) //ʧ��
	{
		//FreeMem2(NULL, *count ); 
		//myp= NULL;
		FreeMem3(&myp, *count);
	}
	else
	{
		*myp3 = myp;  //�ɹ�
	}	
	return ret;
}


//�༶ָ�� ����Ұָ�� 
void main93()
{
	int ret = 0, i = 0;
	char *p1 = "abcdef,acccd,";
	char cTem= ',';
	int nCount;

	char **p = NULL;  //char buf[10][30]


	p= spitString3(p1, cTem, &nCount);
	if (p == NULL)
	{
		printf("fucn spitString() err: %d \n", ret);
		return ret;
	}

	for (i=0; i<nCount; i++ )
	{
		printf("%s \n", p[i]);
	}


	//�ͷ��ڴ�
	for (i=0; i<nCount; i++)
	{
		free(p[i]);
	}
	free(p);
}



void main309()
{
	int ret = 0, i = 0;
	char *p1 = "abcdef,acccd,";
	char cTem= ',';
	int nCount;

	char **p = NULL;  //char buf[10][30]


	ret = spitString4(p1, cTem, &p,  &nCount);
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
	for (i=0; i<nCount; i++)
	{
		free(p[i]);
	}
	free(p);
}