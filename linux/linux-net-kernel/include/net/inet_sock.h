/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for inet_sock
 *
 * Authors:	Many, reorganised here by
 * 		Arnaldo Carvalho de Melo <acme@mandriva.com>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _INET_SOCK_H
#define _INET_SOCK_H


#include <linux/kmemcheck.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/jhash.h>
#include <linux/netdevice.h>

#include <net/flow.h>
#include <net/sock.h>
#include <net/request_sock.h>
#include <net/netns/hash.h>

/** struct ip_options - IP Options
 *
 * @faddr - Saved first hop address
 * @is_data - Options in __data, rather than skb
 * @is_strictroute - Strict source route
 * @srr_is_hit - Packet destination addr was our one
 * @is_changed - IP checksum more not valid
 * @rr_needaddr - Need to record addr of outgoing dev
 * @ts_needtime - Need to record timestamp
 * @ts_needaddr - Need to record addr of outgoing dev
 */
struct ip_options1 {
	__be32		faddr;
	unsigned char	optlen;
	unsigned char	srr;
	unsigned char	rr;
	unsigned char	ts;
	unsigned char	is_strictroute:1,
			srr_is_hit:1,
			is_changed:1,
			rr_needaddr:1,
			ts_needtime:1,
			ts_needaddr:1;
	unsigned char	router_alert;
	unsigned char	cipso;
	unsigned char	__pad2;
	unsigned char	__data[0];
};

/*
 * IPѡ����Ϣ�飬��IPѡ����У�IPѡ����Ϣ��ip_options��
 * ��õĽṹ������������ص�IPѡ��ýṹ��������SKB�У�
 * ��������SKB�����ݱ��д��ڵ�ѡ��μ�IP������Ϣ���ƿ�
 * inet_skb_parm�ṹ��Ҳ���Ե���ʹ�ã���ͨ��IP_OPTIONSѡ��
 * �ͻ�ȡ�����׽��ַ������ݱ�IP�ײ��е�IPѡ��
 */ //setsockopt(sockfd, SOL_IP,IP_OPTIONS,(void*)opt,optlen);Ӧ�ó������ͨ���������
//ip_options�ο�ip_options.c
struct ip_options { //���ݽ��յ�ʱ���SKB�л�ȡIPѡ���ֶΣ���ip_rcv_options
	/*
	 * ���ڿ���Դ·�ɻ��ϸ�Դ·��ѡ��ʱ������
	 * ��¼��һ����IP��ַ
	 */
	__be32		faddr;
	/*
	 * ��ʶIP�ײ���ѡ����ռ���ֽ���������__data֮������ݣ�
	 * ����еĻ�
	 */
	unsigned char	optlen;
	/*
	 * ��¼����Դ·�ɻ��ϸ�Դ·��ѡ����IP�ײ��е�ƫ������
	 * ��ѡ��ĵ�һ���ֽڵĵ�ַ��ȥIP�ײ��ĵ�һ���ֽڵĵ�ַ
	 */
	unsigned char	srr;
	/*
	 * ���ڼ�¼·��ѡ����IP�ײ��е�ƫ����
	 */
	unsigned char	rr;
	/*
	 * ���ڼ�¼ʱ���ѡ����IP�ײ��е�ƫ����
	 */
	unsigned char	ts;
	/*
	 * ��ʶ��IPѡ���Ƿ������ݣ�����������__data�ֶ���ʼ��
	 * �洢�ռ��ڣ���������ip_option�ṹ���档��������ݲ�ֻ
	 * ��ѡ�����ݣ���������ѡ������
	 */
	/*
	 * ��ʶ��ѡ����IPOPT_SSRR��������IPOPT_LSRR
	 */
	unsigned char	is_strictroute:1,
			/*
			 * ��ʾĿ�ĵ�ַ�Ǵ�Դ·��ѡ��ѡ����
			 */
			srr_is_hit:1,
			/*
			 * ��ʶ�Ƿ��޸Ĺ�IP�ײ������������Ҫ����
			 * ����IP�ײ�У���
			 */
			is_changed:1,
			/*
			 * ��ʶ��IPOPT_RRѡ���Ҫ��¼IP��ַ��
			 */
			rr_needaddr:1,
			/*
			 * ts_needtime��ʶ��IPOPT_TIMESTAMPѡ���Ҫ
			 * ��¼ʱ���
			 * ts_needaddr��ʶ��IPOPT_TIMESTAMPѡ���Ҫ
			 * ��¼IP��ַ
			 */
			ts_needtime:1,
			ts_needaddr:1;
	/*
	 * ��ʶIPOPT_RAѡ�·��������ѡ���ʾ·����
	 * Ӧ�ø���ϸ�ؼ��������ݰ�
	 */
	unsigned char	router_alert;
	/*
	 * ���ڼ�¼��ҵIP��ȫѡ����IP�ײ��е�ƫ����
	 */
	unsigned char	cipso;
	/*
	 * δʹ��
	 */
	unsigned char	__pad2;
	/*
	 * ��ѡ����������Ӹ��ֶο�ʼ��ʹ֮������ip_option�ṹ���棬
	 * ��಻����40B
	 */
	unsigned char	__data[0];
};


#define optlength(opt) (sizeof(struct ip_options) + opt->optlen)
/*
 * �ýṹ��Ҫ����˫���ĵ�ַ����֧�ֵ�TCPѡ���
  tcp_request_sock����inet_request_sock��inet_request_sock����request_sock
 *///request_sock_queue�е�listen_sock�����hash��syn_table�д洢������ṹ
struct inet_request_sock {
	struct request_sock	req;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	u16			inet6_rsk_offset;
#endif
	/*
	 * ���ض˿ں�
	 */
	__be16			loc_port;
	/*
	 * ����IP��ַ
	 */
	__be32			loc_addr;
	/*
	 * �Զ�IP��ַ
	 */
	__be32			rmt_addr;
	/*
	 * �Զ˶˿ں�
	 */
	__be16			rmt_port;
	kmemcheck_bitfield_begin(flags);
	/*
	 * ���ʹ����������ӣ���Ҫ��TCP�ײ���ָ���Ļ������ڴ�С
	 * ����snd_wscaleλ����Ϊ�����Ļ������ڴ�С����TCP
	 * �ײ��У��������ڴ�СֵΪ16λ�ģ���snd_wscale��ֵ���
	 * ֻ��Ϊ14�����ԣ������������ɱ���չ��30λ����Э��ջ
	 * ��ʵ��ʵ���У����Կ������ڴ�С����Ϊ5840����������Ϊ2��
	 * ��ʵ�ʵĴ��ڴ�СΪ5840<<2=23360B
	 */
	u16			snd_wscale : 4,
				/*
				 * ���մ�����������
				 */
				rcv_wscale : 4,
				/*
				 * ��ʶTCP���Ƿ����TCPʱ���ѡ��
				 */
				tstamp_ok  : 1,
				/*
				 * ��ʶ�Ƿ�֧��SACK��֧�����ѡ���ܳ�����SYN����
				 */
				sack_ok	   : 1,
				/*
				 * ��ʶ�Ƿ�֧�ִ����������ӣ����֧�ָ�ѡ��Ҳֻ�ܳ���
				 * ��SYN����
				 */
				wscale_ok  : 1,
				/*
				 * ��־�Ƿ���������ʽӵ��֪ͨ
				 */
				ecn_ok	   : 1,
				/*
				 * ��ʶ�ѽ��յ����������ֵ�ACK�Σ��������ڷ�������æ
				 * ������ԭ����δ�ܽ��������ӣ���ʱ�ɸ��ݸñ�־����
				 * ���ͻ��˷���SYN+ACK�Σ��ٴν������ӵĽ������ñ�־
				 * ������ͬʱ��sysctl_tcp_abort_on_overflow�Ŀ���
				 */
				acked	   : 1,
				no_srccheck: 1;
	kmemcheck_bitfield_end(flags);
	struct ip_options	*opt;//���������ڽ��յ�SYN�󣬻����SKB�е�ipѡ���ֶΣ���tcp_v4_save_options
};

//tcp_request_sock����inet_request_sock��inet_request_sock����request_sock
struct inet_request_sock1 {
	struct request_sock	req;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	u16			inet6_rsk_offset;
#endif
	__be16			loc_port;
	__be32			loc_addr;
	__be32			rmt_addr;
	__be16			rmt_port;
	kmemcheck_bitfield_begin(flags);
	u16			snd_wscale : 4,
				rcv_wscale : 4,
				tstamp_ok  : 1,
				sack_ok	   : 1,
				wscale_ok  : 1,
				ecn_ok	   : 1,
				acked	   : 1,
				no_srccheck: 1;
	kmemcheck_bitfield_end(flags);
	struct ip_options	*opt;
};

static inline struct inet_request_sock *inet_rsk(const struct request_sock *sk)
{
	return (struct inet_request_sock *)sk;
}

struct ip_mc_socklist;
struct ipv6_pinfo;
struct rtable;

/** struct inet_sock - representation of INET sockets
 *
 * @sk - ancestor class
 * @pinet6 - pointer to IPv6 control block
 * @inet_daddr - Foreign IPv4 addr
 * @inet_rcv_saddr - Bound local IPv4 addr
 * @inet_dport - Destination port
 * @inet_num - Local port
 * @inet_saddr - Sending source
 * @uc_ttl - Unicast TTL
 * @inet_sport - Source port
 * @inet_id - ID counter for DF pkts
 * @tos - TOS
 * @mc_ttl - Multicasting TTL
 * @is_icsk - is this an inet_connection_sock?
 * @mc_index - Multicast device index
 * @mc_list - Group array
 * @cork - info to build ip hdr on each ip frag while socket is corked
�׽����б��κͶԶ˵������Ϣ������inet_sock�У����Ա�֤��Э���޹أ�����Э�鶼�øýṹ�洢���ص�ַ�˿ںͶԶ˵�ַ�˿��Ѿ�����״̬��
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

//�׽����б��˺ͶԶ˵������Ϣ�����������棬�Ӷ����Ӧ��tcp_proc udp_proc��Э���޹أ���֤�����Э���޹أ�����Э�鶼��������ýṹ
struct inet_sock {
    /* sk and pinet6 has to be the first two members of inet_sock */
    /*
     * sock�ṹ��ͨ�õ�����������飬���ɴ�����ƿ�Ļ���
     */
    struct sock     sk;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
    /* ���֧��IPv6���ԣ�pinet6��ָ��IPv6���ƿ��ָ�� */
    struct ipv6_pinfo   *pinet6;
#endif
    /* Socket demultiplex comparisons on incoming packets. */
    /* Ŀ��IP��ַ*/
    __be32          daddr;
    /*
     * �Ѱ󶨵ı���IP��ַ����������ʱ����Ϊ������һ���ֲ���
     * ���������Ĵ�����ƿ�
     */
    __be32          rcv_saddr;
    /* Ŀ�Ķ˿ں�*/
    __be16          dport;
    /* �����ֽ���洢�ı��ض˿ڡ�*/
    __u16           num;
    /*
     * Ҳ��ʶ����IP��ַ�����ڷ���ʱʹ�á�rcv_saddr��saddr
     * ����������IP��ַ������;��ͬ
     */
    __be32          saddr;
    /*
     * �������ĵ�TTL,Ĭ��ֵΪ-1����ʾʹ��Ĭ�ϵ�TTLֵ��
     * �����IP���ݰ�ʱ��TTLֵ���ȴ������ȡ����û��
     * ���ã����·�ɻ����metric�л�ȡ���μ�IP_TTL
     * �׽���ѡ�� 
     */
    __s16           uc_ttl;
    /* 
     * ���һЩIPPROTO_IP�����ѡ��ֵ�����ܵ�ȡֵΪIP_CMSG_PKTINFO��
     */
    __u16           cmsg_flags;
    /* ָ��IP���ݰ�ѡ���ָ��*/
    struct ip_options   *opt;
    /* ��numת���ɵ������ֽ����Դ�˿ڣ�Ҳ���Ǳ��ض˿� */
    __be16          sport;
    /* һ������������ֵ����������IP�ײ��е�id�� */
    __u16           id;
    /* ��������IP���ݰ��ײ���TOS�򣬲μ�IP_TOS�׽���ѡ�� */
    __u8            tos;
    /* �������öಥ���ݰ���TTL */
    __u8            mc_ttl;
    /* 
     * ��ʶ�׽����Ƿ�����·��MTU���ֹ��ܣ���ʼֵ����ϵͳ
     * ���Ʋ���ip_no_pmtu_disc��ȷ�����μ�IP_MTU_DISCOVER
     * �׽���ѡ����ܵ�ȡֵ��IP_PMTUDISC_DO��
     */
    __u8            pmtudisc;
    /*
     * ��ʶ�Ƿ����������չ�Ŀɿ�������Ϣ��
     * �μ�IP_RECVERR�׽���ѡ��
     */
    __u8            recverr:1,
    /*
     * ��ʶ�Ƿ�Ϊ�������ӵĴ�����ƿ飬���Ƿ�Ϊ����
     * inet_connection_sock�ṹ�Ĵ�����ƿ飬��TCP�Ĵ�����ƿ�
     */
                is_icsk:1,
    /*
     * ��ʶ�Ƿ�����󶨷�������ַ���μ�IP_FREEBIND�׽���ѡ�� 
     *
     */
                freebind:1,
    /*
     * ��ʶIP�ײ��Ƿ����û����ݹ������ñ�ʶֻ����RAW�׽��֣�
     * һ�����ú�IPѡ���е�IP_TTL��IP_TOS����������
     */
                hdrincl:1,
    /* ��ʶ�鲥�Ƿ����· */
                mc_loop:1,
                transparent:1,
                mc_all:1;
    /*
     * �����鲥���ĵ������豸�����š����Ϊ0�����ʾ
     * ���Դ��κ������豸����
     */
    int         mc_index;
    /* �����鲥���ĵ�Դ��ַ */
    __be32          mc_addr;
    /* �����׽��ּ�����鲥��ַ�б� */
    struct ip_mc_socklist   *mc_list;
    struct {
        /* ���ܵ�ֵΪIPCORK_OPT��IPCORK_ALLFRAG*/
        unsigned int        flags;
        /* UDP���ݰ���ԭʼIP���ݰ���Ƭ��С */
        unsigned int        fragsize;
        /* ָ��˴η������ݰ���IPѡ�� */
        struct ip_options   *opt;
        /* �������ݰ�ʹ�õ����·�ɻ����� */
        struct dst_entry    *dst;
        /* ��ǰ���͵����ݰ������ݳ��� */
        int         length; /* Total length of all frames */
        /* ���IP���ݰ���Ŀ�ĵ�ַ */
        __be32          addr;
        /* 
         * ��flowi�ṹ������Ŀ�ĵ�ַ��Ŀ�Ķ˿ڡ�Դ��ַ��Դ�˿ڣ�
         * ����UDP����ʱ�й���Ϣ��ȡ������ 
         */
        struct flowi        fl;
    } cork; /* UDP��ԭʼIP��ÿ�η���ʱ�����һЩ��ʱ��Ϣ����UDP
     ���ݰ���ԭʼIP���ݰ���Ƭ�Ĵ�С*/
};

struct inet_sock {
	/* sk and pinet6 has to be the first two members of inet_sock */
	struct sock		sk;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	struct ipv6_pinfo	*pinet6;
#endif
	/* Socket demultiplex comparisons on incoming packets. */
	__be32			inet_daddr;
	__be32			inet_rcv_saddr;
	__be16			inet_dport;
	__u16			inet_num;
	__be32			inet_saddr;
	__s16			uc_ttl;
	__u16			cmsg_flags;
	__be16			inet_sport;
	__u16			inet_id;

	struct ip_options	*opt;
	__u8			tos;
	__u8			min_ttl;
	__u8			mc_ttl;
	__u8			pmtudisc;
	__u8			recverr:1,
				is_icsk:1,
				freebind:1,
				hdrincl:1,
				mc_loop:1,
				transparent:1,
				mc_all:1;
	int			mc_index;
	__be32			mc_addr;
	struct ip_mc_socklist	*mc_list;
	struct {
		unsigned int		flags;
		unsigned int		fragsize;
		struct ip_options	*opt;
		struct dst_entry	*dst;
		int			length; /* Total length of all frames */
		__be32			addr;
		struct flowi		fl;
	} cork;
};

#define IPCORK_OPT	1	/* ip-options has been held in ipcork.opt */
#define IPCORK_ALLFRAG	2	/* always fragment (for ipv6 for now) */

static inline struct inet_sock *inet_sk(const struct sock *sk)
{
	return (struct inet_sock *)sk;
}

static inline void __inet_sk_copy_descendant(struct sock *sk_to,
					     const struct sock *sk_from,
					     const int ancestor_size)
{
	memcpy(inet_sk(sk_to) + 1, inet_sk(sk_from) + 1,
	       sk_from->sk_prot->obj_size - ancestor_size);
}
#if !(defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE))
static inline void inet_sk_copy_descendant(struct sock *sk_to,
					   const struct sock *sk_from)
{
	__inet_sk_copy_descendant(sk_to, sk_from, sizeof(struct inet_sock));
}
#endif

extern int inet_sk_rebuild_header(struct sock *sk);

extern u32 inet_ehash_secret;
extern void build_ehash_secret(void);

static inline unsigned int inet_ehashfn(struct net *net,
					const __be32 laddr, const __u16 lport,
					const __be32 faddr, const __be16 fport)
{
	return jhash_3words((__force __u32) laddr,
			    (__force __u32) faddr,
			    ((__u32) lport) << 16 | (__force __u32)fport,
			    inet_ehash_secret + net_hash_mix(net));
}

static inline int inet_sk_ehashfn(const struct sock *sk)
{
	const struct inet_sock *inet = inet_sk(sk);
	const __be32 laddr = inet->inet_rcv_saddr;
	const __u16 lport = inet->inet_num;
	const __be32 faddr = inet->inet_daddr;
	const __be16 fport = inet->inet_dport;
	struct net *net = sock_net(sk);

	return inet_ehashfn(net, laddr, lport, faddr, fport);
}

static inline struct request_sock *inet_reqsk_alloc(struct request_sock_ops *ops)
{
	struct request_sock *req = reqsk_alloc(ops);
	struct inet_request_sock *ireq = inet_rsk(req);

	if (req != NULL) {
		kmemcheck_annotate_bitfield(ireq, flags);
		ireq->opt = NULL;
	}

	return req;
}

static inline __u8 inet_sk_flowi_flags(const struct sock *sk)
{
	return inet_sk(sk)->transparent ? FLOWI_FLAG_ANYSRC : 0;
}

#endif	/* _INET_SOCK_H */
