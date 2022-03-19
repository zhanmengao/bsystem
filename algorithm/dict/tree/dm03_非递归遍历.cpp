
#include <iostream>
using namespace std;
#include "stack"




	//二叉链表示法
struct BiTNode
{
	int data;
	struct BiTNode *lchild, *rchild;
};
typedef struct BiTNode		BiTNode;
typedef struct BiTNode*		BiTree;

/*
步骤1：
如果结点有左子树，该结点入栈；
如果结点没有左子树，访问该结点；
步骤2：
如果结点有右子树，重复步骤1；
如果结点没有右子树（结点访问完毕），根据栈顶指示回退，访问栈顶元素，并访问右子树，重复步骤1
如果栈为空，表示遍历结束。 

*/

//一直向左走,找到中序遍历的起点
BiTNode * goLeft(BiTNode *T, stack<BiTNode *> &s)
{
	if (T == NULL)
	{
		return NULL;
	}
	//判断T有没有左孩子, 没有把Treturn ;
	//如果有 T入栈 ,再往左走
	while(T->lchild != NULL)
	{
		s.push(T);
		T = T->lchild;
	}
	return T;
}
void Inorder2(BiTNode *T)
{
	BiTNode *t = NULL;
	stack<BiTNode *> s;

	t = goLeft(T, s);

	while (t)
	{
		printf("%d ", t->data); //

		//如果t有右子树 重复步骤1
		if (t->rchild != NULL)
		{
			t = goLeft(t->rchild, s); //右子树中中序遍历的起点
		}
		else if (!s.empty()) //如果t没有右子树 根据栈顶指示 回退
		{
			t = s.top();
			s.pop();
		}
		else //如果t没有右子树 并且栈为空  
		{
			t = NULL;
		}
	}

}

static void inOrder(BiTNode *root)
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


void main333()
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

	printf("\nolder inorder\n");
	inOrder(&t1);

	printf("\n 非递归遍历\n");
	Inorder2(&t1);
	printf("hello...\n");

	system("pause");
	return ;
}
