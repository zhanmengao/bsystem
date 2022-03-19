/*
 * NET		Generic infrastructure for Network protocols.
 *
 *		Definitions for request_sock 
 *
 * Authors:	Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *
 * 		From code originally in include/net/tcp.h
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _REQUEST_SOCK_H
#define _REQUEST_SOCK_H

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/bug.h>

#include <net/sock.h>

struct request_sock;
struct sk_buff;
struct dst_entry;
struct proto;

/* empty to "strongly type" an otherwise void parameter.
 */
struct request_values {
};

/*
 * ������������ĺ���ָ������а������ڷ���SYN+ACK�Ρ�
 * ACK�Ρ�RST�εĺ�����TCP�У�ָ���ʵ��Ϊtcp_request_sock_ops�� timewaitΪtcp_timewait_sock_ops
 */
struct request_sock_ops {
	/*
	 * ����Э����
	 */
	int		family;
	/*
	 * obj_size��tcp_request_sock�ṹ���ȣ����ڴ�������
	 * ���������ĸ��ٻ���slab���û�����ע�ᴫ���Э��
	 * ʱ�������μ�proto_register()��
	 */
	int		obj_size;
	struct kmem_cache	*slab;
	char		*slab_name;
	/*
	 * ����SYN+ACK�εĺ���ָ�룬TCP��Ϊtcp_v4_send_synack()
	 */
	int		(*rtx_syn_ack)(struct sock *sk,
				       struct request_sock *req);
	/*
	 * ����ACK�εĺ���ָ�룬TCP��Ϊtcp_v4_reqsk_send_ack()
	 */
	void		(*send_ack)(struct sock *sk, struct sk_buff *skb,
				    struct request_sock *req);
	/*
	 * ����RST�εĺ���ָ�룬TCP��Ϊtcp_v4_send_reset()
	 */
	void		(*send_reset)(struct sock *sk,
				      struct sk_buff *skb);
	/*
	 * �������������ͷ����������ʱ�����ã����������ͷ���Դ��TCP��
	 * Ϊtcp_v4_reqsk_destructor()��
	 */
	void		(*destructor)(struct request_sock *req);
};

struct request_sock_ops1 {
	int		family;
	int		obj_size;
	struct kmem_cache	*slab;
	char		*slab_name;
	int		(*rtx_syn_ack)(struct sock *sk,
				       struct request_sock *req,
				       struct request_values *rvp);
	void		(*send_ack)(struct sock *sk, struct sk_buff *skb,
				    struct request_sock *req);
	void		(*send_reset)(struct sock *sk,
				      struct sk_buff *skb);
	void		(*destructor)(struct request_sock *req);
	void		(*syn_ack_timeout)(struct sock *sk,
					   struct request_sock *req);
};

/* struct request_sock - mini sock to represent a connection request
 */   
 //tcp_request_sock����inet_request_sock��inet_request_sock����request_sock
 /*
 * ��Ҫ�����ԶԶ˵�MSS�����˵Ľ��մ��ڴ�С�Լ��������Ӳ�������Ϣ������
 * ��ʱʱ���
  tcp_request_sock����inet_request_sock��inet_request_sock����request_sock
 */  //request_sock_queue�е�listen_sock�����hash��syn_table�д洢������ṹ
 struct request_sock {
	/*
	 * ������request_sock�ṹʵ�����ӳ�����
	 */
	struct request_sock		*dl_next; /* Must be first member! */
	/*
	 * �ͻ��������������ͨ���MSS�������ͨ�棬��Ϊ��ʼֵ��
	 * ��RFC�н����536
	 */
	u16				mss;
	/*
	 * ����SYN+ACK�εĴ������ڴﵽϵͳ�趨������ʱ��ȡ�����Ӳ���
	 */
	u8				retrans;
	u8				cookie_ts; /* syncookie: encode tcpopts in timestamp */
	/* The following two fields can be easily recomputed I think -AK */
	/*
	 * ��ʶ���˵����ͨ�洰�ڣ�������SYN+ACK��ʱ�����ֵ
	 */
	u32				window_clamp; /* window clamp at creation time */
	/*
	 * ��ʶ�����ӽ���ʱ���˽��մ��ڴ�С����ʼ��Ϊ0��������
	 * SYN+ACK��ʱ�����ֵ��
	 */
	u32				rcv_wnd;	  /* rcv_wnd offered first time */
	/*
	 * ��һ����Ҫ���͵�ACK�е�ʱ���ֵ����һ�����������ACKȷ����ŵ�
	 * �ε���ʱ���ö��е�ʱ�����������ts_recent��
	 */
	u32				ts_recent;
	/*
	 * ����˽��յ��������󣬲�����SYN+ACK����ΪӦ��󣬵ȴ�
	 * �ͻ���ȷ�ϵĳ�ʱʱ�䡣һ����ʱ�������·���SYN+ACK�Σ�
	 * ֱ�����ӽ������ط������ﵽ����
	 */
	unsigned long			expires;
	/*
	 * ������������ĺ���ָ���TCP��ָ��
	 * tcp_request_sock_ops
	 */
	const struct request_sock_ops	*rsk_ops;
	/*
	 * ָ���Ӧ״̬�Ĵ�����ƿ顣�����ӽ���֮ǰ��Ч���������ֺ��
	 * ������Ӧ�Ĵ�����ƿ飬����ʱ���������Ҳ�������ʷʹ����
	 * ����accept()�������������ȡ�߲��ͷ�(�ͷŵ���request_sockʵ����
	 * ����sockʵ��)
	 */
	struct sock			*sk;
	u32				secid;
	u32				peer_secid;
};
struct request_sock1 {
	struct request_sock		*dl_next; /* Must be first member! */
	u16				mss;
	u8				retrans;
	u8				cookie_ts; /* syncookie: encode tcpopts in timestamp */
	/* The following two fields can be easily recomputed I think -AK */
	u32				window_clamp; /* window clamp at creation time */
	u32				rcv_wnd;	  /* rcv_wnd offered first time */
	u32				ts_recent;
	unsigned long			expires;
	const struct request_sock_ops	*rsk_ops;
	struct sock			*sk;
	u32				secid;
	u32				peer_secid;
};

static inline struct request_sock *reqsk_alloc(const struct request_sock_ops *ops)
{
	struct request_sock *req = kmem_cache_alloc(ops->slab, GFP_ATOMIC);

	if (req != NULL)
		req->rsk_ops = ops;

	return req;
}

static inline void __reqsk_free(struct request_sock *req)
{
	kmem_cache_free(req->rsk_ops->slab, req);
}

static inline void reqsk_free(struct request_sock *req)
{
	req->rsk_ops->destructor(req);
	__reqsk_free(req);
}

extern int sysctl_max_syn_backlog;

/** struct listen_sock - listen state
 *
 * @max_qlen_log - log_2 of maximal queued SYNs/REQUESTs
 */
/*
 * listen_sock�ṹ�����洢���������(������δ����)���ýṹ
 * ��ʵ����listenϵͳ����֮��Żᱻ����      request_sock_queue��listen_optָ������
 */ //����������P748 ��ͼ�λ����
 //���ӽ�����ʱ�������ʱ����FIN_WAIT_2��ʱ���õ���ͬһ����ʱ�������ʱ��tcp_keepalive_timer
 //��������Ӧ�ó���listen��ʱ�����Կͻ��˵İ����Ӷ���洢������ṹ�е�
 //�ýṹ��reqsk_alloc���ٿռ䣬�ýṹʽ����ǰ��������������ں�TCP SYN����֧�ֿ��Բο�:http://blog.chinaunix.net/uid-20662820-id-3776090.html
struct listen_sock {   //�������˷��͵�syn+ack���ش���inet_csk_reqsk_queue_prune��ʵ�֡��ýṹ��ӵ�sk��inet_csk_reqsk_queue_hash_add�С������ӳ�ʱ���߹ر��׽��֣�ɾ����inet_csk_reqsk_queue_removed
    /*
     * ʵ�ʷ�����������SYN�������ӵ�request_sock�ṹ����ĳ��ȣ����ֵ
     * ��ֵΪnr_table_entries��2Ϊ�׵Ķ�����Ҳ���������syn_table[]�����С
     */
    u8          max_qlen_log;
    /* 3 bytes hole, try to use */
    /*
     * ��ǰ�����������Ŀ   ������������Ŀ��
     */
    int         qlen;
    /*
     * ��ǰδ�ش���SYN+ACK�ε��������Ŀ�����ÿ�ν������Ӷ���˳����
     * �������ֵĶ�û���ش�����qlen_young��qlen��һ�µģ���SYN+ACK��
     * �ش�ʱ��ݼ�
     */
    int         qlen_young;
    /*
     * ������¼���ӽ�����ʱ���������´α�����ʱ�账������������
     * ɢ�б���ڡ��ڱ��δ������ʱ����ǰ����ڱ��浽���ֶ��У���
     * �´δ���ʱ�ʹӸ���ڿ�ʼ����
     */
    int         clock_hand;
    /*
     * ��������SYN�����ɢ�б��ֵ�����������ֵ��
     * reqsk_queue_alloc()���������
     */
    u32         hash_rnd; //��������ѯsyn_talbe hash���ʱ����Ҫ�õ���ֵ
    /*
     * ʵ�ʷ�����������SYN�������ӵ�request_sock�ṹ����ĳ��ȡ���max_qlen_log��Ҳ���������syn_table[]�����С
     */ //���ֵ��struct sock�е�sk_max_ack_backlog��ͬ
    u32         nr_table_entries;//�ں���reqsk_queue_alloc�и�ֵ,���İ������������������ں�TCP SYN����֧�ֿ��Բο�:http://blog.chinaunix.net/uid-20662820-id-3776090.html
    /*
     * ����listenʱ����ʹTCP����LISTEN״̬��ͬʱ��ΪSYN_RECV״̬��
     * �������ӿ��ƿ����ռ䣬����syn_tableɢ�б��С��listenϵͳ
     * ���õĲ���backlog����
     */ //ͼ�λ����ο�������P769
    //�ռ俪����reqsk_queue_alloc�У���ͷ��request_sock�ṹ������Ľڵ���tcp_request_sock�ṹ��
    //��������nr_table_entries��ָ��struct request_sock��ָ�룬Ҳ����ָ��ͷ����������������nr_table_entries����ͬ��
    struct request_sock *syn_table[0];//ͨ��clock_handָ���Ӧ��syn_table[clock_hand]ɢ�б��У���ʵ���Ǹ�hash��
    //�����������洢����request_sock�ṹ,Ҳ���ǿͻ���SYN������ʱ�򴴽��� ����������TCP����������顣�յ�SYN����tcp_v4_conn_request��ʱ�򿪱ٿռ䣬��ӵ�hash���С����������ֳɹ��ڵ������󣬻�����hash��ȡ�������뵽request_sock_queue���������������������
    //���������tcp_request_sock�ڵ�������inet_csk_reqsk_queue_hash_add  ������inet_csk_search_req��������inet_csk_reqsk_queue_prune
};


/** struct request_sock_queue - queue of request_socks
 *
 * @rskq_accept_head - FIFO head of established children
 * @rskq_accept_tail - FIFO tail of established children
 * @rskq_defer_accept - User waits for some data after accept()
 * @syn_wait_lock - serializer
 *
 * %syn_wait_lock is necessary only to avoid proc interface having to grab the main
 * lock sock while browsing the listening hash (otherwise it's deadlock prone).
 *
 * This lock is acquired in read mode only from listening_get_next() seq_file
 * op and it's acquired in write mode _only_ from code that is actively
 * changing rskq_accept_head. All readers that are holding the master sock lock
 * don't need to grab this lock in read mode too as rskq_accept_head. writes
 * are always protected from the main sock lock.
 */
/*
 * ��TCP������ƿ�����һ�����ڴ�����������(����SYN_RECV״̬�Լ�
 * �����ӵ�δ��accept�Ĵ�����ƿ�)������
 */ ////����������P748 ��ͼ�λ����
 //�ýṹ��inet_connection_sock�е�icsk_accept_queue
struct request_sock_queue {
    /*
     * rskq_accept_head��rskq_accept_tail��ʾ�����������
     * ��������ӽ������̵����������  ��������accept��ʱ��struct sock�Ǵ������������ȡ������
     */ //�Ѿ��������ӵ����ӵĽڵ���ӵ������Щ����Ľڵ���Ϣ�ṹ����tcp_request_sock����Ӧ�ó������accept�����󣬻��������ȡ�����tcp_request_sock
    struct request_sock *rskq_accept_head;// //��Ӧ�ó���accept��ʱ�򣬻����reqsk_queue_get_childȡ������´�����sock��ͬʱ����Ҫ�����ȡ����tcp_request_sock�ͷŵ�
    struct request_sock *rskq_accept_tail;
    /*
     * ����listen_opt�Լ�listen_sock�ṹ��Ա��ͬ�����ƶ�д��
     */
    rwlock_t        syn_wait_lock;
    /*
     * ��������׽���TCP���ѡ��TCP_DEFER_ACCEPT��ֵ���μ�
     * TCP_DEFER_ACCEPT
     * �����������TCP_DEFER_ACCEPTʱ�����ش�SYN+ACK�εĴ�����
     * ע��:���������TCP_DEFER_ACCEPTѡ���ʹ��rskq_defer_accept
     * ��Ϊ�����ش�����������������sysctl_tcp_synack_retries��
     * �μ�inet_csk_reqsk_queue_prune()��
     */
    u8          rskq_defer_accept;
    /* 3 bytes hole, try to pack */
    /*
     * ��ʵ���ڼ���ʱ������������Ӧ�ó���δlisten��ʱ����û�иýṹ�洢�ģ�Ҳ����û�������洢�����������hash�ռ�
     */
    struct listen_sock  *listen_opt; //��δ������ӵ�sockӦ�ö��������syn_table hash���У��Ѿ��������ӵ�request_sock���������ȡ�����뵽����ǰ���rskq_accept_head
};


extern int reqsk_queue_alloc(struct request_sock_queue *queue,
			     unsigned int nr_table_entries);

extern void __reqsk_queue_destroy(struct request_sock_queue *queue);
extern void reqsk_queue_destroy(struct request_sock_queue *queue);

static inline struct request_sock *
	reqsk_queue_yank_acceptq(struct request_sock_queue *queue)
{
	struct request_sock *req = queue->rskq_accept_head;

	queue->rskq_accept_head = NULL;
	return req;
}

static inline int reqsk_queue_empty(struct request_sock_queue *queue)
{
	return queue->rskq_accept_head == NULL;
}

static inline void reqsk_queue_unlink(struct request_sock_queue *queue,
				      struct request_sock *req,
				      struct request_sock **prev_req)
{
	write_lock(&queue->syn_wait_lock);
	*prev_req = req->dl_next;
	write_unlock(&queue->syn_wait_lock);
}

static inline void reqsk_queue_add(struct request_sock_queue *queue,
				   struct request_sock *req,
				   struct sock *parent,
				   struct sock *child)
{
	req->sk = child;
	sk_acceptq_added(parent);

	if (queue->rskq_accept_head == NULL)
		queue->rskq_accept_head = req;
	else
		queue->rskq_accept_tail->dl_next = req;

	queue->rskq_accept_tail = req;
	req->dl_next = NULL;
}

static inline struct request_sock *reqsk_queue_remove(struct request_sock_queue *queue)
{
	struct request_sock *req = queue->rskq_accept_head;

	WARN_ON(req == NULL);

	queue->rskq_accept_head = req->dl_next;
	if (queue->rskq_accept_head == NULL)
		queue->rskq_accept_tail = NULL;

	return req;
}

/*
 * �������Ӷ�����ȡ�ߵ�һ����������飬Ȼ���ɸ�������������Ѵ�������
 * ������ƿ飬�����ͷ�����ɽ������ӵ���������飬ͬʱ���¸�������ƿ�
 * ���ѽ������ӵ���Ŀ����󷵻��Ӵ�����ƿ�
 *///�ú�����inet_csk_accept�е��á�
 //���������������ֳɹ��󴴽���sk�ĺ���Ϊtcp_v4_syn_recv_sock
static inline struct sock *reqsk_queue_get_child(struct request_sock_queue *queue,
						 struct sock *parent)
{
	struct request_sock *req = reqsk_queue_remove(queue);

	//�����������ĵط���reqsk_queue_add
	struct sock *child = req->sk;//������������ɺ󣬷����������´���һ��sock,��tcp_v4_syn_recv_sock�����tcp_create_openreq_child  

	WARN_ON(child == NULL);

	sk_acceptq_removed(parent);//��Դsock(Ҳ����Ӧ�ó���socket�����ں˴�����sock�����Ӹ�����1����Ϊȡ����һ�������������child)
	__reqsk_free(req);
	return child;
}

static inline int reqsk_queue_removed(struct request_sock_queue *queue,
				      struct request_sock *req)
{
	struct listen_sock *lopt = queue->listen_opt;

	if (req->retrans == 0)
		--lopt->qlen_young;

	return --lopt->qlen;
}

static inline int reqsk_queue_added(struct request_sock_queue *queue)
{
	struct listen_sock *lopt = queue->listen_opt;
	const int prev_qlen = lopt->qlen;

	lopt->qlen_young++;
	lopt->qlen++;
	return prev_qlen;
}

static inline int reqsk_queue_len(const struct request_sock_queue *queue)
{
	return queue->listen_opt != NULL ? queue->listen_opt->qlen : 0;
}

static inline int reqsk_queue_len_young(const struct request_sock_queue *queue)
{
	return queue->listen_opt->qlen_young;
}

static inline int reqsk_queue_is_full(const struct request_sock_queue *queue)
{
    /*
	 * max_qlen_log�Ǽ������г�����2Ϊ�׵Ķ�����qlen�ǵ�ǰ��������������Ŀ��
	 * �����λ֮��Ϊ0�����ʾ�������л��пռ䣬Ϊ1��ʾ��������
	 */
	return queue->listen_opt->qlen >> queue->listen_opt->max_qlen_log;
}

static inline void reqsk_queue_hash_req(struct request_sock_queue *queue,
					u32 hash, struct request_sock *req,
					unsigned long timeout)
{
	struct listen_sock *lopt = queue->listen_opt;

	req->expires = jiffies + timeout;
	req->retrans = 0;
	req->sk = NULL;
	req->dl_next = lopt->syn_table[hash];

	write_lock(&queue->syn_wait_lock);
	lopt->syn_table[hash] = req;
	write_unlock(&queue->syn_wait_lock);
}

#endif /* _REQUEST_SOCK_H */
