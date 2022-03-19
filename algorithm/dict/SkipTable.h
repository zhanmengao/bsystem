#pragma once
#define SKIP_LIST_MALLOC(size)   ((skip_list*)(malloc(size)));
#define SKIP_LIST_NODE_MALLOC(size)   ((skip_list_node*)(malloc(size)));
#define SKIP_LIST_CALLOC(n,size) ((skip_list_node*)(calloc(n,size)));
#define SKIP_LIST_FREE(p)        free(p);

struct skip_list_node
{
	/*key��Ψһ��*/
	int key;
	/*�洢������*/
	int value;
	/*��ǰ�ڵ�������*/
	int max_level;
	/*��������,���ݸýڵ�����Ĳ�ָͬ���С��ͬ������
	*next[0]��ʾ�ýڵ�ĵ�һ����һ�ڵ��������ַ
	*next[1]��ʾ�ýڵ�ĵڶ�����һ�ڵ��������ַ
	*next[n]��ʾ�ýڵ�ĵ�n����һ�ڵ��������ַ
	*/
	struct skip_list_node *next[];
};

struct skip_list
{
	int level; /*�������������*/
	int num;   /*�ڵ���Ŀ*/
	struct skip_list_node *head;
};

extern struct skip_list* skip_list_creat(int max_level);
extern int skip_list_insert(struct skip_list *list, int key, int value);
extern int skip_list_delete(struct skip_list *list, int key);
extern int skip_list_modify(struct skip_list *list, int key, int value);
extern int skip_list_search(struct skip_list *list, int key, int *value);
extern int skip_list_destroy(struct skip_list *list);
typedef void(*callSkipListNode)(skip_list_node* node);
extern int skip_list_loop(struct skip_list *list, callSkipListNode call);