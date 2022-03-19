#include"SkipTable.h"
#include<stdlib.h>
#include<string.h>
/**
* ��̬��������ڵ�.
*
* @return NULL:�ڴ�����ʧ��
*        !NULL:�ڵ㴴���ɹ�
*/
static skip_list_node* skip_list_node_creat(int level, int key, int value)
{
	struct skip_list_node *node = NULL;

	/* �ڵ�ռ��С Ϊ�ڵ����ݴ�С+ level��������ռ�õĴ�С */
	node = SKIP_LIST_NODE_MALLOC(sizeof(*node) + level * sizeof(node));
	if (node == NULL)
		return NULL;

	/* �������ռ� */
	memset(node, 0, sizeof(*node) + level * sizeof(node));
	node->key = key;
	node->value = value;
	node->max_level = level;

	return node;
}

/**
* ��������ͷ�ڵ�.
*
* @param max_level:����������
* @return NULL:����ʧ��
*        !NULL:�����ɹ�
*/
struct skip_list* skip_list_creat(int max_level)
{
	struct skip_list *list = NULL;

	list = SKIP_LIST_MALLOC(sizeof(*list));
	if (list == NULL)
		return NULL;

	list->level = 1;
	list->num = 0;
	list->head = skip_list_node_creat(max_level, 0, 0);
	if (list->head == NULL)
	{
		SKIP_LIST_FREE(list);
		return NULL;
	}

	return list;
}

/**
* �����������Ԫ�ص���������,���������.
*
* @param list:����
* @return �ڵ���������
*
*/
static int skip_list_level(struct skip_list *list)
{
	int i = 0, level = 1; /*������������Ϊ1,���Դ�1��ʼ*/

	for (i = 1; i<list->head->max_level; i++)
	{
		if ((rand() % 2) == 1)
		{
			level++;
		}
	}
	return level;
}

/**
* ��������ڵ�.
*
* @param list:����
* @param key:
* @param value:
* @return -1:����Ϊ��
*         -2:�ռ����ʧ��
*         -3:key�Ѿ�����
*          0:����ɹ�
*/
int skip_list_insert(struct skip_list *list, int key, int value)
{
	struct skip_list_node **update = NULL; /*��������ÿ������ָ�룬��Ų���λ�õ�ǰ������ڵ�����*/
	struct skip_list_node *cur = NULL;
	struct skip_list_node *prev = NULL;
	struct skip_list_node *insert = NULL;
	int i = 0, level = 0;

	if (list == NULL)
		return -1;

	/*����update�ռ����ڱ���ÿ�������ָ��*/
	update = (struct skip_list_node **)SKIP_LIST_MALLOC(sizeof(list->head->max_level * sizeof(struct skip_list_node *)));
	if (update == NULL)
		return -2;

	/*����ѯ,���Ҳ���λ�õ�ǰ������ڵ�����
	*update[0] ��ŵ�һ��Ĳ���λ��ǰ���ڵ㣬update[0]->next[0]��ʾ����λ�õ�ǰ���ڵ����һ�ڵ�(update[0]->next[0])�ĵ�һ������ֵ
	*update[1] ��ŵڶ���Ĳ���λ��ǰ���ڵ㣬update[1]->next[1]��ʾ����λ�õ�ǰ���ڵ����һ�ڵ�(update[1]->next[0])�ĵڶ�������ֵ
	*update[n] ��ŵ�һ��Ĳ���λ��ǰ���ڵ㣬update[n]->next[n]��ʾ����λ�õ�ǰ���ڵ����һ�ڵ�(update[n]->next[0])�ĵ�n������ֵ
	*/
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL) && (cur->key < key))
		{
			prev = cur; /* ����ƶ� */
		}
		update[i] = prev; /* ����Ҫ����ڵ��ǰ���ڵ� */
	}

	/* ��ǰkey�Ѿ�����,���ش��� */
	if ((cur != NULL) && (cur->key == key))
	{
		return -3;
	}

	/*��ȡ����Ԫ�ص��������,�����������������*/
	level = skip_list_level(list);
	/*������ǰ�ڵ�*/
	insert = skip_list_node_creat(level, key, value);
	/*���������������,���²���ڵ��ǰ���ڵ�,ǰ���Ѿ����µ���[0] - [(list->level-1)]*/
	if (level > list->level)
	{
		for (i = list->level; i<level; i++)
		{
			update[i] = list->head;/*�ⲿ��Ϊ��������������,����ǰ���ڵ�Ĭ��Ϊͷ���*/
		}
		list->level = level;/*��������������������*/
	}

	/*�����½ڵ��ָ��*/
	for (i = 0; i<level; i++)
	{
		insert->next[i] = update[i]->next[i];
		update[i]->next[i] = insert;
	}

	/*�ڵ���Ŀ��1*/
	list->num++;

	return 0;
}

/**
* ɾ������ڵ�.
*
* @param list:����
* @param key:
* @param value:
* @return -1:����Ϊ�� �� ����ڵ�����Ϊ0
*         -2:�ռ����ʧ��
*         -3:key������
*          0:ɾ���ɹ�
*/
int skip_list_delete(struct skip_list *list, int key)
{
	struct skip_list_node **update = NULL; /*��������ÿ������ָ�룬��Ų���λ�õ�ǰ������ڵ�����*/
	struct skip_list_node *cur = NULL;
	struct skip_list_node *prev = NULL;
	int i = 0;

	if (list == NULL && list->num == 0)
		return -1;

	/*����update�ռ����ڱ���ÿ��Ľڵ�����ָ��*/
	update = (struct skip_list_node **)SKIP_LIST_MALLOC(sizeof(list->level * sizeof(struct skip_list_node *)));
	if (update == NULL)
		return -2;

	/*����ѯ,����ɾ��λ�õ�ǰ������ڵ�����
	*update[0] ��ŵ�һ���ɾ��λ��ǰ���ڵ㣬update[0]->next[0]��ʾɾ��λ�õ�ǰ���ڵ����һ�ڵ�(update[0]->next[0])�ĵ�һ������ֵ
	*update[1] ��ŵڶ����ɾ��λ��ǰ���ڵ㣬update[1]->next[1]��ʾɾ��λ�õ�ǰ���ڵ����һ�ڵ�(update[1]->next[0])�ĵڶ�������ֵ
	*update[n] ��ŵ�һ���ɾ��λ��ǰ���ڵ㣬update[n]->next[n]��ʾɾ��λ�õ�ǰ���ڵ����һ�ڵ�(update[n]->next[0])�ĵ�n������ֵ
	*/
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL) && (cur->key < key))
		{
			prev = cur; /* ����ƶ� */
		}
		update[i] = prev; /* ����Ҫɾ���ڵ��ǰ���ڵ� */
	}

	/* ��ǰkey���� */
	if ((cur != NULL) && (cur->key == key))
	{
		/*���ɾ��*/
		for (i = 0; i<list->level; i++)
		{
			if (update[i]->next[i] == cur)
			{
				update[i]->next[i] = cur->next[i];
			}
		}

		SKIP_LIST_FREE(cur);
		cur = NULL;

		/*���������Ĳ���*/
		for (i = list->level - 1; i >= 0; i--)
		{
			/*���ɾ���ڵ��,ĳ���ͷ�������ڵ�Ϊ��,��˵���ò�������ָ��,����������Ҫ��1*/
			if (list->head->next[i] == NULL)
			{
				list->level--;
			}
			else
			{
				break;
			}
		}

		list->num--; /*�ڵ�����1*/
	}
	else
	{
		return -3;
	}

	return 0;
}

/**
* ��ѯ��ǰkey�Ƿ���������,�����޸�key���ڵ�value��ֵ.
*
* @param list:����
* @param key:�޸�key
* @param value:�޸ĵ�����
* @return -1:����Ϊ�� �� ����ڵ�����Ϊ0
*         -3:key������
*          0:�޸ĳɹ�
*/
int skip_list_modify(struct skip_list *list, int key, int value)
{
	struct skip_list_node *cur = NULL;
	struct skip_list_node *prev = NULL;
	int i = 0;

	if (list == NULL && list->num == 0)
		return -1;

	/*������,���Ҳ�ѯλ��ԭʼ����Ľڵ�*/
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL) && (cur->key < key))
		{
			prev = cur; /* ����ƶ� */
		}
	}

	/* ��ǰkey���� */
	if ((cur != NULL) && (cur->key == key))
	{
		cur->value = value;
	}
	else
	{
		return -3;
	}
	return 0;
}

/**
* ��ѯ��ǰkey�Ƿ���������,���ڷ��ز�ѯ��value��ֵ.
*
* @param list:����
* @param key:
* @param value:��ѯ������
* @return -1:����Ϊ�� �� ����ڵ�����Ϊ0
*         -3:key������
*          0:���ҳɹ�
*/
int skip_list_search(struct skip_list *list, int key, int *value)
{
	struct skip_list_node *cur = NULL;
	struct skip_list_node *prev = NULL;
	int i = 0;

	if (list == NULL && value == NULL && list->num == 0)
		return -1;

	/*������,���Ҳ�ѯλ��ԭʼ����Ľڵ�*/
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL) && (cur->key < key))
		{
			prev = cur; /* ����ƶ� */
		}
	}

	/* ��ǰkey���� */
	if ((cur != NULL) && (cur->key == key))
	{
		*value = cur->value;
	}
	else
	{
		return -3;
	}
	return 0;
}

/**
* ��������.
*
* @param list:����
* @return -1:����Ϊ��
*          0:�ɹ�
*/
int skip_list_destroy(struct skip_list *list)
{
	struct skip_list_node *cur = NULL;

	if (list == NULL && list->head == NULL)
		return -1;

	while ((cur = list->head->next[0]) != NULL)
	{
		list->head->next[0] = cur->next[0];
		SKIP_LIST_FREE(cur);
		cur = NULL;
	}

	SKIP_LIST_FREE(list->head);
	SKIP_LIST_FREE(list);

	return 0;
}
typedef void(*callSkipListNode)(skip_list_node* node);
int skip_list_loop(struct skip_list *list, callSkipListNode call)
{
	struct skip_list_node *cur = NULL;
	struct skip_list_node *prev = NULL;
	int i = 0;

	if (list == NULL  && list->num == 0)
		return -1;

	/*������,���Ҳ�ѯλ��ԭʼ����Ľڵ�*/
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		call(prev);
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL))
		{
			prev = cur; /* ����ƶ� */
		}
	}
	return 0;
}