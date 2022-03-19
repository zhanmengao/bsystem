#pragma once
#define SKIP_LIST_MALLOC(size)   ((skip_list*)(malloc(size)));
#define SKIP_LIST_NODE_MALLOC(size)   ((skip_list_node*)(malloc(size)));
#define SKIP_LIST_CALLOC(n,size) ((skip_list_node*)(calloc(n,size)));
#define SKIP_LIST_FREE(p)        free(p);

struct skip_list_node
{
	/*key是唯一的*/
	int key;
	/*存储的内容*/
	int value;
	/*当前节点最大层数*/
	int max_level;
	/*柔性数组,根据该节点层数的不同指向大小不同的数组
	*next[0]表示该节点的第一层下一节点的索引地址
	*next[1]表示该节点的第二层下一节点的索引地址
	*next[n]表示该节点的第n层下一节点的索引地址
	*/
	struct skip_list_node *next[];
};

struct skip_list
{
	int level; /*跳表的索引层数*/
	int num;   /*节点数目*/
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