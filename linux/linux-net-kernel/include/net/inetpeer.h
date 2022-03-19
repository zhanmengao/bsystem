/*
 *		INETPEER - A storage for permanent information about peers
 *
 *  Authors:	Andrey V. Savochkin <saw@msu.ru>
 */

#ifndef _NET_INETPEER_H
#define _NET_INETPEER_H

#include <linux/types.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

//�ýṹΪ�Զ���Ϣ��ṹ����v4addrΪ�ؼ�֮��peer_rootΪ������֯��AVL������������P238
//�Զ���Ϣ����Ҫ������װip���ݰ�ʱ��ֹ��Ƭ�������ڽ���tcp����ʱ�������������Ƿ���Ч�Լ������к��Ƿ����
struct inet_peer {
	/* group together avl_left,avl_right,v4daddr to speedup lookups */
	struct inet_peer	*avl_left, *avl_right;  //�������Զ���Ϣ�����AVL����AVL���ĸ�Ϊpeer_root��
	__be32			v4daddr;	/* peer's address */
	__u32			avl_height;
	struct list_head	unused; //�������ӵ�inet_peer_unused_head�����ϡ��� �����ϵĶԶ���Ϣ�鶼�ǵ�ǰ���õģ� �ɻ��յġ�  unused_peers
	__u32			dtime;		/* the time of last use of not
						 * referenced entries */  //��¼�öԶ���Ϣ�����ü���Ϊ0��ʱ�䡣 �����õ�ʱ�䳬��ָ����ʱ��ʱ�� �ͻᱻ���ա�
	atomic_t		refcnt; //���ü���������ʶ��ǰ��ʹ�õĴ����� �����ü���Ϊ0����ʾ�öԶ���Ϣ�� û�б�ʹ�á�
	atomic_t		rid;		/* Frag reception counter */ //����ID���Զ˷��ͷ�Ƭ�ļ������� �μ�ipq�ṹ�е�rid��Ա��
	atomic_t		ip_id_count;	/* IP ID for the next packet */// һ����������ֵ����������IP��Ƭ�ײ��е�id�򡣸��ݶԶ˵�ַ��ʼ��Ϊ���ֵ��
	__u32			tcp_ts;//TCP�У���¼���һ��ACK�ε���� ʱ�䡣�μ�tcp_options_received�ṹ�� ��ts_recent��Ա��
	__u32			tcp_ts_stamp; //TCP�У���¼���յ��Ķ��е�ʱ����� ����ts_recent��ʱ�䡣�μ�tcp_options_received �ṹ�е�ts_recent_stamp��Ա��
};

void			inet_initpeers(void) __init;

/* can be called with or without local BH being disabled */
struct inet_peer	*inet_getpeer(__be32 daddr, int create);

/* can be called from BH context or outside */
extern void inet_putpeer(struct inet_peer *p);

/* can be called with or without local BH being disabled */
static inline __u16	inet_getid(struct inet_peer *p, int more)
{
	more++;
	return atomic_add_return(more, &p->ip_id_count) - more;
}

#endif /* _NET_INETPEER_H */
