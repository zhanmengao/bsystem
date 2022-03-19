/*
 * net/sched/sch_htb.c	Hierarchical token bucket, feed tree version
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Martin Devera, <devik@cdi.cz>
 *
 * Credits (in time order) for older HTB versions:
 *              Stef Coene <stef.coene@docum.org>
 *			HTB support at LARTC mailing list
 *		Ondrej Kraus, <krauso@barr.cz>
 *			found missing INIT_QDISC(htb)
 *		Vladimir Smelhaus, Aamer Akhter, Bert Hubert
 *			helped a lot to locate nasty class stall bug
 *		Andi Kleen, Jamal Hadi, Bert Hubert
 *			code review and helpful comments on shaping
 *		Tomasz Wrona, <tw@eter.tym.pl>
 *			created test case so that I was able to fix nasty bug
 *		Wilfried Weissmann
 *			spotted bug in dequeue code and helped with fix
 *		Jiri Fojtasek
 *			fixed requeue routine
 *		and many others. thanks.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/list.h>
#include <linux/compiler.h>
#include <linux/rbtree.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>

/* HTB algorithm.
    Author: devik@cdi.cz
    ========================================================================
    HTB is like TBF with multiple classes. It is also similar to CBQ because
    it allows to assign priority to each class in hierarchy.
    In fact it is another implementation of Floyd's formal sharing.

    Levels:
    Each class is assigned level. Leaf has ALWAYS level 0 and root
    classes have level TC_HTB_MAXDEPTH-1. Interior nodes has level
    one less than their parent.
*/

static int htb_hysteresis = 0;// __read_mostly = 0; /* whether to use mode hysteresis for speedup */
#define HTB_VER 0x30011		/* major must be matched with number suplied by TC as version */

#if HTB_VER >> 16 != TC_HTB_PROTOVER
#error "Mismatched sch_htb.c and pkt_sch.h"
#endif

/* Module parameter and sysfs export */
module_param    (htb_hysteresis, int, 0640);
MODULE_PARM_DESC(htb_hysteresis, "Hysteresis mode, less CPU load, less accurate");

/* used internaly to keep status of single class */

/*
��HTBģʽ��HTB_CAN_SENDʱ, ��ʾ�ǿ��Է���, û������; ΪHTB_CANT_SENDʱ��ʾ����, ��������
�������ݰ���; ΪHTB_MAY_BORROWʱҲ��������״̬, �������������������������.
*/
enum htb_cmode {
    // ���ܷ���
	HTB_CANT_SEND,		/* class can't send and can't borrow */

	// �����
	HTB_MAY_BORROW,		/* class can't send but may borrow */

	// �ɷ���
	HTB_CAN_SEND		/* class can send */
};

/*
��������α�֤һ��n�����ĺ�����ĸ߶�ʼ�ձ�����logn���أ���������˺������5�����ʣ�

1.ÿ�����Ҫô�Ǻ��Ҫô�Ǻڵġ�  
2.������Ǻڵġ�  
3.ÿ��Ҷ��㣨Ҷ��㼴ָ��β��NILָ���NULL��㣩���Ǻڵġ�  
4.���һ������Ǻ�ģ���ô�����������Ӷ��Ǻڵġ�  
5.������������ԣ��䵽Ҷ�����β��NILָ���ÿ��·����������ͬ��Ŀ�ĺڽ�㡣 
*/

/*
Usage: ... qdisc add ... htb [default N] [r2q N]
 default  minor id of class to which unclassified packets are sent {0}
 r2q      DRR quantums are computed as rate in Bps/r2q {10}
 debug    string of 16 numbers each 0-3 {0}

... class add ... htb rate R1 [burst B1] [mpu B] [overhead O]
                      [prio P] [slot S] [pslot PS]
                      [ceil R2] [cburst B2] [mtu MTU] [quantum Q]
 rate     rate allocated to this class (class can still borrow)
 burst    max bytes burst which can be accumulated during idle period {computed}
 mpu      minimum packet size used in rate computations
 overhead per-packet size overhead used in rate computations
 linklay  adapting to a linklayer e.g. atm
 ceil     definite upper class rate (no borrows) {rate}
 cburst   burst but for ceil {computed}
 mtu      max packet size we create rate map for {1600}
 prio     priority of leaf; lower are served first {0}
 quantum  how much bytes to serve from leaf at once {use r2q}
*/
/* interior & leaf nodes; props specific to leaves are marked L: */
//tc class add ���κ󣬻ᴴ������htb_class�ṹ���������ṹͨ��Qdisc_class_common -> hnode���հ�htb_class���뵽htb_sched(htb qdisc��˽������)->clhash�У���htb_change_class -> qdisc_class_hash_insert
//htb_class������cl��ʾ������tc class�ṹ�ĵ�ַ����htb_get
//�ο�<HTB�����Լ�ʹ��.doc>
struct htb_class {//htb_change_class�д��������޸��࣬�ڴ���htb�����ʱ�򣬻�Ĭ�ϼ�һ��pfifo_qdisc_opsҶ�ӽڵ㣬����SKB������絽��Ҷ�ӽڵ��SKB��������
	struct Qdisc_class_common common; //ͨ�������htb˽�����ݹ�����������htb_change_class -> qdisc_class_hash_insert,���ӵ�Qdisc_class_hash�е�hash��
	/* general class parameters */
	struct gnet_stats_basic_packed bstats; // �ֽ���, ����ͳ��
	struct gnet_stats_queue qstats;// ������Ϣͳ��
	struct gnet_stats_rate_est rate_est;// ����ͳ��, �ֽ���, ����
	struct tc_htb_xstats xstats;	/* our special stats */// HTBͳ����Ϣ, ���, ����, ���ƵȲ���
	int refcnt;		/* usage count of this class */// HTB������ü���  �ڴ���class��ʱ���ʼ��Ϊ1����htb_change_class

	/* topology */
	// �����еĲ��, 0��ʾҶ�ӽڵ�, ���ڵ�����TC_HTB_MAXDEPTH-1(7)
	int level;		/* our level (see above) */
	unsigned int children;

	//�ýṹ�������common���ܱ�֤���е�htb_class(tc class add��ʱ�򴴽���class��Ϣ�ڵ�)������һ��
	struct htb_class *parent;	/* parent class */ //tc class add parent 5:2 classid 6: xxxx�� parentΪ��class

    /*
    Usage: ... qdisc add ... htb [default N] [r2q N]
     default  minor id of class to which unclassified packets are sent {0}
     r2q      DRR quantums are computed as rate in Bps/r2q {10}
     debug    string of 16 numbers each 0-3 {0}

    ... class add ... htb rate R1 [burst B1] [mpu B] [overhead O]
                          [prio P] [slot S] [pslot PS]
                          [ceil R2] [cburst B2] [mtu MTU] [quantum Q]
     rate     rate allocated to this class (class can still borrow)
     burst    max bytes burst which can be accumulated during idle period {computed}
     mpu      minimum packet size used in rate computations
     overhead per-packet size overhead used in rate computations
     linklay  adapting to a linklayer e.g. atm
     ceil     definite upper class rate (no borrows) {rate}
     cburst   burst but for ceil {computed}
     mtu      max packet size we create rate map for {1600}
     prio     priority of leaf; lower are served first {0}
����HTB�Ĺٷ��ĵ���ʾ��quantum���ڿ��ԡ��衱������£�һ�ο��ԡ��衱���٣�����˵���ֵ��þ�����С����Ҫ����MTU���������
ֵ�ǲ����ֶ����ã��������r2q��ֵ���������
     quantum  how much bytes to serve from leaf at once {use r2q}
     */ //���Ӧ�ò�����Ϊ����8����Ĭ���޸�Ϊ7����htb_change_class
	int prio;		/* these two are used only by leaves... */ //ȡֵ��ΧС��TC_HTB_NUMPRIO      ��htb_activate
	//quantum������htb_dequeue_tree�л�ʹ�õ�
	int quantum;		/* but stored for parent-to-leaf return */ // �������, ȱʡ��ȡ���������Ķ��г���ֵ  ��С����Ϊ1000 ���200000 ����ͨ��Ӧ�ò����quantum����

	union {
		struct htb_class_leaf { // ����ýڵ���Ҷ�ӽڵ㣬 ��ʹ��leaf�ṹ, ʵ�־�������ش���
			struct Qdisc *q; //�½���htb class��������Ĭ��Ҷ��qdiscΪpfifo_qdisc_ops
			int deficit[TC_HTB_MAXDEPTH];// ��ͬ�����ȵĳ���        ���ӵ�ʱ���õ�����htb_dequeue_tree�� û����SKB->LEN�����ݰ�����ֵ����len
			struct list_head drop_list;// �ҽӵ���������, ��ӵ�htb_sched->drops[]�����У���htb_activate
		} leaf;
		// �����Ҷ�ӽڵ�, ʹ��HTB�ڲ����ṹinner, �����γɷ�����
		struct htb_class_inner {
            // �ṩ���ݰ��ĺ�����ṹ, ��һ�������ID��������������, �Զ�����ʵ��, 
            // ��ͬ����Ȩ��Ӧ��ͬ�Ķ�����
            // feed�����������yellow�Ľڵ㣬������Ҫ������ȣ��ſ��Խ��е���; ��htb_activate_prios
			struct rb_root feed[TC_HTB_NUMPRIO];	/* feed trees */  //class����ͨ��prio��ӵ����ڵ��feed[i]�ϣ���htb_add_to_id_tree
			// ��ǰ����Ȩ�������ڴ�����Ǹ��ڵ��ָ��
			struct rb_node *ptr[TC_HTB_NUMPRIO];	/* current class ptr */
			/* When class changes from state 1->2 and disconnects from
			   parent's feed then we lost ptr value and start from the
			   first child again. Here we store classid of the
			   last valid ptr (used when ptr is NULL). */
			   // ��һ����Ч�����ڵ�����ID����htb_deactivate_prios
			u32 last_ptr_id[TC_HTB_NUMPRIO];
		} inner;
	} un;
    // ���ṹ�Լ������ݰ���Ӧ��
	struct rb_node node[TC_HTB_NUMPRIO];	/* node for self or feed tree */
	
    // �¼���, ʵ���ǵȴ���, ������������ʱ�Ὣ�����ڵ�ҽӵ�HTB���ؽڵ��
    // �ȴ�����wait_pq
	struct rb_node pq_node;	/* node for event queue */
	psched_time_t pq_key;

    /*��������Կ���������Ƿ�Ҷ�ӽڵ㣬����������¼�class����Ҷ�ӽڵ�Ļ������
	//��������Ƿ�Ҷ�ӽڵ㣬��һ��level����������ȼ����ڶ���level����������ȼ������߲�level���8�����ȼ�����htb_activate_prios*/
    // ���������Ȩ����, ��0��ʾ��Ӧλ�������ݶ��������ݰ�����  ĳλΪ1��ʾ��λ��Ӧ��un->inner->feed[i]����Ȩ�����ݿ���
	int prio_activity;	/* for which prios are we active */ //��ֵ��htb_activate  �������prioһ�� cl->prio_activity = 1 << cl->prio;
	enum htb_cmode cmode;	/* current mode of the class */ // ��ǰģʽ, ��ʾ�Ƿ�ɷ������ݰ�  Ĭ��HTB_CAN_SEND

	/* class attached filters *///tc filter add dev eth0 parent 1:3 protocol ip prio 100  xxxx
	//��class�����Ӧ�Ĺ�����  //��htb_find_tcf��������Ҳ���class��ֱ�Ӱѹ�������ӵ�����������
	struct tcf_proto *filter_list; //ÿ��������Ϣ�ж���һ�������Ĺ���������������Ϊ�����ĳ����ڵ���ӵ�,���������1:3�������һ��������
	int filter_cnt; //��htb_bind_filter // ������ʹ�ü���  

	/* token bucket parameters */
	struct qdisc_rate_table *rate;	/* rate table of the class itself */ // ������  ͨ��������뵽qdisc_rtab_list����qdisc_put_rtab
	struct qdisc_rate_table *ceil;	/* ceiling rate (limits borrows too) */ // ��ֵ�� ͨ��������뵽qdisc_rtab_list����qdisc_put_rtab
    //��Ӧtc_htb_opt�е�buffer��cbuffer
	long buffer, cbuffer;	/* token bucket depth/rate */ // ������/��ֵ������ ��������htb����Ͱ�㷨�е��ô���:�������һ��skb����skb�������������skb���ݻ��浽buffer�У������ƹ���ʱ���skb���ͳ�ȥ
	psched_tdiff_t mbuffer;	/* max wait time */ // ���ȴ�ʱ��  Ĭ��60 * PSCHED_TICKS_PER_SEC;
	long tokens, ctokens;	/* current number of tokens */// ��ǰ������/��ֵ����  ��Ӧtc_htb_opt�е�buffer��cbuffer
	psched_time_t t_c;	/* checkpoint time */// ����ʱ��
};

struct htb_sched { 
    //tc qdisc add class xxxx htb��ʱ�򴴽���struct htb_class����ӵ���hash����(ͨ��htb_class ->Qdisc_class_common ->hnode�ӵ������棬
    //��htb_change_class -> qdisc_class_hash_insert) ��htb_class��htb_sched����������
    //clhash��ʼ���ط���qdisc_class_hash_init
    ////����htb_class��ʱ�򴴽���struct htb_class�Ǽ��뵽htb_sched(htb qdisc��˽������)->clhash�У�����û���γ�һ�ź�������γɺ��������htb_enqueue->htb_activateʵ��
    struct Qdisc_class_hash clhash; //tc class add parent 2 classid 2:3 xx ,����classid 2:3���뵽�������У�������洢�ľ��Ǽ򵥵�classid���Լ���������tc add class�ж��ٸ�class

    //cl->un.leaf.drop_list�Ǽӵ��ñ��еģ���htb_activate , htb_class->un.leaf.drop_list��ӵ���Ӧ��drops[]��
	struct list_head drops[TC_HTB_NUMPRIO];/* active leaves (for drops) */ //��class->prio�й�ϵ����htb_activate

	/* self list - roots of self generating tree */
	// RB�����ڵ�, ��Ӧÿһ���ÿһ������Ȩֵ����һ��RB���� ��htb_add_class_to_row
	struct rb_root row[TC_HTB_MAXDEPTH][TC_HTB_NUMPRIO]; //row���green�ڵ㣬����token��Ȼ���ʣ�ࡢ���Խ��е��ȵ�class�ڵ㣻 
	int row_mask[TC_HTB_MAXDEPTH];// ����, ��ʾ�ò����Щ����Ȩֵ������Ч����htb_add_class_to_row

    /* ptr��DRR�㷨��һ����ǣ�ָ��ǰ���Խ��е��ȵĽڵ㣨�ࣩ�� �����ǰ�ڵ��deficit�����ˣ�htb_next_rb_node()�Ὣptrָ��ָ��ǰ�ڵ�
    ����һ���ڵ㣬Ȼ���ٴ�ptrָ��Ľڵ���е��ȡ�*/
	struct rb_node *ptr[TC_HTB_MAXDEPTH][TC_HTB_NUMPRIO];// ���ڵ�ָ��
	u32 last_ptr_id[TC_HTB_MAXDEPTH][TC_HTB_NUMPRIO];// �ϴ�ʹ�õķǿո��ڵ�����ID

	/* self wait list - roots of wait PQs per row */
	//wait_pq���yellow��red�ڵ㣬���ض�ʱ����м���Ƿ���԰ѽڵ�ָ���green��   
	struct rb_root wait_pq[TC_HTB_MAXDEPTH];// �ȴ�����, �����ҽ���Щ���������ƵĽڵ�

	/* time of nearest event per level (row) */
	psched_time_t near_ev_cache[TC_HTB_MAXDEPTH];

    //tc qdisc del add eth0 root handle 22 htb default 3333333  // ȱʡ��� minor id of class to which unclassified packets are sent {0}
	int defcls;		/* class where unclassified flows go to */ //�޷�ͨ��������ѡ�������SKBĬ���߸÷��࣬

	/* filters for qdisc itself */
	struct tcf_proto *filter_list;//��htb_find_tcf��������Ҳ���class��ֱ�Ӱѹ�������ӵ�����������

    //DRR quantums are computed as rate in Bps/r2q {10}   Ӧ�ò����õ�Ĭ��ֵΪ10����htb_parse_opt
	int rate2quantum;	/* quant = rate / rate2quantum */  // ���ʵ�����ת������  ��ʼֵΪ1����htb_init
	psched_time_t now;	/* cached dequeue time */ // ��ǰʱ��
	struct qdisc_watchdog watchdog;

	/* non shaped skbs; let them go directly thru */
	//���SKB������ʱ��û��ƥ��Ĺ�����������Ĭ��default���й��Ҳûƥ��ɹ�����ֱ��ʹ��˽����Ϣ�е�direct_queue������ӣ���htb_enqueue
	struct sk_buff_head direct_queue; //�ܷ���Ӿ����øö����ϵ�SKB�����������direct_qlen���Ƚ�,  ���skb��������Ķ����ǲ���Ҫ���ٵģ���htb_dequeue
	int direct_qlen;	/* max qlen of above */ //q->direct_qlen = qdisc_dev(sch)->tx_queue_len;

	long direct_pkts;// ֱ�Ӵ�������ݰ�����

#define HTB_WARN_TOOMANYEVENTS	0x1
	unsigned int warned;	/* only one warning */
	struct work_struct work;
};

/* find class in global hash table using given handle */// �������ID�������ṹ // ���ݾ��handle����HTB�ڵ�
static inline struct htb_class *htb_find(u32 handle, struct Qdisc *sch)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct Qdisc_class_common *clc;

	clc = qdisc_class_find(&q->clhash, handle);
	if (clc == NULL)
		return NULL;
	return container_of(clc, struct htb_class, common);
}

/**
 * htb_classify - classify a packet into class
 *
 * It returns NULL if the packet should be dropped or -1 if the packet
 * should be passed directly thru. In all other cases leaf class is returned.
 * We allow direct class selection by classid in priority. The we examine
 * filters in qdisc and in inner nodes (if higher filter points to the inner
 * node). If we end up with classid MAJOR:0 we enqueue the skb into special
 * internal fifo (direct). These packets then go directly thru. If we still
 * have no valid leaf we try to use MAJOR:default leaf. It still unsuccessfull
 * then finish and return direct queue.
 */ 
    // HTB�������, �����ݰ����з���, Ȼ�������������ز���
    // ����NULL��ʾû�ҵ�, ����-1��ʾ��ֱ��ͨ��(������)�����ݰ�
//��SKBû�ж�Ӧƥ��Ĺ���������ʹ��Ĭ�ϵĹ�������Ҳ����tc qdisc add xxxxx htb default a�е�a����htb_classify
#define HTB_DIRECT (struct htb_class*)-1 
static struct htb_class *htb_classify(struct sk_buff *skb, struct Qdisc *sch,
				      int *qerr)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct htb_class *cl;
	struct tcf_result res;
	struct tcf_proto *tcf;
	int result;

	/* allow to select class by setting skb->priority to valid classid;
	   note that nfmark can be used too by attaching filter fw with no
	   rules in it */
	if (skb->priority == sch->handle)// ������ݰ�����Ȩֵ�͵������ؽڵ�;��handle, ���ڸ��ڵ����, ֱ�Ӵ���
		return HTB_DIRECT;	/* X:0 (direct flow) selected */
	if ((cl = htb_find(skb->priority, sch)) != NULL && cl->level == 0)// ���Һ����ݰ�����Ȩֵ��Ӧ��HTBҶ�ӽڵ�, �ҵ��򷵻�
		return cl;

    /*
    // ���´�����û���ҵ���skb->priorityֱ�Ӷ�Ӧ��HTBҶ�ӽڵ�, Ӧ��˵ʵ��Ӧ���д󲿷ֶ���skb->priorityΪ0��, ����һ�㶼�����е�����
    */
	*qerr = NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
	tcf = q->filter_list;//����������
	//ͨ��skb������ƥ��tc filter����tp���ҵ����ض�Ӧ�ķ���ڵ㣬ƥ��ɹ�����0����ƥ��Ĺ��������ڵ�tc class����ڵ���Ϣ�浽res�У�ƥ��ɹ�����0
	while (tcf && (result = tc_classify(skb, tcf, &res)) >= 0) {
#ifdef CONFIG_NET_CLS_ACT // �����˿ɶԷ��������ж������ں�ѡ������
		switch (result) {
		case TC_ACT_QUEUED:
		case TC_ACT_STOLEN:
			*qerr = NET_XMIT_SUCCESS | __NET_XMIT_STOLEN;
		case TC_ACT_SHOT:
			return NULL;
		}
#endif
		if ((cl = (void *)res.class) == NULL) { //������صĽ���е�res.class=0
			if (res.classid == sch->handle)// �����������ID�������ؾ��, ֱ�Ӵ���
				return HTB_DIRECT;	/* X:0 (direct flow) */
			if ((cl = htb_find(res.classid, sch)) == NULL)
				break;	/* filter selected invalid classid */
		}
		if (!cl->level)
			return cl;	/* we hit leaf; return it */

		/* we have got inner class; apply inner filter chain */
		tcf = cl->filter_list;
	}
	// ѭ������û�ҵ���������
	/* classification failed; try to use default class */// ��ȱʡ���ID����, ���Ƿ�����ȱʡ���
	cl = htb_find(TC_H_MAKE(TC_H_MAJ(sch->handle), q->defcls), sch);
	if (!cl || cl->level) //��SKBû�ж�Ӧƥ��Ĺ���������ʹ��Ĭ�ϵĹ��������ң�Ҳ����tc qdisc add xxxxx htb default a�е�a�����Ĭ�ϵķ���Ҳû�ҵ����򷵻�HTB_DIRECT
		return HTB_DIRECT;	/* bad default .. this is safe bet */
	return cl;
}

/**
 * htb_add_to_id_tree - adds class to the round robin list
 *
 * Routine adds class to the list (actually tree) sorted by classid.
 * Make sure that class is not already on such list for given prio.
 */ //��class  cl��ӵ��������
static void htb_add_to_id_tree(struct rb_root *root,
			       struct htb_class *cl, int prio)
{
	struct rb_node **p = &root->rb_node, *parent = NULL;
    
    // RB���������, �������ID����, ֵ��ĵ��ҽڵ�, С�ĵ���ڵ�
    // ѭ��, �������к��ʵ�λ�ò������ڵ�cl
	while (*p) {//��Ҫ�ҵ������Ǹ�rh���ڵ����
		struct htb_class *c;
		parent = *p;
		c = rb_entry(parent, struct htb_class, node[prio]);

		if (cl->common.classid > c->common.classid)
			p = &parent->rb_right;
		else
			p = &parent->rb_left;
	}

	// ����RB���Ĳ������, RB����׼��������
	rb_link_node(&cl->node[prio], parent, p);
	rb_insert_color(&cl->node[prio], root);
}

/**
 * htb_add_to_wait_tree - adds class to the event queue with delay
 *
 * The class is added to priority event queue to indicate that class will
 * change its mode in cl->pq_key microseconds. Make sure that class is not
 * already in the queue.
 *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/
static void htb_add_to_wait_tree(struct htb_sched *q,
				 struct htb_class *cl, long delay)
{
	struct rb_node **p = &q->wait_pq[cl->level].rb_node, *parent = NULL;

	cl->pq_key = q->now + delay;
	if (cl->pq_key == q->now)
		cl->pq_key++;

	/* update the nearest event cache */
	if (q->near_ev_cache[cl->level] > cl->pq_key)
		q->near_ev_cache[cl->level] = cl->pq_key;

	while (*p) {
		struct htb_class *c;
		parent = *p;
		c = rb_entry(parent, struct htb_class, pq_node);
		if (cl->pq_key >= c->pq_key)
			p = &parent->rb_right;
		else
			p = &parent->rb_left;
	}
	rb_link_node(&cl->pq_node, parent, p);
	rb_insert_color(&cl->pq_node, &q->wait_pq[cl->level]);
}

/**
 * htb_next_rb_node - finds next node in binary tree
 *
 * When we are past last key we return NULL.
 * Average complexity is 2 steps per call.
 *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/ //�ú���ִ�к�n��ִ�иú��������һ���ڵ�
static inline void htb_next_rb_node(struct rb_node **n)
{
	*n = rb_next(*n);
}

/**
 * htb_add_class_to_row - add class to its row
 *
 * The class is added to row at priorities marked in mask.
 * It does nothing if mask == 0.
 */ 
//��cl��ӵ�q->row[cl->level] + prio(cl->level�㣬prio���ȼ�)�ĺ�������С�
// RB�����ڵ�, ��Ӧÿһ���ÿһ������Ȩֵ����һ��RB���� Ҳ���Ǹ���cl->level��mask�е�prio��ȷ����cl��ӵ�htb_sched˽�����ݲ��ֵ��Ǹ���row[][]��
static inline void htb_add_class_to_row(struct htb_sched *q,
					struct htb_class *cl, int mask)
{
    // ��cl��ζ�Ӧ��ROW��row_mask�����µ�mask, ��ʾ�ж�Ӧprio��������
	q->row_mask[cl->level] |= mask;
	while (mask) {
		int prio = ffz(~mask);
		mask &= ~(1 << prio);//��ȡmask��ÿλΪ1��λ������0X23,��whileѭ����prio������0 1 5��������
		htb_add_to_id_tree(q->row[cl->level] + prio, cl, prio);//q->row[cl->level] + prio��Ӧcl->level���prio���ȼ��������
	}
}

/* If this triggers, it is a bug in this code, but it need not be fatal */
/*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/
static void htb_safe_rb_erase(struct rb_node *rb, struct rb_root *root)
{
	if (RB_EMPTY_NODE(rb)) {
		WARN_ON(1);
	} else {
		rb_erase(rb, root);
		RB_CLEAR_NODE(rb);
	}
}


/**
 * htb_remove_class_from_row - removes class from its row
 *
 * The class is removed from row at priorities marked in mask.
 * It does nothing if mask == 0.
 */
static inline void htb_remove_class_from_row(struct htb_sched *q,
						 struct htb_class *cl, int mask)
{
	int m = 0;

	while (mask) {
		int prio = ffz(~mask);// prioΪmask��һ��1λ��λ��

		mask &= ~(1 << prio);// ȥ����λ

		// ������ؽڵ�ĸò��prio��rb���ڵ�ָ�����cl��prio��rb���ڵ�, ���µ�������һ��rb�ڵ�
		if (q->ptr[cl->level][prio] == cl->node + prio)
			htb_next_rb_node(q->ptr[cl->level] + prio);

		htb_safe_rb_erase(cl->node + prio, q->row[cl->level] + prio);// ��ROW���жϿ�cl
		if (!q->row[cl->level][prio].rb_node)// ����ò��prio��rb��λ��, ��¼��λ��
			m |= 1 << prio;
	}
	q->row_mask[cl->level] &= ~m;// ��ROW�����н���rb��Ϊ�յ���Щprioλ���
}

/**
 * htb_activate_prios - creates active classe's feed chain
 *
 * The class is connected to ancestors and/or appropriate rows
 * for priorities it is participating on. cl->cmode must be new
 * (activated) mode. It does nothing if cl->prio_activity == 0.
 */
// �������, ���������ṩ��
// cl->prio_activityΪ0ʱ����һ���պ���, ������ǰ�濴prio_activity�ƺ��ǲ���Ϊ0��
//�ú������ǰ�cl��ӵ�p->un.inner.feed[prio]�ϣ�ͬʱ���¸�p->prio_activity�е�prioλ��1��ͬʱ���ĸ��϶�Ӧ��p->prio_activity�е�prioλ��1��
//Ҳ�����������Ӹ�cl���϶����á� �ο�http://luxik.cdi.cz/~devik/qos/htb/manual/theory.htm        http://blog.chinaunix.net/uid-7220314-id-208698.html 
static void htb_activate_prios(struct htb_sched *q, struct htb_class *cl)
{
	struct htb_class *p = cl->parent;
	long m, mask = cl->prio_activity;// prio_activity����Ϊһ������, ��Ӧ��ֻ��һλΪ1

    // �ڵ�ǰģʽ��HTB_MAY_BORROW����½���ѭ��, ĳЩ�������Щ����ǿ��Լ����
    // ����������p��mask�ĳ�ʼֵӦ�ö��Ƿ�0ֵ
	while (cl->cmode == HTB_MAY_BORROW && p && mask) {  //ͼ��ο�<Hierachical token bucket theory>  http://luxik.cdi.cz/~devik/qos/htb/manual/theory.htm
		m = mask;// ����maskֵ
		while (m) {
            // ����ȡ��, �ҵ�һ��0λ��λ��, Ҳ����ԭ�����Ϊ1��λ��λ��
            // prioԽС, �ȼ�Խ��, ȡ���ݰ�Ҳ���ȴ�prioֵС�Ľڵ�ȡ
			int prio = ffz(~m); //ͨ��while(m)����֪����prio����mask��ÿλΪ1���ڵ�λ��
			m &= ~(1 << prio);// �����λ   ���õ��Ľ������m��ֵΪmask�����λ1��0���ֵ
     
            // p�Ǹ��ڵ�, ����inner�ṹ�϶���Ч, ����ʹ��leaf�ṹ��
            // ������ڵ��prio����Ȩ�����ݰ����ṩ���Ѿ�����, ��������ȥ����λ
			if (p->un.inner.feed[prio].rb_node)
				/* parent already has its feed in use so that
				   reset bit in mask as parent is already ok */
				mask &= ~(1 << prio);                           

            // �������ӵ����ڵ��prio����Ȩ�ṩ���ݰ��Ľڵ�����
			htb_add_to_id_tree(p->un.inner.feed + prio, cl, prio);
		}
		
        // ���ڵ��prio_activity����mask�е���1λ, ĳλΪ1��ʾ��λ��Ӧ������Ȩ�����ݿ���
        //��������Կ���������Ƿ�Ҷ�ӽڵ㣬����������¼�class����Ҷ�ӽڵ�Ļ������
		//��������Ƿ�Ҷ�ӽڵ㣬��һ��level����������ȼ����ڶ���level����������ȼ������߲�level���8�����ȼ�
		p->prio_activity |= mask; 
		// ѭ������һ��, ��ǰ�����¸��ڵ�, ���ڵ����Ϊ�游�ڵ�
		cl = p;
		p = cl->parent;
	}
	
    // ���cl��HTB_CAN_SENDģʽ, ���������ӵ����ʵ�ROW��
    // ��ʱ��cl�����Ѿ�����ԭ����cl��,����ԭcl�ĳ����ڵ���
	if (cl->cmode == HTB_CAN_SEND && mask)
		htb_add_class_to_row(q, cl, mask);
}

/**
 * htb_deactivate_prios - remove class from feed chain
 *
 * cl->cmode must represent old mode (before deactivation). It does
 * nothing if cl->prio_activity == 0. Class is removed from all feed
 * chains and rows.
 */
static void htb_deactivate_prios(struct htb_sched *q, struct htb_class *cl)
{
	struct htb_class *p = cl->parent;
	long m, mask = cl->prio_activity; // ���ṹ������Ȩ����ֵ��Ϊ����, �����0�Ļ��������൱�ڿպ���

    
    // �ڵ�ǰģʽ��HTB_MAY_BORROW����½���ѭ��, 
    // ����������p��mask�ĳ�ʼֵӦ�ö��Ƿ�0ֵ
	while (cl->cmode == HTB_MAY_BORROW && p && mask) {
		m = mask;// ��������
		mask = 0;// ��������
		while (m) {
			int prio = ffz(~m);// prioΪm�ĵ�һ��1ֵ��λ(ȡ�����һ��0ֵ��λ)
			m &= ~(1 << prio);// ȥ����λ

			if (p->un.inner.ptr[prio] == cl->node + prio) {// ��������prio��Ӧ��rb���Ǹ��ڵ������ڴ����
				/* we are removing child which is pointed to from
				   parent feed - forget the pointer but remember
				   classid */
				p->un.inner.last_ptr_id[prio] = cl->common.classid;// ��cl�����ID���浽last_ptr_id��prio��Ӧλ��
				p->un.inner.ptr[prio] = NULL;
			}

			htb_safe_rb_erase(cl->node + prio, p->un.inner.feed + prio);// ���ڵ����prio��Ӧrb���жϿ�

			if(!p->un.inner.feed[prio].rb_node)//���Ѿ����˵�rb��������λ��
				mask |= 1 << prio;
		}

		p->prio_activity &= ~mask;// ���Ѿ����˵�rb������Ӹ��ڵ�Ļ���ֵ������ȥ��
		cl = p;// ת����һ�㴦��
		p = cl->parent;

	}
	if (cl->cmode == HTB_CAN_SEND && mask)// �����ǰ���cl��ģʽ�ǿ��Է���(������, �޽����), ��cl��ROW��������жϿ�
		htb_remove_class_from_row(q, cl, mask);
}

static inline long htb_lowater(const struct htb_class *cl)
{
	if (htb_hysteresis)
		return cl->cmode != HTB_CANT_SEND ? -cl->cbuffer : 0;
	else
		return 0;
}
static inline long htb_hiwater(const struct htb_class *cl)
{
	if (htb_hysteresis)
		return cl->cmode == HTB_CAN_SEND ? -cl->buffer : 0;
	else
		return 0;
}


/**
 * htb_class_mode - computes and returns current class mode
 *
 * It computes cl's mode at time cl->t_c+diff and returns it. If mode
 * is not HTB_CAN_SEND then cl->pq_key is updated to time difference
 * from now to time when cl will change its state.
 * Also it is worth to note that class mode doesn't change simply
 * at cl->{c,}tokens == 0 but there can rather be hysteresis of
 * 0 .. -cl->{c,}buffer range. It is meant to limit number of
 * mode transitions per time unit. The speed gain is about 1/6.
 */
static inline enum htb_cmode
htb_class_mode(struct htb_class *cl, long *diff)
{
	long toks;

	if ((toks = (cl->ctokens + *diff)) < htb_lowater(cl)) {// ��������Ceil����
		*diff = -toks;// �������С�ڵ��� 
		return HTB_CANT_SEND;
	}

    
    // ����������ͨ����
    // ������ƴ��ڸ���, ģʽΪ�ɷ���
	if ((toks = (cl->tokens + *diff)) >= htb_hiwater(cl))
		return HTB_CAN_SEND;

	*diff = -toks;
	return HTB_MAY_BORROW;// ����ģʽΪ�ɽ�
}

/**
 * htb_change_class_mode - changes classe's mode
 *
 * This should be the only way how to change classe's mode under normal
 * cirsumstances. Routine will update feed lists linkage, change mode
 * and add class to the wait event queue if appropriate. New mode should
 * be different from old one and cl->pq_key has to be valid if changing
 * to mode other than HTB_CAN_SEND (see htb_add_to_wait_tree).
 *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/ // �������ڵ�ķ���ģʽ
static void
htb_change_class_mode(struct htb_sched *q, struct htb_class *cl, long *diff) //����classģʽ��ͬʱͨ��htb_activate_prios�����γ�row leaf����
{
	enum htb_cmode new_mode = htb_class_mode(cl, diff);// ���ݱ仯ֵ������ģʽ

	if (new_mode == cl->cmode)// ģʽû��, ����
		return;

	if (cl->prio_activity) {	/* not necessary: speed optimization */// cl->prio_activity��0��ʾ�ǻ�Ľڵ�, ��Ҫֹͣ���ٸ���ģʽ
		if (cl->cmode != HTB_CANT_SEND)// ��ԭ����ģʽ���ɷ�������, ��ͣ�ýڵ�
			htb_deactivate_prios(q, cl);
		cl->cmode = new_mode;
		if (new_mode != HTB_CANT_SEND)// �����ģʽ���ǽ�ֹ����, ���¼���ڵ�
			htb_activate_prios(q, cl);
	} else // �ǻ���ڵ�, ֱ�Ӹ���ģʽֵ
		cl->cmode = new_mode;
}

/**
 * htb_activate - inserts leaf cl into appropriate active feeds
 *
 * Routine learns (new) priority of leaf and activates feed chain
 * for the prio. It can be called on already active leaf safely.
 * It also adds leaf into droplist.
 */
// �������ṹ, �������ڵ���Ϊ���ݰ��ṩ��, �����������ṩ��һ��
// �����, ��RB����ʽʵ��
//����htb_class��ʱ�򴴽���struct htb_class�Ǽ��뵽htb_sched(htb qdisc��˽������)->clhash�У�����û���γ�һ�ź�������γɺ��������htb_enqueue->htb_activateʵ��
static inline void htb_activate(struct htb_sched *q, struct htb_class *cl)
{
	WARN_ON(cl->level || !cl->un.leaf.q || !cl->un.leaf.q->q.qlen);

	if (!cl->prio_activity) {// �������prio_activity����Ϊ0�Ž��в���, ��0��ʾ�Ѿ�������	    
        // prio_activity��ͨ��Ҷ�ӽڵ��prioֵ�����õ�, ������1, �����1<<7, ��0ֵ
        // leaf.aprio���浱ǰ��leaf.prio
		cl->prio_activity = 1 << cl->prio;
		htb_activate_prios(q, cl);// ����ʵ�ʵļ������
		list_add_tail(&cl->un.leaf.drop_list,
			      q->drops + cl->prio);// ����leaf.aprio��ӵ�ָ��������Ȩλ�õĶ�������
	}
}

/**
 * htb_deactivate - remove leaf cl from active feeds
 *
 * Make sure that leaf is active. In the other words it can't be called
 * with non-active leaf. It also removes class from the drop list.
 *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/
//�����Ҷ�ӽڵ�ӻ�����ݰ��ṩ����ȥ��,cl��Ҷ�ӽڵ�
static inline void htb_deactivate(struct htb_sched *q, struct htb_class *cl)
{
	WARN_ON(!cl->prio_activity);

	htb_deactivate_prios(q, cl);// �ر�
	cl->prio_activity = 0;// ���Ļ���ֵprio_activity����
	list_del_init(&cl->un.leaf.drop_list);
}

static int htb_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	int uninitialized_var(ret);
	struct htb_sched *q = qdisc_priv(sch);

	//����ͨ�����������ƥ���classΪҶ�����Ķ��й�̣�����������м���й������ֱ������Ҷ�ӣ�ֱ����ӡ��ο�<HTB�����Լ�ʹ��.doc>
	struct htb_class *cl = htb_classify(skb, sch, &ret);//�������ȵ��õ��Ƿ��������鿴���ݰ�Ҫ�ŵ��Ǹ������У�Ȼ���ٽ��н��Ӳ���

	if (cl == HTB_DIRECT) { //�ڹ�������û���ҵ�ƥ��ķ��࣬����Ĭ�ϵ�classҲû�ҵ���Ҳ����tc adisc add xxxxx defualt 5�е�5����ֱ��ʹ��htb_sched->direct_queue���
		/* enqueue to helper queue */
		if (q->direct_queue.qlen < q->direct_qlen) {
			__skb_queue_tail(&q->direct_queue, skb);
			q->direct_pkts++;
		} else {
			kfree_skb(skb);
			sch->qstats.drops++;
			return NET_XMIT_DROP;
		}
#ifdef CONFIG_NET_CLS_ACT
	} else if (!cl) {
		if (ret & __NET_XMIT_BYPASS)
			sch->qstats.drops++;
		kfree_skb(skb);
		return ret;
#endif
	} else { //���SKB�������������Ӧ�ķ�����򣬻���Ĭ��default�еĶ��й�̴��ڣ���������
	    if ((ret = qdisc_enqueue(skb, cl->un.leaf.q)) != NET_XMIT_SUCCESS) { //�ݹ�ѭ����ӣ�ֱ���ҵ�Ҷ�ӽڵ�,htbĬ�ϵ���pfifo_qdisc_ops
    		if (net_xmit_drop_count(ret)) { //���ʧ��
    			sch->qstats.drops++;
    			cl->qstats.drops++;
    		}
    		return ret;
    	} else { //��ӳɹ������ʵ�����Ǽӵ�Ҷ��������й�����棬����pfifo fifo_fast��
    		cl->bstats.packets +=
    			skb_is_gso(skb)?skb_shinfo(skb)->gso_segs:1;

    		/*
                // ����HTB���, ���������������ṩ��, ����dequeueʱ���Դ���ȡ���ݰ�
                // ֻ�����ڵ��ģʽ�ǿɷ��ͺͿ���������²Żἤ��, ����ڵ�������
                // ģʽ, �򲻻ᱻ����
    		    */
    		cl->bstats.bytes += qdisc_pkt_len(skb);
    		htb_activate(q, cl);
    	}
	}

	sch->q.qlen++;
	sch->bstats.packets += skb_is_gso(skb)?skb_shinfo(skb)->gso_segs:1;
	sch->bstats.bytes += qdisc_pkt_len(skb);
	return NET_XMIT_SUCCESS;
}

static inline void htb_accnt_tokens(struct htb_class *cl, int bytes, long diff)
{
	long toks = diff + cl->tokens;

	if (toks > cl->buffer)
		toks = cl->buffer;
	toks -= (long) qdisc_l2t(cl->rate, bytes);
	if (toks <= -cl->mbuffer)
		toks = 1 - cl->mbuffer;

	cl->tokens = toks;
}

static inline void htb_accnt_ctokens(struct htb_class *cl, int bytes, long diff)
{
	long toks = diff + cl->ctokens;

	if (toks > cl->cbuffer)
		toks = cl->cbuffer;
		
	toks -= (long) qdisc_l2t(cl->ceil, bytes);
	if (toks <= -cl->mbuffer)
		toks = 1 - cl->mbuffer;

	cl->ctokens = toks;
}

/**
 * htb_charge_class - charges amount "bytes" to leaf and ancestors
 *
 * Routine assumes that packet "bytes" long was dequeued from leaf cl
 * borrowing from "level". It accounts bytes to ceil leaky bucket for
 * leaf and all ancestors and to rate bucket for ancestors at levels
 * "level" and higher. It also handles possible change of mode resulting
 * from the update. Note that mode can also increase here (MAY_BORROW to
 * CAN_SEND) because we can use more precise clock that event queue here.
 * In such case we remove class from event queue first.
 *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/// ��������ؽڵ�cl�Լ������и��ڵ�q���������, ����������ģʽcmode         //����classģʽ��ͬʱͨ��htb_activate_prios�����γ�row leaf����
static void htb_charge_class(struct htb_sched *q, struct htb_class *cl,
			     int level, struct sk_buff *skb)
{
	int bytes = qdisc_pkt_len(skb);
	enum htb_cmode old_mode;
	long diff;

    // ѭ�����ϵ����ڵ�
	while (cl) {
	    // ʱ����
		diff = psched_tdiff_bounded(q->now, cl->t_c, cl->mbuffer);
		
        // ����θߵĽ������
		if (cl->level >= level) {
			if (cl->level == level)
				cl->xstats.lends++;

			//������ͨ����
			htb_accnt_tokens(cl, bytes, diff);
		} else {
            // ����ε�
            // ��������
			cl->xstats.borrows++;
			cl->tokens += diff;	/* we moved t_c; update tokens */// ��������
		}
		htb_accnt_ctokens(cl, bytes, diff);// ����Ceil����
		cl->t_c = q->now;

		old_mode = cl->cmode;// �������ڵ�ԭ����ģʽ
		diff = 0;
		
        // �����µ�����,�����������������ڵ��ģʽ, ��Ϊǰ��diff�����Ѿ����������޸Ĺ���
        // ��������diff����ֵ��Ϊ0��, ��������, ���ģʽ���ǿɷ���ʱ, diff�б��浱ǰ������
        // �ĸ�ֵ
		htb_change_class_mode(q, cl, &diff);//����classģʽ��ͬʱͨ��htb_activate_prios�����γ�row leaf����
		if (old_mode != cl->cmode) { // ������ģʽ�����˱仯�� 
			if (old_mode != HTB_CAN_SEND)// �����ģʽ���ǿ���ֱ�ӷ��͵�ģʽ(HTB_CAN_SEND), ˵���ڵȴ�RB����, Ҫ�Ӹ�RB����ɾ��
				htb_safe_rb_erase(&cl->pq_node, q->wait_pq + cl->level);
			if (cl->cmode != HTB_CAN_SEND)
				htb_add_to_wait_tree(q, cl, diff);// �����ǰ��ģʽ���ǿ���ֱ�ӷ��͵�ģʽ(HTB_CAN_SEND), �ҽӵ����ʵĵȴ�RB��
		}

		/* update byte stats except for leaves which are already updated */
		if (cl->level) {// ������м�ڵ�, ������ͳ��ֵ, ��Ϊ����Ҷ�ӽڵ��Ѿ������ݰ�����ʱ�������
			cl->bstats.bytes += bytes;
			cl->bstats.packets += skb_is_gso(skb)?
					skb_shinfo(skb)->gso_segs:1;
		}
		cl = cl->parent;
	}
}

/**
 * htb_do_events - make mode changes to classes at the level
 *
 * Scans event queue for pending events and applies them. Returns time of
 * next pending event (0 for no event in pq, q->now for too many events).
 * Note: Applied are events whose have cl->pq_key <= q->now.
 */
/*
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by
*/ // �Ե�level�ŵȴ��������ڵ����ģʽ����
static psched_time_t htb_do_events(struct htb_sched *q, int level,
				   unsigned long start)
{
	/* don't run for longer than 2 jiffies; 2 is used instead of
	   1 to simplify things when jiffy is going to be incremented
	   too soon */
	unsigned long stop_at = start + 2;
	while (time_before(jiffies, stop_at)) {
		struct htb_class *cl;
		long diff;
		struct rb_node *p = rb_first(&q->wait_pq[level]);

		if (!p)
			return 0;

		cl = rb_entry(p, struct htb_class, pq_node);
		if (cl->pq_key > q->now)
			return cl->pq_key;

		htb_safe_rb_erase(p, q->wait_pq + level);
		diff = psched_tdiff_bounded(q->now, cl->t_c, cl->mbuffer);
		htb_change_class_mode(q, cl, &diff);
		if (cl->cmode != HTB_CAN_SEND)
			htb_add_to_wait_tree(q, cl, diff);
	}

	/* too much load - let's continue after a break for scheduling */
	if (!(q->warned & HTB_WARN_TOOMANYEVENTS)) {
		printk(KERN_WARNING "htb: too many events!\n");
		q->warned |= HTB_WARN_TOOMANYEVENTS;
	}

	return q->now;
}

/* Returns class->node+prio from id-tree where classe's id is >= id. NULL
   is no such one exists. */
static struct rb_node *htb_id_find_next_upper(int prio, struct rb_node *n,
					      u32 id)
{
	struct rb_node *r = NULL;
	while (n) {
		struct htb_class *cl =
		    rb_entry(n, struct htb_class, node[prio]);

		if (id > cl->common.classid) {
			n = n->rb_right;
		} else if (id < cl->common.classid) {
			r = n;
			n = n->rb_left;
		} else {
			return n;
		}
	}
	return r;
}

/**
 * htb_lookup_leaf - returns next leaf class in DRR order
 *
 * Find leaf where current feed pointers points to.
 *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by
*/ // ����Ҷ�ӷ���ڵ�  pptr�洢����ҵ���Ҷ�ӽڵ�ĸ�class��   ͼ�λ����ο�<Linux htb Դ������� >
static struct htb_class *htb_lookup_leaf(struct rb_root *tree, int prio,
					 struct rb_node **pptr, u32 * pid)
{
	int i;
	struct {
		struct rb_node *root;// ���ڵ�
		struct rb_node **pptr;// ���ڵ��ַ
		u32 *pid;
	} stk[TC_HTB_MAXDEPTH], *sp = stk; //������8��Ԫ�ص����飬��ʼspָ���0��Ԫ�ء� 

	BUG_ON(!tree->rb_node);
	sp->root = tree->rb_node;
	sp->pptr = pptr;
	sp->pid = pid;

	for (i = 0; i < 65535; i++) {// 64K�ε�ѭ��, Ϊʲô��64K��
		if (!*sp->pptr && *sp->pid) {// ���ڵ�Ϊ��, �ɸ�ID��0, ���²��Ҹ��ڵ�
			/* ptr was invalidated but id is valid - try to recover
			   the original or next ptr */
			*sp->pptr =
			    htb_id_find_next_upper(prio, sp->root, *sp->pid);
		}

		// ��ID����
		*sp->pid = 0;	/* ptr is valid now so that remove this hint as it
				   can become out of date quickly */
		if (!*sp->pptr) {	/* we are at right end; rewind & go up */ // ������ڵ㻹��Ϊ��
			*sp->pptr = sp->root;// ���ڵ�����Ϊ���ڵ�
			while ((*sp->pptr)->rb_left)
				*sp->pptr = (*sp->pptr)->rb_left;// ���ڵ�����Ϊ���²����Ҷ�ӽڵ�

			// ��������������0Ԫ��, �����������ִ�й������
			if (sp > stk) {
				sp--;// �Ƶ�ǰһԪ��
				if (!*sp->pptr) {// �����Ԫ�صĸ��ڵ�Ϊ��, ���ؿ�, ������ѭ������1
					WARN_ON(1);
					return NULL;
				}
				htb_next_rb_node(sp->pptr);// pptr����Ϊ��һ���ڵ�
			}
		} else {
			struct htb_class *cl;
			// ��ȡ���ڵ��еĵ�prio�Žڵ��Ӧ��HTB���ṹ
			cl = rb_entry(*sp->pptr, struct htb_class, node[prio]);
			if (!cl->level)// �����Ҷ�ӽڵ�, ����, ������ѭ������2
				return cl;
			
            // �ƶ���stk�������һ��
            // �ø�HTB���ṹ��������ʼ����������Ĳ�������ѭ��
			(++sp)->root = cl->un.inner.feed[prio].rb_node;
			sp->pptr = cl->un.inner.ptr + prio;
			sp->pid = cl->un.inner.last_ptr_id + prio;
		}
	}
	WARN_ON(1);
	// ѭ������Ҳû�ҵ����ʽڵ�, ���ؿ�
	return NULL;
}

/* dequeues packet at given priority and level; call only if
   you are sure that there is active class at prio/level 
   3.DRR����ôʵ�ֵģ� 
   
   ��νdeficit round robin������htb_dequeue_tree()������ĩβʵ�ֵġ� 
   
   860 if (likely(skb != NULL)) { 
   861      bstats_update(&cl->bstats, skb); 
   862      cl->un.leaf.deficit[level] -= qdisc_pkt_len(skb); 
   863      if (cl->un.leaf.deficit[level] < 0) { 
   864           cl->un.leaf.deficit[level] += cl->quantum; 
   865           htb_next_rb_node(level ? &cl->parent->un.inner.clprio[prio].ptr : 
   866                                              &q->hlevel[0].hprio[prio].ptr);   
   867      }  
   874 } 
   �����ѭ���ȣ�������ѭ�������ѯ
   *//*
HTB���ӹ���
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue
htb_dequeue
  -> __skb_dequeue
  -> htb_do_events
    -> htb_safe_rb_erase
    -> htb_change_class_mode
    -> htb_add_to_wait_tree
  -> htb_dequeue_tree
    -> htb_lookup_leaf
    -> htb_deactivate
    -> q->dequeue
    -> htb_next_rb_node
    -> htb_charge_class
      -> htb_change_class_mode
      -> htb_safe_rb_erase
      -> htb_add_to_wait_tree
  -> htb_delay_by

*/
// ��ָ���Ĳ�κ�����Ȩ��RB���ڵ���ȡ���ݰ�
static struct sk_buff *htb_dequeue_tree(struct htb_sched *q, int prio,
					int level)
{
	struct sk_buff *skb = NULL;
	struct htb_class *cl, *start;
	/* look initial class up in the row */
	start = cl = htb_lookup_leaf(q->row[level] + prio, prio,
				     q->ptr[level] + prio,
				     q->last_ptr_id[level] + prio); // ���ݲ�κ�����Ȩֵ������ʼ���ڵ�

	do {
next:
		if (unlikely(!cl))// ������Ϊ��, �������ݰ�Ϊ��
			return NULL;

		/* class can be empty - it is unlikely but can be true if leaf
		   qdisc drops packets in enqueue routine or if someone used
		   graft operation on the leaf since last dequeue;
		   simply deactivate and skip such class */
		if (unlikely(cl->un.leaf.q->q.qlen == 0)) {// ������г���Ϊ0, ���пյ����, �����Խ�С
			struct htb_class *next;
			htb_deactivate(q, cl);// ����������û���ݰ���, ֹͣ�����ṹ

			/* row/level might become empty */
			if ((q->row_mask[level] & (1 << prio)) == 0)// �����λΪ0�� ��ʾ�ò��prio��rb��Ϊ��, û�������ṩ���� �������ݰ�Ϊ��
				return NULL;

			next = htb_lookup_leaf(q->row[level] + prio,
					       prio, q->ptr[level] + prio,
					       q->last_ptr_id[level] + prio);// �������²��Ҹò������Ȩ��RB����Ҳ���ǲ���q->row[level] + prio������е���һ��class

            //�����ҵ���������ṹcl��ʼѭ��, �Ҷ��зǿյĽڵ�
			if (cl == start)	/* fix start if we just deleted it */
				start = next;
			cl = next;
			goto next; // ���goto�γ��˴�ѭ���е�Сѭ��, �Ҷ��г��ȷǿյ����ڵ�
		}
        
        // �����Ƕ��г��ȷǿյ����, ���и����ṹ���ڲ����ؽڵ�ĳ��Ӳ���, 
        // ����Ҫ���ýڵ�ʹ�����������㷨��, ��tbf֮��Ϳ���ʵ���������� 
		skb = cl->un.leaf.q->dequeue(cl->un.leaf.q);
		// ȡ�����ݰ�, �ж�ѭ��׼������
		if (likely(skb != NULL))
			break;

		qdisc_warn_nonwc("htb", cl->un.leaf.q);// ûȡ�����ݰ�, ��ӡ������Ϣ, ����Ϣ��ѭ����ֻ��ӡһ��
		htb_next_rb_node((level ? cl->parent->un.inner.ptr : q->ptr[0]) + prio);// ���µ���һ��rb���ڵ�
		cl = htb_lookup_leaf(q->row[level] + prio, prio,
				     q->ptr[level] + prio,
				     q->last_ptr_id[level] + prio);// �������Ҹò������Ȩ��RB������Ҷ�����ڵ�, ѭ��
    
    // ���ҵ����½ڵ㲻����ʼ�ڵ�ͽ���ѭ��ֱ��ȡ�����ݰ�, �����������ֻص�start�ڵ�
    // ���ж�ѭ��
	} while (cl != start);

	if (likely(skb != NULL)) {    
        // �ҵ����ݰ������, �����Ժܴ�
        // �������deficit, �����ݰ�����, ��deficit�ǳ�ʼ��Ϊ0��
		cl->un.leaf.deficit[level] -= qdisc_pkt_len(skb);
		// ��������ڵ�ĳ���Ϊ��, ����һ��������, ȱʡ�����������Ķ��г���
		if (cl->un.leaf.deficit[level] < 0) {
			cl->un.leaf.deficit[level] += cl->quantum;
            // ���µ���һ��rb���ڵ�, ������м�ڵ�, ����¸��ڵ���ڲ��ṹ�е�ָ��, ����
            // �����ؽṹ�и���, ʵ��ͬһ������в�ͬ������ڵ��ת��, ����һֱ������һ���ڵ�
            //�ú������ú����cl->parent->un.inner.ptr + prio����q->ptr[0]  + prioָ����һ���ڵ�
			htb_next_rb_node((level ? cl->parent->un.inner.ptr : q->ptr[0]) + prio);
		}
		// �������Ϊ���Ͳ������RB���ڵ�ĸ���
		/* this used to be after charge_class but this constelation
		   gives us slightly better performance */
		// ������п���, ֹͣ�����
		if (!cl->un.leaf.q->q.qlen)
			htb_deactivate(q, cl);
		// ��������ؽڵ��Լ������и��ڵ���������, ����������ģʽcmode
		htb_charge_class(q, cl, level, skb);
	}
	return skb;
}

/*
HTB�ĳ����Ǹ��ǳ����ӵĴ������, �������ù���Ϊ:
__qdisc_run -> qdisc_restart -> dequeue_skb -> htb_dequeue

htb_dequeue
	__skb_dequeue
	-> htb_do_events
	 -> htb_safe_rb_erase
	-> htb_change_class_mode
	-> htb_add_to_wait_tree
	-> htb_dequeue_tree
	-> htb_lookup_leaf
	-> htb_deactivate
	-> q->dequeue
	-> htb_next_rb_node
	-> htb_charge_class
	-> htb_change_class_mode
	-> htb_safe_rb_erase
	-> htb_add_to_wait_tree
	-> htb_delay_by
*/
static struct sk_buff *htb_dequeue(struct Qdisc *sch)
{
	struct sk_buff *skb = NULL;
	struct htb_sched *q = qdisc_priv(sch);
	int level;
	psched_time_t next_event;
	unsigned long start_at;

	/* try to dequeue direct packets as high prio (!) to minimize cpu work */
	skb = __skb_dequeue(&q->direct_queue); //�ȰѲ���Ҫ���ٵ�SKB���ķ��ͳ�ȥ
	if (skb != NULL) {
		sch->flags &= ~TCQ_F_THROTTLED;// ȡ�����ݰ�, ���²���, ������, ����
		sch->q.qlen--;
		return skb;
	}

	if (!sch->q.qlen)
		goto fin;
	q->now = psched_get_time();// ��ȡ��ǰ��Чʱ��ֵ
	start_at = jiffies;// ���浱ǰʱ��δ���

	next_event = q->now + 5 * PSCHED_TICKS_PER_SEC;

	for (level = 0; level < TC_HTB_MAXDEPTH; level++) {// �����������в��, ��Ҷ�ӽڵ㿪ʼ
		/* common case optimization - skip event handler quickly */
		int m;
		psched_time_t event;
     
        // �����ӳ�ֵ, ��ȡ���ݰ�ʧ�ܵ�����¸���HTB��ʱ�����ӳ�ʱ��
        // �Ƚ�ROW���иò�ڵ�������¼���ʱʱ���Ƿ��Ѿ����� 
		if (q->now >= q->near_ev_cache[level]) {
			event = htb_do_events(q, level, start_at);// ʱ�䵽��, ����HTB�¼�, ����ֵ����һ���¼����ӳ�ʱ��
			if (!event)
				event = q->now + PSCHED_TICKS_PER_SEC;
			q->near_ev_cache[level] = event;// ���±��������ʱʱ��
		} else
			event = q->near_ev_cache[level];// ʱ�仹û��, ��������ʱ���

        // ������С�ӳ�ֵ, ע��������ѭ��������и��µ�, ѭ���ҳ���С���ӳ�ʱ��
		if (next_event > event)
			next_event = event;

        //�ò�ε�row_maskȡ��, ʵ����Ϊ�ҵ�row_mask[level]��Ϊ1��λ, Ϊ1��ʾ���������ݰ�����
		m = ~q->row_mask[level];
		while (m != (int)(-1)) { //����ÿ��level�е�����prio, q->row[level] + prio
            //m������λ�е�һ��0λ��λ����Ϊ���ȼ�ֵ, �ӵ�λ��ʼ��, Ҳ����prioԽС, ʵ�����ݵ�����ȨԽ��, Խ�ȳ���
			int prio = ffz(m);
			m |= 1 << prio;
            // �Ӹ�����Ȩֵ���������н��г��Ӳ���HTB�����ؾ��ڸú���������
			skb = htb_dequeue_tree(q, prio, level);// ��ָ���Ĳ�κ�����Ȩ��RB���ڵ���ȡ���ݰ�
			if (likely(skb != NULL)) {
                // ���ݰ����ӳɹ�, ���²���, �˳�ѭ��, �������ݰ�
                // ȡ���ݰ��ɹ���Ҫȥ�����ؽڵ��������־
				sch->q.qlen--;
				sch->flags &= ~TCQ_F_THROTTLED;
				goto fin;
			}
		}
	}
	sch->qstats.overlimits++;
	if (likely(next_event > q->now))
		qdisc_watchdog_schedule(&q->watchdog, next_event);
	else
		schedule_work(&q->work);
fin:
	return skb;
}

/* try to drop from each class (by prio) until one succeed */
static unsigned int htb_drop(struct Qdisc *sch)
{
	struct htb_sched *q = qdisc_priv(sch);
	int prio;

	for (prio = TC_HTB_NUMPRIO - 1; prio >= 0; prio--) {// ������������Ķ�������, ���Ȳ�������7������, ����������0������
		struct list_head *p;
		list_for_each(p, q->drops + prio) {
			struct htb_class *cl = list_entry(p, struct htb_class,
							  un.leaf.drop_list);
			unsigned int len;
			if (cl->un.leaf.q->ops->drop &&
			    (len = cl->un.leaf.q->ops->drop(cl->un.leaf.q))) {// ���������Ҷ�ӽڵ����ض����˶�������, ������Ӧ��������
				sch->q.qlen--;
				if (!cl->un.leaf.q->q.qlen)// �����ؽڵ�Ϊ��, ֹͣ�����
					htb_deactivate(q, cl);
				return len;
			}
		}
	}
	return 0;
}

/* reset all classes */
/* always caled under BH & queue lock */
static void htb_reset(struct Qdisc *sch)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct htb_class *cl;
	struct hlist_node *n;
	unsigned int i;

	for (i = 0; i < q->clhash.hashsize; i++) {
		hlist_for_each_entry(cl, n, &q->clhash.hash[i], common.hnode) {
			if (cl->level)
				memset(&cl->un.inner, 0, sizeof(cl->un.inner));
			else {
				if (cl->un.leaf.q)
					qdisc_reset(cl->un.leaf.q);
				INIT_LIST_HEAD(&cl->un.leaf.drop_list);
			}
			cl->prio_activity = 0;
			cl->cmode = HTB_CAN_SEND;

		}
	}
	qdisc_watchdog_cancel(&q->watchdog);
	__skb_queue_purge(&q->direct_queue);
	sch->q.qlen = 0;
	memset(q->row, 0, sizeof(q->row));
	memset(q->row_mask, 0, sizeof(q->row_mask));
	memset(q->wait_pq, 0, sizeof(q->wait_pq));
	memset(q->ptr, 0, sizeof(q->ptr));
	for (i = 0; i < TC_HTB_NUMPRIO; i++)
		INIT_LIST_HEAD(q->drops + i);
}

static const struct nla_policy htb_policy[TCA_HTB_MAX + 1] = {
	[TCA_HTB_PARMS]	= { .len = sizeof(struct tc_htb_opt) },
	[TCA_HTB_INIT]	= { .len = sizeof(struct tc_htb_glob) },
	[TCA_HTB_CTAB]	= { .type = NLA_BINARY, .len = TC_RTAB_SIZE },
	[TCA_HTB_RTAB]	= { .type = NLA_BINARY, .len = TC_RTAB_SIZE },
};

static void htb_work_func(struct work_struct *work)
{
	struct htb_sched *q = container_of(work, struct htb_sched, work);
	struct Qdisc *sch = q->watchdog.qdisc;

	__netif_schedule(qdisc_root(sch));
}

static int htb_init(struct Qdisc *sch, struct nlattr *opt)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct nlattr *tb[TCA_HTB_INIT + 1];
	struct tc_htb_glob *gopt;
	int err;
	int i;

	if (!opt)
		return -EINVAL;

	err = nla_parse_nested(tb, TCA_HTB_INIT, opt, htb_policy);// ����û��ռ䴫�����ĳ�ʼ�����ݵĺϷ���
	if (err < 0)
		return err;

	if (tb[TCA_HTB_INIT] == NULL) {
		printk(KERN_ERR "HTB: hey probably you have bad tc tool ?\n");
		return -EINVAL;
	}
	gopt = nla_data(tb[TCA_HTB_INIT]);
	if (gopt->version != HTB_VER >> 16) {// ���汾��Ϣ�Ƿ�ƥ��
		printk(KERN_ERR
		       "HTB: need tc/htb version %d (minor is %d), you have %d\n",
		       HTB_VER >> 16, HTB_VER & 0xffff, gopt->version);
		return -EINVAL;
	}

	err = qdisc_class_hash_init(&q->clhash);
	if (err < 0)
		return err;
	for (i = 0; i < TC_HTB_NUMPRIO; i++)
		INIT_LIST_HEAD(q->drops + i);

	qdisc_watchdog_init(&q->watchdog, sch);
	INIT_WORK(&q->work, htb_work_func);
	skb_queue_head_init(&q->direct_queue);

	q->direct_qlen = qdisc_dev(sch)->tx_queue_len;
	if (q->direct_qlen < 2)	/* some devices have zero tx_queue_len */
		q->direct_qlen = 2;

	if ((q->rate2quantum = gopt->rate2quantum) < 1) // ����������ת������, ��TC�����е�r2q����
		q->rate2quantum = 1;
	q->defcls = gopt->defcls;// ȱʡ���

	return 0;
}

//���HTB����
static int htb_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	spinlock_t *root_lock = qdisc_root_sleeping_lock(sch);
	struct htb_sched *q = qdisc_priv(sch);
	struct nlattr *nest;
	struct tc_htb_glob gopt;

	spin_lock_bh(root_lock);

	gopt.direct_pkts = q->direct_pkts;// ֱ�ӷ��͵����ݰ�����
	gopt.version = HTB_VER;
	gopt.rate2quantum = q->rate2quantum;
	gopt.defcls = q->defcls;
	gopt.debug = 0;

    // �������������ݰ��еľ���λ��
	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;
	NLA_PUT(skb, TCA_HTB_INIT, sizeof(gopt), &gopt);// ����ѡ�����
	nla_nest_end(skb, nest);

	spin_unlock_bh(root_lock);
	return skb->len;

nla_put_failure:
	spin_unlock_bh(root_lock);
	nla_nest_cancel(skb, nest);
	return -1;
}
/*
root@mc_core:~# tc class show dev eth0
class htb 2:3 root prio 0 rate 328bit ceil 4440bit burst 1599b cburst 1599b 
*///Ӧ�ò�ͨ��tc class show��ȡhtb������Ϣ
static int htb_dump_class(struct Qdisc *sch, unsigned long arg,
			  struct sk_buff *skb, struct tcmsg *tcm)
{
	struct htb_class *cl = (struct htb_class *)arg;
	spinlock_t *root_lock = qdisc_root_sleeping_lock(sch);
	struct nlattr *nest;
	struct tc_htb_opt opt;

	spin_lock_bh(root_lock);
	// ���ڵ�����ID
	tcm->tcm_parent = cl->parent ? cl->parent->common.classid : TC_H_ROOT;
	// ���ڵ�����ID
	tcm->tcm_handle = cl->common.classid;
	if (!cl->level && cl->un.leaf.q)// �����Ҷ�ӽڵ�, �ṩҶ�ӽڵ�����ؽڵ��ID
		tcm->tcm_info = cl->un.leaf.q->handle;

	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	memset(&opt, 0, sizeof(opt));// �����ṩ�����ĸ��ֲ���

	opt.rate = cl->rate->rate;// ����
	opt.buffer = cl->buffer;// ���ݻ�����
	opt.ceil = cl->ceil->rate;// ��ֵ����
	opt.cbuffer = cl->cbuffer;// ��ֵ���ݻ�����
	opt.quantum = cl->quantum;// ����
	opt.prio = cl->prio;
	opt.level = cl->level;// ���ֵ
	NLA_PUT(skb, TCA_HTB_PARMS, sizeof(opt), &opt);

	nla_nest_end(skb, nest);// ʵ�����ݳ���
	spin_unlock_bh(root_lock);
	return skb->len;

nla_put_failure:
	spin_unlock_bh(root_lock);
	nla_nest_cancel(skb, nest);
	return -1;
}

//Ӧ�ò�tc -s qdisc ls dev eth0    ���ͳ����Ϣ��� 
static int
htb_dump_class_stats(struct Qdisc *sch, unsigned long arg, struct gnet_dump *d)
{
	struct htb_class *cl = (struct htb_class *)arg;

	if (!cl->level && cl->un.leaf.q)// Ҷ�ӽڵ�, �ṩ��ǰ�ڲ����ؽṹ�Ķ��г���
		cl->qstats.qlen = cl->un.leaf.q->q.qlen;
	cl->xstats.tokens = cl->tokens; //��ǰ������
	cl->xstats.ctokens = cl->ctokens;// ��ֵ������

    // �ֱ𽫻�������, ���ʲ���, ���в���������Ŀ�Ļ���, ��Щ���Ǳ�׼����
	if (gnet_stats_copy_basic(d, &cl->bstats) < 0 ||
	    gnet_stats_copy_rate_est(d, NULL, &cl->rate_est) < 0 ||
	    gnet_stats_copy_queue(d, &cl->qstats) < 0)
		return -1;

    // ��Ӧ������(HTB����ͳ������)������Ŀ�Ļ���
	return gnet_stats_copy_app(d, &cl->xstats, sizeof(cl->xstats));
}

//ʹ��htb_graft()����������Ҷ�ӽڵ�����ط���.

static int htb_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		     struct Qdisc **old)
{
	struct htb_class *cl = (struct htb_class *)arg;

	if (cl->level)// ���ṹ�ǿն��Ҳ��Ϊ0(Ҷ�ӽڵ�)
		return -EINVAL;
	if (new == NULL &&
	    (new = qdisc_create_dflt(qdisc_dev(sch), sch->dev_queue,
				     &pfifo_qdisc_ops,
				     cl->common.classid)) == NULL)// ���û����ר�ŵ����ط���, ��ȱʡ����pfifo��Ϊȱʡ�����ط���
		return -ENOBUFS;

	sch_tree_lock(sch);
	*old = cl->un.leaf.q;
	cl->un.leaf.q = new;// ���µ����ط�����Ϊ���ṹҶ�ӽڵ�����ط���
	if (*old != NULL) {
        // �������𻹴��ڻ״̬, ֹͣ, ��Ϊ��ԭ�������ط����Ѿ�Ҫ���ͷŵ�, 
        // ���ٴ������ݰ�
		qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
		qdisc_reset(*old);// �������ؽڵ��ͷŵ�
	}
	sch_tree_unlock(sch);
	return 0;
}

//���ں��л�ʹ��htb_leaf()����HTBҶ�ӽڵ�
static struct Qdisc *htb_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct htb_class *cl = (struct htb_class *)arg;
	return !cl->level ? cl->un.leaf.q : NULL;// ������ṹ�ǿն�����Ҷ�ӽڵ�, ���ظ����Ҷ�ӽڵ������
}

static void htb_qlen_notify(struct Qdisc *sch, unsigned long arg)
{
	struct htb_class *cl = (struct htb_class *)arg;

	if (cl->un.leaf.q->q.qlen == 0)
		htb_deactivate(qdisc_priv(sch), cl);
}

//����tc class add parent 3 classid 3:4�е�classid 3:4�ҵ���Ӧ���ں˷�����Ϣ�ṹhtb_class�����ѵ�ַǿ��ת��Ϊlong����
static unsigned long htb_get(struct Qdisc *sch, u32 classid) //�����������ü���,����htb_put
{
	struct htb_class *cl = htb_find(classid, sch);
	if (cl)
		cl->refcnt++;
	return (unsigned long)cl;
}

static inline int htb_parent_last_child(struct htb_class *cl)
{
	if (!cl->parent)
		/* the root class */
		return 0;
	if (cl->parent->children > 1)
		/* not the last child */
		return 0;
	return 1;
}

static void htb_parent_to_leaf(struct htb_sched *q, struct htb_class *cl,
			       struct Qdisc *new_q)
{
	struct htb_class *parent = cl->parent;

	WARN_ON(cl->level || !cl->un.leaf.q || cl->prio_activity);

	if (parent->cmode != HTB_CAN_SEND)
		htb_safe_rb_erase(&parent->pq_node, q->wait_pq + parent->level);

	parent->level = 0;
	memset(&parent->un.inner, 0, sizeof(parent->un.inner));
	INIT_LIST_HEAD(&parent->un.leaf.drop_list);
	parent->un.leaf.q = new_q ? new_q : &noop_qdisc;
	parent->tokens = parent->buffer;
	parent->ctokens = parent->cbuffer;
	parent->t_c = psched_get_time();
	parent->cmode = HTB_CAN_SEND;
}

static void htb_destroy_class(struct Qdisc *sch, struct htb_class *cl)
{
	if (!cl->level) {
		WARN_ON(!cl->un.leaf.q);
		qdisc_destroy(cl->un.leaf.q);
	}
	gen_kill_estimator(&cl->bstats, &cl->rate_est);
	qdisc_put_rtab(cl->rate);
	qdisc_put_rtab(cl->ceil);

	tcf_destroy_chain(&cl->filter_list);
	kfree(cl);
}

static void htb_destroy(struct Qdisc *sch)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct hlist_node *n, *next;
	struct htb_class *cl;
	unsigned int i;

	cancel_work_sync(&q->work);
	qdisc_watchdog_cancel(&q->watchdog);
	/* This line used to be after htb_destroy_class call below
	   and surprisingly it worked in 2.4. But it must precede it
	   because filter need its target class alive to be able to call
	   unbind_filter on it (without Oops). */
	tcf_destroy_chain(&q->filter_list);

	for (i = 0; i < q->clhash.hashsize; i++) {
		hlist_for_each_entry(cl, n, &q->clhash.hash[i], common.hnode)
			tcf_destroy_chain(&cl->filter_list);
	}
	for (i = 0; i < q->clhash.hashsize; i++) {
		hlist_for_each_entry_safe(cl, n, next, &q->clhash.hash[i],
					  common.hnode)
			htb_destroy_class(sch, cl);
	}
	qdisc_class_hash_destroy(&q->clhash);
	__skb_queue_purge(&q->direct_queue);
}

static int htb_delete(struct Qdisc *sch, unsigned long arg)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct htb_class *cl = (struct htb_class *)arg;
	unsigned int qlen;
	struct Qdisc *new_q = NULL;
	int last_child = 0;

	// TODO: why don't allow to delete subtree ? references ? does
	// tc subsys quarantee us that in htb_destroy it holds no class
	// refs so that we can remove children safely there ?
	if (cl->children || cl->filter_cnt)
		return -EBUSY;

	if (!cl->level && htb_parent_last_child(cl)) {
		new_q = qdisc_create_dflt(qdisc_dev(sch), sch->dev_queue,
					  &pfifo_qdisc_ops,
					  cl->parent->common.classid);
		last_child = 1;
	}

	sch_tree_lock(sch);

	if (!cl->level) {
		qlen = cl->un.leaf.q->q.qlen;
		qdisc_reset(cl->un.leaf.q);
		qdisc_tree_decrease_qlen(cl->un.leaf.q, qlen);
	}

	/* delete from hash and active; remainder in destroy_class */
	qdisc_class_hash_remove(&q->clhash, &cl->common);
	if (cl->parent)
		cl->parent->children--;

	if (cl->prio_activity)
		htb_deactivate(q, cl);

	if (cl->cmode != HTB_CAN_SEND)
		htb_safe_rb_erase(&cl->pq_node, q->wait_pq + cl->level);

	if (last_child)
		htb_parent_to_leaf(q, cl, new_q);

	BUG_ON(--cl->refcnt == 0);
	/*
	 * This shouldn't happen: we "hold" one cops->get() when called
	 * from tc_ctl_tclass; the destroy method is done from cops->put().
	 */

	sch_tree_unlock(sch);
	return 0;
}

static void htb_put(struct Qdisc *sch, unsigned long arg)
{
	struct htb_class *cl = (struct htb_class *)arg;

	if (--cl->refcnt == 0)
		htb_destroy_class(sch, cl);
}

//�������ṹ�ڲ�����
static int htb_change_class(struct Qdisc *sch, u32 classid,
			    u32 parentid, struct nlattr **tca,
			    unsigned long *arg)
{
	int err = -EINVAL;
	struct htb_sched *q = qdisc_priv(sch);
	struct htb_class *cl = (struct htb_class *)*arg, *parent;// ���ṹָ��, ���ϲ㴫��
	struct nlattr *opt = tca[TCA_OPTIONS];// ͨ��netlink�ӿڴ��������ò���
	struct qdisc_rate_table *rtab = NULL, *ctab = NULL;// ���ʱ�, ��ֵ���ʱ�ṹ
	struct nlattr *tb[TCA_HTB_RTAB + 1];// ���������Ĳ���
	struct tc_htb_opt *hopt;// HTBѡ��

	/* extract all subattrs from opt attr */
	if (!opt)
		goto failure;

	err = nla_parse_nested(tb, TCA_HTB_RTAB, opt, htb_policy);// �����������, ������غϷ��Լ��
	if (err < 0)
		goto failure;

	err = -EINVAL;
	if (tb[TCA_HTB_PARMS] == NULL)
		goto failure;

    // ������ڵ�ID���Ǹ�ID, ���ݴ�ID���Ҹ��ڵ�, ����Ϊ���ڵ��
	parent = parentid == TC_H_ROOT ? NULL : htb_find(parentid, sch);

	hopt = nla_data(tb[TCA_HTB_PARMS]);

    // ����������л�ȡ���ʱ�ṹ: ��ͨ���ʺͷ�ֵ����
	rtab = qdisc_get_rtab(&hopt->rate, tb[TCA_HTB_RTAB]);
	ctab = qdisc_get_rtab(&hopt->ceil, tb[TCA_HTB_CTAB]);
	if (!rtab || !ctab)
		goto failure;

	if (!cl) {		/* new class */ //clΪ�գ���ʾ��Ҫ����һ��class�࣬��Ϊ�ձ�ʾ���޸�class����
		struct Qdisc *new_q;
		int prio;
		struct {
			struct nlattr		nla;
			struct gnet_estimator	opt;
		} est = {
			.nla = {
				.nla_len	= nla_attr_size(sizeof(est.opt)),
				.nla_type	= TCA_RATE,
			},
			.opt = {
				/* 4s interval, 16s averaging constant */
				.interval	= 2,
				.ewma_log	= 2,
			},
		};

		/* check for valid classid */
		if (!classid || TC_H_MAJ(classid ^ sch->handle) ||
		    htb_find(classid, sch))// ���ID�Ϸ��Լ��
			goto failure;

		/* check maximal depth */// ����游�ڵ��ζ�С��2, Ҳ���������1, ��ʾHTB�ڵ���̫����, Ҷ�ӽڵ㶼û����ʾ��
		if (parent && parent->parent && parent->parent->level < 2) {
			printk(KERN_ERR "htb: tree is too deep\n");
			goto failure;
		}
		err = -ENOBUFS;
		if ((cl = kzalloc(sizeof(*cl), GFP_KERNEL)) == NULL) //Ϊhtb_class������Ϣ����ռ�
			goto failure;

		err = gen_new_estimator(&cl->bstats, &cl->rate_est,
					qdisc_root_sleeping_lock(sch),
					tca[TCA_RATE] ? : &est.nla);
		if (err) {
			kfree(cl);
			goto failure;
		}

		cl->refcnt = 1;// ��ʼ�����ü���
		cl->children = 0;
		INIT_LIST_HEAD(&cl->un.leaf.drop_list);// ��ʼ����������
		RB_CLEAR_NODE(&cl->pq_node);// ����Ϊ�սڵ�(���ڵ��Ǳ���)

		for (prio = 0; prio < TC_HTB_NUMPRIO; prio++)
			RB_CLEAR_NODE(&cl->node[prio]);// ��ʼ��self or feed tree�ڵ�

		/* create leaf qdisc early because it uses kmalloc(GFP_KERNEL)
		   so that can't be used inside of sch_tree_lock
		   -- thanks to Karlis Peisenieks */
		new_q = qdisc_create_dflt(qdisc_dev(sch), sch->dev_queue,
					  &pfifo_qdisc_ops, classid);// �µ����ؽڵ�ȱʡ��ʹ��pfifo
		sch_tree_lock(sch);
		if (parent && !parent->level) { // ������ڵ�ԭ����Ҷ�ӽڵ�, ����תΪ�м�ڵ�, ��Ϊ�����Ѿ����µ�Ҷ�ӽڵ���Ϊ���ӽڵ�
			unsigned int qlen = parent->un.leaf.q->q.qlen;

			/* turn parent into inner node */
			qdisc_reset(parent->un.leaf.q);// �ͷŸ��ڵ�����ؽṹ
			qdisc_tree_decrease_qlen(parent->un.leaf.q, qlen);
			qdisc_destroy(parent->un.leaf.q);
			if (parent->prio_activity)// ����ø��ڵ������ڻ���, ֹͣ
				htb_deactivate(q, parent);

			/* remove from evt list because of level change */
			if (parent->cmode != HTB_CAN_SEND) {// �������HTB_CAN_SENDģʽ, ˵���ýڵ��ڵȴ��ڵ�����, �Ӹ�����ɾ��
				htb_safe_rb_erase(&parent->pq_node, q->wait_pq);
				parent->cmode = HTB_CAN_SEND;
			}
			parent->level = (parent->parent ? parent->parent->level
					 : TC_HTB_MAXDEPTH) - 1;
		    // ����ʹ���ڲ�Ҷ�ӽṹ, ���Ǹ�Ϊʹ��HTB�ڲ��ṹ, ��������
			memset(&parent->un.inner, 0, sizeof(parent->un.inner));
		}
		/* leaf (we) needs elementary qdisc */
		cl->un.leaf.q = new_q ? new_q : &noop_qdisc;// �������ṹ��Ҷ�����ؽڵ�

        // ���ṹ��ID�͸�
		cl->common.classid = classid;
		cl->parent = parent;

		/* set class to be in HTB_CAN_SEND state */ // ���ƺͷ�ֵ����
		cl->tokens = hopt->buffer;
		cl->ctokens = hopt->cbuffer;
		cl->mbuffer = 60 * PSCHED_TICKS_PER_SEC;	/* 1min */ // ��������С
		cl->t_c = psched_get_time();// ��ʼ��ʱ��
		cl->cmode = HTB_CAN_SEND;

		/* attach to the hash list and parent's family */
		qdisc_class_hash_insert(&q->clhash, &cl->common);// �ҽӵ���ϣ����
		if (parent)
			parent->children++;
	} else {
		if (tca[TCA_RATE]) {
			err = gen_replace_estimator(&cl->bstats, &cl->rate_est,
						    qdisc_root_sleeping_lock(sch),
						    tca[TCA_RATE]);
			if (err)
				return err;
		}
		sch_tree_lock(sch);
	}

	/* it used to be a nasty bug here, we have to check that node
	   is really leaf before changing cl->un.leaf ! */
	if (!cl->level) {// �����Ҷ�ӽڵ�, �����䶨��, �����ֳ���ʱ�ᰴ�����С����
		cl->quantum = rtab->rate.rate / q->rate2quantum;
		if (!hopt->quantum && cl->quantum < 1000) {
			printk(KERN_WARNING
			       "HTB: quantum of class %X is small. Consider r2q change.\n",
			       cl->common.classid);
			cl->quantum = 1000;
		}

		// ���������Ķ�����̫С��̫��, ˵��rate2quantum�����õ�����, �����tc�����е�r2q����
		if (!hopt->quantum && cl->quantum > 200000) {
			printk(KERN_WARNING
			       "HTB: quantum of class %X is big. Consider r2q change.\n",
			       cl->common.classid);
			cl->quantum = 200000;
		}
		if (hopt->quantum)
			cl->quantum = hopt->quantum;
			
		if ((cl->prio = hopt->prio) >= TC_HTB_NUMPRIO)
			cl->prio = TC_HTB_NUMPRIO - 1;
	}

	cl->buffer = hopt->buffer;
	cl->cbuffer = hopt->cbuffer;
	if (cl->rate)
		qdisc_put_rtab(cl->rate);
	cl->rate = rtab;
	// ��ֵ���ʿ��ƽṹ����
	if (cl->ceil)
		qdisc_put_rtab(cl->ceil);
	cl->ceil = ctab;
	sch_tree_unlock(sch);

	qdisc_class_hash_grow(sch, &q->clhash);

	*arg = (unsigned long)cl;
	return 0;

failure:
	if (rtab)
		qdisc_put_rtab(rtab);
	if (ctab)
		qdisc_put_rtab(ctab);
	return err;
}

//���ҹ��˹����
static struct tcf_proto **htb_find_tcf(struct Qdisc *sch, unsigned long arg)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct htb_class *cl = (struct htb_class *)arg;
	// ������ṹ�ǿ�,ʹ�����ṹ�Ĺ��˱�, ����ʹ��HTB˽�нṹ�Ĺ��˱�
	struct tcf_proto **fl = cl ? &cl->filter_list : &q->filter_list;

	return fl;
}

static unsigned long htb_bind_filter(struct Qdisc *sch, unsigned long parent,
				     u32 classid)
{
	struct htb_class *cl = htb_find(classid, sch);// �������ID�������ṹ

	/*if (cl && !cl->level) return 0;
	   The line above used to be there to prevent attaching filters to
	   leaves. But at least tc_index filter uses this just to get class
	   for other reasons so that we have to allow for it.
	   ----
	   19.6.2002 As Werner explained it is ok - bind filter is just
	   another way to "lock" the class - unlike "get" this lock can
	   be broken by class during destroy IIUC.
	 */
	if (cl)// ����������ṹ��Ч, ������ʹ�ü���
		cl->filter_cnt++;
	return (unsigned long)cl;
}

static void htb_unbind_filter(struct Qdisc *sch, unsigned long arg)
{
	struct htb_class *cl = (struct htb_class *)arg;

	if (cl)
		cl->filter_cnt--;
}

//rtnl_register -> rtnl_dump_all�����յ�rtlnetlinkӦ�ò�������Ϣ��ʱ���ִ��cb->fn ,��rtnetlink_init -> rtnl_dump_all
//����HTB,ִ�ж�Ӧ��fn
static void htb_walk(struct Qdisc *sch, struct qdisc_walker *arg)
{
	struct htb_sched *q = qdisc_priv(sch);
	struct htb_class *cl;
	struct hlist_node *n;
	unsigned int i;

	if (arg->stop)// �������ֹͣ��־, ����
		return;

	for (i = 0; i < q->clhash.hashsize; i++) {// ��������HTB��ϣ��

	    // ������ϣ����ÿ��Ԫ��, ��HTB���ṹ
		hlist_for_each_entry(cl, n, &q->clhash.hash[i], common.hnode) {
			if (arg->count < arg->skip) {// ���Ҫ����skip����ʼ��һЩ�ڵ�, ������Щ�ڵ�
				arg->count++;
				continue;
			}
			if (arg->fn(sch, (unsigned long)cl, arg) < 0) { //����Ϊqdisc_class_dump  ��tc_dump_tclass_qdisc
				arg->stop = 1;
				return;
			}
			arg->count++;
		}
	}
}

//prio��Ӧprio_class_ops htb��Ӧhtb_class_ops cbq��Ӧcbq_class_ops�ȵ�
static const struct Qdisc_class_ops htb_class_ops = { //tcfһ���ʾtcf_proto������
	.graft		=	htb_graft,
	.leaf		=	htb_leaf,
	.qlen_notify	=	htb_qlen_notify,
	.get		=	htb_get,
	.put		=	htb_put,
	.change		=	htb_change_class,
	.delete		=	htb_delete,
	.walk		=	htb_walk,
	.tcf_chain	=	htb_find_tcf,  
	.bind_tcf	=	htb_bind_filter,
	.unbind_tcf	=	htb_unbind_filter,
	.dump		=	htb_dump_class,
	.dump_stats	=	htb_dump_class_stats,
};

//HTB(�ֲ�����Ͱ)���Բο�:RouterOS QoS HTB����ԭ��  HTB�ֲ�����Ͱ�Ŷӹ������
static struct Qdisc_ops htb_qdisc_ops {//__read_mostly = { �ο�<HTB�����Լ�ʹ��.doc>
	.next		=	NULL,
	.cl_ops		=	&htb_class_ops,
	.id		=	"htb",
	.priv_size	=	sizeof(struct htb_sched),
	.enqueue	=	htb_enqueue,
	.dequeue	=	htb_dequeue,
	.peek		=	qdisc_peek_dequeued,
	.drop		=	htb_drop,
	.init		=	htb_init,
	.reset		=	htb_reset,
	.destroy	=	htb_destroy,
	.change		=	NULL /* htb_change */,
	.dump		=	htb_dump,
	.owner		=	THIS_MODULE,
};

static int __init htb_module_init(void)
{
	return register_qdisc(&htb_qdisc_ops);
}
static void __exit htb_module_exit(void)
{
	unregister_qdisc(&htb_qdisc_ops);
}

module_init(htb_module_init)
module_exit(htb_module_exit)
MODULE_LICENSE("GPL");
