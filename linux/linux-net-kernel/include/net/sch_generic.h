#ifndef __NET_SCHED_GENERIC_H
#define __NET_SCHED_GENERIC_H

#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/module.h>
#include <linux/pkt_sched.h>
#include <linux/pkt_cls.h>
#include <net/gen_stats.h>
#include <net/rtnetlink.h>

struct Qdisc_ops;
struct qdisc_walker;
struct tcf_walker;
struct module;

// �������ʿ��Ʊ�ṹ ��һ��������Դ�����㷨
struct qdisc_rate_table { //���еĶ���ӵ�qdisc_rtab_list
	struct tc_ratespec rate;
	u32		data[256];//�ο�Ӧ�ò�tc_calc_rtable   //����õ��ľ���2047���ֽ������ĵĿ�����Դ��
	struct qdisc_rate_table *next;
	int		refcnt;
};

//qdisc->state
enum qdisc_state_t {
	__QDISC_STATE_RUNNING,//��__qdisc_run�������λ�� __QDISC_STATE_RUNNING��־���ڱ�֤һ�����ض��󲻻�ͬʱ������������С�
	__QDISC_STATE_SCHED,
	__QDISC_STATE_DEACTIVATED,
};

struct qdisc_size_table {
	struct list_head	list;
	struct tc_sizespec	szopts;
	int			refcnt;
	u16			data[];
};
/*
tc����ʹ�����������QDisc����͹��������в�����
add����һ���ڵ������һ��QDisc������߹����������ʱ����Ҫ����һ��������Ϊ���������ݲ���ʱ�ȿ���ʹ��IDҲ����ֱ�Ӵ����豸�ĸ������Ҫ����һ��QDisc���߹�����������ʹ�þ��(handle)�����������Ҫ����һ���࣬����ʹ����ʶ���(classid)��������
remove��ɾ����ĳ�����(handle)ָ����QDisc����QDisc(root)Ҳ����ɾ������ɾ��QDisc�ϵ����������Լ������ڸ�����Ĺ��������ᱻ�Զ�ɾ����
change��������ķ�ʽ�޸�ĳЩ��Ŀ�����˾��(handle)�����Ȳ����޸����⣬change������﷨��add������ͬ�����仰˵��change�����һ���ڵ��λ�á�
replace����һ�����нڵ���н���ԭ�Ӳ�����ɾ������ӡ�����ڵ㲻���ڣ��������ͻὨ���ڵ㡣
link��ֻ������DQisc�����һ�����еĽڵ㡣
tc qdisc [ add | change | replace | link ] dev DEV [ parent qdisc-id | root ] [ handle qdisc-id ] qdisc [ qdisc specific parameters ]
tc class [ add | change | replace ] dev DEV parent qdisc-id [ classid class-id ] qdisc [ qdisc specific parameters ]
tc filter [ add | change | replace ] dev DEV [ parent qdisc-id | root ] protocol protocol prio priority filtertype [ filtertype specific parameters ] flowid flow-id
tc [-s | -d ] qdisc show [ dev DEV ]
tc [-s | -d ] class show dev DEV tc filter show dev DEV

tc qdisc show dev eth0
tc class show dev eth0
*/
//tc qdisc add dev eth0 parent 22:4 handle 33�е�22:4�е�4ʵ���϶�Ӧ����Qdisc˽�����ݲ��ַ�����Ϣ�е�3,parent 22:x�е�x�Ǵ�1��ʼ�ţ����Ƕ�Ӧ�����������о�������ʱ���Ǵ�0��ʼ�ţ�����Ҫ��1������prio�ο�prio_get
//ǰ��linux�ں����ṩ���������Ƶ���ش����ܣ���ش�����net/schedĿ¼�£���Ӧ�ò��ϵĿ�����ͨ��iproute2������е�tc��ʵ�֣�tc��sched�Ĺ�ϵ�ͺ���iptables��netfilter�Ĺ�ϵһ����һ�����û���ӿڣ�һ���Ǿ���ʵ�֣�����tc��ʹ�÷������꽫Linux Advanced Routing HOWTO��������Ҫ�����ں��еľ���ʵ�֡�
//�ýṹ���ĳƺ�Ϊ:���ض���(���й涨)
//Qdisc���ٿռ�qdisc_alloc���������priv_size���ݣ���pfifo_qdisc_ops prio_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops ingress_qdisc_ops(������ض��� ) ���е�priv_size�� ͼ�λ��ο�TC��������ʵ�ַ����������� 
/*
���й�̷�Ϊ������й�̺�������˹�̣�����Ķ��й�̿��Դ�������Ӷ��й��(�����Ƿ����Ҳ����������Ķ��й��)�����ֻ����һ��������й�̾��൱��һ��Ҷ�ӹ��
��SKBֱ����ӵ��ö��й�̵�skb�����С�����Ǵ���һ������Ķ��й�̣����һ�������Ķ��й�̾��Ǹ���������԰�������Ӷ��й�̣������Է�����й�̱����ж�Ӧ
��Ҷ��������й�̣���Ϊ������й��������û��skb���еġ�
��һ��SKB��������й�̵ĸ���ʱ�򣬸�ѡ���������Ӷ��й�������? ����ǹ����������ã�����������ͨ��IP MASK����Ϣ��ȷ�����Ǹ��Ӷ��й�̷�֧�����û������
����������һ�����skb->priority��ȷ�����Ǹ���֧��
tc qdisc add dev eth0 root handle 1: htb ���������й�� (�ڴ����������̵�ʱ��һ��Ĭ���ǻ����Զ��й�̵ģ�����pfifo������)
tc class add dev eth0 parent 1: classid 1:2 htb xxxx  ��1:���й������ĵ�1:2��֧�ϣ���htb����һ����������й��htb��������xxx��ָ��htb�Ĳ�����Ϣ
tc class add dev eth0 parent 1: classid 1:1 htb xxxx  ��1:���й������ĵ�1:1��֧�ϣ���htb����һ����������й��htb��������xxx��ָ��htb�Ĳ�����Ϣ
tc filter add dev eth0 protocol ip parent 1: prio 2 u32 match ip dst 4.3.2.1/32 flowid 1:2 ����յ�����ip��ַΪ4.3.2.1��SKB���������Ӷ��й��1:2��ӣ���������1:1�������
*/ //��õ�Դ�����ο�<<linux�ں�����������>>
struct Qdisc { /* �ο� TC��������ʵ�ַ�����������*/ //prio_sched_data�е�queuesָ���Qdisc              #ע�������е�ID(parent 1:2 xxx flowid 3:3)�����������Ϊ16���Ƶ���
//qdisc_alloc��������struct Qdisc�ṹ�����˽������Ϊpfifo_qdisc_ops prio_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops ingress_qdisc_ops�е�priv_size����
    //enqueue��dequeue�ĸ�ֵ��qdisc_alloc
	int 			(*enqueue)(struct sk_buff *skb, struct Qdisc *dev); /* ��ӽӿ� */
	struct sk_buff *	(*dequeue)(struct Qdisc *dev);  /* ���Խӿ� */
	unsigned		flags; //�Ŷӹ����־��ȡֵΪ�����⼸�ֺ궨��  TCQ_F_THROTTLED
#define TCQ_F_BUILTIN		1 //��ʾ�Ŷӹ����ǿյ��Ŷӹ�����ɾ���ͷ�ʱ����Ҫ���������Դ�ͷ�
#define TCQ_F_THROTTLED		2 //��ʶ�Ŷӹ����������������ƶ���ʱ���ӵ�״̬�� 
#define TCQ_F_INGRESS		4 //��ʾ�Ŷӹ���Ϊ�����Ŷӹ���
#define TCQ_F_CAN_BYPASS	8
#define TCQ_F_MQROOT		16
#define TCQ_F_WARN_NONWC	(1 << 16)// ��Ϊ�Ѿ���ӡ�˾�����Ϣ�ı�־
    /*
    �����Ŷӹ�����ڴ���Ҫ32�ֽڶ��룬��ͨ����̬����õ����ڴ���ʼ��ַ��һ����32�ֽ�
    ���룬�����Ҫͨ����佫���й�����뵽32�ֽڴ���
    */
	int			padded;

	/*pfifo_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops�⼸����Ϊ���ڣ�ingress_qdisc_opsΪ��� */
	struct Qdisc_ops	*ops;//prio���й���opsΪpfifo_qdisc_ops����������prio_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops ingress_qdisc_ops(������ض��� ) �ȣ� 
	struct qdisc_size_table	*stab;
	struct list_head	list;//���ӵ������õ������豸��

	/*�Ŷӹ���ʵ���ı�ʶ��Ϊ����Ų��ֺ͸���Ų��֣���������Ų������û����䣬��Χ��
	0X0001��0X7FFFF������û�ָ�������Ϊ0����ô�ں˽���0X8000��0XFFFF֮�����һ�������
	��ʶ�ڵ��������豸��Ψһ�ģ����ڶ�������豸֮��������ظ�*/
	u32			handle; //��Qdisc�ľ����tc qdisc add dev eth0 root handle 22�е�22
	u32			parent;//�����й���ľ��ֵ  tc qdisc add dev eth0 parent 22:4 handle 33 ��handleΪ33 parentΪ22
	atomic_t		refcnt;//���ü���
	struct gnet_stats_rate_est	rate_est;//���е�ǰ�����ʣ��������ֽںͱ�����Ϊ��λ����

    /*����ʵ�ָ����ӵ��������ƻ��ƣ������Ŷӹ����ʵ�ִ˽ӿڡ���һ���ⲿ�������ڲ�����
    ���ݱ���ʱ�����ܳ��ֱ��ı��뱻������������統û�п��û�����ʱ������Ŷӹ���ʵ���˸ûص�
    ��������ô��ʱ�Ϳ��Ա��ڲ��Ŷӹ������*/
	int			(*reshape_fail)(struct sk_buff *skb,
					struct Qdisc *q);

	void			*u32_node;//ָ��tc_u_common����u32_init  ָ�����ָ�����й�̵ĵ�һ��u32������

	/* This field is deprecated, but it is still used by CBQ
	 * and it will live until better solution will be invented.
	 */
	struct Qdisc		*__parent;
	struct netdev_queue	*dev_queue;
	struct Qdisc		*next_sched;

	struct sk_buff		*gso_skb;
	/*
	 * For performance sake on SMP, we put highly modified fields at the end
	 */
	unsigned long		state;
	struct sk_buff_head	q; //SKB������ӵ��ö����е�  pfifo����ӵ�ʱ��ֱ�Ӽ����skb���������ǵ��͵��Ƚ��ȳ�
	struct gnet_stats_basic_packed bstats;//��¼��ӱ������ֽ�������ӱ�������
	struct gnet_stats_queue	qstats;//��¼�������ͳ������
	struct rcu_head     rcu_head;//ͨ�����ֽ���û�ж�����ʹ�ø��Ŷӹ���ʱ�ͷŸ��Ŷӹ���
};

/*
//����Ķ��й涨������prio cbq htb����Щ���й���Qdisc�����Ӧһ����ӿڣ����������Ķ��й涨����û�и�������ӿ�
//prio��Ӧprio_class_ops htb��Ӧhtb_class_ops cbq��Ӧcbq_class_ops�ȵ�

//������й��Qdisc ops�е�Qdisc_class_ops��Ҫ���ڴ�����Qdisc��ʱ�򣬰���parent 22:4�е�22:4�Ը�Qdisc���з��࣬�Ӷ�ͨ��22:4��Ϊ������
//ѡ������QdiscӦ�üӵ��Ǹ�����Qdisc���档���Բο�prio_qdisc_ops�е�prio_get��prio_graft���ͺܺ�������
*/ //�����Ӷ��й������class��ʱ�򣬸ýṹ�����þ���ͨ��parent 22:8�е�8��prio_get(��prio������й��Ϊ��)ѡ����prize_size˽�����ݲ��������е���һ��������Ϣ��
struct Qdisc_class_ops { //��Ҫ��qdisc_graftִ���������غ���       ���Բο�prio_qdisc_ops����prioΪ��        tc_ctl_tclass
	/* Child qdisc manipulation */
	struct netdev_queue *	(*select_queue)(struct Qdisc *, struct tcmsg *);

	//����qdisc_graft�е���
	int			(*graft)(struct Qdisc *, unsigned long cl,
					struct Qdisc *, struct Qdisc **);�//���ڽ�һ�����й���Qdisc�󶨵�һ���࣬��������ǰ�󶨵������Ķ��й���
    //��ȡ��ǰ�󶨵�������Ķ��й���
	struct Qdisc *		(*leaf)(struct Qdisc *, unsigned long cl);

	//������Ӧ���г��ȱ仯
	void			(*qlen_notify)(struct Qdisc *, unsigned long);

	/* Class manipulation routines */
    //���ݸ���������������Ŷӹ����в��Ҷ�Ӧ���࣬�����ø��࣬��������ü�������
    //��ʾʹ�ö��й������ĵڼ���������Ϣ��һ��������й�����涼���кü���������Ϣ��ͨ��classid������ѡһ��������prio������ͨ��prio_get��ȡ����Ƶ���еĵڼ���Ƶ��
    //���ݸú�����ȷ��ʹ�ø�Qdisc���Ǹ��࣬�ж�����Ϊtc qdisc add dev eth0 parent 22:4 handle 33�е�22:4,��prio������й��Ϊ������prio_get
	unsigned long		(*get)(struct Qdisc *, u32 classid); //ͨ��qdisc_graft����
    //�ݼ�ָ��������ü�����������ü���Ϊ0����ɾ���ͷŴ��ࡣ
	void			(*put)(struct Qdisc *, unsigned long); //����qdisc_graft�е���
    //���ڱ��ָ����Ĳ�����������಻�������½�֮��
	int			(*change)(struct Qdisc *, u32, u32,
					struct nlattr **, unsigned long *);
    //����ɾ�����ͷ�ָ�����ࡣ���Ȼ�ݼ���������ü�����������ü����ݼ���Ϊ0��ɾ���ͷ�֮��
	int			(*delete)(struct Qdisc *, unsigned long);
    //����һ���Ŷӹ���������࣬ȡ��ʵ���˻ص���������������ݼ�ͳ����Ϣ
	void			(*walk)(struct Qdisc *, struct qdisc_walker * arg);

	/* Filter manipulation */
	//��ȡ�󶨵�����Ĺ���������������׽ڵ�
	struct tcf_proto **	(*tcf_chain)(struct Qdisc *, unsigned long);

    //��һ����������׼���󶨵�ָ������֮ǰ�����ã�ͨ�����ʶ����ȡ�࣬���ȵ������ü�����Ȼ����һЩ�����ļ��
	unsigned long		(*bind_tcf)(struct Qdisc *, unsigned long,
					u32 classid); //��tcf_bind_filter

    //�ڹ�������ɰ󶨵�ָ������󱻵��ã��ݼ������ü���
    void			(*unbind_tcf)(struct Qdisc *, unsigned long);

	/* rtnetlink specific */
	int			(*dump)(struct Qdisc *, unsigned long,
					struct sk_buff *skb, struct tcmsg*);
	int			(*dump_stats)(struct Qdisc *, unsigned long,
					struct gnet_dump *);
};

//���е�Qdisc_ops�ṹͨ��register_qdisc��ӵ�qdisc_base������
//Qdisc�е�opsָ������              /*pfifo_fast_ops pfifo_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops prio_class_ops�⼸����Ϊ���ڣ�ingress_qdisc_opsΪ��� */
struct Qdisc_ops { //prio���й���opsΪpfifo_qdisc_ops����������tbf_qdisc_ops sfq_qdisc_ops�ȣ� 
	struct Qdisc_ops	*next;//ָ����һ��Qdisc_ops
    //���й����ṩ��������ӿڡ�
	const struct Qdisc_class_ops	*cl_ops; //����Ķ���pfifo bfifo����û��class����ops��
	char			id[IFNAMSIZ]; //�Ŷӹ�����
	//�������Ŷӹ����ϵ�˽����Ϣ���С������Ϣ��ͨ�����Ŷӹ���һ������ڴ棬�������Ŷ�
	//������棬����qdisc_priv��ȡ�� 
	int			priv_size; //�������˽�����ݴ�С Qdisc_alloc����Qdisc�ռ��ʱ���࿪��priv_size�ռ�

//enqueue����ֵNET_XMIT_SUCCESS��
	int 			(*enqueue)(struct sk_buff *, struct Qdisc *); //���õط�qdisc_enqueue   //dev_xmit_queueһֱ��ȥ����
//����ǰ���ӵı����������뵽�����еĺ�������ͬ��enqueue���ǣ�������ӵı�����Ҫ����������
//����ǰ���Ŷӹ��������������λ���ϡ��ýӿ�ͨ�����ڱ���Ҫ���ͳ�ȥ����dequeue���Ӻ���ĳ������Ԥ����ԭ������δ�ܷ��͵������
	struct sk_buff *	(*dequeue)(struct Qdisc *);//dequeue_skb�е���
	struct sk_buff *	(*peek)(struct Qdisc *);

	//�Ӷ����Ƴ�������һ�����ĵĺ���
	unsigned int		(*drop)(struct Qdisc *);

    //��qdisc_create�е���
	int			(*init)(struct Qdisc *, struct nlattr *arg); //�����ʼ������  //����Ķ��й����ڳ�ʼ����ʱ���Ĭ��ָ��noop_qdisc������prio_qdisc_ops�е�init
	void			(*reset)(struct Qdisc *); //��λΪ��ʼ״̬��ɾ����ʱ�� �ͷſռ��
	void			(*destroy)(struct Qdisc *);
	int			(*change)(struct Qdisc *, struct nlattr *arg); //����Qdisc����
	void			(*attach)(struct Qdisc *);


	int			(*dump)(struct Qdisc *, struct sk_buff *);
    //��������Ŷӹ�������ò�����ͳ�����ݵĺ�����
   	int			(*dump_stats)(struct Qdisc *, struct gnet_dump *);

	struct module		*owner;
};

//ͨ������SKB�е�������ƥ�������tc filter��ƥ�����浽�ýṹ�С�Ҳ����ֱ�ӻ�ȡ�ù���������class��(tc add class��ʱ�򴴽���class���ڵ�)htb_class
struct tcf_result {
	unsigned long	class; //���ʵ������һ��ָ���ַ��ָ�����tc filter add xxxx flowid 22:4��Ӧ��htb_class�ṹ����tcf_bind_filter
	u32		classid;//��u32_set_parms����ֵΪ//tc filter add dev eth0 protocol ip parent 22: prio 2 u32 match ip dst 4.3.2.1/32 flowid 22:4�е�flowid����ʾ�ù����������Ǹ����й�����ڵ�
};

//tcf_proto�е�ops�����е�tcf_proto_opsͨ��tcf_proto_base������һ�𣬼�register_tcf_proto_ops
//��Ҫ��cls_u32_ops cls_basic_ops  cls_cgroup_ops  cls_flow_ops cls_route4_ops RSVP_OPS
struct tcf_proto_ops {
	struct tcf_proto_ops	*next; //��������ע����������ӵ�tcf_proto_base�����ϵ�ָ��
	char			kind[IFNAMSIZ];//���������� 

	int			(*classify)(struct sk_buff*, struct tcf_proto*,
					struct tcf_result *); //���ຯ�������������tcf_result�У�����ֵ��TC_POLICE_OK��
	int			(*init)(struct tcf_proto*); //tc_ctl_tclass�е���

    //�ͷŲ�ɾ������������
	void			(*destroy)(struct tcf_proto*);

    //��һ��������Ԫ�صľ��ӳ�䵽һ���ڲ���������ʶ����ʵ�����ǹ�����ʵ��ָ�룬�����䷵��
	unsigned long		(*get)(struct tcf_proto*, u32 handle); //��ȡ��Ӧ�Ĺ�����
    //�ͷŶ�get�õ��Ĺ�����������
	void			(*put)(struct tcf_proto*, unsigned long);
	//��������һ���¹��������Ǳ��һ���Ѵ��ڵĹ��������á�
	int			(*change)(struct tcf_proto*, unsigned long,
					u32 handle, struct nlattr **,
					unsigned long *);
	int			(*delete)(struct tcf_proto*, unsigned long);
    //�������е�Ԫ�ز��ҵ��ûص�����ȡ���������ݺ�ͳ������
	void			(*walk)(struct tcf_proto*, struct tcf_walker *arg);

	/* rtnetlink specific */  //����������е�Ԫ�ز��ҵ��ûص�����ȡ���������ݺ�ͳ������
	int			(*dump)(struct tcf_proto*, unsigned long,
					struct sk_buff *skb, struct tcmsg*);

	struct module		*owner;
};
/* ���ȼ����й涨��bandΪ16��,�ο�TC��������ʵ�ַ���(����)-ͼ3  ������prio�����͵ĸ����ض���_2 */   //��ϸ���Ҳ���Բο�<<LINUX�߼�·�ɺ���������>>
//tc filter add dev eth0 protocol ip parent 22: prio 2 u32 match ip dst 4.3.2.1/32 flowid 22:4
/*�������ݰ�������������£�
1.      ������Ĺ��������ǿգ�����������Ĺ���������������һ��ƥ��Ĺ������ͷ��أ������ݷ��صĽ��ѡ�����ࡣ
2.      ÿ����������������Ӧ�ķ��ຯ���������ݹ�������˽��������ƥ�����ݰ���
*/
//tc filter u32�������Ľṹ    ������������tc_ctl_tfilter�У����ڸú����г�ʼ��
struct tcf_proto { //�ýṹ�Ǽ��뵽prio_sched_data�е�filter_list������  ÿ����һ��tc filter add�ͻᴴ��һ��tcf_proto�ṹ�����ö��tc filter add��ʱ��ʹ������tcf_proto�ṹ��ͨ��next����
	/* Fast access part */ //tcfһ���ʾtcf_proto�������ļ�д
	struct tcf_proto	*next;
	void			*root; //���Ϊu32���ͣ�ָ���������tc_u_hnode�� ��u32_init���ù��������������tc_u_common�ڵ㶼��ӵ���tc_u_hnode����
	int			(*classify)(struct sk_buff*, struct tcf_proto*,
					struct tcf_result *); //���ຯ�������������tcf_result�С�ͨ��SKB�е����ݣ���ƥ�������������������ظ�tcf_result����tc_classify_compat
	__be16			protocol; //Э��ţ�//tc filter add dev eth0 protocol ip��protocol ip��Ӧ��������ETH_P_IP

	/* All the rest */
	u32			prio; //����������ȼ����뵽prio_sched_data�е�filter_list�����С�tc filter add dev eth0 protocol ip parent 22: prio 2Ϊ2
	u32			classid; //ָ����Qdisc�е�����λ��=22:4
	struct Qdisc		*q; //��Qdisc,���Ǹù����������Ķ��й���ڵ���ϼ���Qdisc
	void			*data; //�����󴴽���u32���͹������ڵ�tc_u_common����u32_init
	struct tcf_proto_ops	*ops; //cls_u32_ops //��Ҫ��cls_u32_ops cls_basic_ops  cls_cgroup_ops  cls_flow_ops cls_route4_ops RSVP_OPS
};

struct qdisc_skb_cb {
	unsigned int		pkt_len;//��qdisc_enqueue_root������ӵ�ʱ�򣬸�ֵΪSKB->len
	char			data[];
};

static inline int qdisc_qlen(struct Qdisc *q)
{
	return q->q.qlen;
}

static inline struct qdisc_skb_cb *qdisc_skb_cb(struct sk_buff *skb)
{
	return (struct qdisc_skb_cb *)skb->cb;
}

static inline spinlock_t *qdisc_lock(struct Qdisc *qdisc)
{
	return &qdisc->q.lock;
}

static inline struct Qdisc *qdisc_root(struct Qdisc *qdisc)
{
	return qdisc->dev_queue->qdisc;
}

static inline struct Qdisc *qdisc_root_sleeping(struct Qdisc *qdisc)
{
	return qdisc->dev_queue->qdisc_sleeping;
}

/* The qdisc root lock is a mechanism by which to top level
 * of a qdisc tree can be locked from any qdisc node in the
 * forest.  This allows changing the configuration of some
 * aspect of the qdisc tree while blocking out asynchronous
 * qdisc access in the packet processing paths.
 *
 * It is only legal to do this when the root will not change
 * on us.  Otherwise we'll potentially lock the wrong qdisc
 * root.  This is enforced by holding the RTNL semaphore, which
 * all users of this lock accessor must do.
 */
static inline spinlock_t *qdisc_root_lock(struct Qdisc *qdisc)
{
	struct Qdisc *root = qdisc_root(qdisc);

	ASSERT_RTNL();
	return qdisc_lock(root);
}

static inline spinlock_t *qdisc_root_sleeping_lock(struct Qdisc *qdisc)
{
	struct Qdisc *root = qdisc_root_sleeping(qdisc);

	ASSERT_RTNL();
	return qdisc_lock(root);
}

static inline struct net_device *qdisc_dev(struct Qdisc *qdisc)
{
	return qdisc->dev_queue->dev;
}

static inline void sch_tree_lock(struct Qdisc *q)
{
	spin_lock_bh(qdisc_root_sleeping_lock(q));
}

static inline void sch_tree_unlock(struct Qdisc *q)
{
	spin_unlock_bh(qdisc_root_sleeping_lock(q));
}

#define tcf_tree_lock(tp)	sch_tree_lock((tp)->q)
#define tcf_tree_unlock(tp)	sch_tree_unlock((tp)->q)

extern struct Qdisc noop_qdisc;
extern struct Qdisc_ops noop_qdisc_ops;
extern struct Qdisc_ops pfifo_fast_ops;
extern struct Qdisc_ops mq_qdisc_ops;

//�ýṹΪhtb_class -> common
struct Qdisc_class_common {//�����Qdisc_class_hash��, ����class��qdisc_class_find
	u32			classid;// ���IDֵ, ��16λ�������ֲ�ͬ��HTB����, ��16λΪ����ͬһHTB�����еĲ�ͬ���
	struct hlist_node	hnode; //ͨ�����hnode���հ�htb_class���뵽htb_sched->clhash�У���htb_change_class -> qdisc_class_hash_insert
};

//�ýṹΪhtb˽������htb_sched�е�clhash�������洢����tc class add������htb_class
struct Qdisc_class_hash { //hash���̼�qdisc_class_hash_grow
	struct hlist_head	*hash;//�������д�ŵ���Qdisc_class_common,��hash��ռ���qdisc_class_hash_init����     qdisc_class_find
	unsigned int		hashsize; //Ĭ�ϳ�ʼֵ��qdisc_class_hash_init�����hash�ڵ���hashelems�������õ�hashsize��0.75�������hash��hashsize����֮ǰhashsize��������qdisc_class_hash_grow
	unsigned int		hashmask;  //qdisc_class_hash_init
	unsigned int		hashelems; //ʵ�ʵ�hash class�ڵ��� //hashelems��hashsize��ϵ��qdisc_class_hash_grow
};

static inline unsigned int qdisc_class_hash(u32 id, u32 mask)
{
	id ^= id >> 8;
	id ^= id >> 4;
	return id & mask;
}

//����
static inline struct Qdisc_class_common *
qdisc_class_find(struct Qdisc_class_hash *hash, u32 id)
{
	struct Qdisc_class_common *cl;
	struct hlist_node *n;
	unsigned int h;

	h = qdisc_class_hash(id, hash->hashmask);
	hlist_for_each_entry(cl, n, &hash->hash[h], hnode) {// ���ݾ�������ϣֵ, Ȼ������ù�ϣ����
		if (cl->classid == id)
			return cl;
	}
	return NULL;
}

extern int qdisc_class_hash_init(struct Qdisc_class_hash *);
extern void qdisc_class_hash_insert(struct Qdisc_class_hash *, struct Qdisc_class_common *);
extern void qdisc_class_hash_remove(struct Qdisc_class_hash *, struct Qdisc_class_common *);
extern void qdisc_class_hash_grow(struct Qdisc *, struct Qdisc_class_hash *);
extern void qdisc_class_hash_destroy(struct Qdisc_class_hash *);

extern void dev_init_scheduler(struct net_device *dev);
extern void dev_shutdown(struct net_device *dev);
extern void dev_activate(struct net_device *dev);
extern void dev_deactivate(struct net_device *dev);
extern struct Qdisc *dev_graft_qdisc(struct netdev_queue *dev_queue,
				     struct Qdisc *qdisc);
extern void qdisc_reset(struct Qdisc *qdisc);
extern void qdisc_destroy(struct Qdisc *qdisc);
extern void qdisc_tree_decrease_qlen(struct Qdisc *qdisc, unsigned int n);
extern struct Qdisc *qdisc_alloc(struct netdev_queue *dev_queue,
				 struct Qdisc_ops *ops);
extern struct Qdisc *qdisc_create_dflt(struct net_device *dev,
				       struct netdev_queue *dev_queue,
				       struct Qdisc_ops *ops, u32 parentid);
extern void qdisc_calculate_pkt_len(struct sk_buff *skb,
				   struct qdisc_size_table *stab);
extern void tcf_destroy(struct tcf_proto *tp);
extern void tcf_destroy_chain(struct tcf_proto **fl);

/* Reset all TX qdiscs greater then index of a device.  */
static inline void qdisc_reset_all_tx_gt(struct net_device *dev, unsigned int i)
{
	struct Qdisc *qdisc;

	for (; i < dev->num_tx_queues; i++) {
		qdisc = netdev_get_tx_queue(dev, i)->qdisc;
		if (qdisc) {
			spin_lock_bh(qdisc_lock(qdisc));
			qdisc_reset(qdisc);
			spin_unlock_bh(qdisc_lock(qdisc));
		}
	}
}

static inline void qdisc_reset_all_tx(struct net_device *dev)
{
	qdisc_reset_all_tx_gt(dev, 0);
}

/* Are all TX queues of the device empty?  */
static inline bool qdisc_all_tx_empty(const struct net_device *dev)
{
	unsigned int i;
	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *txq = netdev_get_tx_queue(dev, i);
		const struct Qdisc *q = txq->qdisc;

		if (q->q.qlen)
			return false;
	}
	return true;
}

/* Are any of the TX qdiscs changing?  */
static inline bool qdisc_tx_changing(struct net_device *dev)
{
	unsigned int i;
	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *txq = netdev_get_tx_queue(dev, i);
		if (txq->qdisc != txq->qdisc_sleeping)
			return true;
	}
	return false;
}

/* Is the device using the noop qdisc on all queues?  */
static inline bool qdisc_tx_is_noop(const struct net_device *dev)
{
	unsigned int i;
	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *txq = netdev_get_tx_queue(dev, i);
		if (txq->qdisc != &noop_qdisc)
			return false;
	}
	return true;
}

static inline unsigned int qdisc_pkt_len(struct sk_buff *skb)
{
	return qdisc_skb_cb(skb)->pkt_len;
}

/* additional qdisc xmit flags (NET_XMIT_MASK in linux/netdevice.h) */
enum net_xmit_qdisc_t {
	__NET_XMIT_STOLEN = 0x00010000,
	__NET_XMIT_BYPASS = 0x00020000,
};

#ifdef CONFIG_NET_CLS_ACT
#define net_xmit_drop_count(e)	((e) & __NET_XMIT_STOLEN ? 0 : 1)
#else
#define net_xmit_drop_count(e)	(1)
#endif


static inline int qdisc_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
#ifdef CONFIG_NET_SCHED
	if (sch->stab)
		qdisc_calculate_pkt_len(skb, sch->stab);
#endif
	return sch->enqueue(skb, sch);///*prio_qdisc_ops pfifo_qdisc_ops tbf_qdisc_ops sfq_qdisc_ops�⼸����Ϊ���ڣ�ingress_qdisc_opsΪ��� */
}

//ingressͨ��ing_filter���
static inline int qdisc_enqueue_root(struct sk_buff *skb, struct Qdisc *sch) //sch dev�豸��qdisc
{
	qdisc_skb_cb(skb)->pkt_len = skb->len;
	return qdisc_enqueue(skb, sch) & NET_XMIT_MASK;
}

static inline void __qdisc_update_bstats(struct Qdisc *sch, unsigned int len)
{
	sch->bstats.bytes += len;
	sch->bstats.packets++;
}

static inline int __qdisc_enqueue_tail(struct sk_buff *skb, struct Qdisc *sch,
				       struct sk_buff_head *list)
{
	__skb_queue_tail(list, skb);
	sch->qstats.backlog += qdisc_pkt_len(skb);
	__qdisc_update_bstats(sch, qdisc_pkt_len(skb));

	return NET_XMIT_SUCCESS;
}

static inline int qdisc_enqueue_tail(struct sk_buff *skb, struct Qdisc *sch)
{
	return __qdisc_enqueue_tail(skb, sch, &sch->q);
}

static inline struct sk_buff *__qdisc_dequeue_head(struct Qdisc *sch,
						   struct sk_buff_head *list)
{
	struct sk_buff *skb = __skb_dequeue(list);

	if (likely(skb != NULL))
		sch->qstats.backlog -= qdisc_pkt_len(skb);

	return skb;
}

//__qdisc_run -> qdisc_restart -> dequeue_skb -> prio_dequeue(�������и��ݹ���ù���) -> qdisc_dequeue_head
static inline struct sk_buff *qdisc_dequeue_head(struct Qdisc *sch)
{
	return __qdisc_dequeue_head(sch, &sch->q);
}

static inline unsigned int __qdisc_queue_drop_head(struct Qdisc *sch,
					      struct sk_buff_head *list)
{
	struct sk_buff *skb = __qdisc_dequeue_head(sch, list);

	if (likely(skb != NULL)) {
		unsigned int len = qdisc_pkt_len(skb);
		kfree_skb(skb);
		return len;
	}

	return 0;
}

static inline unsigned int qdisc_queue_drop_head(struct Qdisc *sch)
{
	return __qdisc_queue_drop_head(sch, &sch->q);
}

static inline struct sk_buff *__qdisc_dequeue_tail(struct Qdisc *sch,
						   struct sk_buff_head *list)
{
	struct sk_buff *skb = __skb_dequeue_tail(list);

	if (likely(skb != NULL))
		sch->qstats.backlog -= qdisc_pkt_len(skb);

	return skb;
}

static inline struct sk_buff *qdisc_dequeue_tail(struct Qdisc *sch)
{
	return __qdisc_dequeue_tail(sch, &sch->q);
}

static inline struct sk_buff *qdisc_peek_head(struct Qdisc *sch)
{
	return skb_peek(&sch->q);
}

/* generic pseudo peek method for non-work-conserving qdisc */
static inline struct sk_buff *qdisc_peek_dequeued(struct Qdisc *sch)
{
	/* we can reuse ->gso_skb because peek isn't called for root qdiscs */
	if (!sch->gso_skb) {
		sch->gso_skb = sch->dequeue(sch);
		if (sch->gso_skb)
			/* it's still part of the queue */
			sch->q.qlen++;
	}

	return sch->gso_skb;
}

/* use instead of qdisc->dequeue() for all qdiscs queried with ->peek() */
static inline struct sk_buff *qdisc_dequeue_peeked(struct Qdisc *sch)
{
	struct sk_buff *skb = sch->gso_skb;

	if (skb) {
		sch->gso_skb = NULL;
		sch->q.qlen--;
	} else {
		skb = sch->dequeue(sch);
	}

	return skb;
}

static inline void __qdisc_reset_queue(struct Qdisc *sch,
				       struct sk_buff_head *list)
{
	/*
	 * We do not know the backlog in bytes of this list, it
	 * is up to the caller to correct it
	 */
	__skb_queue_purge(list);
}

static inline void qdisc_reset_queue(struct Qdisc *sch)
{
	__qdisc_reset_queue(sch, &sch->q);
	sch->qstats.backlog = 0;
}

static inline unsigned int __qdisc_queue_drop(struct Qdisc *sch,
					      struct sk_buff_head *list)
{
	struct sk_buff *skb = __qdisc_dequeue_tail(sch, list);

	if (likely(skb != NULL)) {
		unsigned int len = qdisc_pkt_len(skb);
		kfree_skb(skb);
		return len;
	}

	return 0;
}

//����qdisc�Ŷӹ��skb�����ϵ�����
static inline unsigned int qdisc_queue_drop(struct Qdisc *sch)
{
	return __qdisc_queue_drop(sch, &sch->q);
}

static inline int qdisc_drop(struct sk_buff *skb, struct Qdisc *sch)
{
	kfree_skb(skb);
	sch->qstats.drops++;

	return NET_XMIT_DROP;
}

static inline int qdisc_reshape_fail(struct sk_buff *skb, struct Qdisc *sch)
{
	sch->qstats.drops++;

#ifdef CONFIG_NET_CLS_ACT
	if (sch->reshape_fail == NULL || sch->reshape_fail(skb, sch))
		goto drop;

	return NET_XMIT_SUCCESS;

drop:
#endif
	kfree_skb(skb);
	return NET_XMIT_DROP;
}

/*
嫺����� ˵

�Ǹ���Ϻ������һ�½ṹ
qdisc_rate_table{
  struct tc_ratespec rate;
  u32 data[256];
  struct qdisc_rate_table *next;
  int refcnt;
}
����ڿ�����Դ�룬����tbfʱ�����ǿ���������ط���
���о���
    qdisc_l2t()����ô��İ�����ô���ǿ������أ�
ϣ�����Ĵ�Ϻ������һ�£�С����лл��
emmoblin ˵

�Ǻǣ�����ҿ����൱һ��ʱ��ſ����ס�

����ṹ��Ҫ���������ں˼�������ʱ�õġ�
������⣬�������е�˵�����ס�
�ں˵���С���ȵ�λ��һ��tick�������ں�Ҫ������ʱ��ת��Ϊ�ں˵�tickʱ�䡣
���ںú����һ�£����൱����һ�����ʣ�����ʱ���100ms��ת�����ں�tickʱ����Ҫ��һ��ϵ���ġ�



��һ��������Դ�����㷨
�㷨��������λʱ���ڲ����Ŀ�����Դһ����ÿ����һ���ֽڶ�Ҫ������Ӧ��С�Ŀ�����Դ����������Դ����ʱֹͣ�������ݰ����趨������Խ��
����һ���ֽ������ĵĿ�����Դ��ԽС��ͨ�����÷���һ���ֽ������ĵĿ�����Դ���������ٿ��ơ�

��������:

1. ������Դ������һ�����ݰ����������Ŀ�����Դ�����ĳ������Ŀ�����ԴΪ0�����޷��������ݰ���ֻҪ������Դ�㹻��Ϳ��Է������ݰ���
(TC�û��ռ����ÿ������Ŀ�����Դ��TIME_UNITS_PER_SEC       1000000����TC�ں˸��ݿ���ʱ�������������Դ��)

2.����ʱ�䣺����������һ�η������ݰ���ʱ����T1��ϵͳ��ǰ��ʱ����T2�������ʱ��tk = T1 �C T2��

2. ����rate��ÿ�������͵ĵ��ֽڸ�����
3. ������Դ���������Կ���ʱ��Ϊ��������һ�����㷨�õ���ֵ��������Խ�����ʱ�����һ��������������Ҫ��֤����ʱ��Խ�󣬶�Ӧ�Ŀ�����Դ�Ļ������ض�ҪԽ��
4. ������Դʣ���������һ�η������ݰ��Ժ󣬿�����Դ��ʣ������
5. ��ǰ���ÿ�����Դ���Կ�����Դ��ʣ�����Ϳ�����Դ�Ļ�����Ϊ��������һ�����㷨�õ���ֵ��������� = 1/6������Դ��ʣ���� + (1 �C 1/6)������Դ�Ļ��ۣ���
����Ҫ��֤��ǰ���ÿ�����Դ���ǿ�����Դʣ�����Ϳ�����Դ�������ĵ���������

Ϊ�˸��õ���������Դ�����㷨����Ҫ�������ٸ���ĵڶ���������Ҳ���ǣ�ʹ�ÿ�����Դ���������ٵĸ��
 

6.����kc(�ÿ�����Դ����)������ÿ������Ŀ�����Դ��TIME_UNITS_PER_SEC������rate(ÿ�������͵���������rate���ֽ�)������һ���ֽڵ�������Ҫ���ĵ�
������Դ��kc = TIME_UNITS_PER_SEC/rate
�����kc�����������������������������rateԽ��kc��ԽС��

���Ҫ����size�ֽڵ����ݰ���Ҫ����size*(TIME_UNITS_PER_SEC/rate)�Ŀ�����Դ��

ֻҪ������Դ�㹻�࣬�Ϳ��Է������ݰ���ÿ����һ�����ݰ���������Դ��ȥ��Ӧ����������

ֻҪ����ʱ��һֱ�ۻ���������Դ�����úܴ���ʱ��ʧȥ�˵������ٵ����壬����������������Դ����ʹ������Դ����̫��

�������ٵĹ��̣�
����ֻҪ������Դ���㣬����ͼ����һ��������L�����ݰ���������kc��
1.      ��ʼʱ�̿�����Դ�Ϳ���ʱ�䶼Ϊ0����Ȼ�����������ݰ���
2.      ����һ��ʱ�䣬����ʱ�����0�����������Դ�ۻ����������㵱ǰ���ÿ�����Դtu��
3.      ����L���ȵ����ݰ���Ҫ����kc*L�Ŀ�����Դ�����tu > a*L���������ݰ�������������һ��ʱ�䡣
4.      �������ݰ�����ٿ�����Դ��tu = tu �C a*L�����tu > 0���ظ�3�Ĺ��̣�ֱ���ٴ����ߡ�
5.      �������״̬�ǣ����ǳ���ts = a*L��

������ʱ���Դﵽ���ص�Ŀ�ģ����ǽ���ǲ�׼ȷ�ģ���ͬ���㷨����ͬ�Ĳ������ڲ�ͬ�����绷������Ҫ��Ӳ�������ò�ͬ�������صĽ���϶���ͬ��
���ǿ��Ը��ݾ�������绷������ѡ���ʵ��Ĳ���������㷨��׼ȷ�ȡ�
���Ե����Ĳ��������ࣺ1. �㷨������2. ���ò�����
�ɵ����㷨�����У�1. ����ʱ��Ϳ�����Դ�Ļ������ 2. ÿ��ɲ����Ŀ�����ԴTIME_UNITS_PER_SEC��

*/
/* Length to Time (L2T) lookup in a qdisc_rate_table, to determine how
   long it will take to send a packet given its size.
 
 */ // ������ת��Ϊ������ �ο�<��һ��������Դ�����㷨>  �ο�Ӧ�ò�tc_calc_rtable   
static inline u32 qdisc_l2t(struct qdisc_rate_table* rtab, unsigned int pktlen) //��ʾ����ptklen������Ҫ���Ķ��ٿ�����Դʱ��
{
	int slot = pktlen + rtab->rate.cell_align + rtab->rate.overhead;// ���ݴ�С������ʵĲ�λ
	if (slot < 0)
		slot = 0;
	slot >>= rtab->rate.cell_log;
	if (slot > 255)// ���������255, ����Ϊ255
		return (rtab->data[255]*(slot >> 8) + rtab->data[slot & 0xFF]);
	return rtab->data[slot];//Ĭ�������//����õ��ľ���2047���ֽ������ĵĿ�����Դ��
}

#ifdef CONFIG_NET_CLS_ACT
static inline struct sk_buff *skb_act_clone(struct sk_buff *skb, gfp_t gfp_mask)
{
	struct sk_buff *n = skb_clone(skb, gfp_mask);

	if (n) {
		n->tc_verd = SET_TC_VERD(n->tc_verd, 0);
		n->tc_verd = CLR_TC_OK2MUNGE(n->tc_verd);
		n->tc_verd = CLR_TC_MUNGED(n->tc_verd);
	}
	return n;
}
#endif

#endif
