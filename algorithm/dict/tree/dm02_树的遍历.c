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


void preOrder(BiTNode *root)
{
	if (root == NULL)
	{
		return;
	}
	//printf("%d ", root->data);

	//遍历左子树
	preOrder(root->lchild);

	//遍历右子树
	preOrder(root->rchild);
}

static void inOrder(BiTNode *root)
{
	if (root == NULL)
	{
		return;
	}
	//遍历左子树
	inOrder(root->lchild);
	//printf("%d ", root->data);
	//遍历右子树
	inOrder(root->rchild);
}

void postOrder(BiTNode *root)
{
	if (root == NULL)
	{
		return;
	}
	//遍历左子树
	postOrder(root->lchild);

	//遍历右子树
	postOrder(root->rchild);
	//printf("%d ", root->data);
}

int sum;

void coutLeaf2(BiTNode *T)
{
	if (T != NULL)
	{
		if (T->lchild == NULL && T->rchild==NULL)
		{
			sum ++ ;
		}
		if (T->lchild )
		{
			coutLeaf2(T->lchild);
		}

		if (T->rchild)
		{
			coutLeaf2(T->rchild);
		}
	}
}


void coutLeaf(BiTNode *T)
{
	if (T != NULL)
	{
		if (T->lchild == NULL && T->rchild==NULL)
		{
			sum ++ ;
		}
		if (T->lchild )
		{
			coutLeaf(T->lchild);
		}

		if (T->rchild)
		{
			coutLeaf(T->rchild);
		}
	}
}

void coutLeaf3(BiTNode *T, int *sum)
{
	if (T != NULL)
	{
		if (T->lchild == NULL && T->rchild==NULL)
		{
			(*sum) ++ ; //++  *p  p++ sum//
		}
		if (T->lchild )
		{
			coutLeaf3(T->lchild, sum);
		}

		if (T->rchild)
		{
			coutLeaf3(T->rchild, sum);
		}
	}
}

void coutLeaf4(BiTNode *T, int *sum)
{
	if (T != NULL)
	{
		if (T->lchild )
		{
			coutLeaf4(T->lchild, sum);
		}

		if (T->rchild)
		{
			coutLeaf4(T->rchild, sum);
		}

		if (T->lchild == NULL && T->rchild==NULL)
		{
			(*sum) ++ ; //++  *p  p++ sum//
		}
	}
}

void main222()
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

	sum = 0;

	coutLeaf(&t1);
	printf("sum:%d \n", sum);

	{
		int mysum = 0;
		coutLeaf3(&t1, &mysum);
		printf("mysum:%d \n", mysum);
	}

	{
		int mysum4 = 0;
		coutLeaf4(&t1, &mysum4);
		printf("mysum4:%d \n", mysum4);
		
	}


	/*

	//树的遍历
	printf("preOrder\n");
	preOrder(&t1);
	printf("inorder\n");
	inOrder(&t1);

	printf("postOrder\n");
	postOrder(&t1);

	printf("hello...\n");
	*/
	system("pause");
	return ;
}
