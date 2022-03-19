
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seqlist.h"


//在结构体中套1级指针
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

	//根据capacity 的大小分配节点的空间
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

//清空链表 //回到初始化状态
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

	//判断是不是满了
	if (tlist->length >= tlist->capacity)
	{
		ret = -2;
		printf("fun SeqList_Insert() (tlist->length >= tlist->capacity) err:%d \n", ret);
		return ret;
	}

	//容错修正  6个长度 容量20;用户pos10位置插入..
	if (pos>=tlist->length)
	{
		pos = tlist->length; //
	}

	//1 元素后移
	for(i=tlist->length; i>pos; i--)
	{
		tlist->node[i] = tlist->node[i-1];
		//a[7] = a[6]
	}
	// i = 3
	// 2插入元素
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

	if (list == NULL ||  pos<0) //检查
	{
		printf("fun SeqList_Delete() err:%d \n", ret);
		return NULL;
	}
	tlist = (TSeqList*)list;

	ret = (SeqListNode *)tlist->node[pos]; //缓存pos的位置
	 
	for (i=pos+1; i<tlist->length; i++)  //pos位置后面的元素前移
	{
		tlist->node[i-1] = tlist->node[i];
	}
	tlist->length --;
	return ret;
}
