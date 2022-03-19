/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the BSD Socket
 *		interface as the means of communication with the user level.
 *
 * Authors:	Lotsa people, from code originally in tcp
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#ifndef _INET_HASHTABLES_H
#define _INET_HASHTABLES_H


#include <linux/interrupt.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/vmalloc.h>

#include <net/inet_connection_sock.h>
#include <net/inet_sock.h>
#include <net/sock.h>
#include <net/route.h>
#include <net/tcp_states.h>
#include <net/netns/hash.h>

#include <asm/atomic.h>
#include <asm/byteorder.h>

/* This is for all connections with a full identity, no wildcards.
 * One chain is dedicated to TIME_WAIT sockets.
 * I'll experiment with dynamic table growth later.
 */
 /*
 * ��������TCP״̬��LISTEN֮��Ĵ�����ƿ��ɢ�б�
 */ //inet_ehash_bucket��ͼ�������Բο������������²�P735
 //hash��ͷΪinet_hashinfo��ehash
struct inet_ehash_bucket {
	/*
	 * �������Ӵ�����ƿ�
//���������������ֳɹ����Ӧ��struct tcp_sock��ӵ����hash��ͷ�У��ͻ����ڷ���syn֮ǰ������˿ڵȿ��ã�
����__inet_check_established�е�__sk_nulls_add_node_rcu��sk��ӵ�ehash��

	 */
	struct hlist_nulls_head chain;
	/*
	 * ��������TIME_WAIT״̬�Ĵ�����ƿ�
	 */
	struct hlist_nulls_head twchain;//���������ӵ���inet_timewait_sock�ṹ����__inet_twsk_hashdance
};

/* There are a few simple rules, which allow for local port reuse by
 * an application.  In essence:
 *
 *	1) Sockets bound to different interfaces may share a local port.
 *	   Failing that, goto test 2.
 *	2) If all sockets have sk->sk_reuse set, and none of them are in
 *	   TCP_LISTEN state, the port may be shared.
 *	   Failing that, goto test 3.
 *	3) If all sockets are bound to a specific inet_sk(sk)->rcv_saddr local
 *	   address, and none of them are the same, the port may be
 *	   shared.
 *	   Failing this, the port cannot be shared.
 *
 * The interesting point, is test #2.  This is what an FTP server does
 * all day.  To optimize this case we use a specific flag bit defined
 * below.  As we add sockets to a bind bucket list, we perform a
 * check of: (newsk->sk_reuse && (newsk->sk_state != TCP_LISTEN))
 * As long as all sockets added to a bind bucket pass this test,
 * the flag bit will be set.
 * The resulting situation is that tcp_v[46]_verify_bind() can just check
 * for this flag bit, if it is set and the socket trying to bind has
 * sk->sk_reuse set, we don't even have to walk the owners list at all,
 * we return that it is ok to bind this socket to the requested local port.
 *
 * Sounds like a lot of work, but it is worth it.  In a more naive
 * implementation (ie. current FreeBSD etc.) the entire list of ports
 * must be walked for each data port opened by an ftp server.  Needless
 * to say, this does not scale at all.  With a couple thousand FTP
 * users logged onto your box, isn't it nice to know that new data
 * ports are created in O(1) time?  I thought so. ;-)	-DaveM
 */
struct inet_bind_bucket1 {
#ifdef CONFIG_NET_NS
	struct net		*ib_net;
#endif
	unsigned short		port;
	signed short		fastreuse;
	int			num_owners;
	struct hlist_node	node;
	struct hlist_head	owners;
};
//��ֵ�����������inet_bind_hash ��
//��ŵ��Ƕ˿ڰ���Ϣ�� inet_bind_bucket_create����   __inet_put_port�ͷ�
//�ýṹ������ӵ�inet_bind_hashbucket�ṹ��chain��
//�ýṹ���ڿͻ���connect����ͻ��˶˿ڵ�ʱ�����õ��������ͻ���connect��ʱ�򴴽�inet_bind_bucket�ռ�
//�ýṹ�����յĽ���ǣ�struct sock����ӵ�inet_bind_bucket�ṹ��owners������(inet_bind_hash)��Ȼ���inet_bind_bucketͨ��node�ڵ���뵽tcp_hashinfo��chain�У�inet_bind_hashbucket�е�(inet_bind_bucket_create)
/*�ýṹ���������ֳɹ���ʱ������ͨ��owners���ӵ�inet_connection_sock�е�icsk_bind_hash�����ڶ˿����ӹ����н���timewait�Ĺ����и�inet_bind_bucket��������ӵ�
tw->tw_bind_node,��inet_timewait_sock�У������������ֶ˿ڵ�timewait �ͷ�sk��ʱ�򣬲�û���ͷŵ���bindͰ����__inet_twsk_hashdance*/
//ͼ�λ��ṹ���ο���������P775            �ͷŵĵط���__inet_put_port
struct inet_bind_bucket { //�洢��inet_hashinfo�е�bhash���У���Ӧ��hashbucketΪinet_bind_hashbucket        
//���е�inet_bind_bucketʵ��ͨ���������node��Ա������һ��Ȼ����ӵ�inet_bind_hashbucket��chain�У�����ڰ�chain���ӵ�inet_hashinfo�е�bhash��
//ͬʱ��Ӧ�ó���ͻ���connect���߷�������bind��ʱ��ͨ��owners�Ѹ�inet_bind_bucket��ӵ�struct sock�е�sk_bind_node��,ͬʱinet_csk(sk)->icsk_bind_hashָ�����inet_bind_bucket��
//����inet_bind_bucket�Ϳ��Ժ�struct sock������������inet_bind_hash
#ifdef CONFIG_NET_NS
	struct net		*ib_net;
#endif
	/*
	 * �Ѱ󶨵Ķ˿ں�
	 */
	unsigned short		port;
	/*
	 * ��ʶ�˿��Ƿ�������
	 * 0: �˶˿���ͨ��bindϵͳ���ð󶨣�����������
	 * 1: �˶˿��Ѿ�ͨ��bindϵͳ���ð󶨣���������  sk->sk_reuse�������õĻ������ֵ�Ϳ�������
	 * -1: �˶˿ڱ��ͻ��˶�̬��(��inet_hash_connection()���а�)
	 */
	signed short		fastreuse;//__inet_hash_connect�ͻ��������ڼ��fastreuse�Ƿ�ɸ���ʱ�����ü������__inet_check_established��inet_csk_get_port
	int			num_owners;//��sk���ô�����inet_bind_hash
	/*
	 * �����洢bhashɢ�б�Ľڵ㣬 ��inet_bind_bucketͨ����node��ӵ�inet_bind_hashbucket��chain��   inet_bind_hash  inet_bind_bucket_create
	 */
	struct hlist_node	node;
	/*
	 * Ϊ�󶨸ö˿��ϵĴ�����ƿ����� 
	 *///ͨ��owners�Ѹ�inet_bind_bucket��ӵ�struct sock�е�sk_bind_node�У�����inet_bind_bucket�Ϳ��Ժ�struct sock����������
	 //����ʼָ�����sk->sk_bind_node��������timewait��ʱ��ָ��tw->tw_bind_node������bindͰ��wait1״̬�л���timewait״̬��ʱ�����ͷ�skʱ�������ͷŵ�bind��Ϣ����tcp_time_wait
	struct hlist_head	owners; //��Ӧ��struct sockͨ��sk_bind_node��Ա��ӵ���owners��  ��inet_bind_hash
	//�ýṹͨ��owners���뵽struct sock�����sk_bind_node��
	/*��time_wait�����У���inet_hash_bucket��ͨ��ownersָ��sk,��Ϊָ��tw->tw_bind_node,��inet_timewait_sock�������ڽ���time_wait״̬��
	��tcp_time_wait���ͷ���sk,����֮ǰbind��ص�inet_bind_bucketͰ��û���ͷţ��������close״̬��ʱ����ͷţ���__inet_twsk_hashdance*/
};

static inline struct net *ib_net(struct inet_bind_bucket *ib)
{
	return read_pnet(&ib->ib_net);
}

#define inet_bind_bucket_for_each(tb, pos, head) \
	hlist_for_each_entry(tb, pos, head, node)

struct inet_bind_hashbucket1 {
	spinlock_t		lock;
	struct hlist_head	chain;
};
//�ýṹ��inet_hashinfo�е�bhashָ��  �ýṹ���ڿͻ���connect����ͻ��˶˿ڵ�ʱ�����õ�
//TCP��ʹ��inet_bind_hashbucketɢ�б��������Ѱ󶨶˿ڡ�inet_bind_bucketʵ��ͨ���������node��Ա���ӵ�inet_hashinfo�е�bhash��
//ͼ�λ������Բο��������� P775��hashͰ��ͷָ��Ϊ��������е�inet_bind_bucketͨ��nodeȫ����ӵ�chain������
struct inet_bind_hashbucket {//���е�inet_bind_bucketʵ��ͨ���������node��Ա������һ��Ȼ����ӵ�inet_bind_hashbucket��chain�У�����ڰ�chain���ӵ�inet_hashinfo�е�bhash��
	/*
	 * ���Ƹ�����Ķ�д��
	 */
	spinlock_t		lock;
	/*
	 * ���ڽ����˿ڰ���Ϣ�飬�ṹ��Ϊinet_bind_bucket�� ��inet_bind_bucket�ṹ���� inet_bind_bucket_create
	 */
	struct hlist_head	chain;
};

/*
 * Sockets can be hashed in established or listening table
 * We must use different 'nulls' end-of-chain value for listening
 * hash table, or we might find a socket that was closed and
 * reallocated/inserted into established hash table
 */
#define LISTENING_NULLS_BASE (1U << 29)
struct inet_listen_hashbucket {
	spinlock_t		lock;
	struct hlist_nulls_head	head;
};

/* This is for listening sockets, thus all sockets which possess wildcards. */
#define INET_LHTABLE_SIZE	32	/* Yes, really, this is all you need. */

struct inet_hashinfo1 {
	/* This is for sockets with full identity only.  Sockets here will
	 * always be without wildcards and will have the following invariant:
	 *
	 *          TCP_ESTABLISHED <= sk->sk_state < TCP_CLOSE
	 *
	 * TIME_WAIT sockets use a separate chain (twchain).
	 */
	struct inet_ehash_bucket	*ehash;
	spinlock_t			*ehash_locks;
	unsigned int			ehash_mask;
	unsigned int			ehash_locks_mask;

	/* Ok, let's try this, I give up, we do need a local binding
	 * TCP hash as well as the others for fast bind/connect.
	 */
	struct inet_bind_hashbucket	*bhash;

	unsigned int			bhash_size;
	/* 4 bytes hole on 64 bit */

	struct kmem_cache		*bind_bucket_cachep;

	/* All the above members are written once at bootup and
	 * never written again _or_ are predominantly read-access.
	 *
	 * Now align to a new cache line as all the following members
	 * might be often dirty.
	 */
	/* All sockets in TCP_LISTEN state will be in here.  This is the only
	 * table where wildcard'd TCP sockets can exist.  Hash function here
	 * is just local port number.
	 */
	struct inet_listen_hashbucket	listening_hash[INET_LHTABLE_SIZE]
					____cacheline_aligned_in_smp;

	atomic_t			bsockets;
};
/*
 * TCP���������һ��inet_hashinfo�ṹ���͵�ȫ�ֱ���
 * tcp_hashinfo�����е�ɢ�б���м��й���
  //tcp_death_row�����hashinfoָ�����tcp_hashinfo��Ҳ�������е�inethash_info��ͷ������
 */
/*
tcp udp��raw��hash
union {
		struct inet_hashinfo	*hashinfo; //tcp_hashinfo
		struct udp_table	*udp_table; //udp_table
		struct raw_hashinfo	*raw_hash; //raw_v4_hashinfo
	} h;
*/

//tcp_hashinfo��ͼ�������Բο������������²�P734
struct inet_hashinfo {
	/* This is for sockets with full identity only.  Sockets here will
	 * always be without wildcards and will have the following invariant:
	 *
	 *          TCP_ESTABLISHED <= sk->sk_state < TCP_CLOSE
	 *
	 * TIME_WAIT sockets use a separate chain (twchain).
	 */
	/*
	 * ehashָ��һ����СΪehash_size��inet_ehash_bucket�ṹ���͵�ɢ��
	 * ����������TCP״̬��LISTEN֮��Ĵ�����ƿ��ɢ�б�

	 tcp���ֳַ������ű�ehash, bhash, listening_hash������ehash, listening_hash��Ӧ��socket����TCP��ESTABLISHED, LISTEN״̬��bhash��Ӧ��socket
	 �Ѱ��˱��ص�ַ�����߼䲢�����⣬��һ��socket��ͬʱ��bhash��ehash�У�����TIME_WAIT��һ���Ƚ������״̬������ehash�ֳַ���chain��twchain��
	 ΪTIME_WAIT��socket�����γ�һ�ű�
	 */
	 //inet_timewait_sockͨ��__inet_twsk_hashdance���뵽��ehash�е�twrefcnt += hash(sk, tw);����������accept��ʱ�����ڽ������ӳɹ���ʱ��ŷŵ���ehash�еġ�
	 //�ͻ���connect��ʱ��������뵽��hash�У���__inet_hash_connect
	struct inet_ehash_bucket	*ehash;//��inet_ehash_bucket����  ���hashͰ������Ƕ�����ӳɹ���sock,timewait��sockҲ��������
	spinlock_t			*ehash_locks;
	unsigned int			ehash_size;
	unsigned int			ehash_locks_mask;

	/* Ok, let's try this, I give up, we do need a local binding
	 * TCP hash as well as the others for fast bind/connect.
	 */
	/*
	 * ��Ҫ�����洢�Ѱ󶨶˿ڵ���Ϣ  inet_bind_bucket  �ýṹ���ڿͻ���connect����ͻ��˶˿ڵ�ʱ�����õ�,�����ӽ���������ֻ��ehash��listening_hash
	 */////���е�inet_bind_bucketʵ��ͨ���������node��Ա������һ��Ȼ����ӵ�inet_bind_hashbucket��chain�У�����ڰ�chain���ӵ�inet_hashinfo�е�bhash��
    //����ǿͻ������ӣ�����connect��ʱ�����inet_bind_hash��Ȼ����ӵ���hashͰ�У���__inet_hash_connect
    //�����Ӷ˿ڣ�����timewait״̬��ʱ��inet_timewait_sockҲ����ӵ���bhash�У���__inet_twsk_hashdance
    //�ͷź�����inet_bind_bucket
	struct inet_bind_hashbucket	*bhash;//���󶨵Ķ˿���Ϣ��һֱ�ڸá��´ΰ������˿ڻ��߷���˿ڵ�ʱ����Ҫ�����������顣ͼ�λ��ṹ���ο���������P775
	unsigned int			bhash_size;
	/* 4 bytes hole on 64 bit */
	/*
	 * ��������inet_bind_hashbucket�ṹ�ĺ󱸸��ٻ���
	 */
	struct kmem_cache		*bind_bucket_cachep;

	/* All the above members are written once at bootup and
	 * never written again _or_ are predominantly read-access.
	 *
	 * Now align to a new cache line as all the following members
	 * might be often dirty.
	 */
	/* All sockets in TCP_LISTEN state will be in here.  This is the only
	 * table where wildcard'd TCP sockets can exist.  Hash function here
	 * is just local port number.
	 */
	/*
	 * �����洢����LISTEN״̬�Ĵ�����ƿ��ɢ�б���listen״̬��ʱ����룬��inet_csk_listen_start
	 */
	struct inet_listen_hashbucket	listening_hash[INET_LHTABLE_SIZE];// ____cacheline_aligned_in_smp;
	/*
	 * Ӧ�����Ѱ󶨵��׽��ֵ����� inet_bind_hash
	 */
	atomic_t			bsockets;
};

static inline struct inet_ehash_bucket *inet_ehash_bucket(
	struct inet_hashinfo *hashinfo,
	unsigned int hash)
{
	return &hashinfo->ehash[hash & hashinfo->ehash_mask];
}

static inline spinlock_t *inet_ehash_lockp(
	struct inet_hashinfo *hashinfo,
	unsigned int hash)
{
	return &hashinfo->ehash_locks[hash & hashinfo->ehash_locks_mask];
}

static inline int inet_ehash_locks_alloc(struct inet_hashinfo *hashinfo)
{
	unsigned int i, size = 256;
#if defined(CONFIG_PROVE_LOCKING)
	unsigned int nr_pcpus = 2;
#else
	unsigned int nr_pcpus = num_possible_cpus();
#endif
	if (nr_pcpus >= 4)
		size = 512;
	if (nr_pcpus >= 8)
		size = 1024;
	if (nr_pcpus >= 16)
		size = 2048;
	if (nr_pcpus >= 32)
		size = 4096;
	if (sizeof(spinlock_t) != 0) {
#ifdef CONFIG_NUMA
		if (size * sizeof(spinlock_t) > PAGE_SIZE)
			hashinfo->ehash_locks = vmalloc(size * sizeof(spinlock_t));
		else
#endif
		hashinfo->ehash_locks =	kmalloc(size * sizeof(spinlock_t),
						GFP_KERNEL);
		if (!hashinfo->ehash_locks)
			return ENOMEM;
		for (i = 0; i < size; i++)
			spin_lock_init(&hashinfo->ehash_locks[i]);
	}
	hashinfo->ehash_locks_mask = size - 1;
	return 0;
}

static inline void inet_ehash_locks_free(struct inet_hashinfo *hashinfo)
{
	if (hashinfo->ehash_locks) {
#ifdef CONFIG_NUMA
		unsigned int size = (hashinfo->ehash_locks_mask + 1) *
							sizeof(spinlock_t);
		if (size > PAGE_SIZE)
			vfree(hashinfo->ehash_locks);
		else
#endif
		kfree(hashinfo->ehash_locks);
		hashinfo->ehash_locks = NULL;
	}
}

extern struct inet_bind_bucket *
		    inet_bind_bucket_create(struct kmem_cache *cachep,
					    struct net *net,
					    struct inet_bind_hashbucket *head,
					    const unsigned short snum);
extern void inet_bind_bucket_destroy(struct kmem_cache *cachep,
				     struct inet_bind_bucket *tb);

static inline int inet_bhashfn(struct net *net,
		const __u16 lport, const int bhash_size)
{
	return (lport + net_hash_mix(net)) & (bhash_size - 1);
}

extern void inet_bind_hash(struct sock *sk, struct inet_bind_bucket *tb,
			   const unsigned short snum);

/* These can have wildcards, don't try too hard. */
static inline int inet_lhashfn(struct net *net, const unsigned short num)
{
	return (num + net_hash_mix(net)) & (INET_LHTABLE_SIZE - 1);
}

//ͨ��sk������һ��hash ��ֵ
static inline int inet_sk_listen_hashfn(const struct sock *sk)
{
	return inet_lhashfn(sock_net(sk), inet_sk(sk)->inet_num);
}

/* Caller must disable local BH processing. */
extern void __inet_inherit_port(struct sock *sk, struct sock *child);

extern void inet_put_port(struct sock *sk);

void inet_hashinfo_init(struct inet_hashinfo *h);

extern int __inet_hash_nolisten(struct sock *sk, struct inet_timewait_sock *tw);
extern void inet_hash(struct sock *sk);
extern void inet_unhash(struct sock *sk);

extern struct sock *__inet_lookup_listener(struct net *net,
					   struct inet_hashinfo *hashinfo,
					   const __be32 daddr,
					   const unsigned short hnum,
					   const int dif);

static inline struct sock *inet_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo,
		__be32 daddr, __be16 dport, int dif)
{
	return __inet_lookup_listener(net, hashinfo, daddr, ntohs(dport), dif);
}

/* Socket demux engine toys. */
/* What happens here is ugly; there's a pair of adjacent fields in
   struct inet_sock; __be16 dport followed by __u16 num.  We want to
   search by pair, so we combine the keys into a single 32bit value
   and compare with 32bit value read from &...->dport.  Let's at least
   make sure that it's not mixed with anything else...
   On 64bit targets we combine comparisons with pair of adjacent __be32
   fields in the same way.
*/
typedef __u32 __bitwise __portpair;
#ifdef __BIG_ENDIAN
#define INET_COMBINED_PORTS(__sport, __dport) \
	((__force __portpair)(((__force __u32)(__be16)(__sport) << 16) | (__u32)(__dport)))
#else /* __LITTLE_ENDIAN */
#define INET_COMBINED_PORTS(__sport, __dport) \
	((__force __portpair)(((__u32)(__dport) << 16) | (__force __u32)(__be16)(__sport)))
#endif

#if (BITS_PER_LONG == 64)
typedef __u64 __bitwise __addrpair;
#ifdef __BIG_ENDIAN
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const __addrpair __name = (__force __addrpair) ( \
				   (((__force __u64)(__be32)(__saddr)) << 32) | \
				   ((__force __u64)(__be32)(__daddr)));
#else /* __LITTLE_ENDIAN */
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const __addrpair __name = (__force __addrpair) ( \
				   (((__force __u64)(__be32)(__daddr)) << 32) | \
				   ((__force __u64)(__be32)(__saddr)));
#endif /* __BIG_ENDIAN */
#define INET_MATCH(__sk, __net, __hash, __cookie, __saddr, __daddr, __ports, __dif)\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net)) &&	\
	 ((*((__addrpair *)&(inet_sk(__sk)->inet_daddr))) == (__cookie))  &&	\
	 ((*((__portpair *)&(inet_sk(__sk)->inet_dport))) == (__ports))   &&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#define INET_TW_MATCH(__sk, __net, __hash, __cookie, __saddr, __daddr, __ports, __dif)\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net)) &&	\
	 ((*((__addrpair *)&(inet_twsk(__sk)->tw_daddr))) == (__cookie)) &&	\
	 ((*((__portpair *)&(inet_twsk(__sk)->tw_dport))) == (__ports)) &&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#else /* 32-bit arch */
#define INET_ADDR_COOKIE(__name, __saddr, __daddr)
#define INET_MATCH(__sk, __net, __hash, __cookie, __saddr, __daddr, __ports, __dif)	\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net))	&&	\
	 (inet_sk(__sk)->inet_daddr	== (__saddr))		&&	\
	 (inet_sk(__sk)->inet_rcv_saddr	== (__daddr))		&&	\
	 ((*((__portpair *)&(inet_sk(__sk)->inet_dport))) == (__ports))	&&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#define INET_TW_MATCH(__sk, __net, __hash,__cookie, __saddr, __daddr, __ports, __dif)	\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net))	&&	\
	 (inet_twsk(__sk)->tw_daddr	== (__saddr))		&&	\
	 (inet_twsk(__sk)->tw_rcv_saddr	== (__daddr))		&&	\
	 ((*((__portpair *)&(inet_twsk(__sk)->tw_dport))) == (__ports)) &&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#endif /* 64-bit arch */

/*
 * Sockets in TCP_CLOSE state are _always_ taken out of the hash, so we need
 * not check it for lookups anymore, thanks Alexey. -DaveM
 *
 * Local BH must be disabled here.
 */
extern struct sock * __inet_lookup_established(struct net *net,
		struct inet_hashinfo *hashinfo,
		const __be32 saddr, const __be16 sport,
		const __be32 daddr, const u16 hnum, const int dif);

static inline struct sock *
	inet_lookup_established(struct net *net, struct inet_hashinfo *hashinfo,
				const __be32 saddr, const __be16 sport,
				const __be32 daddr, const __be16 dport,
				const int dif)
{
	return __inet_lookup_established(net, hashinfo, saddr, sport, daddr,
					 ntohs(dport), dif);
}

static inline struct sock *__inet_lookup(struct net *net,
					 struct inet_hashinfo *hashinfo,
					 const __be32 saddr, const __be16 sport,
					 const __be32 daddr, const __be16 dport,
					 const int dif)
{
	u16 hnum = ntohs(dport);
	struct sock *sk = __inet_lookup_established(net, hashinfo,
				saddr, sport, daddr, hnum, dif);

	return sk ? : __inet_lookup_listener(net, hashinfo, daddr, hnum, dif);
}

static inline struct sock *inet_lookup(struct net *net,
				       struct inet_hashinfo *hashinfo,
				       const __be32 saddr, const __be16 sport,
				       const __be32 daddr, const __be16 dport,
				       const int dif)
{
	struct sock *sk;

	local_bh_disable();
	sk = __inet_lookup(net, hashinfo, saddr, sport, daddr, dport, dif);
	local_bh_enable();

	return sk;
}

static inline struct sock *__inet_lookup_skb(struct inet_hashinfo *hashinfo,
					     struct sk_buff *skb,
					     const __be16 sport,
					     const __be16 dport)
{
	struct sock *sk;
	const struct iphdr *iph = ip_hdr(skb);

	if (unlikely(sk = skb_steal_sock(skb)))
		return sk;
	else
		return __inet_lookup(dev_net(skb_dst(skb)->dev), hashinfo,
				     iph->saddr, sport,
				     iph->daddr, dport, inet_iif(skb));
}

extern int __inet_hash_connect(struct inet_timewait_death_row *death_row,
		struct sock *sk,
		u32 port_offset,
		int (*check_established)(struct inet_timewait_death_row *,
			struct sock *, __u16, struct inet_timewait_sock **),
		int (*hash)(struct sock *sk, struct inet_timewait_sock *twp));

extern int inet_hash_connect(struct inet_timewait_death_row *death_row,
			     struct sock *sk);
#endif /* _INET_HASHTABLES_H */
