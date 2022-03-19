/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the TCP module.
 *
 * Version:	@(#)tcp.h	1.0.5	05/23/93
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _TCP_H
#define _TCP_H

#define TCP_DEBUG 1
#define FASTRETRANS_DEBUG 1

#include <linux/list.h>
#include <linux/tcp.h>
#include <linux/slab.h>
#include <linux/cache.h>
#include <linux/percpu.h>
#include <linux/skbuff.h>
#include <linux/dmaengine.h>
#include <linux/crypto.h>
#include <linux/cryptohash.h>
#include <linux/kref.h>

#include <net/inet_connection_sock.h>
#include <net/inet_timewait_sock.h>
#include <net/inet_hashtables.h>
#include <net/checksum.h>
#include <net/request_sock.h>
#include <net/sock.h>
#include <net/snmp.h>
#include <net/ip.h>
#include <net/tcp_states.h>
#include <net/inet_ecn.h>
#include <net/dst.h>

#include <linux/seq_file.h>

extern struct inet_hashinfo tcp_hashinfo;

extern struct percpu_counter tcp_orphan_count;
extern void tcp_time_wait(struct sock *sk, int state, int timeo);

#define MAX_TCP_HEADER	(128 + MAX_HEADER)
#define MAX_TCP_OPTION_SPACE 40

/* 
 * Never offer a window over 32767 without using window scaling. Some
 * poor stacks do signed 16bit maths! 
 */
#define MAX_TCP_WINDOW		32767U

/* Minimal accepted MSS. It is (60+60+8) - (20+20). */
#define TCP_MIN_MSS		88U

/* The least MTU to use for probing */
#define TCP_BASE_MSS		512

/* After receiving this amount of duplicate ACKs fast retransmit starts. */
#define TCP_FASTRETRANS_THRESH 3

/* Maximal reordering. */
#define TCP_MAX_REORDERING	127

/* Maximal number of ACKs sent quickly to accelerate slow-start. */
#define TCP_MAX_QUICKACKS	16U

/* urg_data states */
/* ��ʶ��������ʱ��Ч�ģ��û����Զ�ȡ*/
#define TCP_URG_VALID	0x0100
/* ��ʶ���յ��Ķ��д��ڽ�������*/
#define TCP_URG_NOTYET	0x0200
/* ��ʶ����������ȫ������ȡ*/
#define TCP_URG_READ	0x0400


#define TCP_RETR1	3	/*
				 * This is how many retries it does before it
				 * tries to figure out if the gateway is
				 * down. Minimal RFC value is 3; it corresponds
				 * to ~3sec-8min depending on RTO.
				 */

#define TCP_RETR2	15	/*
				 * This should take at least
				 * 90 minutes to time out.
				 * RFC1122 says that the limit is 100 sec.
				 * 15 is ~13-30min depending on RTO.
				 */

#define TCP_SYN_RETRIES	 5	/* number of times to retry active opening a
				 * connection: ~180sec is RFC minimum	*/

#define TCP_SYNACK_RETRIES 5	/* number of times to retry passive opening a
				 * connection: ~180sec is RFC minimum	*/


#define TCP_ORPHAN_RETRIES 7	/* number of times to retry on an orphaned
				 * socket. 7 is ~50sec-16min.
				 */


#define TCP_TIMEWAIT_LEN (60*HZ) /* how long to wait to destroy TIME-WAIT
				  * state, about 60 seconds	*/
#define TCP_FIN_TIMEOUT	TCP_TIMEWAIT_LEN
                                 /* BSD style FIN_WAIT2 deadlock breaker.
				  * It used to be 3min, new value is 60sec,
				  * to combine FIN-WAIT-2 timeout with
				  * TIME-WAIT timer.
				  */

#define TCP_DELACK_MAX	((unsigned)(HZ/5))	/* maximal time to delay before sending an ACK */
#if HZ >= 100
#define TCP_DELACK_MIN	((unsigned)(HZ/25))	/* minimal time to delay before sending an ACK */
#define TCP_ATO_MIN	((unsigned)(HZ/25))
#else
#define TCP_DELACK_MIN	4U
#define TCP_ATO_MIN	4U
#endif
#define TCP_RTO_MAX	((unsigned)(120*HZ))
#define TCP_RTO_MIN	((unsigned)(HZ/5))
#define TCP_TIMEOUT_INIT ((unsigned)(3*HZ))	/* RFC 1122 initial RTO value	*/

#define TCP_RESOURCE_PROBE_INTERVAL ((unsigned)(HZ/2U)) /* Maximal interval between probes
					                 * for local resources.
					                 */

#define TCP_KEEPALIVE_TIME	(120*60*HZ)	/* two hours */
#define TCP_KEEPALIVE_PROBES	9		/* Max of 9 keepalive probes	*/
#define TCP_KEEPALIVE_INTVL	(75*HZ)

#define MAX_TCP_KEEPIDLE	32767
#define MAX_TCP_KEEPINTVL	32767
#define MAX_TCP_KEEPCNT		127
#define MAX_TCP_SYNCNT		127

#define TCP_SYNQ_INTERVAL	(HZ/5)	/* Period of SYNACK timer */

#define TCP_PAWS_24DAYS	(60 * 60 * 24 * 24)
#define TCP_PAWS_MSL	60		/* Per-host timestamps are invalidated
					 * after this time. It should be equal
					 * (or greater than) TCP_TIMEWAIT_LEN
					 * to provide reliability equal to one
					 * provided by timewait state.
					 */
#define TCP_PAWS_WINDOW	1		/* Replay window for per-host
					 * timestamps. It must be less than
					 * minimal timewait lifetime.
					 */
/*
 *	TCP option
 */
//��Щһ����SYN���У��ο�tcp_parse_options
#define TCPOPT_NOP		1	/* Padding */
#define TCPOPT_EOL		0	/* End of options */
#define TCPOPT_MSS		2	/* Segment size negotiating */
#define TCPOPT_WINDOW		3	/* Window scaling */
#define TCPOPT_SACK_PERM        4       /* SACK Permitted */
#define TCPOPT_SACK             5       /* SACK Block */
#define TCPOPT_TIMESTAMP	8	/* Better RTT estimations/PAWS */
#define TCPOPT_MD5SIG		19	/* MD5 Signature (RFC2385) */
#define TCPOPT_COOKIE		253	/* Cookie extension (experimental) */

/*
 *     TCP option lengths
 */

#define TCPOLEN_MSS            4 //ֻ�ܳ�����SYN����
#define TCPOLEN_WINDOW         3
#define TCPOLEN_SACK_PERM      2
#define TCPOLEN_TIMESTAMP      10
#define TCPOLEN_MD5SIG         18
#define TCPOLEN_COOKIE_BASE    2	/* Cookie-less header extension */
#define TCPOLEN_COOKIE_PAIR    3	/* Cookie pair header extension */
#define TCPOLEN_COOKIE_MIN     (TCPOLEN_COOKIE_BASE+TCP_COOKIE_MIN)
#define TCPOLEN_COOKIE_MAX     (TCPOLEN_COOKIE_BASE+TCP_COOKIE_MAX)

/* But this is what stacks really send out. */
#define TCPOLEN_TSTAMP_ALIGNED		12
#define TCPOLEN_WSCALE_ALIGNED		4
#define TCPOLEN_SACKPERM_ALIGNED	4
#define TCPOLEN_SACK_BASE		2
#define TCPOLEN_SACK_BASE_ALIGNED	4
#define TCPOLEN_SACK_PERBLOCK		8
#define TCPOLEN_MD5SIG_ALIGNED		20
#define TCPOLEN_MSS_ALIGNED		4

/* Flags in tp->nonagle */
#define TCP_NAGLE_OFF		1	/* Nagle's algo is disabled */
#define TCP_NAGLE_CORK		2	/* Socket is corked	    */
#define TCP_NAGLE_PUSH		4	/* Cork is overridden for already queued data */

/* TCP thin-stream limits */
#define TCP_THIN_LINEAR_RETRIES 6       /* After 6 linear retries, do exp. backoff */


/*
/proc/sys/net/ipv4/icmp_timeexceed_rate
�����tracerouteʱ���������ġ�Solaris middle star��������ļ����Ʒ���ICMP Time Exceeded��Ϣ�ı��ʡ�
/proc/sys/net/ipv4/igmp_max_memberships
����������ж��ٸ�igmp (�ಥ)�׽��ֽ��м�����
/proc/sys/net/ipv4/inet_peer_gc_maxtime
�� ��: Add a little explanation about the inet peer storage? Minimum interval between garbage collection passes. This interval is in effect under low (or absent) memory pressure on the pool. Measured in jiffies.
/proc/sys/net/ipv4/inet_peer_gc_mintime
ÿһ����Ƭ�ռ�֮�����Сʱ���������ڴ�ѹ���Ƚϴ��ʱ�򣬵�������������Ч����jiffies�ơ�
/proc/sys/net/ipv4/inet_peer_maxttl
entries����������ڡ���poolû���ڴ�ѹ���������(���磬pool��entries���������ٵ�ʱ��)��δʹ�õ�entries����һ��ʱ��ͻ���ڡ���jiffies�ơ�
/proc/sys/net/ipv4/inet_peer_minttl
entries����С�����ڡ�Ӧ�ò�С�ڻ�۶˷�Ƭ�������ڡ���pool�Ĵ�С������inet_peer_thresholdʱ�������С�����ڱ������Ա�֤����jiffies�ơ�
/proc/sys/net/ipv4/inet_peer_threshold
The approximate size of the INET peer storage. Starting from this threshold entries will be thrown aggressively. This threshold also determines entries' time-to-live an�� time intervals between garbage collection passes. More entries, less time-to-live, less GC interval.
/proc/sys/net/ipv4/ip_autoconfig
����ļ�����д��һ�����֣���ʾ�����Ƿ�ͨ��RARP��BOOTP��DHCP������������ȡ����IP���á��������0��
/proc/sys/net/ipv4/ip_default_ttl
���ݰ��������ڡ�����Ϊ64�ǰ�ȫ�ġ������������ģ�޴��������ֵ����Ҫ��Ϊ�������ô����������������к���·�ɻ�·��ʵ���ϣ��ںܶ��������Ҫ�����ܷ��С���ֵ��
/proc/sys/net/ipv4/ip_dynaddr/proc/sys/net/ipv4/icmp_destunreach_rate
�������һ����̬��ַ���Զ����Žӿڣ��͵���������������Զ����Žӿڼ����ʱ�򣬱�������û���յ��𸴵�TCP�׽��ֻ����°󶨵���ȷ�ĵ�ַ�ϡ�����Խ���������ŵ��׽��ֱ����޷�����������һ��ȴ���Ե����⡣
/proc/sys/net/ipv4/ip_forward
�ں��Ƿ�ת�����ݰ���ȱʡ��ֹ��
/proc/sys/net/ipv4/ip_local_port_range
�����������ӵĶ˿ڷ�Χ��ȱʡ�������ʵ��С��1024��4999��
/proc/sys/net/ipv4/ip_no_pmtu_disc
��������ֹ����;MTU���֡���������������;MTU���֡���һ�ּ����������ڴ���·���ϼ��������ܵ�MTUֵ���μ�Cookbookһ���й��ڡ���;MTU���֡������ݡ�
/proc/sys/net/ipv4/ipfrag_high_thresh
�� ��IP��Ƭ��۵�����ڴ���������������ô���ֽڵ��ڴ��һ���þ�����Ƭ�������ͻᶪ����Ƭ��When ipfrag_high_thresh bytes of memory is allocated for this purpose, the fragment handler will toss packets until ipfrag_low_thresh is reached.
/proc/sys/net/ipv4/ip_nonlocal_bind
�����ϣ�����Ӧ�ó����ܹ��󶨵������ڱ��������ĵ�ַ��ʱ���������ѡ������Ļ���û��ר������(�����Ƕ�̬����)ʱ�ǳ����ã���ʹ������ӶϿ�����ķ���Ҳ��������������һ��ָ���ĵ�ַ�ϡ�
/proc/sys/net/ipv4/ipfrag_low_thresh
����IP��Ƭ��۵���С�ڴ�������
/proc/sys/net/ipv4/ipfrag_time
IP��Ƭ���ڴ��еı���ʱ��(����)��
/proc/sys/net/ipv4/tcp_abort_on_overflow
һ���������͵ı�־�������ŵ��кܶ����������ʱ�ں˵���Ϊ�����õĻ�����������أ��ں˽������ط���RST����
/proc/sys/net/ipv4/tcp_fin_timeout
�� ���׽����ɱ���Ҫ��رգ����������������������FIN-WAIT-2״̬��ʱ�䡣�Զ˿��Գ�����Զ���ر����ӣ��������⵱����ȱʡֵ��60�롣2.2 �ں˵�ͨ��ֵ��180�룬����԰�������ã���Ҫ��ס���ǣ���ʹ��Ļ�����һ�����ص�WEB��������Ҳ����Ϊ���������׽��ֶ��ڴ�����ķ��գ�FIN- WAIT-2��Σ���Ա�FIN-WAIT-1ҪС����Ϊ�����ֻ�ܳԵ�1.5K�ڴ棬�������ǵ������ڳ�Щ���μ�tcp_max_orphans��
/proc/sys/net/ipv4/tcp_keepalive_time
��keepalive���õ�ʱ��TCP����keepalive��Ϣ��Ƶ�ȡ�ȱʡ��2Сʱ��
/proc/sys/net/ipv4/tcp_keepalive_intvl
��̽��û��ȷ��ʱ�����·���̽���Ƶ�ȡ�ȱʡ��75�롣
/proc/sys/net/ipv4/tcp_keepalive_probes
���϶�����ʧЧ֮ǰ�����Ͷ��ٸ�TCP��keepalive̽�����ȱʡֵ��9�����ֵ����tcp_keepalive_intvl֮������ˣ�һ�����ӷ�����keepalive֮������ж���ʱ��û�л�Ӧ��
/proc/sys/net/ipv4/tcp_max_orphans
ϵ ͳ������ж��ٸ�TCP�׽��ֲ����������κ�һ���û��ļ�����ϡ��������������֣��¶����ӽ����̱���λ����ӡ��������Ϣ��������ƽ�����Ϊ�˷�ֹ�򵥵�DoS����������Բ��ܹ���������������Ϊ�ؼ�С���ֵ����Ӧ���������ֵ(����������ڴ�֮��)��This limit exists only to prevent simple DoS attacks, you _must_ not rely on this o�� lower the limit artificially, but rather increase it (probably, after increasing installed memory), if network conditions require more than default value, an�� tune network services to linger an�� kill such states more aggressively. �����ٴ������㣺ÿ���¶��׽�������ܹ��Ե���64K���ɽ������ڴ档
/proc/sys/net/ipv4/tcp_orphan_retries
������ͼ�ر�TCP����֮ǰ���Զ��ٴΡ�ȱʡֵ��7���൱��50��~16����(ȡ����RTO)�������Ļ�����һ�����ص�WEB����������Ӧ�ÿ��Ǽ������ֵ����Ϊ�������׽��ֻ����ĺܶ���Ҫ����Դ���μ�tcp_max_orphans��
/proc/sys/net/ipv4/tcp_max_syn_backlog
�� ¼����Щ��δ�յ��ͻ���ȷ����Ϣ��������������ֵ��������128M�ڴ��ϵͳ���ԣ�ȱʡֵ��1024��С�ڴ��ϵͳ����128����������������ظ����� ��������ֵ��ע�⣡������������ֵ����1024�����ͬʱ����include/net/tcp.h�е�TCP_SYNQ_HSIZE���Ա�֤ TCP_SYNQ_HSIZE*16 ��tcp_max_syn_backlo��Ȼ�����±����ںˡ�
/proc/sys/net/ipv4/tcp_max_tw_buckets
ϵ ͳͬʱ����timewait�׽��ֵ�����������������������֣�time-wait�׽��ֽ����̱��������ӡ������Ϣ��������ƽ�����Ϊ�˷�ֹ�򵥵� DoS����������Բ��ܹ���������������Ϊ�ؼ�С���ֵ���������ʵ����Ҫ����ȱʡֵ����Ӧ���������ֵ(����������ڴ�֮��)��
/proc/sys/net/ipv4/tcp_retrans_collapse
Ϊ����ĳЩ���Ĵ�ӡ�����õġ�����ʹ�ѡ��ٴη���ʱ�������ݰ�����һЩ��������ĳЩTCPЭ��ջ��BUG��
/proc/sys/net/ipv4/tcp_retries1
���϶�������������ύ���󱨸�֮ǰ�����Զ��ٴΡ�ȱʡ����ΪRFC�涨����Сֵ��3���൱��3��~8���ӣ�ȡ����RIO����
/proc/sys/net/ipv4/tcp_retries2
��ɱ��һ�����TCP����֮ǰ���Զ��ٴΡ�RFC 1122�涨�������Ӧ�ó���100�롣���ֵ̫С�ˡ�ȱʡֵ��15���൱��13~30���ӣ�ȡ����RIO����
/proc/sys/net/ipv4/tcp_rfc1337
������ؿ�������������RFC1337�������ġ�tcp��time-wait��ɱΣ����������޸������ú��ں˽�������Щ����time-wait״̬TCP�׽��ֵ�RST����ȴʡΪ0��
/proc/sys/net/ipv4/tcp_sack
�ر���Զ�ʧ�����ݰ�ʹ��ѡ����ACK�����������ڿ��ٻָ���
/proc/sys/net/ipv4/tcp_stdurg
ʹ��TCP����ָ�������������͡���Ϊ���������������BSD���ͣ������������Linux�ϴ��������ܻ�Ӱ��������������������ͨѶ��ȱʡ��FALSE��
/proc/sys/net/ipv4/tcp_syn_retries
���ں˷�����������֮ǰ����SYN����������
/proc/sys/net/ipv4/tcp_synack_retries
Ϊ�˴򿪶Զ˵����ӣ��ں���Ҫ����һ��SYN������һ����Ӧǰ��һ��SYN��ACK��Ҳ������ν���������еĵڶ������֡�������þ������ں˷�������֮ǰ����SYN+ACK����������
/proc/sys/net/ipv4/tcp_timestamps
ʱ������Ա������кŵľ��ơ�һ��1Gbps����·�϶���������ǰ�ù������кš�ʱ����ܹ����ں˽������֡��쳣�������ݰ���
/proc/sys/net/ipv4/tcp_tw_recycle
�ܹ�����ػ���TIME-WAIT�׽��֡�ȱʡֵ��1�������м���ר�ҵĽ����Ҫ�󣬷���Ӧ�޸ġ�
/proc/sys/net/ipv4/tcp_window_scaling
һ����˵TCP/IP�����ڳߴ�ﵽ65535�ֽڡ������ٶ�ȷʵ�ܸߵ�����������ֵ���ܻ���̫С�����ѡ������������G�ֽڵĴ��ڴ�С���������ڴ���*�ӳٺܴ�Ļ�����ʹ�á�
һ���ں���Ϊ���޷��������ͻᶪ������������򷢰�����������ICMP֪ͨ��
/proc/sys/net/ipv4/icmp_echo_ignore_all
������Ҫ��Ӧecho�����벻Ҫ����Ϊȱʡ�������������������ó�ΪDoS����������ʱ�������á�
/proc/sys/net/ipv4/icmp_echo_ignore_broadcasts [Useful]
�����ping������������ַ�����еĻ�����Ӧ�����Ի�Ӧ������ܳ�Ϊ�ǳ����õľܾ����񹥻����ߡ�����Ϊ1��������Щ�����㲥��Ϣ��
/proc/sys/net/ipv4/icmp_echoreply_rate
������������������Ӧecho����ı��ʡ�
/proc/sys/net/ipv4/icmp_ignore_bogus_error_responses
������֮�󣬿��Ժ����������е���Щ���ƻ�Ӧ��ַ�ǹ㲥��ַ���������ɵ�ICMP����
/proc/sys/net/ipv4/icmp_paramprob_rate
һ����Բ�����ȷ��ICMP��Ϣ��������ӦIPͷ��TCPͷ�𻵵��쳣���ݰ��������ͨ������ļ�������Ϣ�ķ��ͱ��ʡ�
*/

//��Щֵ��Ӧ��proc�ļ�������ipv4_table��Ӧ�ò�����������Щֵ����ipv4_table�е�data��Ч

extern struct inet_timewait_death_row tcp_death_row;

/* sysctl variables for tcp */
//��tcp_syn_options
//tcp_timestamps�������������Ƿ�����ʱ���ѡ�tcp_tw_recycle�����������ÿ��ٻ���TIME_WAIT�׽��֡�tcp_timestamps������Ӱ�쵽tcp_tw_recycle������Ч�������û��ʱ���ѡ��Ļ���tcp_tw_recycle������Ч
extern int sysctl_tcp_timestamps;//  tcp_timestamps�������������Ƿ�����ʱ���ѡ���������ˣ����ͱ��ĵ�ʱ��TCP�ײ����Ȼ����ⲿ�ֳ���
extern int sysctl_tcp_window_scaling;
extern int sysctl_tcp_sack;

//���ڱ��˶Ͽ���socket���ӣ�TCP������FIN_WAIT_2״̬��ʱ�䡣�Է����ܻ�Ͽ����ӻ�һֱ���������ӻ򲻿�Ԥ�ϵĽ�������,����ͨ���������ʱ����wait2�˳���CLOSE״̬
extern int sysctl_tcp_fin_timeout;

//tcpkeepalivetime�ĵ�λ���룬��ʾTCP�����ڶ�����֮��û�����ݱ��Ĵ�������̽�ⱨ��; 
//tcpkeepaliveintvl��λ��Ҳ��,��ʾǰһ��̽�ⱨ�ĺͺ�һ��̽�ⱨ��֮���ʱ������
//tcpkeepaliveprobes��ʾ̽��Ĵ�����
extern int sysctl_tcp_keepalive_time;
extern int sysctl_tcp_keepalive_probes;
extern int sysctl_tcp_keepalive_intvl;
extern int sysctl_tcp_syn_retries;

/*
 * ������õ�ֵ��ֻ���ش�������Ĭ��ֵ�����
 * �����Ӷ����а����������������Ӷ��г��ȵ�һ�룬
 * ��ݼ��ش�������
 * �����������һ�����ã�����
 * ���������ش����������Ǽ�ʹ������TCP_DEFER_ACCEPT
 * ѡ��ܵ��ش�����Ҳ���ܳ������������ֵ��
 * �μ�inet_csk_reqsk_queue_prune()��
 */
extern int sysctl_tcp_synack_retries;
extern int sysctl_tcp_retries1; //����ʾ�����������Դ����������������ֵ�����Ǿ���Ҫ���·�ɱ��ˡ�
/*
 * ��ȡȷ���Ͽ�����ǰ������ʱ�������Է���
 * TCP�ε���Ŀ���ޣ����ڳ�����ʱ����������
 * ���ļ�⡣
 */ //���ֵҲ�Ǳ�ʾ������������ֻ�������ֵһ��Ҫ�������ֵ�󡣺������Ǹ���ͬ���ǣ������Դ����������ֵ�����Ǿͱ�����������ˡ�
extern int sysctl_tcp_retries2;

//��Ҫ����Թ�����socket(Ҳ�����Ѿ��ӽ�����������ɾ���ˣ����ǻ���һЩ������û�����).��������socket���������Ե����Ĵ�����������
extern int sysctl_tcp_orphan_retries;
extern int sysctl_tcp_syncookies;
extern int sysctl_tcp_retrans_collapse;
extern int sysctl_tcp_stdurg;
extern int sysctl_tcp_rfc1337;
extern int sysctl_tcp_abort_on_overflow;
extern int sysctl_tcp_max_orphans;//��tcp_close��ʱ���ڹرչ����л����ӣ���ʾ��δ�Ĵλ��ֳ������׽���������tcp_too_many_orphans�����ιر�close��ʱ���Ѿ��ﵽ�����ֵ�����������������̣�����ֱ�ӷ���rst
extern int sysctl_tcp_fack;
extern int sysctl_tcp_reordering;
extern int sysctl_tcp_ecn;
extern int sysctl_tcp_dsack;
//��tcp_memory_allocated����sysctl_tcp_mem[1]ʱ��TCP���������뾯��״̬��tcp_memory_pressure��Ϊ1�� �⼸�������浽proto�еĶ�Ӧ�����С�������뾯��״̬�����ڽ������ݵ�ʱ���tcp_should_expand_sndbuf
//��tcp_memory_allocatedС��sysctl_tcp_mem[0]ʱ��TCP��������˳�����״̬��tcp_memory_pressure��Ϊ0�� 
extern int sysctl_tcp_mem[3];///proc/sys/net/ipv4/tcp_mem�в鿴   tcp_init�����ڴ������ֵ��ʼ��
extern int sysctl_tcp_wmem[3];

/*
 * 3��������Ĭ��ֵΪ: 4096,87380,174760,�ֱ��Ӧ��min��
 * default��max��
 * min:���ն����б��������ܳ���(sock�ṹ��sk_rmem_alloc)������
 * default: ���ջ������������޵ĳ�ʼֵ��������ʼ��sock�ṹ
 *             �ĳ�Աsk_rcvbuf
 * max: ���ջ������������޵����ֵ����������sock
 *          �ṹ�ĳ�Աsk_rcvbuf
 */
extern int sysctl_tcp_rmem[3];
extern int sysctl_tcp_app_win;
extern int sysctl_tcp_adv_win_scale;

// ��ʾ�������á�����TIME-WAIT sockets���������µ�TCP���ӣ�Ĭ��Ϊ0����ʾ�رգ�ע������������SO_REUSEADDR
extern int sysctl_tcp_tw_reuse;
extern int sysctl_tcp_frto;
extern int sysctl_tcp_frto_response;
extern int sysctl_tcp_low_latency;
extern int sysctl_tcp_dma_copybreak;
extern int sysctl_tcp_nometrics_save;
/*��ô�����ú���󻺴����ƺ�͸����������𣿶���һ��TCP������˵�������Ѿ��������������Դ��ʹ�ô󴰿ڡ��󻺴������ָ��ٴ����ˡ������ڳ��������У��������޿��ܻᱻ����Ϊ��ʮ���ֽڣ���ϵͳ�����ڴ�ȴ�����޵ģ���ÿһ�����Ӷ�ȫ�ٷɱ�ʹ�õ���󴰿�ʱ��1������Ӿͻ�ռ���ڴ浽����G�ˣ���������˸߲���������ʹ�ã���ƽ��Ҳ�ò�����֤������ϣ���ĳ����ǣ��ڲ������ӱȽ���ʱ���ѻ������ƷŴ�һЩ����ÿһ��TCP���ӿ����������������������Ӻܶ�ʱ����ʱϵͳ�ڴ���Դ���㣬��ô�Ͱѻ���������СһЩ��ʹÿһ��TCP���ӵĻ��澡����СһЩ�������ɸ�������ӡ�

linuxΪ��ʵ�����ֳ������������Զ������ڴ����Ĺ��ܣ���tcp_moderate_rcvbuf���þ��������£�
net.ipv4.tcp_moderate_rcvbuf = 1
Ĭ��tcp_moderate_rcvbuf����Ϊ1����ʾ����TCP�ڴ��Զ��������ܡ�������Ϊ0��������ܽ�������Ч�����ã���

������ע�⣺�������ڱ���ж�����������SO_SNDBUF��SO_RCVBUF������ʹlinux�ں˲��ٶ�����������ִ���Զ��������ܣ�*/
extern int sysctl_tcp_moderate_rcvbuf;
extern int sysctl_tcp_tso_win_divisor;
extern int sysctl_tcp_abc;
extern int sysctl_tcp_mtu_probing;
extern int sysctl_tcp_base_mss; //��tcp_mtup_init
extern int sysctl_tcp_workaround_signed_windows;
extern int sysctl_tcp_slow_start_after_idle;
extern int sysctl_tcp_max_ssthresh;
extern int sysctl_tcp_cookie_size;
extern int sysctl_tcp_thin_linear_timeouts;
extern int sysctl_tcp_thin_dupack;

extern atomic_t tcp_memory_allocated;
extern struct percpu_counter tcp_sockets_allocated;

//��tcp_memory_allocated����sysctl_tcp_mem[1]ʱ��TCP���������뾯��״̬��tcp_memory_pressure��Ϊ1�� �⼸�������浽proto�еĶ�Ӧ�����С�������뾯��״̬�����ڽ������ݵ�ʱ���tcp_should_expand_sndbuf
//��tcp_memory_allocatedС��sysctl_tcp_mem[0]ʱ��TCP��������˳�����״̬��tcp_memory_pressure��Ϊ0�� 
extern int tcp_memory_pressure;
/*
 * ������Ϊ���Ͷ�����SKB�����ǽ����Ľ��յ�TCP����㣬����Ҫ���½��봫����ƿ�Ļ������ȷ�ϡ�ȷ��ʱ����׽��ֻ����е����ݳ��ȴ���
 * Ԥ���������������ȫ���ȷ�ϣ����������__sk_mem_schedule()ʵ�֡�
 * @size:Ҫȷ�ϵĻ��泤��
 * @kind:���ͣ�0Ϊ���ͻ��棬1Ϊ���ջ��档
 */

/*
 * The next routines deal with comparing 32 bit unsigned ints
 * and worry about wraparound (automatic with unsigned arithmetic).
 */

static inline int before(__u32 seq1, __u32 seq2)
{
        return (__s32)(seq1-seq2) < 0;
}
#define after(seq2, seq1) 	before(seq1, seq2)

/* is s2<=s1<=s3 ? */
static inline int between(__u32 seq1, __u32 seq2, __u32 seq3)
{
	return seq3 - seq2 >= seq1 - seq2;
}

/*
 * ����������·���1:
 * a. �����ٵ�sock�ṹ����Ŀ����sysctl_tcp_max_orphans��ֵ
 *     ��ϵͳ�������
 * b. sock�ķ��Ͷ��������ݵ��ܳ��ȴ���SOCK_MIN_SNDBUF
 *     ���ҵ�ǰTCP��Ϊ������������ڴ����TCP�����pressure״̬��
 *     �ڴ�����
 */
static inline bool tcp_too_many_orphans(struct sock *sk, int shift)
{
	struct percpu_counter *ocp = sk->sk_prot->orphan_count;
	int orphans = percpu_counter_read_positive(ocp);

	if (orphans << shift > sysctl_tcp_max_orphans) {
		orphans = percpu_counter_sum_positive(ocp);
		if (orphans << shift > sysctl_tcp_max_orphans)
			return true;
	}

	if (sk->sk_wmem_queued > SOCK_MIN_SNDBUF &&
	    atomic_read(&tcp_memory_allocated) > sysctl_tcp_mem[2])
		return true;
	return false;
}

/* syncookies: remember time of last synqueue overflow */
static inline void tcp_synq_overflow(struct sock *sk)
{
	tcp_sk(sk)->rx_opt.ts_recent_stamp = jiffies;
}

/* syncookies: no recent synqueue overflow on this listening socket? */
static inline int tcp_synq_no_recent_overflow(const struct sock *sk)
{
	unsigned long last_overflow = tcp_sk(sk)->rx_opt.ts_recent_stamp;
	return time_after(jiffies, last_overflow + TCP_TIMEOUT_INIT);
}

extern struct proto tcp_prot;

#define TCP_INC_STATS(net, field)	SNMP_INC_STATS((net)->mib.tcp_statistics, field)
#define TCP_INC_STATS_BH(net, field)	SNMP_INC_STATS_BH((net)->mib.tcp_statistics, field)
#define TCP_DEC_STATS(net, field)	SNMP_DEC_STATS((net)->mib.tcp_statistics, field)
#define TCP_ADD_STATS_USER(net, field, val) SNMP_ADD_STATS_USER((net)->mib.tcp_statistics, field, val)
#define TCP_ADD_STATS(net, field, val)	SNMP_ADD_STATS((net)->mib.tcp_statistics, field, val)

extern void			tcp_v4_err(struct sk_buff *skb, u32);

extern void			tcp_shutdown (struct sock *sk, int how);

extern int			tcp_v4_rcv(struct sk_buff *skb);

extern int			tcp_v4_remember_stamp(struct sock *sk);

extern int		    	tcp_v4_tw_remember_stamp(struct inet_timewait_sock *tw);

extern int			tcp_sendmsg(struct kiocb *iocb, struct socket *sock,
					    struct msghdr *msg, size_t size);
extern ssize_t			tcp_sendpage(struct socket *sock, struct page *page, int offset, size_t size, int flags);

extern int			tcp_ioctl(struct sock *sk, 
					  int cmd, 
					  unsigned long arg);

extern int			tcp_rcv_state_process(struct sock *sk, 
						      struct sk_buff *skb,
						      struct tcphdr *th,
						      unsigned len);

extern int			tcp_rcv_established(struct sock *sk, 
						    struct sk_buff *skb,
						    struct tcphdr *th, 
						    unsigned len);

extern void			tcp_rcv_space_adjust(struct sock *sk);

extern void			tcp_cleanup_rbuf(struct sock *sk, int copied);

extern int			tcp_twsk_unique(struct sock *sk,
						struct sock *sktw, void *twp);

extern void			tcp_twsk_destructor(struct sock *sk);

extern ssize_t			tcp_splice_read(struct socket *sk, loff_t *ppos,
					        struct pipe_inode_info *pipe, size_t len, unsigned int flags);

static inline void tcp_dec_quickack_mode(struct sock *sk,
					 const unsigned int pkts)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_ack.quick) {
		if (pkts >= icsk->icsk_ack.quick) {
			icsk->icsk_ack.quick = 0;
			/* Leaving quickack mode we deflate ATO. */
			icsk->icsk_ack.ato   = TCP_ATO_MIN;
		} else
			icsk->icsk_ack.quick -= pkts;
	}
}

extern void tcp_enter_quickack_mode(struct sock *sk);

#define	TCP_ECN_OK		1
#define	TCP_ECN_QUEUE_CWR	2
#define	TCP_ECN_DEMAND_CWR	4

static __inline__ void
TCP_ECN_create_request(struct request_sock *req, struct tcphdr *th)
{
	if (sysctl_tcp_ecn && th->ece && th->cwr)
		inet_rsk(req)->ecn_ok = 1;
}

enum tcp_tw_status {
	TCP_TW_SUCCESS = 0,
	TCP_TW_RST = 1,
	TCP_TW_ACK = 2,
	TCP_TW_SYN = 3
};


extern enum tcp_tw_status	tcp_timewait_state_process(struct inet_timewait_sock *tw,
							   struct sk_buff *skb,
							   const struct tcphdr *th);

extern struct sock *		tcp_check_req(struct sock *sk,struct sk_buff *skb,
					      struct request_sock *req,
					      struct request_sock **prev);
extern int			tcp_child_process(struct sock *parent,
						  struct sock *child,
						  struct sk_buff *skb);
extern int			tcp_use_frto(struct sock *sk);
extern void			tcp_enter_frto(struct sock *sk);
extern void			tcp_enter_loss(struct sock *sk, int how);
extern void			tcp_clear_retrans(struct tcp_sock *tp);
extern void			tcp_update_metrics(struct sock *sk);

extern void			tcp_close(struct sock *sk, 
					  long timeout);
extern unsigned int		tcp_poll(struct file * file, struct socket *sock, struct poll_table_struct *wait);

extern int			tcp_getsockopt(struct sock *sk, int level, 
					       int optname,
					       char __user *optval, 
					       int __user *optlen);
extern int			tcp_setsockopt(struct sock *sk, int level, 
					       int optname, char __user *optval, 
					       unsigned int optlen);
extern int			compat_tcp_getsockopt(struct sock *sk,
					int level, int optname,
					char __user *optval, int __user *optlen);
extern int			compat_tcp_setsockopt(struct sock *sk,
					int level, int optname,
					char __user *optval, unsigned int optlen);
extern void			tcp_set_keepalive(struct sock *sk, int val);
extern void			tcp_syn_ack_timeout(struct sock *sk,
						    struct request_sock *req);
extern int			tcp_recvmsg(struct kiocb *iocb, struct sock *sk,
					    struct msghdr *msg,
					    size_t len, int nonblock, 
					    int flags, int *addr_len);

extern void			tcp_parse_options(struct sk_buff *skb,
						  struct tcp_options_received *opt_rx,
						  u8 **hvpp,
						  int estab);

extern u8			*tcp_parse_md5sig_option(struct tcphdr *th);

/*
 *	TCP v4 functions exported for the inet6 API
 */

extern void		       	tcp_v4_send_check(struct sock *sk,
						  struct sk_buff *skb);

extern int			tcp_v4_conn_request(struct sock *sk,
						    struct sk_buff *skb);

extern struct sock *		tcp_create_openreq_child(struct sock *sk,
							 struct request_sock *req,
							 struct sk_buff *skb);

extern struct sock *		tcp_v4_syn_recv_sock(struct sock *sk,
						     struct sk_buff *skb,
						     struct request_sock *req,
							struct dst_entry *dst);

extern int			tcp_v4_do_rcv(struct sock *sk,
					      struct sk_buff *skb);

extern int			tcp_v4_connect(struct sock *sk,
					       struct sockaddr *uaddr,
					       int addr_len);

extern int			tcp_connect(struct sock *sk);

extern struct sk_buff *		tcp_make_synack(struct sock *sk,
						struct dst_entry *dst,
						struct request_sock *req,
						struct request_values *rvp);

extern int			tcp_disconnect(struct sock *sk, int flags);


/* From syncookies.c */
extern __u32 syncookie_secret[2][16-4+SHA_DIGEST_WORDS];
extern struct sock *cookie_v4_check(struct sock *sk, struct sk_buff *skb, 
				    struct ip_options *opt);
extern __u32 cookie_v4_init_sequence(struct sock *sk, struct sk_buff *skb, 
				     __u16 *mss);

extern __u32 cookie_init_timestamp(struct request_sock *req);
extern void cookie_check_timestamp(struct tcp_options_received *tcp_opt);

/* From net/ipv6/syncookies.c */
extern struct sock *cookie_v6_check(struct sock *sk, struct sk_buff *skb);
extern __u32 cookie_v6_init_sequence(struct sock *sk, struct sk_buff *skb,
				     __u16 *mss);

/* tcp_output.c */

extern void __tcp_push_pending_frames(struct sock *sk, unsigned int cur_mss,
				      int nonagle);
extern int tcp_may_send_now(struct sock *sk);
extern int tcp_retransmit_skb(struct sock *, struct sk_buff *);
extern void tcp_retransmit_timer(struct sock *sk);
extern void tcp_xmit_retransmit_queue(struct sock *);
extern void tcp_simple_retransmit(struct sock *);
extern int tcp_trim_head(struct sock *, struct sk_buff *, u32);
extern int tcp_fragment(struct sock *, struct sk_buff *, u32, unsigned int);

extern void tcp_send_probe0(struct sock *);
extern void tcp_send_partial(struct sock *);
extern int  tcp_write_wakeup(struct sock *);
extern void tcp_send_fin(struct sock *sk);
extern void tcp_send_active_reset(struct sock *sk, gfp_t priority);
extern int  tcp_send_synack(struct sock *);
extern void tcp_push_one(struct sock *, unsigned int mss_now);
extern void tcp_send_ack(struct sock *sk);
extern void tcp_send_delayed_ack(struct sock *sk);

/* tcp_input.c */
extern void tcp_cwnd_application_limited(struct sock *sk);

/* tcp_timer.c */
extern void tcp_init_xmit_timers(struct sock *);
static inline void tcp_clear_xmit_timers(struct sock *sk)
{
	inet_csk_clear_xmit_timers(sk);
}

extern unsigned int tcp_sync_mss(struct sock *sk, u32 pmtu);
extern unsigned int tcp_current_mss(struct sock *sk);

/* Bound MSS / TSO packet size with the half of the window */
static inline int tcp_bound_to_half_wnd(struct tcp_sock *tp, int pktsize)
{
	int cutoff;

	/* When peer uses tiny windows, there is no use in packetizing
	 * to sub-MSS pieces for the sake of SWS or making sure there
	 * are enough packets in the pipe for fast recovery.
	 *
	 * On the other hand, for extremely large MSS devices, handling
	 * smaller than MSS windows in this way does make sense.
	 */
	if (tp->max_window >= 512)
		cutoff = (tp->max_window >> 1);
	else
		cutoff = tp->max_window;

	if (cutoff && pktsize > cutoff)
		return max_t(int, cutoff, 68U - tp->tcp_header_len);
	else
		return pktsize;
}

/* tcp.c */
extern void tcp_get_info(struct sock *, struct tcp_info *);

/* Read 'sendfile()'-style from a TCP socket */
typedef int (*sk_read_actor_t)(read_descriptor_t *, struct sk_buff *,
				unsigned int, size_t);
extern int tcp_read_sock(struct sock *sk, read_descriptor_t *desc,
			 sk_read_actor_t recv_actor);

extern void tcp_initialize_rcv_mss(struct sock *sk);

extern int tcp_mtu_to_mss(struct sock *sk, int pmtu);
extern int tcp_mss_to_mtu(struct sock *sk, int mss);
extern void tcp_mtup_init(struct sock *sk);

static inline void tcp_bound_rto(const struct sock *sk)
{
	if (inet_csk(sk)->icsk_rto > TCP_RTO_MAX)
		inet_csk(sk)->icsk_rto = TCP_RTO_MAX;
}

static inline u32 __tcp_set_rto(const struct tcp_sock *tp)
{
	return (tp->srtt >> 3) + tp->rttvar;
}

static inline void __tcp_fast_path_on(struct tcp_sock *tp, u32 snd_wnd)
{
	tp->pred_flags = htonl((tp->tcp_header_len << 26) |
			       ntohl(TCP_FLAG_ACK) |
			       snd_wnd);
}

/*
 * tcp_fast_path_on()�Ƕ�__tcp_fast_path_on()�ķ�װ���ṩ����
 * ���ڴ�С����snd_wnd�ʹ����������Ӽ���õ�
 */
static inline void tcp_fast_path_on(struct tcp_sock *tp)
{
	__tcp_fast_path_on(tp, tp->snd_wnd >> tp->rx_opt.snd_wscale);
}

/*
 * ��������Ԥ���־����Ȼ������������
 * Ԥ���־������
 */
static inline void tcp_fast_path_check(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);

	/*
	 * ����Ԥ���־��������:
	 * 1)�����������Ϊ�գ�˵������Ƚϳ�ͨ��
	 * 2)���մ��ڲ�Ϊ0��˵����ǰ���ܽ������ݡ�
	 * 3)��ǰ��ʹ�õĽ��ջ���δ�ﵽ���ޣ�Ҳ˵��Ŀǰ���ܽ�������
	 * 4)û���յ�����ָ�룬����·��������������ݡ�
	 */
	if (skb_queue_empty(&tp->out_of_order_queue) &&
	    tp->rcv_wnd &&
	    atomic_read(&sk->sk_rmem_alloc) < sk->sk_rcvbuf &&
	    !tp->urg_data)
		tcp_fast_path_on(tp);
}


/* Compute the actual rto_min value */
static inline u32 tcp_rto_min(struct sock *sk)
{
	struct dst_entry *dst = __sk_dst_get(sk);
	u32 rto_min = TCP_RTO_MIN;

	if (dst && dst_metric_locked(dst, RTAX_RTO_MIN))
		rto_min = dst_metric_rtt(dst, RTAX_RTO_MIN);
	return rto_min;
}

/* Compute the actual receive window we are currently advertising.
 * Rcv_nxt can be after the window if our peer push more data
 * than the offered window.
 */
static inline u32 tcp_receive_window(const struct tcp_sock *tp)
{
	s32 win = tp->rcv_wup + tp->rcv_wnd - tp->rcv_nxt;

	if (win < 0)
		win = 0;
	return (u32) win;
}

/* Choose a new window, without checks for shrinking, and without
 * scaling applied to the result.  The caller does these things
 * if necessary.  This is a "raw" window selection.
 */
extern u32	__tcp_select_window(struct sock *sk);

/* TCP timestamps are only 32-bits, this causes a slight
 * complication on 64-bit systems since we store a snapshot
 * of jiffies in the buffer control blocks below.  We decided
 * to use only the low 32-bits of jiffies and hide the ugly
 * casts with the following macro.
 */
#define tcp_time_stamp		((__u32)(jiffies))

/* This is what the send packet queuing engine uses to pass
 * TCP per-packet control information to the transmission
 * code.  We also store the host-order sequence numbers in
 * here too.  This is 36 bytes on 32-bit architectures,
 * 40 bytes on 64-bit machines, if this grows please adjust
 * skbuff.h:skbuff->cb[xxx] size appropriately.
 */
struct tcp_skb_cb {
	union {
		struct inet_skb_parm	h4;
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
		struct inet6_skb_parm	h6;
#endif
	} header;	/* For incoming frames		*/
	__u32		seq;		/* Starting sequence number	*/
	__u32		end_seq;	/* SEQ + FIN + SYN + datalen	*/
	__u32		when;		/* used to compute rtt's	*/
	__u8		flags;		/* TCP header flags.		*/

	/* NOTE: These must match up to the flags byte in a
	 *       real TCP header.
	 */
#define TCPCB_FLAG_FIN		0x01
#define TCPCB_FLAG_SYN		0x02
#define TCPCB_FLAG_RST		0x04
#define TCPCB_FLAG_PSH		0x08
#define TCPCB_FLAG_ACK		0x10
#define TCPCB_FLAG_URG		0x20
#define TCPCB_FLAG_ECE		0x40
#define TCPCB_FLAG_CWR		0x80

	__u8		sacked;		/* State flags for SACK/FACK.	*/
#define TCPCB_SACKED_ACKED	0x01	/* SKB ACK'd by a SACK block	*/
#define TCPCB_SACKED_RETRANS	0x02	/* SKB retransmitted		*/
#define TCPCB_LOST		0x04	/* SKB is lost			*/
#define TCPCB_TAGBITS		0x07	/* All tag bits			*/

#define TCPCB_EVER_RETRANS	0x80	/* Ever retransmitted frame	*/
#define TCPCB_RETRANS		(TCPCB_SACKED_RETRANS|TCPCB_EVER_RETRANS)

	__u32		ack_seq;	/* Sequence number ACK'd	*/
};

/*
 * TCP����SKB���е�˽����Ϣ���ƿ飬��skb_buff�ṹ��cb��Ա��TCP����
 *
 ����ֶδ洢��һ��tcp_skb_cb�ṹ����TCP�㣬�ú�TCP_SKB_CBʵ�ַ�
 * �ʸ���Ϣ���ƿ飬����ǿ����Ŀɶ��ԡ������˽����Ϣ���ƿ�ĸ�ֵ
 * һ���ڱ�����յ��λ��Ͷ�֮ǰ���С�����:tcp_v4_rcv��TCP�������ں��������յ�TCP�β�������б�Ҫ��У��󣬾ͻ�Դ˶ε�tcp_skb_cb�������á������͹����У�
 �󲿷���������TCP��ʱ�������ڶ�TCP�ν��зֶ�ʱ���ã����紴��TCP�ֶκ���tcp_fragment����MAC�㷢��ǰ����tso�ֶεĺ���tso_fragment������·��MTU̽��ĺ���tcp_mtu_probe��
 ����FIN�εĺ���tcp_send_fin,��Щ�������ᴴ��һ��TCP�Ρ��ڷ���TCP��ǰ�����tcp_skb_cb��ֵ���д�������ȡֵ���緢��TCP�εĺ���tcp_transmit_skb���Ӵ�TCP�ε�
 ����tcp_retransmit_skb.
 */ //TCP���չ����е�TCPѡ���ֶδӽ��յ�SKB�н�����������tcp_parse_options������TCPѡ���ֶδ����inet_request_sock��,IPѡ���ֶδ洢��skb->cb��
struct tcp_skb_cb { //��������TCP��IP�ײ�ѡ���ֶΣ����δ0����ʾû��Я����Ӧ��ѡ���ֶ�                
// //�ڷ�������tcp_sendmsg�е�skb_entail�����и�ֵ
    /*
     * ��TCP������յ���TCP��֮ǰ���²�Э��(IPv4��IPv6)���ȴ���öΣ��һ�
     * ����SKB�еĿ��ƿ�����¼ÿһ�����е���Ϣ������IPv4���¼��IP�ײ���
     * ��������IP�ײ�ѡ�Ϊ�˲��ƻ�����Э��˽�����ݣ���SKB��TCP���ƿ�
     * ��ǰ������������ṹ�������IPv4��IPv6��
     */
	union {
		struct inet_skb_parm	h4;
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
		struct inet6_skb_parm	h6;
#endif
	} header;	/* For incoming frames	����IPѡ��������Щ��TCPѡ��	*/ 
    /*
     * seqΪ��ǰ�ο�ʼ��ţ���end_seqΪ��ǰ�ο�ʼ��ż��ϵ�ǰ��
     * ���ݳ��ȣ������־���д���SYN��FIN��־������Ҫ��1����Ϊ
     * SYN��FIN��־��������һ����š�����end_seq��seq�ͱ�־����
     * ���׵õ����ݳ���
     */ //�⼸������Ǵ�TCP�ײ�����ȡ�����ģ���tcp_v4_rcv

     //�����seq end_seqӦ����Ӧ�ò������������ֱ�Ӵ浽SKB�У���ʱ�����ݳ���Ӧ��û�зֶεģ����ܴ���1500
	__u32		seq;		/* Starting sequence number	*/
	__u32		end_seq;	/* SEQ + FIN + SYN + datalen	*/
    /*
     * �η���ʱ�估�η���ʱ��¼�ĵ�ǰjiffiesֵ����Ҫʱ����ֵҲ
     * ��������RTT��
     * ��ֵͨ�������ⷢ��SKBʱʹ��tcp_time_stamp���ã�����tcp_write_xmit()��
     */
	__u32		when;		/* used to compute rtt's	*/
    /*
     * ��¼ԭʼTCP�ײ���־�����͹����У�tcp_transmit_skb()�ڷ���TCP
     * ��֮ǰ����ݴ˱�־����䷢�Ͷε�TCP�ײ��ı�־�򣻽��չ����У�
     * ����ȡ���նε�TCP�ײ���־�����ֶ���  ֵΪTCPCB_FLAG_SYN  FEN PUSH��
     */ //��tcp_sendmsg�е�TCP_SKB_CB�����øñ�־�� ��tcp_transmit_skb
	__u8		flags;		/* TCP header flags.		*/

	/* NOTE: These must match up to the flags byte in a
	 *       real TCP header.
	 */
#define TCPCB_FLAG_FIN		0x01
#define TCPCB_FLAG_SYN		0x02
#define TCPCB_FLAG_RST		0x04
#define TCPCB_FLAG_PSH		0x08
#define TCPCB_FLAG_ACK		0x10
#define TCPCB_FLAG_URG		0x20
#define TCPCB_FLAG_ECE		0x40
#define TCPCB_FLAG_CWR		0x80

    /*
     * ��Ҫ���������ε��ش�״̬��ͬʱ��ʶ���Ƿ�����������ݣ����ܵ�ȡֵ
     * ΪTCPCB_SACKED_ACKED�ȡ������յ���SACK��������Ҫ����TCPCB_TAGBITS
     * ��־λ���ش��������ݸñ�־λ��ȷ���Ƿ���Ҫ�ش���һ���ش���ʱ������
     * ���е�SACK״̬��־�����������Ϊ�����ٹ�����״̬������ͨ�����ַ�ʽ
     * �ش��˰����ش���ʱ������ش�����������TCPCB_EVER_RETRANS��־λ��
     * tcp_retransmit_skb()������TCPCB_SACKED_RETRANS��TCPCB_EVER_RETRANS
     * ��־λ��tcp_enter_loss()�������TCPCB_SACKED_RETRANS��־λ��
     * ֵ��ע����ǣ������������ش�״̬֮ǰ��sackedֵ���Ƕε��ش���־������
     * SACKѡ����TCP�ײ��е�ƫ�ƣ���ֵ�ڽ���TCP��֮���tcp_parse_options()��
     * ����TCPѡ��ʱ����ֵ��������tcp_sacktag_write_queue()�в���������SACK
     * ѡ���Ƕε��ش�״̬��
     */
	__u8		sacked;		/* State flags for SACK/FACK.	*/
/*
 * �ö�ͨ��SACK��ȷ��
 */
#define TCPCB_SACKED_ACKED	0x01	/* SKB ACK'd by a SACK block	*/
/*
 * �ö��Ѿ��ش�
 */
#define TCPCB_SACKED_RETRANS	0x02	/* SKB retransmitted		*/
/*
 * �ö��ڴ���������Ѷ�ʧ
 */
#define TCPCB_LOST		0x04	/* SKB is lost			*/
#define TCPCB_TAGBITS		0x07	/* All tag bits			*/

#define TCPCB_EVER_RETRANS	0x80	/* Ever retransmitted frame	*/
#define TCPCB_RETRANS		(TCPCB_SACKED_RETRANS|TCPCB_EVER_RETRANS)

    /*
     * ���յ���TCP���ײ��е�ȷ�����
     */
	__u32		ack_seq;	/* Sequence number ACK'd	*/
};


#define TCP_SKB_CB(__skb)	((struct tcp_skb_cb *)&((__skb)->cb[0]))

/* Due to TSO, an SKB can be composed of multiple actual
 * packets.  To keep these tracked properly, we use this.
 */
static inline int tcp_skb_pcount(const struct sk_buff *skb)
{
	return skb_shinfo(skb)->gso_segs;
}

/* This is valid iff tcp_skb_pcount() > 1. */
static inline int tcp_skb_mss(const struct sk_buff *skb)
{
	return skb_shinfo(skb)->gso_size;
}

/* Events passed to congestion control interface */
enum tcp_ca_event {
	CA_EVENT_TX_START,	/* first transmit when no packets in flight */
	CA_EVENT_CWND_RESTART,	/* congestion window restart */
	CA_EVENT_COMPLETE_CWR,	/* end of congestion recovery */
	CA_EVENT_FRTO,		/* fast recovery timeout */
	CA_EVENT_LOSS,		/* loss timeout */
	CA_EVENT_FAST_ACK,	/* in sequence ack */
	CA_EVENT_SLOW_ACK,	/* other ack */
};

/*
 * Interface for adding new TCP congestion control handlers
 */
#define TCP_CA_NAME_MAX	16
#define TCP_CA_MAX	128
#define TCP_CA_BUF_MAX	(TCP_CA_NAME_MAX*TCP_CA_MAX)

#define TCP_CONG_NON_RESTRICTED 0x1
#define TCP_CONG_RTT_STAMP	0x2

/*
 * tcp_congestion_ops�ṹ�ṩ��֧�ֶ���ӵ�������㷨�Ļ��ơ�
 * ӵ�������㷨ֻҪΪtcp_congestion_ops�ṹʵ��һ��ʵ����
 * ����ʵ�����е�һЩ�ӿڡ����磬����ʵ�ֽӿ�
 * ssthresh()��cong_avoid()�������ӿڿ�ѡ��
 */
struct tcp_congestion_ops {
	/*
	 * ����ע�ᵽϵͳ�в�ͬ�ĸ���ӵ���㷨��
	 */
	struct list_head	list;
	unsigned long flags; //ȡֵTCP_CONG_RTT_STAMP��

	/* initialize private data (optional) */
	/*
	 * ӵ���㷨�ĳ�ʼ�������������ض���
	 * ��ʼ�����̣���������ƿ��ĳ��
	 * ӵ�������㷨��ѡ��ʱ�����á�
	 */
	void (*init)(struct sock *sk);
	/* cleanup private data  (optional) */
	/*
	 * ���ر��׽��֣�������ƿ�ѡ������һ��ӵ��
	 * �����㷨ʱ��ԭ��ӵ�������㷨�Ĵ˽ӿ�����ʽ
	 * ����ǰ�ͻᱻ���ã����������������ر��׽�
	 * ��ʱ���˽ӿ�Ҳ�ᱻ���á��������Ҫ��������
	 * ����Ҳ���Բ�ʵ�ִ˽ӿڡ�
	 */
	void (*release)(struct sock *sk);

	/* return slow start threshold (required) */
	/*
	 * ���㲢�������������ޡ�
	 */
	u32 (*ssthresh)(struct sock *sk);
	/* lower bound for congestion window (optional) */
	/*
	 * ���㲢����ӵ��������Сֵ��
	 */
	u32 (*min_cwnd)(const struct sock *sk);
	/* do new cwnd calculation (required) */
	/*
	 * ��ӵ������ģʽ�����¼���ӵ�����ڡ�
	 * �ڽӿ���tcp_cong_avoid()�����л���á�
	 */
	void (*cong_avoid)(struct sock *sk, u32 ack, u32 in_flight);
	/* call before changing ca_state (optional) */
	/*
	 * ��ӵ������״̬�ı�ǰ���˽ӿڻᱻ���á�
	 */
	void (*set_state)(struct sock *sk, u8 new_state);
	/* call when cwnd event occurs (optional) */
	/*
	 * ����֪ͨӵ�������㷨�ڲ��¼��Ľӿڣ�����
	 * ӵ�����Ƶ��¼�����ʱ�����á�
	 */
	void (*cwnd_event)(struct sock *sk, enum tcp_ca_event ev);
	/* new value of cwnd after loss (optional) */
	/*
	 * �ڳ���"��Сӵ������"ʱ�����ʵ�ִ˽ӿڣ���
	 * ����ô˽ӿڳ���ӵ�����ڡ�
	 */
	u32  (*undo_cwnd)(struct sock *sk);
	/* hook for packet ack accounting (optional) */
	/*
	 * �����ͷ����յ�ACK���жα�ȷ��ʱ���˽ӿڱ����á�
	 * ����num_ackedΪ�˴�ACKȷ�ϵĶ�����
	 */
	void (*pkts_acked)(struct sock *sk, u32 num_acked, s32 rtt_us);
	/* get info for inet_diag (optional) */
	/*
	 * �ṩ��inet_diag�Ļ�ȡ��Ϣ�Ľӿڡ�
	 */
	void (*get_info)(struct sock *sk, u32 ext, struct sk_buff *skb);

	/*
	 * ӵ�������㷨�����ơ�
	 */
	char 		name[TCP_CA_NAME_MAX];
	struct module 	*owner;
};

struct tcp_congestion_ops1 {
	struct list_head	list;
	unsigned long flags;

	/* initialize private data (optional) */
	void (*init)(struct sock *sk);
	/* cleanup private data  (optional) */
	void (*release)(struct sock *sk);

	/* return slow start threshold (required) */
	u32 (*ssthresh)(struct sock *sk);
	/* lower bound for congestion window (optional) */
	u32 (*min_cwnd)(const struct sock *sk);
	/* do new cwnd calculation (required) */
	void (*cong_avoid)(struct sock *sk, u32 ack, u32 in_flight);
	/* call before changing ca_state (optional) */
	void (*set_state)(struct sock *sk, u8 new_state);
	/* call when cwnd event occurs (optional) */
	void (*cwnd_event)(struct sock *sk, enum tcp_ca_event ev);
	/* new value of cwnd after loss (optional) */
	u32  (*undo_cwnd)(struct sock *sk);
	/* hook for packet ack accounting (optional) */
	void (*pkts_acked)(struct sock *sk, u32 num_acked, s32 rtt_us);
	/* get info for inet_diag (optional) */
	void (*get_info)(struct sock *sk, u32 ext, struct sk_buff *skb);

	char 		name[TCP_CA_NAME_MAX];
	struct module 	*owner;
};

extern int tcp_register_congestion_control(struct tcp_congestion_ops *type);
extern void tcp_unregister_congestion_control(struct tcp_congestion_ops *type);

extern void tcp_init_congestion_control(struct sock *sk);
extern void tcp_cleanup_congestion_control(struct sock *sk);
extern int tcp_set_default_congestion_control(const char *name);
extern void tcp_get_default_congestion_control(char *name);
extern void tcp_get_available_congestion_control(char *buf, size_t len);
extern void tcp_get_allowed_congestion_control(char *buf, size_t len);
extern int tcp_set_allowed_congestion_control(char *allowed);
extern int tcp_set_congestion_control(struct sock *sk, const char *name);
extern void tcp_slow_start(struct tcp_sock *tp);
extern void tcp_cong_avoid_ai(struct tcp_sock *tp, u32 w);

extern struct tcp_congestion_ops tcp_init_congestion_ops;
extern u32 tcp_reno_ssthresh(struct sock *sk);
extern void tcp_reno_cong_avoid(struct sock *sk, u32 ack, u32 in_flight);
extern u32 tcp_reno_min_cwnd(const struct sock *sk);
extern struct tcp_congestion_ops tcp_reno;

static inline void tcp_set_ca_state(struct sock *sk, const u8 ca_state)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_ca_ops->set_state)
		icsk->icsk_ca_ops->set_state(sk, ca_state);
	icsk->icsk_ca_state = ca_state;
}

static inline void tcp_ca_event(struct sock *sk, const enum tcp_ca_event event)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_ca_ops->cwnd_event)
		icsk->icsk_ca_ops->cwnd_event(sk, event);
}

/* These functions determine how the current flow behaves in respect of SACK
 * handling. SACK is negotiated with the peer, and therefore it can vary
 * between different flows.
 *
 * tcp_is_sack - SACK enabled
 * tcp_is_reno - No SACK
 * tcp_is_fack - FACK enabled, implies SACK enabled
 */
static inline int tcp_is_sack(const struct tcp_sock *tp)
{
	return tp->rx_opt.sack_ok;
}

static inline int tcp_is_reno(const struct tcp_sock *tp)
{
	return !tcp_is_sack(tp);
}

static inline int tcp_is_fack(const struct tcp_sock *tp)
{
	return tp->rx_opt.sack_ok & 2;
}

static inline void tcp_enable_fack(struct tcp_sock *tp)
{
	tp->rx_opt.sack_ok |= 2;
}

static inline unsigned int tcp_left_out(const struct tcp_sock *tp)
{
	return tp->sacked_out + tp->lost_out;
}

/* This determines how many packets are "in the network" to the best
 * of our knowledge.  In many cases it is conservative, but where
 * detailed information is available from the receiver (via SACK
 * blocks etc.) we can make more aggressive calculations.
 *
 * Use this for decisions involving congestion control, use just
 * tp->packets_out to determine if the send queue is empty or not.
 *
 * Read this equation as:
 *
 *	"Packets sent once on transmission queue" MINUS
 *	"Packets left network, but not honestly ACKed yet" PLUS
 *	"Packets fast retransmitted"
 *//*
	 * ��ȡ���ڴ����еĶ���
	 */
static inline unsigned int tcp_packets_in_flight(const struct tcp_sock *tp)
{
    /*
	  * "�ѷ��͵���δȷ�ϵĶ���Ŀ"��ȥ"�Ѿ����������еĶ�
	  * ��Ŀ"���ټ���"�ش���δ�õ�ȷ�ϵĶ���Ŀ"���Ϳ���
	  * �õ��������еĶ���Ŀ��
	  */
	return tp->packets_out - tcp_left_out(tp) + tp->retrans_out;
}

#define TCP_INFINITE_SSTHRESH	0x7fffffff

static inline bool tcp_in_initial_slowstart(const struct tcp_sock *tp)
{
	return tp->snd_ssthresh >= TCP_INFINITE_SSTHRESH;
}

/* If cwnd > ssthresh, we may raise ssthresh to be half-way to cwnd.
 * The exception is rate halving phase, when cwnd is decreasing towards
 * ssthresh.
 */
static inline __u32 tcp_current_ssthresh(const struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	if ((1 << inet_csk(sk)->icsk_ca_state) & (TCPF_CA_CWR | TCPF_CA_Recovery))
		return tp->snd_ssthresh;
	else
		return max(tp->snd_ssthresh,
			   ((tp->snd_cwnd >> 1) +
			    (tp->snd_cwnd >> 2)));
}

/* Use define here intentionally to get WARN_ON location shown at the caller */
#define tcp_verify_left_out(tp)	WARN_ON(tcp_left_out(tp) > tp->packets_out)

extern void tcp_enter_cwr(struct sock *sk, const int set_ssthresh);
extern __u32 tcp_init_cwnd(struct tcp_sock *tp, struct dst_entry *dst);

/* Slow start with delack produces 3 packets of burst, so that
 * it is safe "de facto".  This will be the default - same as
 * the default reordering threshold - but if reordering increases,
 * we must be able to allow cwnd to burst at least this much in order
 * to not pull it back when holes are filled.
 */
static __inline__ __u32 tcp_max_burst(const struct tcp_sock *tp)
{
	return tp->reordering;
}

/* Returns end sequence number of the receiver's advertised window */
static inline u32 tcp_wnd_end(const struct tcp_sock *tp)
{
	return tp->snd_una + tp->snd_wnd;
}
extern int tcp_is_cwnd_limited(const struct sock *sk, u32 in_flight);

static inline void tcp_minshall_update(struct tcp_sock *tp, unsigned int mss,
				       const struct sk_buff *skb)
{
	if (skb->len < mss)
		tp->snd_sml = TCP_SKB_CB(skb)->end_seq;
}

static inline void tcp_check_probe_timer(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);

	if (!tp->packets_out && !icsk->icsk_pending)
		inet_csk_reset_xmit_timer(sk, ICSK_TIME_PROBE0,
					  icsk->icsk_rto, TCP_RTO_MAX);
}

static inline void tcp_init_wl(struct tcp_sock *tp, u32 seq)
{
	tp->snd_wl1 = seq;
}

static inline void tcp_update_wl(struct tcp_sock *tp, u32 seq)
{
	tp->snd_wl1 = seq;
}

/*
 * Calculate(/check) TCP checksum
 */
static inline __sum16 tcp_v4_check(int len, __be32 saddr,
				   __be32 daddr, __wsum base)
{
	return csum_tcpudp_magic(saddr,daddr,len,IPPROTO_TCP,base);
}

static inline __sum16 __tcp_checksum_complete(struct sk_buff *skb)
{
	return __skb_checksum_complete(skb);
}

/*
 * tcp_checksum_complete��tcp_checksum_complete_user���ǻ���α�ײ��ۼӺ�
 * ���ȫ��У��͵ļ�⡣��֮ͬ�����ڣ�ǰ������У��û�и��ص�TCP�Σ�������
 * ����У����ESTABLISHED״̬�½��յ��ĶΣ���Ȼ������������󶼵���
 * __tcp_checksum_complete���У�飬������ESTABLISHED״̬���漰������ƿ�
 * �Ƿ񱻽�������
 */
static inline int tcp_checksum_complete(struct sk_buff *skb)
{
	return !skb_csum_unnecessary(skb) &&
		__tcp_checksum_complete(skb);
}

/* Prequeue for VJ style copy to user, combined with checksumming. */

static inline void tcp_prequeue_init(struct tcp_sock *tp)
{
	tp->ucopy.task = NULL;
	tp->ucopy.len = 0;
	tp->ucopy.memory = 0;
	skb_queue_head_init(&tp->ucopy.prequeue);
#ifdef CONFIG_NET_DMA
	tp->ucopy.dma_chan = NULL;
	tp->ucopy.wakeup = 0;
	tp->ucopy.pinned_list = NULL;
	tp->ucopy.dma_cookie = 0;
#endif
}

/* Packet is added to VJ-style prequeue for processing in process
 * context, if a reader task is waiting. Apparently, this exciting
 * idea (VJ's mail "Re: query about TCP header on tcp-ip" of 07 Sep 93)
 * failed somewhere. Latency? Burstiness? Well, at least now we will
 * see, why it failed. 8)8)				  --ANK
 *
 * NOTE: is this not too big to inline?
 */
static inline int tcp_prequeue(struct sock *sk, struct sk_buff *skb)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (sysctl_tcp_low_latency || !tp->ucopy.task)
		return 0;

	__skb_queue_tail(&tp->ucopy.prequeue, skb);
	tp->ucopy.memory += skb->truesize;
	if (tp->ucopy.memory > sk->sk_rcvbuf) {
		struct sk_buff *skb1;

		BUG_ON(sock_owned_by_user(sk));

		while ((skb1 = __skb_dequeue(&tp->ucopy.prequeue)) != NULL) {
			sk_backlog_rcv(sk, skb1);
			NET_INC_STATS_BH(sock_net(sk),
					 LINUX_MIB_TCPPREQUEUEDROPPED);
		}

		tp->ucopy.memory = 0;
	} else if (skb_queue_len(&tp->ucopy.prequeue) == 1) {
		wake_up_interruptible_sync_poll(sk_sleep(sk),
					   POLLIN | POLLRDNORM | POLLRDBAND);
		if (!inet_csk_ack_scheduled(sk))
			inet_csk_reset_xmit_timer(sk, ICSK_TIME_DACK,
						  (3 * tcp_rto_min(sk)) / 4,
						  TCP_RTO_MAX);
	}
	return 1;
}


#undef STATE_TRACE

#ifdef STATE_TRACE
static const char *statename[]={
	"Unused","Established","Syn Sent","Syn Recv",
	"Fin Wait 1","Fin Wait 2","Time Wait", "Close",
	"Close Wait","Last ACK","Listen","Closing"
};
#endif
extern void tcp_set_state(struct sock *sk, int state);

extern void tcp_done(struct sock *sk);

static inline void tcp_sack_reset(struct tcp_options_received *rx_opt)
{
	rx_opt->dsack = 0;
	rx_opt->num_sacks = 0;
}

/* Determine a window scaling and initial window to offer. */
extern void tcp_select_initial_window(int __space, __u32 mss,
				      __u32 *rcv_wnd, __u32 *window_clamp,
				      int wscale_ok, __u8 *rcv_wscale,
				      __u32 init_rcv_wnd);

static inline int tcp_win_from_space(int space)
{
	return sysctl_tcp_adv_win_scale<=0 ?
		(space>>(-sysctl_tcp_adv_win_scale)) :
		space - (space>>sysctl_tcp_adv_win_scale);
}

/* Note: caller must be prepared to deal with negative returns */ 
static inline int tcp_space(const struct sock *sk)
{
	return tcp_win_from_space(sk->sk_rcvbuf -
				  atomic_read(&sk->sk_rmem_alloc));
} 

static inline int tcp_full_space(const struct sock *sk)
{
	return tcp_win_from_space(sk->sk_rcvbuf); 
}

static inline void tcp_openreq_init(struct request_sock *req,
				    struct tcp_options_received *rx_opt,
				    struct sk_buff *skb)
{
	struct inet_request_sock *ireq = inet_rsk(req);

	req->rcv_wnd = 0;		/* So that tcp_send_synack() knows! */
	req->cookie_ts = 0;
	tcp_rsk(req)->rcv_isn = TCP_SKB_CB(skb)->seq;
	req->mss = rx_opt->mss_clamp;
	req->ts_recent = rx_opt->saw_tstamp ? rx_opt->rcv_tsval : 0;
	ireq->tstamp_ok = rx_opt->tstamp_ok;
	ireq->sack_ok = rx_opt->sack_ok;
	ireq->snd_wscale = rx_opt->snd_wscale;
	ireq->wscale_ok = rx_opt->wscale_ok;
	ireq->acked = 0;
	ireq->ecn_ok = 0;
	ireq->rmt_port = tcp_hdr(skb)->source;
	ireq->loc_port = tcp_hdr(skb)->dest;
}

extern void tcp_enter_memory_pressure(struct sock *sk);

static inline int keepalive_intvl_when(const struct tcp_sock *tp)
{
	return tp->keepalive_intvl ? : sysctl_tcp_keepalive_intvl;
}

static inline int keepalive_time_when(const struct tcp_sock *tp)
{
	return tp->keepalive_time ? : sysctl_tcp_keepalive_time;
}

static inline int keepalive_probes(const struct tcp_sock *tp)
{
	return tp->keepalive_probes ? : sysctl_tcp_keepalive_probes;
}

/*
 * ��ȡ���һ���յ��Ķε�ĿǰΪֹ��ʱ�䣬
 * ����������ʱ�䡣
 */
static inline u32 keepalive_time_elapsed(const struct tcp_sock *tp)
{
	const struct inet_connection_sock *icsk = &tp->inet_conn;

	return min_t(u32, tcp_time_stamp - icsk->icsk_ack.lrcvtime,
			  tcp_time_stamp - tp->rcv_tstamp);
}

static inline int tcp_fin_time(const struct sock *sk)
{
	int fin_timeout = tcp_sk(sk)->linger2 ? : sysctl_tcp_fin_timeout;
	const int rto = inet_csk(sk)->icsk_rto;

   /*
    ������������Ϊʲôrto��ֵҪѡ��Ϊicsk->icsk_rto��3.5����Ҳ����RTO*3.5��������2����4���أ�����֪������FIN_WAIT_2״̬�½��յ�FIN���󣬻���Զ˷���ACK����
    ���TCP���ӵĹرա������������ACK�����ܶԶ�û���յ����ڹ���RTO����ʱ�ش�ʱ�䣩ʱ��󣬶Զ˻����·���FIN������ʱ��Ҫ�ٴθ��Զ˷���ACK��������TIME_WAIT
    ״̬�ĳ���ʱ��Ҫ��֤�Զ˿����ش�����FIN��������ش����εĻ���TIME_WAIT��ʱ��Ӧ��ΪRTO*��0.5+0.5+0.5��=RTO*1.5����������ȴ��RTO*3.5��������Ϊ���ش�����£�
    �ش���ʱʱ�����һ�ֳ�Ϊ��ָ���˱ܡ��ķ�ʽ���㡣���磺���ش���ʱʱ��Ϊ1S������·����������ش������Ǿ����ش���ʱʱ��Ϊ2S�Ķ�ʱ�����ش����ݣ���һ����4S��
    һֱ���ӵ�64SΪֹ���μ�tcp_retransmit_timer�����������������RTO*3.5=RTO*0.5+RTO*1+RTO*2,����RTO*0.5�ǵ�һ�η���ACK��ʱ�䵽�Զ˵ĳ�ʱʱ�䣨ϵ�����ǳ���RTO
    ��ֵ����RTO*1�ǶԶ˵�һ���ش�FIN����ACK������Զ˵ĳ�ʱʱ�䣬RTO*2�ǶԶ˵ڶ����ش�FIN����ACK������Զ˵ĳ�ʱʱ�䡣ע�⣬�ش���ʱʱ���ָ���˱ܲ���
    �����ǳ���2�������ش�֮��ִ�еģ����Ե�һ���ش��ĳ�ʱʱ��͵�һ�η��͵ĳ�ʱʱ����ͬ���������̼�ʱ��ֲ�����ͼ��ʾ��ע�⣺��ͷ��Ȼָ��Զˣ�ֻ����������
    ���̣����ݰ���δ�����յ�����
    �ο�:http://blog.csdn.net/justlinux2010/article/details/9070057
   
    * ���fin_timeoutʱ��С��3.5*rto,����������fin_timeoutʱ�䡣
    */
	if (fin_timeout < (rto << 2) - (rto >> 1))
		fin_timeout = (rto << 2) - (rto >> 1);  //fin��ʱʱ������Ҫ��֤3.5��rto

	return fin_timeout;
}

static inline int tcp_paws_check(const struct tcp_options_received *rx_opt,
				 int paws_win)
{
	if ((s32)(rx_opt->ts_recent - rx_opt->rcv_tsval) <= paws_win)
		return 1;
	if (unlikely(get_seconds() >= rx_opt->ts_recent_stamp + TCP_PAWS_24DAYS))
		return 1;

	return 0;
}

static inline int tcp_paws_reject(const struct tcp_options_received *rx_opt,
				  int rst)
{
	if (tcp_paws_check(rx_opt, 0))
		return 0;

	/* RST segments are not recommended to carry timestamp,
	   and, if they do, it is recommended to ignore PAWS because
	   "their cleanup function should take precedence over timestamps."
	   Certainly, it is mistake. It is necessary to understand the reasons
	   of this constraint to relax it: if peer reboots, clock may go
	   out-of-sync and half-open connections will not be reset.
	   Actually, the problem would be not existing if all
	   the implementations followed draft about maintaining clock
	   via reboots. Linux-2.2 DOES NOT!

	   However, we can relax time bounds for RST segments to MSL.
	 */
	if (rst && get_seconds() >= rx_opt->ts_recent_stamp + TCP_PAWS_MSL)
		return 0;
	return 1;
}

#define TCP_CHECK_TIMER(sk) do { } while (0)

static inline void tcp_mib_init(struct net *net)
{
	/* See RFC 2012 */
	TCP_ADD_STATS_USER(net, TCP_MIB_RTOALGORITHM, 1);
	TCP_ADD_STATS_USER(net, TCP_MIB_RTOMIN, TCP_RTO_MIN*1000/HZ);
	TCP_ADD_STATS_USER(net, TCP_MIB_RTOMAX, TCP_RTO_MAX*1000/HZ);
	TCP_ADD_STATS_USER(net, TCP_MIB_MAXCONN, -1);
}

/* from STCP */
static inline void tcp_clear_retrans_hints_partial(struct tcp_sock *tp)
{
	tp->lost_skb_hint = NULL;
	tp->scoreboard_skb_hint = NULL;
}

static inline void tcp_clear_all_retrans_hints(struct tcp_sock *tp)
{
	tcp_clear_retrans_hints_partial(tp);
	tp->retransmit_skb_hint = NULL;
}

/* MD5 Signature */
struct crypto_hash;

/* - key database */
struct tcp_md5sig_key {
	u8			*key;
	u8			keylen;
};

struct tcp4_md5sig_key {
	struct tcp_md5sig_key	base;
	__be32			addr;
};

struct tcp6_md5sig_key {
	struct tcp_md5sig_key	base;
#if 0
	u32			scope_id;	/* XXX */
#endif
	struct in6_addr		addr;
};

/* - sock block */
struct tcp_md5sig_info {
	struct tcp4_md5sig_key	*keys4;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	struct tcp6_md5sig_key	*keys6;
	u32			entries6;
	u32			alloced6;
#endif
	u32			entries4;
	u32			alloced4;
};

/* - pseudo header */
struct tcp4_pseudohdr {
	__be32		saddr;
	__be32		daddr;
	__u8		pad;
	__u8		protocol;
	__be16		len;
};

struct tcp6_pseudohdr {
	struct in6_addr	saddr;
	struct in6_addr daddr;
	__be32		len;
	__be32		protocol;	/* including padding */
};

union tcp_md5sum_block {
	struct tcp4_pseudohdr ip4;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	struct tcp6_pseudohdr ip6;
#endif
};

/* - pool: digest algorithm, hash description and scratch buffer */
struct tcp_md5sig_pool {
	struct hash_desc	md5_desc;
	union tcp_md5sum_block	md5_blk;
};

#define TCP_MD5SIG_MAXKEYS	(~(u32)0)	/* really?! */

/* - functions */
extern int			tcp_v4_md5_hash_skb(char *md5_hash,
						    struct tcp_md5sig_key *key,
						    struct sock *sk,
						    struct request_sock *req,
						    struct sk_buff *skb);

extern struct tcp_md5sig_key	*tcp_v4_md5_lookup(struct sock *sk,
						   struct sock *addr_sk);

extern int			tcp_v4_md5_do_add(struct sock *sk,
						  __be32 addr,
						  u8 *newkey,
						  u8 newkeylen);

extern int			tcp_v4_md5_do_del(struct sock *sk,
						  __be32 addr);

#ifdef CONFIG_TCP_MD5SIG
#define tcp_twsk_md5_key(twsk)	((twsk)->tw_md5_keylen ? 		 \
				 &(struct tcp_md5sig_key) {		 \
					.key = (twsk)->tw_md5_key,	 \
					.keylen = (twsk)->tw_md5_keylen, \
				} : NULL)
#else
#define tcp_twsk_md5_key(twsk)	NULL
#endif

extern struct tcp_md5sig_pool * __percpu *tcp_alloc_md5sig_pool(struct sock *);
extern void			tcp_free_md5sig_pool(void);

extern struct tcp_md5sig_pool	*tcp_get_md5sig_pool(void);
extern void			tcp_put_md5sig_pool(void);

extern int tcp_md5_hash_header(struct tcp_md5sig_pool *, struct tcphdr *);
extern int tcp_md5_hash_skb_data(struct tcp_md5sig_pool *, struct sk_buff *,
				 unsigned header_len);
extern int tcp_md5_hash_key(struct tcp_md5sig_pool *hp,
			    struct tcp_md5sig_key *key);

/* write queue abstraction */
static inline void tcp_write_queue_purge(struct sock *sk)
{
	struct sk_buff *skb;

	while ((skb = __skb_dequeue(&sk->sk_write_queue)) != NULL)
		sk_wmem_free_skb(sk, skb);
	sk_mem_reclaim(sk);
	tcp_clear_all_retrans_hints(tcp_sk(sk));
}

static inline struct sk_buff *tcp_write_queue_head(struct sock *sk)
{
	return skb_peek(&sk->sk_write_queue);
}

static inline struct sk_buff *tcp_write_queue_tail(struct sock *sk)
{
	return skb_peek_tail(&sk->sk_write_queue);
}

static inline struct sk_buff *tcp_write_queue_next(struct sock *sk, struct sk_buff *skb)
{
	return skb_queue_next(&sk->sk_write_queue, skb);
}

static inline struct sk_buff *tcp_write_queue_prev(struct sock *sk, struct sk_buff *skb)
{
	return skb_queue_prev(&sk->sk_write_queue, skb);
}

#define tcp_for_write_queue(skb, sk)					\
	skb_queue_walk(&(sk)->sk_write_queue, skb)

#define tcp_for_write_queue_from(skb, sk)				\
	skb_queue_walk_from(&(sk)->sk_write_queue, skb)

#define tcp_for_write_queue_from_safe(skb, tmp, sk)			\
	skb_queue_walk_from_safe(&(sk)->sk_write_queue, skb, tmp)

static inline struct sk_buff *tcp_send_head(struct sock *sk)
{
	return sk->sk_send_head;
}

static inline bool tcp_skb_is_last(const struct sock *sk,
				   const struct sk_buff *skb)
{
	return skb_queue_is_last(&sk->sk_write_queue, skb);
}

static inline void tcp_advance_send_head(struct sock *sk, struct sk_buff *skb)
{
	if (tcp_skb_is_last(sk, skb))
		sk->sk_send_head = NULL;
	else
		sk->sk_send_head = tcp_write_queue_next(sk, skb);
}

static inline void tcp_check_send_head(struct sock *sk, struct sk_buff *skb_unlinked)
{
	if (sk->sk_send_head == skb_unlinked)
		sk->sk_send_head = NULL;
}

static inline void tcp_init_send_head(struct sock *sk)
{
	sk->sk_send_head = NULL;
}

static inline void __tcp_add_write_queue_tail(struct sock *sk, struct sk_buff *skb)
{
	__skb_queue_tail(&sk->sk_write_queue, skb);
}

static inline void tcp_add_write_queue_tail(struct sock *sk, struct sk_buff *skb)
{
	__tcp_add_write_queue_tail(sk, skb);

	/* Queue it, remembering where we must start sending. */
	if (sk->sk_send_head == NULL) {
		sk->sk_send_head = skb;

		if (tcp_sk(sk)->highest_sack == NULL)
			tcp_sk(sk)->highest_sack = skb;
	}
}

static inline void __tcp_add_write_queue_head(struct sock *sk, struct sk_buff *skb)
{
	__skb_queue_head(&sk->sk_write_queue, skb);
}

/* Insert buff after skb on the write queue of sk.  */
static inline void tcp_insert_write_queue_after(struct sk_buff *skb,
						struct sk_buff *buff,
						struct sock *sk)
{
	__skb_queue_after(&sk->sk_write_queue, skb, buff);
}

/* Insert new before skb on the write queue of sk.  */
static inline void tcp_insert_write_queue_before(struct sk_buff *new,
						  struct sk_buff *skb,
						  struct sock *sk)
{
	__skb_queue_before(&sk->sk_write_queue, skb, new);

	if (sk->sk_send_head == skb)
		sk->sk_send_head = new;
}

static inline void tcp_unlink_write_queue(struct sk_buff *skb, struct sock *sk)
{
	__skb_unlink(skb, &sk->sk_write_queue);
}

static inline int tcp_write_queue_empty(struct sock *sk)
{
	return skb_queue_empty(&sk->sk_write_queue);
}

static inline void tcp_push_pending_frames(struct sock *sk)
{
	if (tcp_send_head(sk)) {
		struct tcp_sock *tp = tcp_sk(sk);

		__tcp_push_pending_frames(sk, tcp_current_mss(sk), tp->nonagle);
	}
}

/* Start sequence of the highest skb with SACKed bit, valid only if
 * sacked > 0 or when the caller has ensured validity by itself.
 */
static inline u32 tcp_highest_sack_seq(struct tcp_sock *tp)
{
	if (!tp->sacked_out)
		return tp->snd_una;

	if (tp->highest_sack == NULL)
		return tp->snd_nxt;

	return TCP_SKB_CB(tp->highest_sack)->seq;
}

static inline void tcp_advance_highest_sack(struct sock *sk, struct sk_buff *skb)
{
	tcp_sk(sk)->highest_sack = tcp_skb_is_last(sk, skb) ? NULL :
						tcp_write_queue_next(sk, skb);
}

static inline struct sk_buff *tcp_highest_sack(struct sock *sk)
{
	return tcp_sk(sk)->highest_sack;
}

static inline void tcp_highest_sack_reset(struct sock *sk)
{
	tcp_sk(sk)->highest_sack = tcp_write_queue_head(sk);
}

/* Called when old skb is about to be deleted (to be combined with new skb) */
static inline void tcp_highest_sack_combine(struct sock *sk,
					    struct sk_buff *old,
					    struct sk_buff *new)
{
	if (tcp_sk(sk)->sacked_out && (old == tcp_sk(sk)->highest_sack))
		tcp_sk(sk)->highest_sack = new;
}

/* Determines whether this is a thin stream (which may suffer from
 * increased latency). Used to trigger latency-reducing mechanisms.
 */
static inline unsigned int tcp_stream_is_thin(struct tcp_sock *tp)
{
	return tp->packets_out < 4 && !tcp_in_initial_slowstart(tp);
}

/* /proc */
enum tcp_seq_states {
	TCP_SEQ_STATE_LISTENING,
	TCP_SEQ_STATE_OPENREQ,
	TCP_SEQ_STATE_ESTABLISHED,
	TCP_SEQ_STATE_TIME_WAIT,
};

struct tcp_seq_afinfo {
	char			*name;
	sa_family_t		family;
	struct file_operations	seq_fops;
	struct seq_operations	seq_ops;
};

struct tcp_iter_state {
	struct seq_net_private	p;
	sa_family_t		family;
	enum tcp_seq_states	state;
	struct sock		*syn_wait_sk;
	int			bucket, sbucket, num, uid;
};

extern int tcp_proc_register(struct net *net, struct tcp_seq_afinfo *afinfo);
extern void tcp_proc_unregister(struct net *net, struct tcp_seq_afinfo *afinfo);

extern struct request_sock_ops tcp_request_sock_ops;
extern struct request_sock_ops tcp6_request_sock_ops;

extern void tcp_v4_destroy_sock(struct sock *sk);

extern int tcp_v4_gso_send_check(struct sk_buff *skb);
extern struct sk_buff *tcp_tso_segment(struct sk_buff *skb, int features);
extern struct sk_buff **tcp_gro_receive(struct sk_buff **head,
					struct sk_buff *skb);
extern struct sk_buff **tcp4_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb);
extern int tcp_gro_complete(struct sk_buff *skb);
extern int tcp4_gro_complete(struct sk_buff *skb);

#ifdef CONFIG_PROC_FS
extern int  tcp4_proc_init(void);
extern void tcp4_proc_exit(void);
#endif

/* TCP af-specific functions */
struct tcp_sock_af_ops {
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key	*(*md5_lookup) (struct sock *sk,
						struct sock *addr_sk);
	int			(*calc_md5_hash) (char *location,
						  struct tcp_md5sig_key *md5,
						  struct sock *sk,
						  struct request_sock *req,
						  struct sk_buff *skb);
	int			(*md5_add) (struct sock *sk,
					    struct sock *addr_sk,
					    u8 *newkey,
					    u8 len);
	int			(*md5_parse) (struct sock *sk,
					      char __user *optval,
					      int optlen);
#endif
};

struct tcp_request_sock_ops {
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key	*(*md5_lookup) (struct sock *sk,
						struct request_sock *req);
	int			(*calc_md5_hash) (char *location,
						  struct tcp_md5sig_key *md5,
						  struct sock *sk,
						  struct request_sock *req,
						  struct sk_buff *skb);
#endif
};

/* Using SHA1 for now, define some constants.
 */
#define COOKIE_DIGEST_WORDS (SHA_DIGEST_WORDS)
#define COOKIE_MESSAGE_WORDS (SHA_MESSAGE_BYTES / 4)
#define COOKIE_WORKSPACE_WORDS (COOKIE_DIGEST_WORDS + COOKIE_MESSAGE_WORDS)

extern int tcp_cookie_generator(u32 *bakery);

/**
 *	struct tcp_cookie_values - each socket needs extra space for the
 *	cookies, together with (optional) space for any SYN data.
 *
 *	A tcp_sock contains a pointer to the current value, and this is
 *	cloned to the tcp_timewait_sock.
 *
 * @cookie_pair:	variable data from the option exchange.
 *
 * @cookie_desired:	user specified tcpct_cookie_desired.  Zero
 *			indicates default (sysctl_tcp_cookie_size).
 *			After cookie sent, remembers size of cookie.
 *			Range 0, TCP_COOKIE_MIN to TCP_COOKIE_MAX.
 *
 * @s_data_desired:	user specified tcpct_s_data_desired.  When the
 *			constant payload is specified (@s_data_constant),
 *			holds its length instead.
 *			Range 0 to TCP_MSS_DESIRED.
 *
 * @s_data_payload:	constant data that is to be included in the
 *			payload of SYN or SYNACK segments when the
 *			cookie option is present.
 */
struct tcp_cookie_values {
	struct kref	kref;
	u8		cookie_pair[TCP_COOKIE_PAIR_SIZE];
	u8		cookie_pair_size;
	u8		cookie_desired;
	u16		s_data_desired:11,
			s_data_constant:1,
			s_data_in:1,
			s_data_out:1,
			s_data_unused:2;
	u8		s_data_payload[0];
};

static inline void tcp_cookie_values_release(struct kref *kref)
{
	kfree(container_of(kref, struct tcp_cookie_values, kref));
}

/* The length of constant payload data.  Note that s_data_desired is
 * overloaded, depending on s_data_constant: either the length of constant
 * data (returned here) or the limit on variable data.
 */
static inline int tcp_s_data_size(const struct tcp_sock *tp)
{
	return (tp->cookie_values != NULL && tp->cookie_values->s_data_constant)
		? tp->cookie_values->s_data_desired
		: 0;
}

/**
 *	struct tcp_extend_values - tcp_ipv?.c to tcp_output.c workspace.
 *
 *	As tcp_request_sock has already been extended in other places, the
 *	only remaining method is to pass stack values along as function
 *	parameters.  These parameters are not needed after sending SYNACK.
 *
 * @cookie_bakery:	cryptographic secret and message workspace.
 *
 * @cookie_plus:	bytes in authenticator/cookie option, copied from
 *			struct tcp_options_received (above).
 */
struct tcp_extend_values {
	struct request_values		rv;
	u32				cookie_bakery[COOKIE_WORKSPACE_WORDS];
	u8				cookie_plus:6,
					cookie_out_never:1,
					cookie_in_always:1;
};

static inline struct tcp_extend_values *tcp_xv(struct request_values *rvp)
{
	return (struct tcp_extend_values *)rvp;
}

extern void tcp_v4_init(void);
extern void tcp_init(void);

#endif	/* _TCP_H */
