
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seqlist.h"


//�ڽṹ������1��ָ��
//
typedef struct _tag_SeqList
{
	int length;
	int capacity;
	unsigned int *node;   //int* node[]
}TSeqList;

SeqList* SeqList_Create(int capacity)
{
	int ret = 0;
	TSeqList *tmp = NULL;

	tmp = (TSeqList *)malloc(sizeof(TSeqList));
	if (tmp == NULL)
	{
		ret = -1;
		printf("func SeqList_Create() err:%d \n", ret);
		return NULL;
	}
	memset(tmp, 0, sizeof(TSeqList));

	//����capacity �Ĵ�С����ڵ�Ŀռ�
	tmp->node = (unsigned int *)malloc(sizeof(unsigned int *) * capacity);
	if (tmp->node  == NULL)
	{
		ret = -2;
		printf("func SeqList_Create() err: malloc err %d \n", ret);
		return NULL;
	}
	tmp->capacity = capacity;
	tmp->length = 0;
	return tmp;
}

void SeqList_Destroy(SeqList* list)
{
	TSeqList *tlist = NULL;
	if (list == NULL)
	{
		return ;
	}
	tlist = (TSeqList *)list;
	if (tlist->node != NULL)
	{
		free(tlist->node);
	}
	
	free(tlist);

	return ;
}

//������� //�ص���ʼ��״̬
void SeqList_Clear(SeqList* list)
{
	TSeqList *tlist = NULL;
	if (list == NULL)
	{
		return ;
	}
	tlist = (TSeqList *)list;
	tlist->length = 0; 
	return ;
}

int SeqList_Length(SeqList* list)
{
	TSeqList *tlist = NULL;
	if (list == NULL)
	{
		return -1;
	}
	tlist = (TSeqList *)list;
	return tlist->length;
}

int SeqList_Capacity(SeqList* list)
{

	TSeqList *tlist = NULL;
	if (list == NULL)
	{
		return -1;
	}
	tlist = (TSeqList *)list;
	return tlist->capacity;
}

int SeqList_Insert(SeqList* list, SeqListNode* node, int pos)
{
	int i =0, ret = 0;
	TSeqList *tlist = NULL;

	if (list == NULL || node==NULL ||  pos<0)
	{
		ret = -1;
		printf("fun SeqList_Insert() err:%d \n", ret);
		return ret;
	}
	tlist = (TSeqList*)list;

	//�ж��ǲ�������
	if (tlist->length >= tlist->capacity)
	{
		ret = -2;
		printf("fun SeqList_Insert() (tlist->length >= tlist->capacity) err:%d \n", ret);
		return ret;
	}

	//�ݴ�����  6������ ����20;�û�pos10λ�ò���..
	if (pos>=tlist->length)
	{
		pos = tlist->length; //
	}

	//1 Ԫ�غ���
	for(i=tlist->length; i>pos; i--)
	{
		tlist->node[i] = tlist->node[i-1];
		//a[7] = a[6]
	}
	// i = 3
	// 2����Ԫ��
	tlist->node[i] = (unsigned int )node;
	tlist->length ++;
	return 0;
}

SeqListNode* SeqList_Get(SeqList* list, int pos)
{
	int i =0;
	SeqListNode *ret = 0;
	TSeqList *tlist = NULL;

	if (list == NULL ||  pos<0)
	{
		printf("fun SeqList_Get() err:%d \n", ret);
		return NULL;
	}
	tlist = (TSeqList*)list;

	ret = (void *)tlist->node[pos];
	return ret;
}

SeqListNode* SeqList_Delete(SeqList* list, int pos)
{
	int i = 0;
	SeqListNode *ret = 0;
	TSeqList *tlist = NULL;

	if (list == NULL ||  pos<0) //���
	{
		printf("fun SeqList_Delete() err:%d \n", ret);
		return NULL;
	}
	tlist = (TSeqList*)list;

	ret = (SeqListNode *)tlist->node[pos]; //����pos��λ��
	 
	for (i=pos+1; i<tlist->length; i++)  //posλ�ú����Ԫ��ǰ��
	{
		tlist->node[i-1] = tlist->node[i];
	}
	tlist->length --;
	return ret;
}
