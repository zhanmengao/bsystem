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

//�ڶ��ֱ�ʾ���� ��������
//��������
typedef struct TriTNode 
{
	int data;
	//���Һ���ָ��
	struct TriTNode *lchild, *rchild;
	struct TriTNode *parent;
}TriTNode, *TriTree;



//

//˫������
#define MAX_TREE_SIZE 100
typedef struct BPTNode
{
	int data;
	int parentPosition; //ָ��˫�׵�ָ�� //�����±�
	char LRTag; //���Һ��ӱ�־��
}BPTNode;

typedef struct BPTree
{
	BPTNode nodes[100]; //��Ϊ�ڵ�֮���Ƿ�ɢ�ģ���Ҫ�ѽڵ�洢��������
	int num_node;  //�ڵ���Ŀ
	int root; //������λ�� //ע�����洢���Ǹ��׽ڵ���������±�
}BPTree;

//��������ݽṹ�ܱ���һ�����������ܣ���ô������why

void main22()
{

	BPTree tree;

	//�����
	tree.nodes[0].parentPosition = 1000;

	//B
	tree.nodes[1].parentPosition = 0;
	tree.nodes[1].data = 'B';
	tree.nodes[1].LRTag = 1;

	//c
	tree.nodes[2].parentPosition = 0;
	tree.nodes[2].data = 'C';
	tree.nodes[2].LRTag = 2;
	system("pause");

}



void main02()
{
	BiTNode *p1, *p2, *p3, *p4, *p5;
	p1 = (BiTNode *)malloc(sizeof(BiTNode));
	p2 = (BiTNode *)malloc(sizeof(BiTNode));
	p3 = (BiTNode *)malloc(sizeof(BiTNode));
	p4 = (BiTNode *)malloc(sizeof(BiTNode));
	p5 = (BiTNode *)malloc(sizeof(BiTNode));

	p1->data = 1;
	p2->data = 2;
	p3->data = 3;
	p4->data = 4;
	p5->data = 5;

	//������ϵ
	p1->lchild = p2;
	p1->rchild = p3;
	p2->lchild = p4;
	p3->lchild = p5;

	//���ı���

	printf("hello...\n");
	system("pause");
	return ;
}

void main111()
{
	BiTNode t1, t2, t3, t4, t5;
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

	printf("hello...\n");
	system("pause");
	return ;
}
