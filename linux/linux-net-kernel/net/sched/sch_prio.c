/*
 * net/sched/sch_prio.c	Simple 3-band priority "scheduler".
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 * Fixes:       19990609: J Hadi Salim <hadi@nortelnetworks.com>:
 *              Init --  EINVAL when opt undefined
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>

/* ���ȼ����й涨��bandΪ16��,�ο�TC��������ʵ�ַ���(����)-ͼ3  ������prio�����͵ĸ����ض���_2 */   //��ϸ���Ҳ���Բο�<<LINUX�߼�·�ɺ���������>>
/*
���ڼ���Ҫ����һ��skb->priorityֵ��8�����ݰ��������������£�
1.      ʹ�������ĸ����ض������Ӻ��������ݰ����
2.      ����δ���ù���������ֱ�Ӹ������ݰ���skb->priority=8�ҵ���Ӧ�Ĵ���0�������ݰ������һ��pfifo���ض���
3.      ����pfifo���ض������Ӻ����������ݰ���������е����ݰ����С�
4.      ����qdisc_run()���������ض���
5.      ���ø����ض���ĳ��Ӻ�������������ѡ���һ��pfifo���ض��󲢵�������Ӻ���ѡ��һ�����ݰ������Ӻ������أ������һ��pfifo���ض���Ϊ�գ�ѡ��ڶ���pfifo���ض��󲢵�������Ӻ���ѡ��һ�����ݰ���ֱ���ҵ�һ�����ݰ���
6.      ����5�ҵ������ݰ���
7.      ֻҪʱ�����������ض���Ϊ�գ���һֱѭ��5��6�Ĺ��̡�
*/
//    ʹ��int register_qdisc(struct Qdisc_ops *qops)ע��������͡�
//    ʹ��int register_tcf_proto_ops(struct tcf_proto_ops *ops)ע����������͡�

/*
����������صĺ���Ϊdev_queue_xmit(); ������������, ����ֻ�Ǹմ������豸���յ�, ��δ���������ϲ㴦��, ����������������ز��Ǳ����,  ����һ��������ض���# tc qdisc add dev eth0 ingress
ȱʡ����²����������أ�����������غ���Ϊing_filter()�������ú�����skb_receive_skb()���á�
*/ //��ȡ���ò�����ĵط���prio_tune���ýṹ��ʼ����prio_tune
struct prio_sched_data //�ں˿ռ��Ӧ�ò�ͨ��netlink�����������ݹ��̣�������pktsched_init��tc qdisc���������������ȷ��
{//tc qdisc add dev eth0 root handle 22 prio band 4 priomap 3 3 2 2 1 2 0 0 1 1 1 1 1 1 1 1
   //band��ʾ��qdisc����м���Ƶ��������qdisc��band�������ܳ�����ֵ���������򷵻ش�(tc qdisc add dev eth0 parent 22:8 handle 33,8���ܳ�����Qdisc��band)����prio_get
	int bands;//��������tc�����е�4����ʾ�õ���prio2band�е�ǰ��4��band(Ƶ��) //bands����ȡֵ��Χ2-16����prio_tune ��������øò�����Ĭ��ֵΪ3����Ӧ�ò�prio_parse_opt
	struct tcf_proto *filter_list; //tc filter��ӹ�������ʱ���õ� ͼ�λ��ο�TC��������ʵ�ַ���(����) //�����ÿ��Ԫ�ص����ָΪǰ���bands ��prio_tune
	u8  prio2band[TC_PRIO_MAX+1]; //priomap����Ĳ�����prio2bandӳ�䣬Ĭ��ӳ��ֻӳ��ǰ3���������ʹ��prio�Ӷ����ڵ�4����������Ҫ��ӹ���������tc filter add dev eth0 protocol ip parent 22: prio 2 u32 match ip dst 4.3.2.1/32 flowid 22:4

    //ͨ��tc qdisc add dev eth0 parent 22:8 handle 33�е�22:8�����з��࣬�Ӷ�ѡ��Ӧ�ð�handleΪ33���Ӷ��й����ӵ������й�̵ĵļ���queue[i]��
    //Ĭ��ָ�����pfifo_qdisc_ops,��qdisc_create -> prio_init -> prio_tune -> qdisc_create_dflt��Ҳ����˵�ڴ���������й�̵�ʱ��ϵͳ��Ĭ�ϸ�������Ϣ����ָ��pfifo������й�̣�Ҳ����queue[]Ĭ��ָ�����pfifo_fast������й��
	struct Qdisc *queues[TCQ_PRIO_BANDS];//��ӣ�������أ�prio���й���opsΪpfifo_qdisc_ops����������tbf_qdisc_ops sfq_qdisc_ops�ȣ� 
};

static struct Qdisc *
prio_classify(struct sk_buff *skb, struct Qdisc *sch, int *qerr)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	u32 band = skb->priority;
	struct tcf_result res;
	int err;

	*qerr = NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
	if (TC_H_MAJ(skb->priority) != sch->handle) {
		err = tc_classify(skb, q->filter_list, &res); //ͨ��skb�͹��������������ѡ�����Ӧ�ķ�������queues[]�еľ�����һ��queues[i]
#ifdef CONFIG_NET_CLS_ACT
		switch (err) {
		case TC_ACT_STOLEN:
		case TC_ACT_QUEUED:
			*qerr = NET_XMIT_SUCCESS | __NET_XMIT_STOLEN;
		case TC_ACT_SHOT:
			return NULL;
		}
#endif
		if (!q->filter_list || err < 0) {
			if (TC_H_MAJ(band))
				band = 0;
		
			return q->queues[q->prio2band[band&TC_PRIO_MAX]];//
		}
		band = res.classid;
	}
	band = TC_H_MIN(band) - 1;
	if (band >= q->bands) //���SKB��priority�ȴ���qdisc��bandƵ������ֱ��ʹ�õ�0��Ƶ����
		return q->queues[q->prio2band[0]];

    //���������й涨û�з���������������й涨����queues[]Ĭ��ָ�����&noop_qdisc;��prio_init��Ҳ����˵���������й涨û�а���������й涨����SKB��ͨ��noop_qdiscֱ�Ӷ���
	return q->queues[band];//���skb->priority��qdisc���ڵ�bands��Χ�ڣ�ֱ��ʹ��q->queues[band]������ʹ��q->queues[q->prio2band[0]]
}

//�����������dev_queue_xmit  schΪ�����й��
//qdisc_enqueue -> prio_enqueue ->pfifo_enqueue
static int
prio_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	struct Qdisc *qdisc;
	int ret;

    //ͨ����������ѡ��ʹ���Ǹ���֧���й�����
	qdisc = prio_classify(skb, sch, &ret);//����skb->priority�Ը����й�̽��з��࣬ѡ��ʹ�ø����й���е���һ��������qdisc
#ifdef CONFIG_NET_CLS_ACT
	if (qdisc == NULL) { //���紴�����Ƿ�����й�̣�û��Ϊ�䴴���Ӷ��й�̣��������˷�����ϢCONFIG_NET_CLS_ACT����ֱ�Ӷ���

		if (ret & __NET_XMIT_BYPASS)
			sch->qstats.drops++;
		kfree_skb(skb);
		return ret;
	}
#endif

	ret = qdisc_enqueue(skb, qdisc); //�ݹ����
	if (ret == NET_XMIT_SUCCESS) {
		sch->bstats.bytes += qdisc_pkt_len(skb);
		sch->bstats.packets++;
		sch->q.qlen++;
		return NET_XMIT_SUCCESS;
	}
	if (net_xmit_drop_count(ret))
		sch->qstats.drops++;
	return ret;
}

static struct sk_buff *prio_peek(struct Qdisc *sch)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;

	for (prio = 0; prio < q->bands; prio++) {
		struct Qdisc *qdisc = q->queues[prio];
		struct sk_buff *skb = qdisc->ops->peek(qdisc);
		if (skb)
			return skb;
	}
	return NULL;
}

//__qdisc_run -> qdisc_restart -> dequeue_skb -> prio_dequeue(�������и��ݹ���ù���) -> qdisc_dequeue_head
static struct sk_buff *prio_dequeue(struct Qdisc* sch)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;

	for (prio = 0; prio < q->bands; prio++) {
		struct Qdisc *qdisc = q->queues[prio];
		struct sk_buff *skb = qdisc->dequeue(qdisc);//һ�α�����Ҷ���ϵ�������й�̣�������Ҷ��qdisc,���ǵع���ã�ʵ��������һ�����������Ҷ�ӣ�Ȼ�����ұ�Ҷ��
		if (skb) {
			sch->q.qlen--;
			return skb;
		}
	}
	return NULL;

}

static unsigned int prio_drop(struct Qdisc* sch)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;
	unsigned int len;
	struct Qdisc *qdisc;

	for (prio = q->bands-1; prio >= 0; prio--) {
		qdisc = q->queues[prio];
		if (qdisc->ops->drop && (len = qdisc->ops->drop(qdisc)) != 0) {
			sch->q.qlen--;
			return len;
		}
	}
	return 0;
}


static void
prio_reset(struct Qdisc* sch)
{
	int prio;
	struct prio_sched_data *q = qdisc_priv(sch);

	for (prio=0; prio<q->bands; prio++)
		qdisc_reset(q->queues[prio]);
	sch->q.qlen = 0;
}

static void
prio_destroy(struct Qdisc* sch)
{
	int prio;
	struct prio_sched_data *q = qdisc_priv(sch);

	tcf_destroy_chain(&q->filter_list);
	for (prio=0; prio<q->bands; prio++)
		qdisc_destroy(q->queues[prio]);
}

static int prio_tune(struct Qdisc *sch, struct nlattr *opt)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	struct tc_prio_qopt *qopt;
	int i;

	if (nla_len(opt) < sizeof(*qopt))
		return -EINVAL;
	qopt = nla_data(opt);

	if (qopt->bands > TCQ_PRIO_BANDS || qopt->bands < 2)
		return -EINVAL;

	for (i=0; i<=TC_PRIO_MAX; i++) {
		if (qopt->priomap[i] >= qopt->bands)
			return -EINVAL;
	}

	sch_tree_lock(sch);
	q->bands = qopt->bands;
	memcpy(q->prio2band, qopt->priomap, TC_PRIO_MAX+1);

	for (i=q->bands; i<TCQ_PRIO_BANDS; i++) {
		struct Qdisc *child = q->queues[i];
		q->queues[i] = &noop_qdisc; 
		if (child != &noop_qdisc) {
			qdisc_tree_decrease_qlen(child, child->q.qlen);
			qdisc_destroy(child);
		}
	}
	sch_tree_unlock(sch);

	for (i=0; i<q->bands; i++) {
		if (q->queues[i] == &noop_qdisc) {
			struct Qdisc *child, *old; // fifo��ʽ�Ǽ򵥵����ٷ�ʽ������Qdisc ��skb������������ɵ�SKB�����������������ʱ���SKB�����еİ������ﵽ���ޣ���ֱ�Ӷ���
			child = qdisc_create_dflt(qdisc_dev(sch), sch->dev_queue,
						  &pfifo_qdisc_ops,
						  TC_H_MAKE(sch->handle, i + 1));//Ĭ���Ǵ���pfifo Qdisc���й�̣�//��ʼ��Ϊ�����Ȼ���ڴ����Ӷ��й�̵�ʱ���ֵָ���Ӧ����Qdisc�У���tc��������/TC��������ʵ�ַ�����������
			if (child) {
				sch_tree_lock(sch);
				old = q->queues[i];
				q->queues[i] = child;

				if (old != &noop_qdisc) {
					qdisc_tree_decrease_qlen(old,
								 old->q.qlen);
					qdisc_destroy(old);
				}
				sch_tree_unlock(sch);
			}
		}
	}
	return 0;
}

//���tc add qdisc��ʱ��û��ָ��ʹ�����ֶ��й涨����prio htb tbq cbf�ȣ���Ĭ�ϸ�qdisc��˽�з�����Ϣ�鴴��pfifo_fast������й涨����prio_tune
static int prio_init(struct Qdisc *sch, struct nlattr *opt) //qdisc_create�е���
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int i;

	for (i=0; i<TCQ_PRIO_BANDS; i++)
		q->queues[i] = &noop_qdisc;

	if (opt == NULL) { //���û��ָ��prio�Ĳ���band����ֱ�ӷ��ش��󣬲�������
		return -EINVAL;
	} else {
		int err;

		if ((err= prio_tune(sch, opt)) != 0)
			return err;
	}
	return 0;
}

static int prio_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned char *b = skb_tail_pointer(skb);
	struct tc_prio_qopt opt;

	opt.bands = q->bands;
	memcpy(&opt.priomap, q->prio2band, TC_PRIO_MAX+1);

	NLA_PUT(skb, TCA_OPTIONS, sizeof(opt), &opt);

	return skb->len;

nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

/*
sch:��Qdisc
arg:tc qdisc add dev eth0 parent 22:8 handle 33�е�8����ʾʹ�ø����й�̵ĵ�8Ƶ��
new:�µ�handleΪ33��qdisc
argΪͨ��22:8�е�8��prio_getѡ���ķ�����Ϣ�����е���һ��������Ϣ
*/
static int prio_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		      struct Qdisc **old)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned long band = arg - 1;

	if (new == NULL)
		new = &noop_qdisc;

	sch_tree_lock(sch);
	*old = q->queues[band];
	q->queues[band] = new;
	qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
	qdisc_reset(*old);
	sch_tree_unlock(sch);

	return 0;
}

static struct Qdisc *
prio_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned long band = arg - 1;

	return q->queues[band];
}

/* ��Qdisc  ��handle */
//tc qdisc add dev eth0 parent 22:4 handle 33 prio bands 5  pΪ22��Ӧ�Ķ��й�� qΪ33��Ӧ�Ķ��й�� ,��qdisc_graft
static unsigned long prio_get(struct Qdisc *sch, u32 classid) //prio_class_ops    �ߵ������ʱ��classid�е�A:B�е�B�϶�����0�����������ͷ��ش��ˣ���Ϊʵ���õ�ʱ��B��-1
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned long band = TC_H_MIN(classid); //��Ӧparent 22:4�е�4

//���縸Qdisc���õ�bandsƵ��Ϊ7����tc qdisc add dev eth0 parent 22:8 handle 33��22:8��ʾʹ�ø�Qdisc�ĵ�8��Ƶ��������Qdiscһ����7��Ƶ��������Խ���ˣ�����0��������qdisc_graft�б�����
	if (band - 1 >= q->bands) //��qdisc_graft���������0����ֱ�Ӵ�qdisc_graft�˳�������
		return 0;
	return band;
}

static unsigned long prio_bind(struct Qdisc *sch, unsigned long parent, u32 classid)
{
	return prio_get(sch, classid);
}


static void prio_put(struct Qdisc *q, unsigned long cl)
{
}

static int prio_dump_class(struct Qdisc *sch, unsigned long cl, struct sk_buff *skb,
			   struct tcmsg *tcm)
{
	struct prio_sched_data *q = qdisc_priv(sch);

	tcm->tcm_handle |= TC_H_MIN(cl);
	tcm->tcm_info = q->queues[cl-1]->handle;
	return 0;
}

static int prio_dump_class_stats(struct Qdisc *sch, unsigned long cl,
				 struct gnet_dump *d)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	struct Qdisc *cl_q;

	cl_q = q->queues[cl - 1];
	cl_q->qstats.qlen = cl_q->q.qlen;
	if (gnet_stats_copy_basic(d, &cl_q->bstats) < 0 ||
	    gnet_stats_copy_queue(d, &cl_q->qstats) < 0)
		return -1;

	return 0;
}

static void prio_walk(struct Qdisc *sch, struct qdisc_walker *arg)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;

	if (arg->stop)
		return;

	for (prio = 0; prio < q->bands; prio++) {
		if (arg->count < arg->skip) {
			arg->count++;
			continue;
		}
		if (arg->fn(sch, prio+1, arg) < 0) {
			arg->stop = 1;
			break;
		}
		arg->count++;
	}
}

static struct tcf_proto ** prio_find_tcf(struct Qdisc *sch, unsigned long cl)
{
	struct prio_sched_data *q = qdisc_priv(sch);

	if (cl)
		return NULL;
	return &q->filter_list;
}

//prio��Ӧprio_class_ops htb��Ӧhtb_class_ops cbq��Ӧcbq_class_ops�ȵ�
//qdisc_graft�����л�����������غ���
//skb��ӵ�ʱ��ͨ������ĺ���ѡ��ʹ��prio_sched_data -> queues[]�е��Ǹ�queues��ӣ����ӵ�ʱ��һ��
static const struct Qdisc_class_ops prio_class_ops = {
	.graft		=	prio_graft,
	.leaf		=	prio_leaf,
	.get		=	prio_get, //�ο�qdisc_graf��ѡ��ʹ��Qdisc_class_ops������Ϣ�����е��Ǹ���Ϣ��ͨ�������ȡѡ����Ĺؼ���Ϣ��Ȼ��ʹ�ø���Ϣ��graft���ʹ��
	.put		=	prio_put,
	.walk		=	prio_walk,
	.tcf_chain	=	prio_find_tcf,
	.bind_tcf	=	prio_bind,
	.unbind_tcf	=	prio_put,
	.dump		=	prio_dump_class,
	.dump_stats	=	prio_dump_class_stats,
};

/*
                                    
*/

/*pfifo_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops prio_class_ops�⼸����Ϊ���ڣ�ingress_qdisc_opsΪ��� */
static struct Qdisc_ops prio_qdisc_ops ;//__read_mostly = {          prio�ĸ���֧���ڸ����洴��class����tc_ctl_tclass
	.next		=	NULL,
	.cl_ops		=	&prio_class_ops,  //prio��������ܴ���������Ϣ������tc class add�� ��Ϊ˽�����ݲ����Ѿ���ǰ�������˷�������
	.id		=	"prio",
	.priv_size	=	sizeof(struct prio_sched_data),
	.enqueue	=	prio_enqueue, //dev_xmit_queueһֱ��ȥ����
	.dequeue	=	prio_dequeue, //dequeue_skb�е���
	.peek		=	prio_peek,
	.drop		=	prio_drop,
	.init		=	prio_init, //����Ķ��й����ڳ�ʼ����ʱ���Ĭ��ָ��noop_qdisc   ��qdisc_create�е��ã����
	.reset		=	prio_reset,
	.destroy	=	prio_destroy,
	.change		=	prio_tune,
	.dump		=	prio_dump,
	.owner		=	THIS_MODULE,
};

static int __init prio_module_init(void)
{
	return register_qdisc(&prio_qdisc_ops);
}

static void __exit prio_module_exit(void)
{
	unregister_qdisc(&prio_qdisc_ops);
}

module_init(prio_module_init)
module_exit(prio_module_exit)

MODULE_LICENSE("GPL");
