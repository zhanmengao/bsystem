/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for a generic INET TIMEWAIT sock
 *
 *		From code originally in net/tcp.h
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _INET_TIMEWAIT_SOCK_
#define _INET_TIMEWAIT_SOCK_


#include <linux/kmemcheck.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/workqueue.h>

#include <net/inet_sock.h>
#include <net/sock.h>
#include <net/tcp_states.h>
#include <net/timewait_sock.h>

#include <asm/atomic.h>

struct inet_hashinfo;

#define INET_TWDR_RECYCLE_SLOTS_LOG	5
#define INET_TWDR_RECYCLE_SLOTS		(1 << INET_TWDR_RECYCLE_SLOTS_LOG) //32

/*
 * If time > 4sec, it is "slow" path, no recycling is required,
 * so that we select tick to get range about 4 seconds.
 */
#if HZ <= 16 || HZ > 4096
# error Unsupported: HZ <= 16 or HZ > 4096
#elif HZ <= 32
# define INET_TWDR_RECYCLE_TICK (5 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#elif HZ <= 64
# define INET_TWDR_RECYCLE_TICK (6 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#elif HZ <= 128
# define INET_TWDR_RECYCLE_TICK (7 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#elif HZ <= 256
# define INET_TWDR_RECYCLE_TICK (8 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#elif HZ <= 512
# define INET_TWDR_RECYCLE_TICK (9 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#elif HZ <= 1024
# define INET_TWDR_RECYCLE_TICK (10 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#elif HZ <= 2048
# define INET_TWDR_RECYCLE_TICK (11 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#else
# define INET_TWDR_RECYCLE_TICK (12 + 2 - INET_TWDR_RECYCLE_SLOTS_LOG)
#endif

/* TIME_WAIT reaping mechanism. */
#define INET_TWDR_TWKILL_SLOTS	8 /* Please keep this a power of 2. */

#define INET_TWDR_TWKILL_QUOTA 100

struct inet_timewait_death_row1 {
	/* Short-time timewait calendar */
	int			twcal_hand;
	unsigned long		twcal_jiffie;
	struct timer_list	twcal_timer;
	struct hlist_head	twcal_row[INET_TWDR_RECYCLE_SLOTS];

	spinlock_t		death_lock;
	int			tw_count;
	int			period;
	u32			thread_slots;
	struct work_struct	twkill_work;
	struct timer_list	tw_timer;
	int			slot;
	struct hlist_head	cells[INET_TWDR_TWKILL_SLOTS];
	struct inet_hashinfo 	*hashinfo;
	int			sysctl_tw_recycle;
	int			sysctl_max_tw_buckets;
};

/*
��TCP���ӵ���ֹ�����У�Ϊ�˱��ڹ�����ص����ݣ����е�timewait���ƿ�Ͳ����ȶ������inet_timewait_death_row�ṹ�м��й���TCP��ʵ��Ϊtcp_death_row�����а�������
�洢timewait���ƿ��ɢ�б����Ӧ�Ķ�ʱ���ȡ�ͼ�λ������Բο��������²�P1002
*/ //��tcp_timewait_sock���д���                      TCP�иñ�����ֵ�ĵط���tcp_death_row
struct inet_timewait_death_row { //tcp_timewait_sock�ĵ��ȹ�����inet_twsk_schedule
    /* ������⼸��2MSL��: 2MSL�ȴ���ʱʱ��϶̳�Ա���� */

    
	/* Short-time timewait calendar */
	/*
	 * ��ʼֵΪ-1����ʾtwcal_timer��ʱ��
	 * δʹ�ù�������ʹ�ú���ɾ����
	 * ����ֵ��Ϊ-1ʱ����ʾ��ǰ��ʹ��
	 * ��slot����Ϊÿ�α���twcal_rowɢ�б�
	 * ����ڡ���������ó�ʱʱ�䣬����
	 * ��ʱ���󣬽�������Ϊ0����ʾ�ѿ�ʼ
	 * ʹ��
	 */
	int			twcal_hand;
	/*
	 * twcal_timer��ʱ����ʱʱ�䣬�����
	 * timewait���ƿ�ķ���
	 */
	unsigned long		twcal_jiffie;
	/*
	 * twcal_timer�ĳ�ʱ��������inet_twdr_twcal_tick(),
	 * ��ɨ������twcal_row��ɾ�����еĳ�ʱtwsk��
	 * ��ʣ�µ�twsk�����趨��ʱʱ��
	 */
	struct timer_list	twcal_timer;

	/*
	 * TIME_WAIT��ʱʱ�����INET_TWDR_RECYCLE_TICK��
	 * ����ȡ���������жϽ���timewait���ƿ����
	 * ��cells����twcal_rowɢ�б��С�
	 * ����õ�ֵ���ڻ����INET_TWDR_RECYCLE_SLOTS��
	 * ������ӵ�cellsɢ�б��У�������ӵ�
	 * twcal_rowɢ�б���
	 */
	/*
	 * ���ڴ洢2MSL�ȴ���ʱʱ��϶̵�timewait
	 * ���ƿ��ɢ�б�
	 */
	struct hlist_head	twcal_row[INET_TWDR_RECYCLE_SLOTS]; //�������ӵ���tcp_timewait_sock

	/*
	 * ����ͬ������twcal_row��cellsɢ�б��������
	 */
	spinlock_t		death_lock;
	/*
	 * ��ǰϵͳ�д���TIME_WAIT״̬���׽���������ֵ����
	 * ����ϵͳ����tcp_max_tw_buckets���μ�
	 * NET_TCP_MAX_TW_BUCKETSϵͳ����
	 */ //��tcp_time_wait�д���inet_timewait_sock����inet_twsk_schedule������1
	int			tw_count; //�ں���inet_twsk_schedule������  �ǵ�ǰTIME_WAIT״̬�׽��ֵ�����





	/* ������⼸��2MSL��: 2MSL�ȴ���ʱʱ��϶̳�Ա���� */

	
	/*
	 * tw_timer��ʱ���ĳ�ʱʱ��Ϊ
	 * TCP_TIMEWAIT_LEN / INET_TWDR_TWKILL_SLOTS��
	 * ����60s�ֳ�8��
	 */
	int			period;
	/*
	 * �ڷ���ɾ�����ͷ�cellsɢ�б��е�timewait���ƿ�
	 * ʱ�����ڱ�ʶ��ɾ��slot��λͼ
	 */
	u32			thread_slots;
	/*
	 * ���з���ɾ�����ͷ�cellsɢ�б��е�timewait
	 * ���ƿ�Ĺ�������
	 */
	struct work_struct	twkill_work;
	/*
	 * tw_timer�ĳ�ʱ��������inet_twdr_hangman()��ÿ
	 * ����һ��period��ʱһ�Σ�ȡcells�ж�Ӧ�Ķ��У�
	 * ɾ�����������е�twsk��ͬʱ��ehashɢ�б�
	 * �ĺ�벿�ֺ�bashɢ�б���ɾ����Ӧ��twsk
	 * ����󶨵ı��ض˿ڡ�����twsk��ʹ����
	 * �����
	 */ //�ö�ʱ��������inet_twsk_schedule�����������Ķ�ʱ��������Ϊinet_twdr_hangman,��tcp_death_row
	struct timer_list	tw_timer;
	/*
	 * tw_timer��ʱ����ʱʱ��ʹ�õ�slot����Ϊcells
	 * ɢ�б�Ĺؼ��� 
	 //��һ��tw_timer��ʱ��ʱ��twdr->slot=0,�Ͷ���tw_timer��ʱ��ʱ�򣬸�ֵ��1������7���ֻص�1��Ҳ����ÿ��8��period(TCP_TIMEWAIT_LEN / INET_TWDR_TWKILL_SLOTS)
	 ѭ��һ�Σ������Ϳ��Ա�֤cells���е�����timewait������(�����϶���INET_TWDR_TWKILL_SLOTSʱ�������һ�α�cells������ʱ�䶼��INET_TWDR_TWKILL_SLOTS�������������
	 ĳ��cells�ϵ�timewait��������INET_TWDR_TWKILL_QUOTA��inet_twdr_do_twkill_work)  ��inet_twdr_hangman
	 */
	int			slot;//ÿ��period��ʱһ��
	/*
	 * ���ڴ洢2MSL�ȴ���ʱʱ��ϳ���timewait
	 * ���ƿ��ɢ�б�
	 */
	struct hlist_head	cells[INET_TWDR_TWKILL_SLOTS];
	/*
	 * ָ��inet_hashinfo�ṹ����ʵ��tcp_hashinfo
	 */
	struct inet_hashinfo 	*hashinfo;
	/*  tcp_timestamps�������������Ƿ�����ʱ���ѡ�tcp_tw_recycle�����������ÿ��ٻ���TIME_WAIT�׽��֡�tcp_timestamps������Ӱ�쵽
	tcp_tw_recycle������Ч�������û��ʱ���ѡ��Ļ���tcp_tw_recycle������Ч����tcp_time_wait
	 * �����洢ϵͳ����tcp_tw_recycle��ֵ, tcp_tw_recycle�����������ÿ��ٻ���TIME_WAIT�׽���
	 */ //���������tcp_tw_resycle,��tcp_time_wait�г�ʱʱ���õ�tw->tw_timeout = rto��������Ĭ�ϵ�TCP_TIMEWAIT_LEN��TCP_TIMEWAIT_LEN����������������»��rto���������øò������Կ��ٻ���timewait
    //���������ʱ����tcp_v4_conn_request�еĺ�����ܻ���� ���TCPʱ���PAWS©������ɷ��������յ�SYN��ʱ�򲻻���SYN+ACK������취�ǶԷ���Ҫ����ʱ���ѡ�ͬʱ�ر�tcp_timestamps��tcp_v4_conn_request
	int			sysctl_tw_recycle;////��Ӧ�ò��/proc/sys/net�����õ�ʱ�򣬶�Ӧ��ֵ��д�뵽data��
	/*
	 * �����洢ϵͳ����tcp_max_tw_buckets��ֵ����ʾ�������ɶ��ٸ�time_wait����
	 */
	int			sysctl_max_tw_buckets;
};

extern void inet_twdr_hangman(unsigned long data);
extern void inet_twdr_twkill_work(struct work_struct *work);
extern void inet_twdr_twcal_tick(unsigned long data);

#if (BITS_PER_LONG == 64)
#define INET_TIMEWAIT_ADDRCMP_ALIGN_BYTES 8
#else
#define INET_TIMEWAIT_ADDRCMP_ALIGN_BYTES 4
#endif

struct inet_bind_bucket;

/*
 * This is a TIME_WAIT sock. It works around the memory consumption
 * problems of sockets in such a state on heavily loaded servers, but
 * without violating the protocol specification.
 *///tcp_timewait_sock����inet_timewait_sock��inet_timewait_sock����sock_common  TCP���ӵ�ʱ���� TCP_TIME_WAIT״̬�������õ�
 /*
 * inet_timewait_sock�ṹ��֧�������������Ե�
 * TCP_TIME_WAIT״̬���������ǹ���tcp_timewait_sock�Ļ���
  tcp_timewait_sock����inet_timewait_sock��inet_timewait_sock����sock_common
 *///������TCP���ӶϿ�����timewait״̬��ʱ�򣬸�inet_timewait_sock��inet_twsk_schedule�б���ӵ���tcp_death_row�е�tw_death_node��
struct inet_timewait_sock {//�ýṹ��__inet_twsk_kill������ͷſռ�
	/*
	 * Now struct sock also uses sock_common, so please just
	 * don't add nothing before this first member (__tw_common) --acme
	 */
	struct sock_common	__tw_common;
#define tw_family		__tw_common.skc_family
#define tw_state		__tw_common.skc_state
#define tw_reuse		__tw_common.skc_reuse
#define tw_bound_dev_if		__tw_common.skc_bound_dev_if
#define tw_node			__tw_common.skc_nulls_node //inet_twsk_add_node_rcu,���뵽
#define tw_bind_node		__tw_common.skc_bind_node////�ڳ�ʱ״̬��inet_bind_bucketͰָ��tw->tw_bind_node������ú����������ͷ�sk��ʱ�򣬻��ͷŵ�bindͰ��Ϣ
#define tw_refcnt		__tw_common.skc_refcnt
#define tw_hash			__tw_common.skc_hash
#define tw_prot			__tw_common.skc_prot
#define tw_net			__tw_common.skc_net
	/*
	 * ���ڼ�¼2MSL��ʱʱ��
	 */
	int			tw_timeout;
	/*
	 * ����TCP״̬Ǩ�Ƶ�FIN_WAIT2��TIME_WAIT״̬ʱ��
	 * ����Ҫ�ɶ�ʱ��������һ����ʱ�׽���
	 * �漴�ͱ��ͷš�һ����timewait���ƿ�ȡ��
	 * tcp_sock������ƿ��������״̬ʱTIME_WAIT��
	 * ���ڲ�״̬����������ģ������Ҫtw_substate
	 * ����ʶFIN_WAIT2��TIME_WAIT
	 */
	volatile unsigned char	tw_substate;
	/* 3 bits hole, try to pack */
	unsigned char		tw_rcv_wscale;
	/* Socket demultiplex comparisons on incoming packets. */
	/* these five are in inet_sock */
	//������Щֵ���Ǵ�inet_sock�л�ȡ
	__be16			tw_sport;
	__be32			tw_daddr __attribute__((aligned(INET_TIMEWAIT_ADDRCMP_ALIGN_BYTES)));
	__be32			tw_rcv_saddr;
	__be16			tw_dport;
	__u16			tw_num;
	kmemcheck_bitfield_begin(flags);
	/* And these are ours. */
	unsigned int		tw_ipv6only     : 1,
				tw_transparent  : 1,
				tw_pad		: 14,	/* 14 bits hole */
				tw_ipv6_offset  : 16;
	kmemcheck_bitfield_end(flags);
	/*
	 * ��timewait���ƿ鳬ʱɾ����ʱ��(��λΪHZ)��
	 * ��proc�ļ�ϵͳ��ʹ��
	 */
	unsigned long		tw_ttd;
	/*
	 * ָ��󶨵ı��ض˿���Ϣ���ɶ�Ӧ��TCP������ƿ�
	 * ��icsk_bind_hash��Ա�õ�����__inet_twsk_hashdance
	 */
	struct inet_bind_bucket	*tw_tb; //
	/*
	 * ������twcal_row��cellsɢ�б��й�������, ��inet_timewait_death_row
	 */
	struct hlist_node	tw_death_node;
};
struct inet_timewait_sock1 {
	/*
	 * Now struct sock also uses sock_common, so please just
	 * don't add nothing before this first member (__tw_common) --acme
	 */
	struct sock_common	__tw_common;
#define tw_family		__tw_common.skc_family
#define tw_state		__tw_common.skc_state
#define tw_reuse		__tw_common.skc_reuse
#define tw_bound_dev_if		__tw_common.skc_bound_dev_if
#define tw_node			__tw_common.skc_nulls_node
#define tw_bind_node		__tw_common.skc_bind_node
#define tw_refcnt		__tw_common.skc_refcnt
#define tw_hash			__tw_common.skc_hash
#define tw_prot			__tw_common.skc_prot
#define tw_net			__tw_common.skc_net
	int			tw_timeout;
	volatile unsigned char	tw_substate;
	/* 3 bits hole, try to pack */
	unsigned char		tw_rcv_wscale;
	/* Socket demultiplex comparisons on incoming packets. */
	/* these five are in inet_sock */
	__be16			tw_sport;
	__be32			tw_daddr __attribute__((aligned(INET_TIMEWAIT_ADDRCMP_ALIGN_BYTES)));
	__be32			tw_rcv_saddr;
	__be16			tw_dport;
	__u16			tw_num;
	kmemcheck_bitfield_begin(flags);
	/* And these are ours. */
	unsigned int		tw_ipv6only     : 1,
				tw_transparent  : 1,
				tw_pad		: 14,	/* 14 bits hole */
				tw_ipv6_offset  : 16;
	kmemcheck_bitfield_end(flags);
	unsigned long		tw_ttd;
	struct inet_bind_bucket	*tw_tb;
	struct hlist_node	tw_death_node;
};

static inline void inet_twsk_add_node_rcu(struct inet_timewait_sock *tw,
				      struct hlist_nulls_head *list)
{
	hlist_nulls_add_head_rcu(&tw->tw_node, list);
}

static inline void inet_twsk_add_bind_node(struct inet_timewait_sock *tw,
					   struct hlist_head *list)
{
	hlist_add_head(&tw->tw_bind_node, list);
}

static inline int inet_twsk_dead_hashed(const struct inet_timewait_sock *tw)
{
	return !hlist_unhashed(&tw->tw_death_node);
}

static inline void inet_twsk_dead_node_init(struct inet_timewait_sock *tw)
{
	tw->tw_death_node.pprev = NULL;
}

static inline void __inet_twsk_del_dead_node(struct inet_timewait_sock *tw)
{
	__hlist_del(&tw->tw_death_node);
	inet_twsk_dead_node_init(tw);
}

static inline int inet_twsk_del_dead_node(struct inet_timewait_sock *tw)
{
	if (inet_twsk_dead_hashed(tw)) {
		__inet_twsk_del_dead_node(tw);
		return 1;
	}
	return 0;
}

#define inet_twsk_for_each(tw, node, head) \
	hlist_nulls_for_each_entry(tw, node, head, tw_node)

#define inet_twsk_for_each_inmate(tw, node, jail) \
	hlist_for_each_entry(tw, node, jail, tw_death_node)

#define inet_twsk_for_each_inmate_safe(tw, node, safe, jail) \
	hlist_for_each_entry_safe(tw, node, safe, jail, tw_death_node)

static inline struct inet_timewait_sock *inet_twsk(const struct sock *sk)
{
	return (struct inet_timewait_sock *)sk;
}

static inline __be32 inet_rcv_saddr(const struct sock *sk)
{
	return likely(sk->sk_state != TCP_TIME_WAIT) ?
		inet_sk(sk)->inet_rcv_saddr : inet_twsk(sk)->tw_rcv_saddr;
}

extern void inet_twsk_put(struct inet_timewait_sock *tw);

extern int inet_twsk_unhash(struct inet_timewait_sock *tw);

extern int inet_twsk_bind_unhash(struct inet_timewait_sock *tw,
				 struct inet_hashinfo *hashinfo);

extern struct inet_timewait_sock *inet_twsk_alloc(const struct sock *sk,
						  const int state);

extern void __inet_twsk_hashdance(struct inet_timewait_sock *tw,
				  struct sock *sk,
				  struct inet_hashinfo *hashinfo);

extern void inet_twsk_schedule(struct inet_timewait_sock *tw,
			       struct inet_timewait_death_row *twdr,
			       const int timeo, const int timewait_len);
extern void inet_twsk_deschedule(struct inet_timewait_sock *tw,
				 struct inet_timewait_death_row *twdr);

extern void inet_twsk_purge(struct inet_hashinfo *hashinfo,
			    struct inet_timewait_death_row *twdr, int family);

static inline
struct net *twsk_net(const struct inet_timewait_sock *twsk)
{
#ifdef CONFIG_NET_NS
	return rcu_dereference_raw(twsk->tw_net); /* protected by locking, */
						  /* reference counting, */
						  /* initialization, or RCU. */
#else
	return &init_net;
#endif
}

static inline
void twsk_net_set(struct inet_timewait_sock *twsk, struct net *net)
{
#ifdef CONFIG_NET_NS
	rcu_assign_pointer(twsk->tw_net, net);
#endif
}
#endif	/* _INET_TIMEWAIT_SOCK_ */
