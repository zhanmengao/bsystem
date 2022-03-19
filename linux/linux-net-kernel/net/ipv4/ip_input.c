/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		The Internet Protocol (IP) module.
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Donald Becker, <becker@super.org>
 *		Alan Cox, <alan@lxorguk.ukuu.org.uk>
 *		Richard Underwood
 *		Stefan Becker, <stefanb@yello.ping.de>
 *		Jorge Cwik, <jorge@laser.satlink.net>
 *		Arnt Gulbrandsen, <agulbra@nvg.unit.no>
 *
 *
 * Fixes:
 *		Alan Cox	:	Commented a couple of minor bits of surplus code
 *		Alan Cox	:	Undefining IP_FORWARD doesn't include the code
 *					(just stops a compiler warning).
 *		Alan Cox	:	Frames with >=MAX_ROUTE record routes, strict routes or loose routes
 *					are junked rather than corrupting things.
 *		Alan Cox	:	Frames to bad broadcast subnets are dumped
 *					We used to process them non broadcast and
 *					boy could that cause havoc.
 *		Alan Cox	:	ip_forward sets the free flag on the
 *					new frame it queues. Still crap because
 *					it copies the frame but at least it
 *					doesn't eat memory too.
 *		Alan Cox	:	Generic queue code and memory fixes.
 *		Fred Van Kempen :	IP fragment support (borrowed from NET2E)
 *		Gerhard Koerting:	Forward fragmented frames correctly.
 *		Gerhard Koerting: 	Fixes to my fix of the above 8-).
 *		Gerhard Koerting:	IP interface addressing fix.
 *		Linus Torvalds	:	More robustness checks
 *		Alan Cox	:	Even more checks: Still not as robust as it ought to be
 *		Alan Cox	:	Save IP header pointer for later
 *		Alan Cox	:	ip option setting
 *		Alan Cox	:	Use ip_tos/ip_ttl settings
 *		Alan Cox	:	Fragmentation bogosity removed
 *					(Thanks to Mark.Bush@prg.ox.ac.uk)
 *		Dmitry Gorodchanin :	Send of a raw packet crash fix.
 *		Alan Cox	:	Silly ip bug when an overlength
 *					fragment turns up. Now frees the
 *					queue.
 *		Linus Torvalds/ :	Memory leakage on fragmentation
 *		Alan Cox	:	handling.
 *		Gerhard Koerting:	Forwarding uses IP priority hints
 *		Teemu Rantanen	:	Fragment problems.
 *		Alan Cox	:	General cleanup, comments and reformat
 *		Alan Cox	:	SNMP statistics
 *		Alan Cox	:	BSD address rule semantics. Also see
 *					UDP as there is a nasty checksum issue
 *					if you do things the wrong way.
 *		Alan Cox	:	Always defrag, moved IP_FORWARD to the config.in file
 *		Alan Cox	: 	IP options adjust sk->priority.
 *		Pedro Roque	:	Fix mtu/length error in ip_forward.
 *		Alan Cox	:	Avoid ip_chk_addr when possible.
 *	Richard Underwood	:	IP multicasting.
 *		Alan Cox	:	Cleaned up multicast handlers.
 *		Alan Cox	:	RAW sockets demultiplex in the BSD style.
 *		Gunther Mayer	:	Fix the SNMP reporting typo
 *		Alan Cox	:	Always in group 224.0.0.1
 *	Pauline Middelink	:	Fast ip_checksum update when forwarding
 *					Masquerading support.
 *		Alan Cox	:	Multicast loopback error for 224.0.0.1
 *		Alan Cox	:	IP_MULTICAST_LOOP option.
 *		Alan Cox	:	Use notifiers.
 *		Bjorn Ekwall	:	Removed ip_csum (from slhc.c too)
 *		Bjorn Ekwall	:	Moved ip_fast_csum to ip.h (inline!)
 *		Stefan Becker   :       Send out ICMP HOST REDIRECT
 *	Arnt Gulbrandsen	:	ip_build_xmit
 *		Alan Cox	:	Per socket routing cache
 *		Alan Cox	:	Fixed routing cache, added header cache.
 *		Alan Cox	:	Loopback didn't work right in original ip_build_xmit - fixed it.
 *		Alan Cox	:	Only send ICMP_REDIRECT if src/dest are the same net.
 *		Alan Cox	:	Incoming IP option handling.
 *		Alan Cox	:	Set saddr on raw output frames as per BSD.
 *		Alan Cox	:	Stopped broadcast source route explosions.
 *		Alan Cox	:	Can disable source routing
 *		Takeshi Sone    :	Masquerading didn't work.
 *	Dave Bonn,Alan Cox	:	Faster IP forwarding whenever possible.
 *		Alan Cox	:	Memory leaks, tramples, misc debugging.
 *		Alan Cox	:	Fixed multicast (by popular demand 8))
 *		Alan Cox	:	Fixed forwarding (by even more popular demand 8))
 *		Alan Cox	:	Fixed SNMP statistics [I think]
 *	Gerhard Koerting	:	IP fragmentation forwarding fix
 *		Alan Cox	:	Device lock against page fault.
 *		Alan Cox	:	IP_HDRINCL facility.
 *	Werner Almesberger	:	Zero fragment bug
 *		Alan Cox	:	RAW IP frame length bug
 *		Alan Cox	:	Outgoing firewall on build_xmit
 *		A.N.Kuznetsov	:	IP_OPTIONS support throughout the kernel
 *		Alan Cox	:	Multicast routing hooks
 *		Jos Vos		:	Do accounting *before* call_in_firewall
 *	Willy Konynenberg	:	Transparent proxying support
 *
 *
 *
 * To Fix:
 *		IP fragmentation wants rewriting cleanly. The RFC815 algorithm is much more efficient
 *		and could be made very efficient with the addition of some virtual memory hacks to permit
 *		the allocation of a buffer that can then be 'grown' by twiddling page tables.
 *		Output fragmentation wants updating along with the buffer management to use a single
 *		interleaved copy algorithm so that fragmenting has a one copy overhead. Actual packet
 *		output should probably do its own fragmentation at the UDP/RAW layer. TCP shouldn't cause
 *		fragmentation anyway.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */

#include <asm/system.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include <linux/net.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include <net/snmp.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <net/route.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/arp.h>
#include <net/icmp.h>
#include <net/raw.h>
#include <net/checksum.h>
#include <linux/netfilter_ipv4.h>
#include <net/xfrm.h>
#include <linux/mroute.h>
#include <linux/netlink.h>

/*
 *	Process Router Attention IP option
 */
int ip_call_ra_chain(struct sk_buff *skb)
{
	struct ip_ra_chain *ra;
	u8 protocol = ip_hdr(skb)->protocol;
	struct sock *last = NULL;
	struct net_device *dev = skb->dev;

	read_lock(&ip_ra_lock);
	for (ra = ip_ra_chain; ra; ra = ra->next) {
		struct sock *sk = ra->sk;

		/* If socket is bound to an interface, only report
		 * the packet if it came  from that interface.
		 */
		if (sk && inet_sk(sk)->inet_num == protocol &&
		    (!sk->sk_bound_dev_if ||
		     sk->sk_bound_dev_if == dev->ifindex) &&
		    net_eq(sock_net(sk), dev_net(dev))) {
			if (ip_hdr(skb)->frag_off & htons(IP_MF | IP_OFFSET)) {
				if (ip_defrag(skb, IP_DEFRAG_CALL_RA_CHAIN)) {
					read_unlock(&ip_ra_lock);
					return 1;
				}
			}
			if (last) {
				struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
				if (skb2)
					raw_rcv(last, skb2);
			}
			last = sk;
		}
	}

	if (last) {
		raw_rcv(last, skb);
		read_unlock(&ip_ra_lock);
		return 1;
	}
	read_unlock(&ip_ra_lock);
	return 0;
}

//���������L4�����
/*
 * ip_local_deliver_finish()���������ݰ�������㴫��
 * ������㡣��������:
 * 1)���ȣ������ݰ����ݸ������֮ǰ��ȥ��IP�ײ�
 * 2)���ţ������RAW�׽��ֽ������ݰ�������Ҫ
 * ����һ�ݸ��������뵽���ո����ݰ����׽��֡�
 * 3)���ͨ�������Ľ������̣������ݰ�����
 * ������㣬�ɴ������д���
 */
static int ip_local_deliver_finish(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dev);

	/*
	 * �����ݰ����ݸ������֮ǰ����ȥ��
	 * IP�ײ���
	 */
	__skb_pull(skb, ip_hdrlen(skb));

	/* Point into the IP datagram, just past the header. */
       /* "ɾ��"IP �ײ�*/
	skb_reset_transport_header(skb);

	rcu_read_lock();
	{     /* ��ȡ��IP�����ص��ϲ�Э��,Ҳ���Ǵ����Э��ţ����ڼ�������hash*/
		int protocol = ip_hdr(skb)->protocol;
		int hash, raw;
		const struct net_protocol *ipprot;

	resubmit:
		/*
		 * ����RAW�׽��֣��ȸ��ݴ����Э���
		 * �õ���ϣֵ��Ȼ��鿴raw_v4_htableɢ�б�
		 * ���Ը�ֵΪ�ؼ��ֵĹ�ϣͰ�Ƿ�Ϊ�գ�
		 * �����Ϊ�գ���˵��������RAW�׽��֣�
		 * ���Ƹ����ݰ��ĸ������뵽ע�ᵽ
		 * ��Ͱ�е������׽��֡�
		 */
/*
ip_local_deliver_finish�������ȼ���ϣ��raw_v4_htable����Ϊ�ڴ��� socketʱ��inet_create���Э���IPPROTO_ICMP��ֵ����socket�ĳ�Աnum��
����numΪ��ֵ����socket����� ���raw_v4_htable��raw_v4_htable[IPPROTO_ICMP&(MAX_INET_PROTOS-1)]�ϼ������ ���socket��ʵ������һ��socket������
�����������socketҪ�����������Ӧ��Ҳ�ᱻ�ŵ�������һ���� ��ip_local_deliver_finish�յ����ݱ���ȡ�����socket����(Ŀǰʵ����ֻ��һ��)��
����raw_v4_input���� skb����ÿһ��socket���д���Ȼ�󣬻���Ҫ�����ݱ�����inet_protos[IPPROTO_ICMP& (MAX_INET_PROTOS-1)]����icmp_rcv����
��Ϊ����icmp���ģ�ÿһ��������Ҫ����Э��ջ����ģ����Ի���Ӧ ��icmp_rcvֻ�Ǽ򵥶�������δʵ�ʴ���
*/
		 ////֮ǰ����֡��ʱ��icmp��ͨ������������ĺ�����sock_queue_rcv_skb����
		raw = raw_local_deliver(skb, protocol); //�����raw�׽��֣�����ú�������Ḵ��һ��skb��Ȼ���͵���������ping 1.2.2.2��ʱ�򣬻��������棬������icmp_recv

		hash = protocol & (MAX_INET_PROTOS - 1);
		ipprot = rcu_dereference(inet_protos[hash]);
		/*
		 * ͨ������inet_portos���飬ȷ���Ƿ�
		 * ע������IP�ײ��д����Э���
		 * һ�µĴ����Э�顣���������У�
		 * ��ִ�ж�Ӧ�Ĵ����Э�����̡�
		 */
		if (ipprot != NULL) {
			int ret;

			if (!net_eq(net, &init_net) && !ipprot->netns_ok) {
				if (net_ratelimit())
					printk("%s: proto %d isn't netns-ready\n",
						__func__, protocol);
				kfree_skb(skb);
				goto out;
			}

			if (!ipprot->no_policy) {
				if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
					kfree_skb(skb);
					goto out;
				}
				nf_reset(skb);
			}
			ret = ipprot->handler(skb);//����������udp tcp�����ȥ
			if (ret < 0) {
				protocol = -ret;
				goto resubmit;
			}
			IP_INC_STATS_BH(net, IPSTATS_MIB_INDELIVERS);
		} else {
			/*
			 * ���û����Ӧ��Э�鴫�����ո����ݰ���
			 * ���ͷŸ����ݰ������ͷ�ǰ�������RAW
			 * �׽���û�н��ջ�����쳣���������
			 * һ��Ŀ�Ĳ��ɴ�ICMP���ĸ����ͷ�����ʾ�ð�rawû�н��ղ���inet_protos��û��ע���Э��
			 */
			if (!raw) {
				if (xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
					IP_INC_STATS_BH(net, IPSTATS_MIB_INUNKNOWNPROTOS);
					icmp_send(skb, ICMP_DEST_UNREACH,
						  ICMP_PROT_UNREACH, 0);
				}
			} else
				IP_INC_STATS_BH(net, IPSTATS_MIB_INDELIVERS);
			kfree_skb(skb);
		}
	}
 out:
	rcu_read_unlock();

	return 0;
}


/*
 * 	Deliver IP Packets to the higher protocol layers.
 */
 /*
  * ��ip_route_input����·��ѡ���������յİ�
  * �Ƿ��͸������������ip_local_deliver�����ݸ��ϲ�Э��
  */
  //ip_route_input_slow->ip_local_deliver
int ip_local_deliver(struct sk_buff *skb)
{
    /*
     *  Reassemble IP fragments.
     */
        /* 
         * frag_off��16λ�����и�3λ������־λ��
         * ��13λ����������ƫ����.
         * �ں˿�ͨ�����õķ�Ƭ��ʶλ���0
         * �ķ�Ƭƫ����ʶ���Ƭ�ķ��顣ƫ��
         * ���ֶ�Ϊ0���������Ƿ�������һ����Ƭ��
         * 
         * ������յ���IP���ݰ�ʱ��Ƭ�������
         * ip_defrag()�������飬���־λIP_DEFRAG_LOCAL_DELIVER��
         */
    if (ip_hdr(skb)->frag_off & htons(IP_MF | IP_OFFSET)) {
       /*
        * ������Ϸ�Ƭ����ĸ������֡�
        * 
        * ���ip_defrag()���ط�0�����ʾIP���ݰ���Ƭ
        * ��δ���룬����û����ɣ����߳���ֱ��
        * ���ء�Ϊ0�����ʾ�����IP���ݰ������飬
        * ��Ҫ���ݵ��������д���
        */
        if (ip_defrag(skb, IP_DEFRAG_LOCAL_DELIVER))
            return 0;
    }

    /*
     * ����netfilter����󣬵���ip_local_deliver_finish()��
     * ����װ��ɵ�IP���ݰ����͵�����㴦��
     */
    return NF_HOOK(PF_INET, NF_INET_LOCAL_IN, skb, skb->dev, NULL,
               ip_local_deliver_finish);
}


static inline int ip_rcv_options(struct sk_buff *skb)
{
	struct ip_options *opt;
	struct iphdr *iph;
	struct net_device *dev = skb->dev;

	/* It looks as overkill, because not all
	   IP options require packet mangling.
	   But it is the easiest for now, especially taking
	   into account that combination of IP options
	   and running sniffer is extremely rare condition.
					      --ANK (980813)
	*/
	if (skb_cow(skb, skb_headroom(skb))) {
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INDISCARDS);
		goto drop;
	}

	iph = ip_hdr(skb);
	opt = &(IPCB(skb)->opt);
	opt->optlen = iph->ihl*4 - sizeof(struct iphdr);

	if (ip_options_compile(dev_net(dev), opt, skb)) {
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INHDRERRORS);
		goto drop;
	}

	if (unlikely(opt->srr)) {
		struct in_device *in_dev = in_dev_get(dev);
		if (in_dev) {
			if (!IN_DEV_SOURCE_ROUTE(in_dev)) {
				if (IN_DEV_LOG_MARTIANS(in_dev) &&
				    net_ratelimit())
					printk(KERN_INFO "source route option %pI4 -> %pI4\n",
					       &iph->saddr, &iph->daddr);
				in_dev_put(in_dev);
				goto drop;
			}

			in_dev_put(in_dev);
		}

		if (ip_options_rcv_srr(skb))
			goto drop;
	}

	return 0;
drop:
	return -1;
}

/*
 * ip_rcv_finish()��ip_rcv()�е�IP���ݰ�����netfilterģ��
 * ����󱻵��á���ɵ���Ҫ�����ǣ����
 * ��û��Ϊ�����ݰ���������·�ɻ��棬��
 * ����ip_route_input()Ϊ���������·�ɻ��档
 * ���Ŵ���IP���ݰ��ײ��е�ѡ����
 * ��������·�ɻ������뵽���ػ�ץ����
 */
static int ip_rcv_finish(struct sk_buff *skb)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct rtable *rt;

	/*
	 *	Initialise the virtual path cache for the packet. It describes
	 *	how the packet travels inside Linux networking.
	 */
	/*
	 * �����û��Ϊ�����ݰ���������·�ɻ��棬
	 * �����ip_route_input()Ϊ���������·�ɻ��档
	 * ������ʧ�ܣ��򽫸����ݰ�������
	 */
	if (skb_dst(skb) == NULL) {
              /* ѡ��·��*/
		int err = ip_route_input(skb, iph->daddr, iph->saddr, iph->tos,
					 skb->dev);//���ջ����ip_local_deliver
		if (unlikely(err)) {
			if (err == -EHOSTUNREACH)
				IP_INC_STATS_BH(dev_net(skb->dev),
						IPSTATS_MIB_INADDRERRORS);
			else if (err == -ENETUNREACH)
				IP_INC_STATS_BH(dev_net(skb->dev),
						IPSTATS_MIB_INNOROUTES);
			goto drop;
		}
	}

#ifdef CONFIG_NET_CLS_ROUTE
	if (unlikely(skb_dst(skb)->tclassid)) {
		struct ip_rt_acct *st = per_cpu_ptr(ip_rt_acct, smp_processor_id());
		u32 idx = skb_dst(skb)->tclassid;
		st[idx&0xFF].o_packets++;
		st[idx&0xFF].o_bytes += skb->len;
		st[(idx>>16)&0xFF].i_packets++;
		st[(idx>>16)&0xFF].i_bytes += skb->len;
	}
#endif

	/*
	 * ���ݳ����ж�IP�ײ����Ƿ����ѡ�����У�
	 * �����ip_rcv_options()����IPѡ�
	 */
	if (iph->ihl > 5 && ip_rcv_options(skb))
		goto drop;

	rt = skb_rtable(skb);
	if (rt->rt_type == RTN_MULTICAST) {
		IP_UPD_PO_STATS_BH(dev_net(rt->u.dst.dev), IPSTATS_MIB_INMCAST,
				skb->len);
	} else if (rt->rt_type == RTN_BROADCAST)
		IP_UPD_PO_STATS_BH(dev_net(rt->u.dst.dev), IPSTATS_MIB_INBCAST,
				skb->len);

	/*
	 * ����������·�ɻ���������뵽���ػ�
	 * ת��������ǰ�ߵ���ip_local_deliver()�����ߵ���
	 * ip_forward()��
	 * �������뵽���ػ�ת�����鲥���ģ��ھ���netfilter����
	 * ֮������ip_rcv_finish()��ʽ��������Ĵ����ȵ���
	 * ip_route_input()��������·�ɵĲ�ѯ���������Ŀ�ĵ�ַ
	 * Ϊ�鲥��ַ���ͻᰴ���鲥��ַ�Ĺ������·�ɣ�����
	 * ���鲥������·�ɺ��鲥���Ľ��մ�����Ϊip_mr_input()��
	 * �μ�ip_route_input_mc().
	 */
	return dst_input(skb);//ip_route_input->ip_route_input_common->ip_route_input_slow����ע��  ip_local_deliver;//������  ip_forwardת��

drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}

EXPORT_SYMBOL_GPL(ip_rcv_finish);

extern int dev_d1300;
/*
 * 	Main IP Receive routine.
 yang add
 ����һ�ڿ��Կ�������·�㽫���ݰ��ϴ���IP��ʱ����IP�����Э��Ĵ������̴�������IPЭ�飬���ע��Ĵ���������ip_rcv()��������
 ��ɺ󽻸�NETFILTE��PRE-ROUTING��R���ˣ����ϵݸ�ip_rcv_finish(), �����������skb���е�·����Ϣ������������ݰ���ת�������Ͻ�
 ���������ɴ˲�������·����һΪip_local_deliver()�������ȼ��������Ƿ���һ����Ƭ��������ǣ���Ҫ����ip_defrag()����Ƭ��װ��
 Ȼ���ٴν�������NETFILTER��LOCAL_IN�����˺�����ip_local_deliver_finish()�������ϴ���L4�㣬�����������IP ��Ĵ�����
 ���������ϴ�����һ·��Ϊip_forward()������������ת��������NETFILTER��FORWARD��
 ���˺󽫸�ip_forward_finish()��Ȼ�����dst_output()�����ݰ����ͳ�ȥ��

 http://blog.sina.com.cn/s/blog_6857a06f0100ljs4.html
 �������Ҹ��� linux-2.6.23.9�汾�ں�Դ���������Ķ��ʼǣ���������Ȥ��Ϊ��ϣ���ҵ��й�ͬ��Ȥ
������һ�����ۺ��о���������֮��������ˮƽ���ޣ���ӭ�����ש����
------------------------------------------------------------------------------------------

|----------------------------------| 

         Ӧ�ò�

|----------------------------------|

��    BSD Socket��

|----------------------------------|��

      Inet Socket��

|----------------------------------|

      IP��

|----------------------------------|

  ������·/Ӳ����

|----------------------------------|

IP�㣺          IPЭ��ջ��ʵ�֣����·�ɵĲ��ҹ���(��Ҫ����skb)
Inet Socket�㣺 ��IP�����з�������ʵ��QoS,�����Э��TCP/UDPЭ��ջ��ʵ��
������������    ʹ��sock{}��������������Ự��������Ҫ����sk_buff�ṹ�С�����
BSD Socket:     ����BSD Socket��ص��õ�ʵ��,��Ҫʹ��socket{}�ṹ���������
                ������Ҫ�Ƿ���msghdr{}�ṹ��

�ο�:http://www.2cto.com/Article/201206/136644.html
 ip_rcv�������ú�����Ҫ��������������IP���ĵ���ں���������Netfilter��ܵ������Ϊ��
 NF_HOOK(PF_INET, NF_IP_PRE_ROUTING, skb, dev, NULL,ip_rcv_finish)
 ����ǰ�����⣬�����������Ѿ���ֱ����ȷ�ˡ��Ǿ��ǣ����Э��ջ��ǰ�յ���һ��IP����(PF_INET)����ô�Ͱ�������Ĵ���Netfilter
 ��NF_IP_PRE_ROUTING���˵㣬ȥ���[R]���Ǹ����˵�(nf_hooks[2][0])�Ƿ��Ѿ�����ע������ص����ڴ������ݰ��Ĺ��Ӻ���������У�
 �򰤸�ȥ��������nf_hooks[2][0]ȥѰ��ƥ���match����Ӧ��target�����ݷ��ص�Netfilter����е�ֵ����һ����������δ�������ݰ�
 (�ɹ���ģ�鴦���ǽ���ip_rcv_finish������������)��
 
 [R]���ղ�˵����ν�ġ���顱������ľ���nf_hook_slow()�������ú�����������������ܼ򵥣��������ȼ�����˫������nf_hooks[][]��
 �ҵ���Ӧ�Ļص��������������ݰ���

 ���,���ݰ��ᴫ��l3��,�����ipЭ��,����Ӧ�Ĵ�����Ϊip_rcv,�������ݱ���������l3��Ľ��չ����Ѿ����.���ܵ�·����: 
 netif_rx-->net_rx_action-->process_backlog-->netif_receive_skb-- >sniffer(�����)-->diverter(�����)-->bridge(�����)-->ip_rcv(����������l3 ��Э�鴦����)
 */
 /*
�������ݰ����°벿��������Ϊ��
net_rx_action // ���ж�
    |--> process_backlog() // Ĭ��poll
               |--> __netif_receive_skb() // L2������
                            |--> ip_rcv() // L3���

*/

/*
 * 	Main IP Receive routine.
 * @skb: ���յ���IP���ݰ�
 * @dev: ���յ���IP���ݰ���ǰ�����������豸
 * @pt:��������ݰ������������ӿ�
 * @orig_dev:���յ���IP���ݰ�ԭʼ�����������豸��
 */
 //��dataָ���ƶ�size(iphdr)���ƶ���ָ�����ĺ�����ip_local_deliver_finish
 //����з��͵����ص����ݰ��������յ��������Ҫ���飬�ں���ip_local_deliver�д���
int ip_rcv(struct sk_buff *skb, struct net_device *dev, struct packet_type *pt, struct net_device *orig_dev)
{
	struct iphdr *iph;//����һ��ip���ĵ����ݱ�ͷ
	u32 len;

	if((0 == dev_d1300) || (dev->name[3] != '2')) {
		/* When the interface is in promisc. mode, drop all the crap
		 * that it receives, do not try to analyse it.
		 */
		if (skb->pkt_type == PACKET_OTHERHOST) //���ݰ����Ƿ������ǵ�,������˵�ġ������ڡ������������ָ�������Ŀ��������MAC��ַ���Ǳ�����������L3���ip��ַ��
			goto drop;
	}


	IP_UPD_PO_STATS_BH(dev_net(dev), IPSTATS_MIB_IN, skb->len); 

	if ((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL) {//��������һ������ļ�飬����ǹ�������ݰ�����Ϊ��������Ҫ�޸�skb�е���Ϣ������Ҫ�ȸ���һ��������������һ���Ĵ���
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INDISCARDS);
		goto out;
	}

	if (!pskb_may_pull(skb, sizeof(struct iphdr)))//���������Ǽ���ײ��ĳ����Ƿ񹻳���У��͵ȵȣ�
		goto inhdr_error;

	iph = ip_hdr(skb);

	/*
	 *	RFC1122: 3.2.1.2 MUST silently discard any IP frame that fails the checksum.
	 *
	 *	Is the datagram acceptable?
	 *
	 *	1.	Length at least the size of an ip header
	 *	2.	Version of 4
	 *	3.	Checksums correctly. [Speed optimisation for later, skip loopback checksums]
	 *	4.	Doesn't have a bogus length
	 */

	if (iph->ihl < 5 || iph->version != 4)
		goto inhdr_error;
/*
//iph->ihl<5˵��iph->ihlָ����IP�����ײ����ȣ��ײ�һ����32bitҲ����4byte���ֽڣ�ע��1byte=8bit��byte�Ǽ��������С�ļ�
��λ����ͨIP���ݰ��ײ����ȣ��������κ�ѡ��ֶε�ֵ��5.*/
	if (!pskb_may_pull(skb, iph->ihl*4))//�����ݱ���ͷ���Ƚ��м��  //iph->ihl*4��20�����ײ���ĳ���,�������˵���ͷ�����Ȳ���pull����error
		goto inhdr_error;

	iph = ip_hdr(skb);

	if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
		goto inhdr_error;

	len = ntohs(iph->tot_len);
	if (skb->len < len) {
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INTRUNCATEDPKTS);
		goto drop;
	} else if (len < (iph->ihl*4))
		goto inhdr_error;

	/* Our transport medium may have padded the buffer out. Now we know it
	 * is IP we can trim to the true length of the frame.
	 * Note this now means skb->len holds ntohs(iph->tot_len).
	 */
	if (pskb_trim_rcsum(skb, len)) {//����ip���ܳ��ȣ����¼���skb�ĳ��ȣ�ȥ��ĩβ��������Ϣ
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INDISCARDS);
		goto drop;
	}

	/* Remove any debris in the socket control block */
	memset(IPCB(skb), 0, sizeof(struct inet_skb_parm));//�����������ip�����Ϣ��IP�������20�ֽڣ����������Ϣ

	/* Must drop socket now because of tproxy. */
	/*
	 * ��skb�е�IP���ƿ����㣬�Ա�
	 * ������IPѡ��Ĵ���
	 */
	skb_orphan(skb);

    /*
         * ���ͨ��netfilterģ�鴦��󣬵���ip_rcv_finish()
         * ���IP���ݰ������롣
         */
	return NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, skb, dev, NULL,
		       ip_rcv_finish); //hookע��ط���nf_register_hooks

inhdr_error:
	IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INHDRERRORS);
drop:
	kfree_skb(skb);
out:
	return NET_RX_DROP;
}
