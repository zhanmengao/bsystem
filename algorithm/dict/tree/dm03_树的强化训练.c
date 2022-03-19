#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
typedef struct BiTNode
{
	int data;
	struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;
*/

//��������ʾ��
struct BiTNode
{
	int data;
	struct BiTNode *lchild, *rchild;
};
typedef struct BiTNode		BiTNode;
typedef struct BiTNode*		BiTree;




void inOrder(BiTNode *root)
{
	if (root == NULL)
	{
		return;
	}
	//����������
	inOrder(root->lchild);
	printf("%d ", root->data);
	//����������
	inOrder(root->rchild);
}

int Depth(BiTNode *T)
{
	int		deptleft = 0;
	int		deptright = 0;
	int		deptval = 0;

	if (T == NULL)
	{
		deptval = 0;
		return deptval;
	}

	//���������ĸ߶�
	deptleft = Depth(T->lchild);

	//���������ĸ߶�
	deptright = Depth(T->rchild);

	//+1
	deptval = 1 + (deptleft>deptright ? deptleft:deptright);
	return deptval;
}


BiTNode * CopyTree(BiTNode *T)
{
	BiTNode *newNode = NULL;
	BiTNode *newLp = NULL;
	BiTNode *newRp = NULL;

	if (T == NULL)
	{
		return NULL;
	}

	//copy������
	if (T->lchild != NULL)
	{
		newLp = CopyTree(T->lchild); //copy������
	}
	else
	{
		newLp = NULL;
	}

	//copy������
	if (T->rchild != NULL)
	{
		newRp = CopyTree(T->rchild);
	}
	else
	{
		newRp = NULL;
	}

	//malloc���ڵ�
	newNode = (BiTNode *)malloc(sizeof(BiTNode));
	if (newNode == NULL)
	{
		return NULL;
	}
	newNode->lchild = newLp;
	newNode->rchild = newRp;
	newNode->data = T->data;
	return newNode;
}

void main444()
{
	BiTNode t1, t2, t3, t4, t5;
	memset(&t1, 0, sizeof(BiTNode));
	memset(&t2, 0, sizeof(BiTNode));
	memset(&t3, 0, sizeof(BiTNode));
	memset(&t4, 0, sizeof(BiTNode));
	memset(&t5, 0, sizeof(BiTNode));
	t1.data = 1;
	t2.data = 2;
	t3.data = 3;
	t4.data = 4;
	t5.data = 5;

	//������ϵ
	t1.lchild = &t2;
	t1.rchild = &t3;
	t2.lchild = &t4;
	t3.lchild = &t5;

	//���ı���
	printf("%d \n", Depth(&t1)) ;


	{
		BiTNode *root = CopyTree(&t1);
		printf("\ncopy inorder\n");
		inOrder(root);
		printf("hello...\n");

	}

	printf("\nolder inorder\n");
	inOrder(&t1);
	printf("hello...\n");

	system("pause");
	return ;
}
