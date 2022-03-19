/*
 * NET		Generic infrastructure for Network protocols.
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

#include <linux/module.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/vmalloc.h>

#include <net/request_sock.h>

/*
 * Maximum number of SYN_RECV sockets in queue per LISTEN socket.
 * One SYN_RECV socket costs about 80bytes on a 32bit machine.
 * It would be better to replace it with a global counter for all sockets
 * but then some measure against one socket starving all other sockets
 * would be needed.
 *
 * It was 128 by default. Experiments with real servers show, that
 * it is absolutely not enough even at 100conn/sec. 256 cures most
 * of problems. This value is adjusted to 128 for very small machines
 * (<=32Mb of memory) and to 1024 on normal or better ones (>=256Mb).
 * Note : Dont forget somaxconn that may limit backlog too.
 */
int sysctl_max_syn_backlog = 256;
/*
 * �����������������ɢ�б�Ȼ�������ӵ����ڴ�����ƿ������
 * �������С�
 �����¼�������Ӱ��: ��http://blog.chinaunix.net/uid-20662820-id-3776090.html
 ��1��net.core.somaxconn
��2��net.ipv4.tcp_max_syn_backlog
��3��listenϵͳ���õ�backlog����
 */
//���ٵĿռ��С��sizeof(struct listen_sock) + nr_table_entries * sizeof(struct request_sock *),����syn_tableָ������nr_table_entries * sizeof(struct request_sock *)���֣���ֻ��hash��ͷ�Ŀռ�
int reqsk_queue_alloc(struct request_sock_queue *queue,
		      unsigned int nr_table_entries)
{
	size_t lopt_size = sizeof(struct listen_sock);
	struct listen_sock *lopt;

    /*
	 * ȡ�û��趨�����Ӷ��г������ֵ����nr_table_entries��ϵͳ���
	 * ��ͬʱ����δ�����������SYN������sysctl_max_syn_backlog���ߵ�
	 * ��Сֵ�����Ƕ������������Ӷ��еĳ��ȣ�ֻ��ǰ�����ĳ�������
	 * �飬�����߿��Ƶ���ȫ�ֵ�

	 ������Կ���listen_sock->max_qlen_log Ϊnr_table_entries��sysctl_max_syn_backlog����Сֵ��1
   ������ȥ����2�Ĵη����log��
   ���磺 nr_table_entries = 128 sysctl_max_syn_backlog=20480��
               min(nr_table_entries, sysctl_max_syn_backlog)= 128
               roundup_pow_of_two(128+1)=256
               max_qlen_log=8
    */
	 
	nr_table_entries = min_t(u32, nr_table_entries, sysctl_max_syn_backlog);
	nr_table_entries = max_t(u32, nr_table_entries, 8);
	/*
	 * ����roundup_pow_of_two��ȷ��nr_table_entries��ֵΪ2��n�η�
	 */
	nr_table_entries = roundup_pow_of_two(nr_table_entries + 1);
	/*
	 * ������������SYN�������ӵ�listen_sock�ṹ�Ĵ�С
	 */
	lopt_size += nr_table_entries * sizeof(struct request_sock *);//ע��(struct request_sock *)��ָ�룬�ռ��СΪ4
	if (lopt_size > PAGE_SIZE)
		lopt = __vmalloc(lopt_size,
			GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO,
			PAGE_KERNEL);
	else
		lopt = kzalloc(lopt_size, GFP_KERNEL); 
	if (lopt == NULL)
		return -ENOMEM;
    //���ٵĿռ��С��sizeof(struct listen_sock) + nr_table_entries * sizeof(struct request_sock *),����syn_tableָ������nr_table_entries * sizeof(struct request_sock *)���֣���ֻ��hash��ͷ�Ŀռ�
	for (lopt->max_qlen_log = 3;
	     (1 << lopt->max_qlen_log) < nr_table_entries;
	     lopt->max_qlen_log++);

	get_random_bytes(&lopt->hash_rnd, sizeof(lopt->hash_rnd));
	rwlock_init(&queue->syn_wait_lock);
	queue->rskq_accept_head = NULL;
	lopt->nr_table_entries = nr_table_entries;

	write_lock_bh(&queue->syn_wait_lock);
	queue->listen_opt = lopt; //queue->listen_optָ��request_sock
	write_unlock_bh(&queue->syn_wait_lock);

	return 0;
}

void __reqsk_queue_destroy(struct request_sock_queue *queue)
{
	struct listen_sock *lopt;
	size_t lopt_size;

	/*
	 * this is an error recovery path only
	 * no locking needed and the lopt is not NULL
	 */

	lopt = queue->listen_opt;
	lopt_size = sizeof(struct listen_sock) +
		lopt->nr_table_entries * sizeof(struct request_sock *);

	if (lopt_size > PAGE_SIZE)
		vfree(lopt);
	else
		kfree(lopt);
}

static inline struct listen_sock *reqsk_queue_yank_listen_sk(
		struct request_sock_queue *queue)
{
	struct listen_sock *lopt;

	write_lock_bh(&queue->syn_wait_lock);
	lopt = queue->listen_opt;
	queue->listen_opt = NULL;
	write_unlock_bh(&queue->syn_wait_lock);

	return lopt;
}

void reqsk_queue_destroy(struct request_sock_queue *queue)
{
	/* make all the listen_opt local to us */
	struct listen_sock *lopt = reqsk_queue_yank_listen_sk(queue);
	size_t lopt_size = sizeof(struct listen_sock) +
		lopt->nr_table_entries * sizeof(struct request_sock *);

	if (lopt->qlen != 0) {
		unsigned int i;

		for (i = 0; i < lopt->nr_table_entries; i++) {
			struct request_sock *req;

			while ((req = lopt->syn_table[i]) != NULL) {
				lopt->syn_table[i] = req->dl_next;
				lopt->qlen--;
				reqsk_free(req);
			}
		}
	}

	WARN_ON(lopt->qlen != 0);
	if (lopt_size > PAGE_SIZE)
		vfree(lopt);
	else
		kfree(lopt);
}

