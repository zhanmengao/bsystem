
#include <iostream>
using namespace std;
#include "stack"




	//��������ʾ��
struct BiTNode
{
	int data;
	struct BiTNode *lchild, *rchild;
};
typedef struct BiTNode		BiTNode;
typedef struct BiTNode*		BiTree;

/*
����1��
�����������������ý����ջ��
������û�������������ʸý�㣻
����2��
�����������������ظ�����1��
������û������������������ϣ�������ջ��ָʾ���ˣ�����ջ��Ԫ�أ����������������ظ�����1
���ջΪ�գ���ʾ���������� 

*/

//һֱ������,�ҵ�������������
BiTNode * goLeft(BiTNode *T, stack<BiTNode *> &s)
{
	if (T == NULL)
	{
		return NULL;
	}
	//�ж�T��û������, û�а�Treturn ;
	//����� T��ջ ,��������
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

		//���t�������� �ظ�����1
		if (t->rchild != NULL)
		{
			t = goLeft(t->rchild, s); //��������������������
		}
		else if (!s.empty()) //���tû�������� ����ջ��ָʾ ����
		{
			t = s.top();
			s.pop();
		}
		else //���tû�������� ����ջΪ��  
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
	//����������
	inOrder(root->lchild);
	printf("%d ", root->data);
	//����������
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

	//������ϵ
	t1.lchild = &t2;
	t1.rchild = &t3;
	t2.lchild = &t4;
	t3.lchild = &t5;

	printf("\nolder inorder\n");
	inOrder(&t1);

	printf("\n �ǵݹ����\n");
	Inorder2(&t1);
	printf("hello...\n");

	system("pause");
	return ;
}
