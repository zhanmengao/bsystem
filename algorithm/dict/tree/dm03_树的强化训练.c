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

//二叉链表示法
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
	//遍历左子树
	inOrder(root->lchild);
	printf("%d ", root->data);
	//遍历右子树
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

	//求左子树的高度
	deptleft = Depth(T->lchild);

	//求右子树的高度
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

	//copy左子树
	if (T->lchild != NULL)
	{
		newLp = CopyTree(T->lchild); //copy左子树
	}
	else
	{
		newLp = NULL;
	}

	//copy右子树
	if (T->rchild != NULL)
	{
		newRp = CopyTree(T->rchild);
	}
	else
	{
		newRp = NULL;
	}

	//malloc根节点
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

	//建立关系
	t1.lchild = &t2;
	t1.rchild = &t3;
	t2.lchild = &t4;
	t3.lchild = &t5;

	//树的遍历
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
