/*
 * NET		Generic infrastructure for INET connection oriented protocols.
 *
 *		Definitions for inet_connection_sock 
 *
 * Authors:	Many people, see the TCP sources
 *
 * 		From code originally in TCP
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _INET_CONNECTION_SOCK_H
#define _INET_CONNECTION_SOCK_H

#include <linux/compiler.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/poll.h>

#include <net/inet_sock.h>
#include <net/request_sock.h>

#define INET_CSK_DEBUG 1

/* Cancel timers, when they are not required. */
#undef INET_CSK_CLEAR_TIMERS

struct inet_bind_bucket;
struct tcp_congestion_ops;

/*
 * Pointers to address related TCP functions
 * (i.e. things that depend on the address family)
 */

/*
 * ��װ��һ���봫�����صĲ�����������������㷢�͵Ľӿڡ�������
 * setsockopt�ӿڵȡ�TCP�е�ʵ��Ϊipv4_specific
*/
struct inet_connection_sock_af_ops {
    /* �Ӵ����������㴫�ݵĽӿڣ�TCP������Ϊip_queue_xmit*/
	int	    (*queue_xmit)(struct sk_buff *skb);

	 /* ���㴫����ײ�У��ͺ�����TCP������Ϊtcp_v4_send_check*/
	void	    (*send_check)(struct sock *sk, struct sk_buff *skb);

	/* 
     * ����˴�����ƿ黹û��·�ɻ����Ϊ������ƿ�ѡ��·�ɻ����
     * TCP������Ϊinet_sk_rebuild_header
     */
	int	    (*rebuild_header)(struct sock *sk);

	/* ������������ӿڣ�TCP������Ϊtcp_v4_conn_request*/
	int	    (*conn_request)(struct sock *sk, struct sk_buff *skb);

   /*
     * ������������ֺ󣬵��ô˽ӿ�������һ���µ��׽��֣���TCP�г�ʼ��
     * Ϊtcp_v4_syn_recv_sock
     */
	struct sock *(*syn_recv_sock)(struct sock *sk, struct sk_buff *skb,
				      struct request_sock *req,
				      struct dst_entry *dst);

    /*
     * ������tw_recycle����£��ر��׽���ʱ����¼���ʱ�����Ϣ���Զ���Ϣ
     * ������С�TCP������Ϊtcp_v4_remember_stamp
     */
	int	    (*remember_stamp)(struct sock *sk);
	/* ��IPv4��ΪIP�ײ��ĳ���(����IP�ײ�ѡ��)*/
	u16	    net_header_len;

	 /* IP�׽��ֵ�ַ���ȣ���IPv4�о���sockaddr_in�ṹ�ĳ���*/
	u16	    sockaddr_len;
	int	    (*setsockopt)(struct sock *sk, int level, int optname, 
				  char __user *optval, unsigned int optlen);
	int	    (*getsockopt)(struct sock *sk, int level, int optname, 
				  char __user *optval, int __user *optlen);
#ifdef CONFIG_COMPAT
	int	    (*compat_setsockopt)(struct sock *sk,
				int level, int optname,
				char __user *optval, unsigned int optlen);
	int	    (*compat_getsockopt)(struct sock *sk,
				int level, int optname,
				char __user *optval, int __user *optlen);
#endif

    /*
     * ��IP�׽��ֵ�ַ�ṹ�еĵ�ַ��Ϣ���Ƶ�������ƿ��У�TCP��Ϊ
     * inet_csk_addr2sockaddr��ʵ���ϸýӿڲ�δʹ��
     */
	void	    (*addr2sockaddr)(struct sock *sk, struct sockaddr *);
	int	    (*bind_conflict)(const struct sock *sk,
				     const struct inet_bind_bucket *tb);
};

/** inet_connection_sock - INET connection oriented sock
 *
 * @icsk_accept_queue:	   FIFO of established children 
 * @icsk_bind_hash:	   Bind node
 * @icsk_timeout:	   Timeout
 * @icsk_retransmit_timer: Resend (no ack)
 * @icsk_rto:		   Retransmit timeout
 * @icsk_pmtu_cookie	   Last pmtu seen by socket
 * @icsk_ca_ops		   Pluggable congestion control hook
 * @icsk_af_ops		   Operations which are AF_INET{4,6} specific
 * @icsk_ca_state:	   Congestion control state
 * @icsk_retransmits:	   Number of unrecovered [RTO] timeouts
 * @icsk_pending:	   Scheduled timer event
 * @icsk_backoff:	   Backoff
 * @icsk_syn_retries:      Number of allowed SYN (or equivalent) retries
 * @icsk_probes_out:	   unanswered 0 window probes
 * @icsk_ext_hdr_len:	   Network protocol overhead (IP/IPv6 options)
 * @icsk_ack:		   Delayed ACK control data
 * @icsk_mtup;		   MTU probing control data
 */
/*�׽����б��κͶԶ˵������Ϣ������inet_sock�У����Ա�֤��Э���޹أ�����Э�鶼�øýṹ�洢���ص�ַ�˿ںͶԶ˵�ַ�˿��Ѿ�����״̬��
��tcpΪ����struct tcp_sock����struct inet_connection_sock,inet_connection_sock���� struct inet_sock��struct inet_sock����struct sock, struct sock������ struct sock_common��������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct tcp_sock)
��udpΪ����struct udp_sock����struct inet_connection_sock inet_connection_sock����struct inet_sock��struct inet_sock����struct sock�� struct sock������ struct sock_common����������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct udp_sock)
��rawΪ����struct raw_sock����struct inet_connection_sock inet_connection_sock����struct inet_sock��struct inet_sock����struct sock�� struct sock������ struct sock_common����������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct raw_sock)
tcp_sock->inet_connection_sock->inet_sock->sock(socket�����skָ��sock)
*/
/*��tcpΪ����struct tcp_sock����struct inet_connection_sock,inet_connection_sock���� struct inet_sock��struct inet_sock����struct sock, struct sock������ struct sock_common��������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct tcp_sock)
��udpΪ����struct udp_sock����struct inet_connection_sock inet_connection_sock����struct inet_sock��struct inet_sock����struct sock�� struct sock������ struct sock_common����������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct udp_sock)
��rawΪ����struct raw_sock����struct inet_connection_sock inet_connection_sock����struct inet_sock��struct inet_sock����struct sock�� struct sock������ struct sock_common����������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct raw_sock)
//tcp_timewait_sock����inet_timewait_sock��inet_timewait_sock����sock_common
tcp_request_sock����inet_request_sock��inet_request_sock����request_sock*/

/*
 * inet_connection_sock�ṹ��֧�������������Ե������飬����
 * IPv4Э����TCP���ƿ�Ļ�������inet_sock�ṹ�Ļ����ϼ�����
 * ֧�����ӵ����ԡ���net_sock���������������� ȷ�� �ش���ѡ��
 */
struct inet_connection_sock {
    /* inet_sock has to be the first member! */
    struct inet_sock      icsk_inet;
    /*
     * ��TCP�������յ��ͻ��˵����������
     * �ᴴ��һ���ͻ����׽��ִ��
     * ��icsk_accept_queue�����У��ȴ�Ӧ�ó���
     * ����accept()���ж�ȡ
     */ 
//����ͻ��������ӵ�������ô�ں���tcp_v4_conn_request�е�inet_csk_reqsk_queue_hash_add��ӵ�icsk_accept_queue�С�ֱ��Ӧ�ò�������accept������ϵͳ����
//��ִ�е�inet_csk_accept�������Ӷ������Ӷ�����ȡ����������ӵ���Ϣ��Ҳ���Ƕ�����������һ����ÿȡһ����һ������������accept��struct sock�ṹ�Ǵ�������ȥ�����ģ������Լ�������
    struct request_sock_queue icsk_accept_queue;  
    /*
     * ָ����֮�󶨵ı��ض˿���Ϣ���ڰ󶨹����б�����  
     *///inet_bind_bucketҲ��ӵ�sock��->sk_bind_node��
    struct inet_bind_bucket   *icsk_bind_hash; //�洢inet_bind_bucket�ṹ  inet_bind_hash
    /*
     * ���TCP����ָ��ʱ����û���յ�ACK������Ϊ����ʧ�ܣ��������ش���
     * ��ʱʱ��jiffies+icsk_rto������jiffies+icsk_rto֮������ش�
     */
    unsigned long         icsk_timeout;
    /*
     * ͨ����ʶ��icsk_pending�������ش���ʱ���ͳ�����ʱ��(����̽��)��ʵ�֡��ڳ�ʱ
     * ʱ����û�н��յ���Ӧ��ACK�λᷢ���ش��������ӶԷ�ͨ����մ���Ϊ
     * 0ʱ������������ʱ��.
     * ���õĶ�ʱ������Ϊtcp_write_timer().
     */ 
    struct timer_list     icsk_retransmit_timer;//inet_csk_init_xmit_timers
    /* �����ӳٷ���ACK�εĶ�ʱ�� 
     * ���õĶ�ʱ������Ϊtcp_delack_timer(),�μ�tcp_v4_init_sock()��
     * tcp_init_xmit_timers().
     * */
    struct timer_list     icsk_delack_timer;//inet_csk_init_xmit_timers  ����tcp_send_delayed_ack�����ö�ʱ��
    /* 
     * ��ʱ�ش���ʱ�䣬��ʼֵΪTCP_TIMEOUT_INIT,������ʱ�䳬����ֵʱ��
     * ��Ϊ����ʧ�ܡ���Ҫע����ǣ���ʱ�ش���ʱ���Ǹ��ݵ�ǰ��������
     * ��̬����ġ���tcp_v4_init_sock()�г�ʼ����
     * ���¼�����̲μ�tcp_clean_rtx_queue()��tcp_ack_update_rtt()��
     * 
     * ���ֵ��TCP_RTO_MAX����Сֵ��TCP_RTO_MIN
     */
    __u32             icsk_rto;
    /* ���һ�θ��µ�·��MTU(PMTU)  �ɽṹdst_entry�е�metrics[RTAX_MAX]��ʼ�������� */ //·��MTU���ֹ����У�������ͱ仯����¡���tcp_sync_mss
    __u32             icsk_pmtu_cookie;
    /*
     * icsk_ca_ops��ָ��ʵ��ĳ��ӵ�������㷨��ָ�롣��ĿǰΪֹ��Linux֧��
     * ����ӵ�������㷨�����û�Ҳ���Ա�д�Լ���ӵ�����ƻ���ģ����ص��ں�
     * �У��μ�TCP_CONGESTIONѡ��
     * 
     * Ĭ���������tcp_init_congestion_ops���μ�
     * �ڳ�ʼ��ʱ�������Ϊtcp_init_congestion_ops(�μ�tcp_v4_init_sock())��������
     * ���ӽ��������л����tcp_init_congestion_control()ѡ��tcp_cong_list�����е�һ��
     * ����ʹ�õ�ӵ�������㷨ģ�顣��centos�£�Ĭ����cubic��
     * ֮����Ĭ����cubic������Ϊ�ڱ����ں˵�ʱ��������CONFIG_TCP_CONG_CUBIC
     * ѡ��μ�/boot/config-2.6.32-358.el6.x86_64.
     */
    const struct tcp_congestion_ops *icsk_ca_ops; //����ͨ��Ӧ�ó���setsockops����tcp_setsockopt
    /*
     * TCP��һ�������ӿڼ���������IP�㷢�͵Ľӿڡ�TCP��setsockopt�ӿڵȡ�
     * ����TCPЭ��ģ��ʱ����tcp_v4_init_sock()�б���ʼ��Ϊinet_connection_sock_af_ops
     * �ṹ���ͳ���ipv4_specific
     */ ////��tcp_prot->init�б���ֵ��inet_connection_sock->icsk_af_ops
    const struct inet_connection_sock_af_ops *icsk_af_ops;
    /*
     * ����PMTUͬ������MSS����ָ�롣����TCPЭ��ģ��ʱ����tcp_v4_init_sock()��
     * ����ʼ��Ϊtcp_sync_mss().
     */
    unsigned int          (*icsk_sync_mss)(struct sock *sk, u32 pmtu);
    /* ӵ������״̬*/
    __u8              icsk_ca_state;
    /* ��¼��ʱ�ش��Ĵ���,Ҳ�����ش��Ķεĸ���*/
    __u8              icsk_retransmits;
    /*
     * ��ʶԤ���Ķ�ʱ���¼������ܵ�ȡֵΪICSK_TIME_RETRANS�ȡ�ʵ���ϣ�ֻȡ
     * ICSK_TIME_RETRANS��ICSK_TIME_PROBE0����Ϊ�����ֶ�ʱ������ʱʹ�õ���
     * ͬһ����ʱ���������Ҫ�������ʶ����������ʹ�õ��ĸ���ʱ�����ش�����
     * ����̽��ʱ�����inet_csk_reset_xmit_timer()���ø��ֶ�
     */
    __u8              icsk_pending;
    /* �������������ʱ������һ���趨ֵ��ָ���˱��㷨ָ�����ڴ��ͳ�ʱʱ����� *///����̽�ⶨʱ������tcp_send_probe0���������������ֶ����ش�ʱ��仯
    __u8              icsk_backoff;
    /*
     * �ڽ���TCP����ʱ����������Է���SYN��SYN+ACK�εĴ������μ�TCP_SYNCNTѡ��
     * ��sysctl_tcp_synack_retriesϵͳ����
     */
    __u8              icsk_syn_retries;
    /*
     * ������ʱ���򱣻ʱ�������Է��ͳ�ȥ��δ��ȷ�ϵ�TCP����Ŀ�����յ�ACK֮��
     * ����
     */
    __u8              icsk_probes_out;
    /* IP�ײ���ѡ��ֳ��� */
    __u16             icsk_ext_hdr_len;
    struct {
        /*
         * ��ʶ��ǰ��Ҫ����ȷ�ϵĽ����̶Ⱥ�״̬�����ܵ�ȡֵΪICSK_ACK_SCHED�ȡ�
         * �����ݴ��ں˿ռ临�Ƶ��û��ռ�ʱ�����״̬�������Ҫ����������ȷ�ϣ�
         * ���ڼ���rcv_mssʱ����������������״̬������pending�ǰ�λ�洢�ģ�
         * ��˶��״̬����ͬʱ����
         */
        __u8          pending;   /* ACK is pending             */
        /*
         * ��ʶ�ڿ��ٷ���ȷ��ģʽ�У����Կ��ٷ���ACK�ε���������pingpong
         * һͬ��Ϊ�ж��Ƿ��ڿ��ٷ���ȷ��ģʽ�µ����������Ҫ��ʱ����ȷ��
         * ���������ʱ����ȷ��ģʽ�� 
         */
        __u8          quick;     /* Scheduled number of quick acks     */
        /*
         * ��ʶ���û��ֹ����ȷ��ģʽ��ͨ��TCP_QUICKACKѡ�����������ֵ������
         * �μ�TCP_QUICKѡ�ȡֵ:
         * 0: ����ʱACK�εķ��ͣ����ǽ��п��ٷ���
         * 1: ������ʱ����ACK
         * �ڿ���ȷ��ģʽ�£�����������ACK.����TCP��������У������Ҫ�������
         * ������ģʽ���У�Ҳ����˵�������־�����ò��������Եģ���ֻ���ڵ�ʱ
         * ����/��ֹ����ȷ��ģʽ������֮�󣬸�����ʱȷ�ϳ�ʱ�����ݴ�������أ�
         * �п��ܻ��ٴν�����뿪����ȷ��ģʽ
         */
        __u8          pingpong;  /* The session is interactive   ��tcp_delack_timer      */
        /*
         * ���жϺ��û������ǲ���ͬʱռ�������׽��ֵģ��������׽����ѱ��û�
         * ��������������ʱ��ʱACK��ʱ�������������߼���˵��ʱӦ�÷���ACK,��
         * �����׽��ֱ��û����������˲��ܷ��ʣ����ֻ����blocked��־λ1����ʾ
         * "�׽��ֱ��û����������ˣ��ֲ��ܷ���ACK������л�����������ACK"����Щ
         * ����������յ�����֮��ͽ����ݸ��Ƶ��û��ռ�֮��
         */
        __u8          blocked;   /* Delayed ACK was blocked by socket lock */
        /*
         * ����������ʱȷ�ϵĹ�ֵ���ڽ��յ�TCP��ʱ����ݱ������ϴν��յ�ʱ����
         * ��������ֵ������������ʱȷ�϶�ʱ��ʱҲ���������������ֵ
         */
        __u32         ato;       /* Predicted tick of soft clock       */
        /* ��ǰ����ʱȷ��ʱ�䣬��ʱʱ�ᷢ��ACK*/
        unsigned long     timeout;   /* Currently scheduled timeout        */
        /* ��ʶ���һ�ν��յ����ݰ���ʱ�� */
        __u32         lrcvtime;  /* timestamp of last received data packet */
        /* ���һ�����յ��Ķεĳ��ȣ���������rcv_mss */
        __u16         last_seg_size; /* Size of last incoming segment      */
        /* ��������յ��μ������MSS����Ҫ����ȷ���Ƿ�ִ����ʱȷ��*/
        __u16         rcv_mss;   /* MSS used for delayed ACK decisions     */ 
    } icsk_ack; /* ��ʱȷ�Ͽ������ݿ� */
    struct {
        /* ��ʶ�Ƿ�����·��MTU���� */
        int       enabled;

        /* Range of MTUs to search */
        /* ���ڱ�ʶ����·��MTU���ֵ������������ */
        int       search_high;
        int       search_low;

        /* Information on the current probe. */
        /*
         * Ϊ��ǰ·��MTU̽��εĳ��ȣ�Ҳ�����ж�·��MTU̽���Ƿ���ɡ�����
         * �ɹ�����ʧ�ܣ�·��MTU̽����ɺ��ֵ������ʼ��Ϊ0
         */
        int       probe_size;
    } icsk_mtup; /* �й�·��MTU���ֵĿ������ݿ飬��tcp_mtup_init()�б���ʼ��*/
    /*
     * �洢�����й�TCPӵ�������㷨��˽�в�������Ȼ���ﶨ�����16���޷������Σ�
     * ����ʵ�ʴ洢ʱ��������ӵ���㷨����
     */
    u32           icsk_ca_priv[16];
#define ICSK_CA_PRIV_SIZE	(16 * sizeof(u32))
};


#define ICSK_TIME_RETRANS	1	/* Retransmit timer */
#define ICSK_TIME_DACK		2	/* Delayed ack timer */
#define ICSK_TIME_PROBE0	3	/* Zero window probe timer */
#define ICSK_TIME_KEEPOPEN	4	/* Keepalive timer */

static inline struct inet_connection_sock *inet_csk(const struct sock *sk)
{
	return (struct inet_connection_sock *)sk;
}

static inline void *inet_csk_ca(const struct sock *sk)
{
	return (void *)inet_csk(sk)->icsk_ca_priv;
}

extern struct sock *inet_csk_clone(struct sock *sk,
				   const struct request_sock *req,
				   const gfp_t priority);

enum inet_csk_ack_state_t {
    /*
        * ��ACK��Ҫ���ͣ����������ͻ�����ʱ���ͣ�����Ҫ��������־��Ҳ���ܷ�
        * ����ȷ�ϵ�ǰ�ᡣ�ڽ��յ��и��ɵ�TCP�κ󣬻����øñ�־
        */
	ICSK_ACK_SCHED	= 1, //inet_csk_schedule_ack����λ    ////���������־������һ��ack�ӳٶ�ʱ��ʱ�䵽��ᷢ��ack   ��tcp_delack_timer
    /* ��ʱ����ACK��ʱ���Ѿ����� */
	ICSK_ACK_TIMER  = 2,
    /* ֻҪ��ACK��Ҫ���ͣ�����pingpongΪ0ʱ��ACK������������ */
	ICSK_ACK_PUSHED = 4,
	/* ֻҪ��ACK��Ҫ���ͣ��������������ͣ������Ƿ��ڿ��ٷ���ģʽ*/
	ICSK_ACK_PUSHED2 = 8
};

extern void inet_csk_init_xmit_timers(struct sock *sk,
				      void (*retransmit_handler)(unsigned long),
				      void (*delack_handler)(unsigned long),
				      void (*keepalive_handler)(unsigned long));
extern void inet_csk_clear_xmit_timers(struct sock *sk);

//���������־������һ��ack�ӳٶ�ʱ��ʱ�䵽��ᷢ��ack   ��tcp_delack_timer
static inline void inet_csk_schedule_ack(struct sock *sk)
{
	inet_csk(sk)->icsk_ack.pending |= ICSK_ACK_SCHED;
}

static inline int inet_csk_ack_scheduled(const struct sock *sk)
{
	return inet_csk(sk)->icsk_ack.pending & ICSK_ACK_SCHED; //tcp_send_delayed_ack
}

static inline void inet_csk_delack_init(struct sock *sk)
{
	memset(&inet_csk(sk)->icsk_ack, 0, sizeof(inet_csk(sk)->icsk_ack));
}

extern void inet_csk_delete_keepalive_timer(struct sock *sk);
extern void inet_csk_reset_keepalive_timer(struct sock *sk, unsigned long timeout);

#ifdef INET_CSK_DEBUG
extern const char inet_csk_timer_bug_msg[];
#endif

static inline void inet_csk_clear_xmit_timer(struct sock *sk, const int what)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	
	if (what == ICSK_TIME_RETRANS || what == ICSK_TIME_PROBE0) {
		icsk->icsk_pending = 0;
#ifdef INET_CSK_CLEAR_TIMERS
		sk_stop_timer(sk, &icsk->icsk_retransmit_timer);
#endif
	} else if (what == ICSK_TIME_DACK) {
		icsk->icsk_ack.blocked = icsk->icsk_ack.pending = 0;
#ifdef INET_CSK_CLEAR_TIMERS
		sk_stop_timer(sk, &icsk->icsk_delack_timer);
#endif
	}
#ifdef INET_CSK_DEBUG
	else {
		pr_debug("%s", inet_csk_timer_bug_msg);
	}
#endif
}

/*
 *	Reset the retransmission timer
 */
static inline void inet_csk_reset_xmit_timer(struct sock *sk, const int what,
					     unsigned long when,
					     const unsigned long max_when)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	if (when > max_when) {
#ifdef INET_CSK_DEBUG
		pr_debug("reset_xmit_timer: sk=%p %d when=0x%lx, caller=%p\n",
			 sk, what, when, current_text_addr());
#endif
		when = max_when;
	}
    //??����:�ش���ʱ����̽�ⶨʱ��Ϊʲô����Ķ�ʱ�������ǰ��sk_reset_timer�Ķ�ʱ�����������أ���ǰ��Ķ�ʱ�����ǲ���������?
    //��Ϊ�������ش���ʱ���Ĺ����У���ʾ�Զ˴����ǲ�Ϊ0�ģ�������̽�ⶨʱ����ʱ��Ҳ�����Ƿ���δ��ȷ�ϵ�ack�ȡ����������ǲ�����ͬʱ���ڵ�
	if (what == ICSK_TIME_RETRANS || what == ICSK_TIME_PROBE0) { 
		icsk->icsk_pending = what;
		icsk->icsk_timeout = jiffies + when;
		sk_reset_timer(sk, &icsk->icsk_retransmit_timer, icsk->icsk_timeout);
	} else if (what == ICSK_TIME_DACK) {
		icsk->icsk_ack.pending |= ICSK_ACK_TIMER;
		icsk->icsk_ack.timeout = jiffies + when;
		sk_reset_timer(sk, &icsk->icsk_delack_timer, icsk->icsk_ack.timeout);
	}
#ifdef INET_CSK_DEBUG
	else {
		pr_debug("%s", inet_csk_timer_bug_msg);
	}
#endif
}

extern struct sock *inet_csk_accept(struct sock *sk, int flags, int *err);

extern struct request_sock *inet_csk_search_req(const struct sock *sk,
						struct request_sock ***prevp,
						const __be16 rport,
						const __be32 raddr,
						const __be32 laddr);
extern int inet_csk_bind_conflict(const struct sock *sk,
				  const struct inet_bind_bucket *tb);
extern int inet_csk_get_port(struct sock *sk, unsigned short snum);

extern struct dst_entry* inet_csk_route_req(struct sock *sk,
					    const struct request_sock *req);

static inline void inet_csk_reqsk_queue_add(struct sock *sk,
					    struct request_sock *req,
					    struct sock *child)
{
	reqsk_queue_add(&inet_csk(sk)->icsk_accept_queue, req, sk, child);
}

extern void inet_csk_reqsk_queue_hash_add(struct sock *sk,
					  struct request_sock *req,
					  unsigned long timeout);

static inline void inet_csk_reqsk_queue_removed(struct sock *sk,
						struct request_sock *req)
{
	if (reqsk_queue_removed(&inet_csk(sk)->icsk_accept_queue, req) == 0)
		inet_csk_delete_keepalive_timer(sk);
}

static inline void inet_csk_reqsk_queue_added(struct sock *sk,
					      const unsigned long timeout)
{
	if (reqsk_queue_added(&inet_csk(sk)->icsk_accept_queue) == 0) //�������sk�ĵ�һ������������Ҫ������ʱ��
		inet_csk_reset_keepalive_timer(sk, timeout);
}

static inline int inet_csk_reqsk_queue_len(const struct sock *sk)
{
	return reqsk_queue_len(&inet_csk(sk)->icsk_accept_queue);
}

static inline int inet_csk_reqsk_queue_young(const struct sock *sk)
{
	return reqsk_queue_len_young(&inet_csk(sk)->icsk_accept_queue);
}

static inline int inet_csk_reqsk_queue_is_full(const struct sock *sk)
{
	return reqsk_queue_is_full(&inet_csk(sk)->icsk_accept_queue);
}

static inline void inet_csk_reqsk_queue_unlink(struct sock *sk,
					       struct request_sock *req,
					       struct request_sock **prev)
{
	reqsk_queue_unlink(&inet_csk(sk)->icsk_accept_queue, req, prev);
}

static inline void inet_csk_reqsk_queue_drop(struct sock *sk,
					     struct request_sock *req,
					     struct request_sock **prev)
{
	inet_csk_reqsk_queue_unlink(sk, req, prev);
	inet_csk_reqsk_queue_removed(sk, req);
	reqsk_free(req);
}

extern void inet_csk_reqsk_queue_prune(struct sock *parent,
				       const unsigned long interval,
				       const unsigned long timeout,
				       const unsigned long max_rto);

extern void inet_csk_destroy_sock(struct sock *sk);

/*
 * LISTEN is a special case for poll..
 */
static inline unsigned int inet_csk_listen_poll(const struct sock *sk)
{
	return !reqsk_queue_empty(&inet_csk(sk)->icsk_accept_queue) ?
			(POLLIN | POLLRDNORM) : 0;
}

extern int  inet_csk_listen_start(struct sock *sk, const int nr_table_entries);
extern void inet_csk_listen_stop(struct sock *sk);

extern void inet_csk_addr2sockaddr(struct sock *sk, struct sockaddr *uaddr);

extern int inet_csk_compat_getsockopt(struct sock *sk, int level, int optname,
				      char __user *optval, int __user *optlen);
extern int inet_csk_compat_setsockopt(struct sock *sk, int level, int optname,
				      char __user *optval, unsigned int optlen);
#endif /* _INET_CONNECTION_SOCK_H */
