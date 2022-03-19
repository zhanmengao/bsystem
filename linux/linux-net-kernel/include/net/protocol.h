/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the protocol dispatcher.
 *
 * Version:	@(#)protocol.h	1.0.2	05/07/93
 *
 * Author:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	Changes:
 *		Alan Cox	:	Added a name field and a frag handler
 *					field for later.
 *		Alan Cox	:	Cleaned up, and sorted types.
 *		Pedro Roque	:	inet6 protocols
 */
 
#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <linux/in6.h>
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
#include <linux/ipv6.h>
#endif

#define MAX_INET_PROTOS	256		/* Must be a power of 2		*/

/* 
 * inet_add_protocol�������ڽ������ṹ��ʵ��(ָ��)
 * �洢��inet_protos ������
 * update:
 *  net_protocol��һ���ǳ���Ҫ�Ľṹ��������Э������֧�ֵ�
 * �����Э���Լ������ı��Ľ���ʵ�����˽ṹ��������
 * �����֮������������������ݰ���������������ʱ��
 * ����ô˽ṹ�еĴ����Э������ʱ������ô˽ṹ�еĴ����
 * Э�����ݱ����մ�������ע�⣺�˴�˵"�����"����׼ȷ��
 * ��ʵ�ϰ���ICMP��IGMPЭ�顣
 *
 * �ں���ΪInternetЭ���嶨����4��net_protocol�ṹʵ��---
 * icmp_protocol��udp_protocol��tcp_protocol��igmp_protocol
 * ,�ֱ���ICMP��UDP��TCP��IGMPЭ��һһ��Ӧ����InternetЭ����
 * ��ʼ��ʱ������inet_add_protocol()������ע�ᵽnet_protocol
 * �ṹָ������inet_protos[MAX_INET_PROTOS]�С���ϵͳ����
 * �����У���ʱ�������ں�ģ�����/ж�ط�ʽ�����ú���inet_add_protocol()
 * /inet_del_protocol()��net_protocol�ṹʵ��ע�ᵽinet_protos[]�����У�
 * �����ɾ����
 *///ops = rcu_dereference(inet_protos[proto]);ͨ���ú�����ȡ��Ӧ��Э��ops
struct net_protocol {//icmp_protocol  tcp_protocol  udp_protocol
       /* ���齫���ݵ��ú������н�һ������*/
    /*
     * �����Э�����ݰ����մ�����ָ�룬����������IP���ݰ�
     * ֮�󣬸���IP���ݰ���ָʾ�����Э�飬���ö�Ӧ�����
     * net_protocol�ṹ�ĸ����̽��ձ��ġ�
     * TCPЭ��Ľ��պ���Ϊtcp_v4_rcv()��UDPЭ��Ľ��պ���Ϊ
     * udp_rcv()��IGMPЭ��Ϊigmp_rcv()��ICMPЭ��Ϊicmp_rcv()��
     */
	int			(*handler)(struct sk_buff *skb);
       /* 
        * �ڽ��յ�ICMP������Ϣ����Ҫ���ݵ����߲�ʱ��
        * ���øú���
        */
    /*
     * ��ICMPģ���н��յ�����ĺ󣬻��������ģ�������
     * �������ԭʼ��IP�ײ������ö�Ӧ�������쳣����
     * ����err_handler��TCPЭ��Ϊtcp_v4_err()��UDPΪ
     * udp_err()��IGMP���ޡ�
     */
	void			(*err_handler)(struct sk_buff *skb, u32 info);
    /*
     * GSO�������豸֧�ִ�����һ�����ܡ�
     * ��GSO���ݰ����ʱ���������豸����������豸��֧��GSO��
     * ���������Ҫ��������������ݰ����½���GSO�ֶκ�
     * У��ͼ��㡣�����Ҫ������ṩ�ӿڸ��豸�㣬�ܹ�
     * ���ʴ�����GSO�ֶκ�У��͵ļ��㹦�ܣ�����������ݰ�
     * ���зֶκ�ִ��У��͡�
     * gso_send_check�ӿھ��ǻص�������ڷֶ�֮ǰ��α�ײ�
     * ����У��͵ļ��㡣
     * gso_segment�ӿھ��ǻص������GSO�ֶη����Դ�ν��зֶΡ�
     * TCP��ʵ�ֵĺ���Ϊtcp_v4_gso_send_check()��tcp_tso_segment()��
     * UDP��֧��GSO��
     */
	int			(*gso_send_check)(struct sk_buff *skb);
	struct sk_buff	       *(*gso_segment)(struct sk_buff *skb,
					       int features);
	struct sk_buff	      **(*gro_receive)(struct sk_buff **head,
					       struct sk_buff *skb);
	int			(*gro_complete)(struct sk_buff *skb);
    /*
     * no_policy��ʶ��·��ʱ�Ƿ���в���·�ɡ�TCP��UDPĬ�ϲ�����
     * ����·�ɡ�
     */
	unsigned int		no_policy:1,
				netns_ok:1;
};


#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
struct inet6_protocol {
	int	(*handler)(struct sk_buff *skb);

	void	(*err_handler)(struct sk_buff *skb,
			       struct inet6_skb_parm *opt,
			       u8 type, u8 code, int offset,
			       __be32 info);

	int	(*gso_send_check)(struct sk_buff *skb);
	struct sk_buff *(*gso_segment)(struct sk_buff *skb,
				       int features);
	struct sk_buff **(*gro_receive)(struct sk_buff **head,
					struct sk_buff *skb);
	int	(*gro_complete)(struct sk_buff *skb);

	unsigned int	flags;	/* INET6_PROTO_xxx */
};

#define INET6_PROTO_NOPOLICY	0x1
#define INET6_PROTO_FINAL	0x2
/* This should be set for any extension header which is compatible with GSO. */
#define INET6_PROTO_GSO_EXTHDR	0x4
#endif

/* This is used to register socket interfaces for IP protocols.  */
struct inet_protosw { //�μ�inetsw_array
	struct list_head list;

        /* These two fields form the lookup key.  */
	unsigned short	 type;	   /* This is the 2nd argument to socket(2). ��sock_type��Ӧ�ó��򴴽��׽���sock�����ĵڶ�������һ�� */
	unsigned short	 protocol; /* This is the L4 protocol number.IPPROTO_TCP UDP��  */

	struct proto	 *prot;//�׽ӿ������ӿڣ���Ӧtcp_prot  udp_prot  raw_prot
	const struct proto_ops *ops; //�׽ӿڴ����ӿڡ�TCPΪinet_stream_ops UDPΪinet_dgram_ops ԭʼ�׽ӿ���Ϊinet_sockraw_ops
  
	char             no_check;   /* checksum on rcv/xmit/none? */ //��ʾ�Ƿ���ҪУ�飬TCPΪ0��ʾ��ҪУ�飬UDPΪUDP_CSUM_NOXMIT  UDP_CSUM_NORCV��
	unsigned char	 flags;      /* See INET_PROTOSW_* below.  */  //������ƿ��ICSK��ʹ��
};
#define INET_PROTOSW_REUSE 0x01	     /* Are ports automatically reusable?  �˿�����*/
#define INET_PROTOSW_PERMANENT 0x02  /* Permanent protocols are unremovable. Э�鲻�ܱ��滻��ж��  �иñ�ʶ���׽ӿڲ�������inet_unregister_protosw*/
#define INET_PROTOSW_ICSK      0x04  /* Is this an inet_connection_sock? ��ʾ�Ƿ����������͵��׽ӿ�*/

extern const struct net_protocol *inet_protos[MAX_INET_PROTOS]; //�ο���ʼ����inet_init���ں���rcu_dereference(inet_protos[hash]);�л�ȡ��Ӧprotocol(tcp_protocol udp_protocol raw_protocol)

#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
extern const struct inet6_protocol *inet6_protos[MAX_INET_PROTOS];
#endif

extern int	inet_add_protocol(const struct net_protocol *prot, unsigned char num);
extern int	inet_del_protocol(const struct net_protocol *prot, unsigned char num);
extern void	inet_register_protosw(struct inet_protosw *p);
extern void	inet_unregister_protosw(struct inet_protosw *p);

#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
extern int	inet6_add_protocol(const struct inet6_protocol *prot, unsigned char num);
extern int	inet6_del_protocol(const struct inet6_protocol *prot, unsigned char num);
extern int	inet6_register_protosw(struct inet_protosw *p);
extern void	inet6_unregister_protosw(struct inet_protosw *p);
#endif

#endif	/* _PROTOCOL_H */
