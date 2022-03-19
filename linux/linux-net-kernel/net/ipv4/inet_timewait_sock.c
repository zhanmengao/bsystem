/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Generic TIME_WAIT sockets functions
 *
 *		From code orinally in TCP
 */

#include <linux/kernel.h>
#include <linux/kmemcheck.h>
#include <linux/slab.h>
#include <net/inet_hashtables.h>
#include <net/inet_timewait_sock.h>
#include <net/ip.h>


/**
 *	inet_twsk_unhash - unhash a timewait socket from established hash
 *	@tw: timewait socket
 *
 *	unhash a timewait socket from established hash, if hashed.
 *	ehash lock must be held by caller.
 *	Returns 1 if caller should call inet_twsk_put() after lock release.
 */
int inet_twsk_unhash(struct inet_timewait_sock *tw)
{
	if (hlist_nulls_unhashed(&tw->tw_node))
		return 0;

	hlist_nulls_del_rcu(&tw->tw_node);
	sk_nulls_node_init(&tw->tw_node);
	/*
	 * We cannot call inet_twsk_put() ourself under lock,
	 * caller must call it for us.
	 */
	return 1;
}

/**
 *	inet_twsk_bind_unhash - unhash a timewait socket from bind hash
 *	@tw: timewait socket
 *	@hashinfo: hashinfo pointer
 *
 *	unhash a timewait socket from bind hash, if hashed.
 *	bind hash lock must be held by caller.
 *	Returns 1 if caller should call inet_twsk_put() after lock release.
 */
int inet_twsk_bind_unhash(struct inet_timewait_sock *tw,
			  struct inet_hashinfo *hashinfo)
{
	struct inet_bind_bucket *tb = tw->tw_tb;

	if (!tb)
		return 0;

	__hlist_del(&tw->tw_bind_node);
	tw->tw_tb = NULL;
	inet_bind_bucket_destroy(hashinfo->bind_bucket_cachep, tb);
	/*
	 * We cannot call inet_twsk_put() ourself under lock,
	 * caller must call it for us.
	 */
	return 1;
}

/* Must be called with locally disabled BHs. */
static void __inet_twsk_kill(struct inet_timewait_sock *tw,
			     struct inet_hashinfo *hashinfo)
{
	struct inet_bind_hashbucket *bhead;
	int refcnt;
	/* Unlink from established hashes. */
	spinlock_t *lock = inet_ehash_lockp(hashinfo, tw->tw_hash);

	spin_lock(lock);
	refcnt = inet_twsk_unhash(tw);
	spin_unlock(lock);

	/* Disassociate with bind bucket. */
	bhead = &hashinfo->bhash[inet_bhashfn(twsk_net(tw), tw->tw_num,
			hashinfo->bhash_size)];

	spin_lock(&bhead->lock);
	refcnt += inet_twsk_bind_unhash(tw, hashinfo);
	spin_unlock(&bhead->lock);

#ifdef SOCK_REFCNT_DEBUG
	if (atomic_read(&tw->tw_refcnt) != 1) {
		printk(KERN_DEBUG "%s timewait_sock %p refcnt=%d\n",
		       tw->tw_prot->name, tw, atomic_read(&tw->tw_refcnt));
	}
#endif
	while (refcnt) {
		inet_twsk_put(tw);
		refcnt--;
	}
}

static noinline void inet_twsk_free(struct inet_timewait_sock *tw)
{
	struct module *owner = tw->tw_prot->owner;
	twsk_destructor((struct sock *)tw);
#ifdef SOCK_REFCNT_DEBUG
	pr_debug("%s timewait_sock %p released\n", tw->tw_prot->name, tw);
#endif
	release_net(twsk_net(tw));
	kmem_cache_free(tw->tw_prot->twsk_prot->twsk_slab, tw);
	module_put(owner);
}

void inet_twsk_put(struct inet_timewait_sock *tw)
{
	if (atomic_dec_and_test(&tw->tw_refcnt))
		inet_twsk_free(tw);
}
EXPORT_SYMBOL_GPL(inet_twsk_put);

/*
 * Enter the time wait state. This is called with locally disabled BH.
 * Essentially we whip up a timewait bucket, copy the relevant info into it
 * from the SK, and mess with hash chains and list linkage.
 *//*
 * ��timewait���ƿ���ӵ�tcp_hashinfo��ebashɢ�б��У�
 * ���������TCP���ƿ��ehashɢ�б���ɾ��������
 * FIN_WAIT2��TIME_WAIT״̬��Ҳ���Խ�������Ĵ���
 * ͬʱ����timewait���ƿ���ӵ�bhashɢ�б��У���
 * ����ɾ����ɢ�б��б������TCP���ƿ飬��Ϊ
 * ֻҪinet->num��Ϊ0������󶨹�ϵ�ʹ��ڣ�
 * ��ʹ���׽����Ѿ��ر�
 */
void __inet_twsk_hashdance(struct inet_timewait_sock *tw, struct sock *sk,
			   struct inet_hashinfo *hashinfo)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);
	struct inet_ehash_bucket *ehead = inet_ehash_bucket(hashinfo, sk->sk_hash);
	spinlock_t *lock = inet_ehash_lockp(hashinfo, sk->sk_hash);
	struct inet_bind_hashbucket *bhead;
	/* Step 1: Put TW into bind hash. Original socket stays there too.
	   Note, that any socket with inet->num != 0 MUST be bound in
	   binding cache, even if it is closed.
	 */
	bhead = &hashinfo->bhash[inet_bhashfn(twsk_net(tw), inet->inet_num,
			hashinfo->bhash_size)];
	spin_lock(&bhead->lock);
	tw->tw_tb = icsk->icsk_bind_hash;
	WARN_ON(!icsk->icsk_bind_hash);

	//��inet_timewait_sock��ӵ�
	inet_twsk_add_bind_node(tw, &tw->tw_tb->owners);//��inet_bind_bucketͰָ��tw->tw_bind_node������ú����������ͷ�sk��ʱ�򣬻��ͷŵ�bindͰ��Ϣ
	spin_unlock(&bhead->lock);

	spin_lock(lock);

	/*
	 * Step 2: Hash TW into TIMEWAIT chain.
	 * Should be done before removing sk from established chain
	 * because readers are lockless and search established first.
	 */
	inet_twsk_add_node_rcu(tw, &ehead->twchain);//���´�����inet_timewait_sock���뵽inet_hash�е�ehash��

	/* Step 3: Remove SK from established hash. */
	if (__sk_nulls_del_node_init_rcu(sk))//��sk��inet_hash�е�ehash����ɾ��
		sock_prot_inuse_add(sock_net(sk), sk->sk_prot, -1);

	/*
	 * Notes :
	 * - We initially set tw_refcnt to 0 in inet_twsk_alloc()
	 * - We add one reference for the bhash link
	 * - We add one reference for the ehash link
	 * - We want this refcnt update done before allowing other
	 *   threads to find this tw in ehash chain.
	 */
	atomic_add(1 + 1 + 1, &tw->tw_refcnt);

	spin_unlock(lock);
}
EXPORT_SYMBOL_GPL(__inet_twsk_hashdance);

struct inet_timewait_sock *inet_twsk_alloc(const struct sock *sk, const int state)
{
	struct inet_timewait_sock *tw =
		kmem_cache_alloc(sk->sk_prot_creator->twsk_prot->twsk_slab,
				 GFP_ATOMIC);
	if (tw != NULL) {
		const struct inet_sock *inet = inet_sk(sk);

		kmemcheck_annotate_bitfield(tw, flags);

		/* Give us an identity. */
		tw->tw_daddr	    = inet->inet_daddr;
		tw->tw_rcv_saddr    = inet->inet_rcv_saddr;
		tw->tw_bound_dev_if = sk->sk_bound_dev_if;
		tw->tw_num	    = inet->inet_num;
		tw->tw_state	    = TCP_TIME_WAIT;
		tw->tw_substate	    = state;
		tw->tw_sport	    = inet->inet_sport;
		tw->tw_dport	    = inet->inet_dport;
		tw->tw_family	    = sk->sk_family;
		tw->tw_reuse	    = sk->sk_reuse;
		tw->tw_hash	    = sk->sk_hash;
		tw->tw_ipv6only	    = 0;
		tw->tw_transparent  = inet->transparent;
		tw->tw_prot	    = sk->sk_prot_creator;
		twsk_net_set(tw, hold_net(sock_net(sk)));
		/*
		 * Because we use RCU lookups, we should not set tw_refcnt
		 * to a non null value before everything is setup for this
		 * timewait socket.
		 */
		atomic_set(&tw->tw_refcnt, 0);
		inet_twsk_dead_node_init(tw);
		__module_get(tw->tw_prot->owner);
	}

	return tw;
}
EXPORT_SYMBOL_GPL(inet_twsk_alloc);

/* Returns non-zero if quota exceeded.  */
/*
 * ɾ��cellsɢ�б��е�ǰ�ؼ��������ϵ�timewait���ƿ顣
 * �����inet_twdr_do_twkill_work()�����timewait���ƿ�������
 * 100������˵������һ������timewait���ƿ���Ҫ����
 * ���ڶ�ʱ�������д�����ʱ�䲻���أ���Ӱ��
 * ϵͳ���ܣ����ʣ�µ�timewait���ƿ����twkill_work
 * ���������д�������twkill_work��������ǰ���ȱ�ʶ
 * ��ɾ��slot��λͼ��������twkill_work�������д����У�
 * ����thread_slotsλͼ������cellsɢ�б�����Ӧ������
 * inet_twdr_do_twkill_work()����ɾ��ָ��cellsɢ�б���slot���
 * ������slot��������ϵ�timewait���ƿ飬Ȼ�����ͷţ�
 * ������ϵͳ��timewait���ƿ�������ɾ�������У�
 * �������ɾ���ĸ����ﵽ100���򷵻ط��㣬��ʾ
 * ���������жϱ��δ������µ��ȡ�
 */
static int inet_twdr_do_twkill_work(struct inet_timewait_death_row *twdr,
				    const int slot)
{
	struct inet_timewait_sock *tw;
	struct hlist_node *node;
	unsigned int killed;
	int ret;

	/* NOTE: compare this to previous version where lock
	 * was released after detaching chain. It was racy,
	 * because tw buckets are scheduled in not serialized context
	 * in 2.3 (with netfilter), and with softnet it is common, because
	 * soft irqs are not sequenced.
	 */
	killed = 0;
	ret = 0;
rescan:
	inet_twsk_for_each_inmate(tw, node, &twdr->cells[slot]) {
		__inet_twsk_del_dead_node(tw);
		spin_unlock(&twdr->death_lock);
		__inet_twsk_kill(tw, twdr->hashinfo);
#ifdef CONFIG_NET_NS
		NET_INC_STATS_BH(twsk_net(tw), LINUX_MIB_TIMEWAITED);
#endif
		inet_twsk_put(tw);
		killed++;
		spin_lock(&twdr->death_lock);
		if (killed > INET_TWDR_TWKILL_QUOTA) {
			ret = 1;
			break;
		}

		/* While we dropped twdr->death_lock, another cpu may have
		 * killed off the next TW bucket in the list, therefore
		 * do a fresh re-read of the hlist head node with the
		 * lock reacquired.  We still use the hlist traversal
		 * macro in order to get the prefetches.
		 */
		goto rescan;
	}

	twdr->tw_count -= killed;
#ifndef CONFIG_NET_NS
	NET_ADD_STATS_BH(&init_net, LINUX_MIB_TIMEWAITED, killed);
#endif
	return ret;
}

/*
 * tw_timer��ʱ�������̣��ö�ʱ����ʱ�󣬻�
 * ����twcal_rowɢ�б��е�ǰ�ؼ���slot�����ϵ�
 * timewait���ƿ顣
 */
void inet_twdr_hangman(unsigned long data)
{
	struct inet_timewait_death_row *twdr;
	int unsigned need_timer;

	twdr = (struct inet_timewait_death_row *)data;
	spin_lock(&twdr->death_lock);

	/*
	 * �����ǰtimewait���ƿ���Ϊ�㣬������
	 * ���������ˡ�
	 */
	if (twdr->tw_count == 0)
		goto out;

	/*
	 * ɾ��cellsɢ�б��е�ǰ�ؼ��������ϵ�timewait���ƿ顣
	 * �����inet_twdr_do_twkill_work()�����timewait���ƿ�������
	 * 100������˵������һ������timewait���ƿ���Ҫ����
	 * ���ڶ�ʱ�������д�����ʱ�䲻���أ���Ӱ��
	 * ϵͳ���ܣ����ʣ�µ�timewait���ƿ����twkill_work
	 * ���������д�������twkill_work��������ǰ���ȱ�ʶ
	 * ��ɾ��slot��λͼ��������twkill_work�������д����У�
	 * ����thread_slotsλͼ������cellsɢ�б�����Ӧ������
	 * inet_twdr_do_twkill_work()����ɾ��ָ��cellsɢ�б���slot���
	 * ������slot��������ϵ�timewait���ƿ飬Ȼ�����ͷţ�
	 * ������ϵͳ��timewait���ƿ�������ɾ�������У�
	 * �������ɾ���ĸ����ﵽ100���򷵻ط��㣬��ʾ
	 * ���������жϱ��δ������µ��ȡ�
	 */
	need_timer = 0;
	if (inet_twdr_do_twkill_work(twdr, twdr->slot)) {//��һ��tw_timer��ʱ��ʱ��twdr->slot=0,�Ͷ���tw_timer��ʱ��ʱ�򣬸�ֵ��1������7���ֻص�1��Ҳ�ͱ�֤��cells�е�����timewait��������
		twdr->thread_slots |= (1 << twdr->slot);
		schedule_work(&twdr->twkill_work);
		need_timer = 1;
	} else {
		/* We purged the entire slot, anything left?  */
		/*
		 * �������timewait���ƿ飬�������ö�ʱ����
		 */
		if (twdr->tw_count)
			need_timer = 1;
		/*
		 * ���γ�ʱ������ɺ��������´γ�ʱ�����
		 * cellsɢ�б���ڡ�
		 */
		twdr->slot = ((twdr->slot + 1) & (INET_TWDR_TWKILL_SLOTS - 1));
	}
	/*
	 * �������timewait���ƿ��账�����ٴ����ö�ʱ����
	 */
	if (need_timer)
		mod_timer(&twdr->tw_timer, jiffies + twdr->period);
out:
	spin_unlock(&twdr->death_lock);
}

void inet_twdr_hangman1(unsigned long data)
{
	struct inet_timewait_death_row *twdr;
	int unsigned need_timer;

	twdr = (struct inet_timewait_death_row *)data;
	spin_lock(&twdr->death_lock);

	if (twdr->tw_count == 0)
		goto out;

	need_timer = 0;
	if (inet_twdr_do_twkill_work(twdr, twdr->slot)) {
		twdr->thread_slots |= (1 << twdr->slot);
		schedule_work(&twdr->twkill_work);
		need_timer = 1;
	} else {
		/* We purged the entire slot, anything left?  */
		if (twdr->tw_count)
			need_timer = 1;
		twdr->slot = ((twdr->slot + 1) & (INET_TWDR_TWKILL_SLOTS - 1));
	}
	if (need_timer)
		mod_timer(&twdr->tw_timer, jiffies + twdr->period);
out:
	spin_unlock(&twdr->death_lock);
}
EXPORT_SYMBOL_GPL(inet_twdr_hangman);

/*
 * twkill_work�������е����̣���tw_timer��ʱ�������д���
 * ��timewait���ƿ�ﵽ100��ʱ�������twkill_work�������У�
 * ����ʣ�µ�timewait���ƿ顣
 * ����ʱ����ݴ�ɾ��slot��λͼ��ɾ����Ӧslot�����ϵ�
 * timewait���ƿ飬��twkill_work��������������ÿ����100��
 * timewait���ƿ���ʱ˯�ߣ�Ȼ���ٴδ���ֱ��ȫ������
 * ��ɡ�
 */
void inet_twdr_twkill_work(struct work_struct *work)
{
	struct inet_timewait_death_row *twdr =
		container_of(work, struct inet_timewait_death_row, twkill_work);
	int i;

	BUILD_BUG_ON((INET_TWDR_TWKILL_SLOTS - 1) >
			(sizeof(twdr->thread_slots) * 8));

	while (twdr->thread_slots) {
		spin_lock_bh(&twdr->death_lock);
		for (i = 0; i < INET_TWDR_TWKILL_SLOTS; i++) {
			if (!(twdr->thread_slots & (1 << i)))
				continue;

			while (inet_twdr_do_twkill_work(twdr, i) != 0) {
				if (need_resched()) {
					spin_unlock_bh(&twdr->death_lock);
					schedule();
					spin_lock_bh(&twdr->death_lock);
				}
			}

			twdr->thread_slots &= ~(1 << i);
		}
		spin_unlock_bh(&twdr->death_lock);
	}
}
EXPORT_SYMBOL_GPL(inet_twdr_twkill_work);

/* These are always called from BH context.  See callers in
 * tcp_input.c to verify this.
 */

/* This is for handling early-kills of TIME_WAIT sockets. */
void inet_twsk_deschedule(struct inet_timewait_sock *tw,
			  struct inet_timewait_death_row *twdr)
{
	spin_lock(&twdr->death_lock);
	if (inet_twsk_del_dead_node(tw)) {
		inet_twsk_put(tw);
		if (--twdr->tw_count == 0)
			del_timer(&twdr->tw_timer);
	}
	spin_unlock(&twdr->death_lock);
	__inet_twsk_kill(tw, twdr->hashinfo);
}
EXPORT_SYMBOL(inet_twsk_deschedule);

/*
 * ��������FIN_WAIT_2��TIME_WAIT��ʱ������Ȼ
 * ������������ʱ���õ���ͬһ���ӿڣ�����
 * ����timewait���ƿ��tw_substate����ȷ������
 * ��ǰ���������ĸ���ʱ��
 * @tw: �Ѿ����TCP������ƿ��timewait���ƿ�
 * @twdr: ������ص����ݵ�������ͨ������ȫ��
 *             ����tcp_death_row��
 * @timeo: �趨��ʱ���ĳ�ʱʱ��
 * @timewait_len: ��ʱʱ�����ޣ�ΪTCP_TIMEWAIT_LEN��
 */
void inet_twsk_schedule(struct inet_timewait_sock *tw,
		       struct inet_timewait_death_row *twdr,
		       const int timeo, const int timewait_len)
{
	struct hlist_head *list;
	int slot;

	/*
	 * RTO�ǳ�ʱ�ش�ʱ��(Retransmission timeout)����˼��
	 * ��ʱ����RTOӦ�Դ���RTT(ƽ������ʱ�䣬
	 * Round-Trip Time)
        * ����RTO��b*RTT
        * ����b�Ǹ�����1��ϵ����
        * ��ȡb�ܽӽ���1�����Ͷ˿ɼ�ʱ���ش���ʧ
        * �ı��ĶΣ����Ч�ʵõ���ߡ�
        * �������Ķβ�δ��ʧ��������������һ��ʱ��,
        * ��ô������ش��������������ĸ�����
        * ���TCPԭ�ȵı�׼�Ƽ���bֵȡΪ2��
	 */
	 /*
	 * ���ش�����£��ش���ʱʱ�����һ�ֳ�Ϊ
	 * ��ָ���˱ܡ��ķ�ʽ���㡣���磺���ش���
	 * ʱʱ��Ϊ1S������·����������ش�������
	 * �����ش���ʱʱ��Ϊ2S�Ķ�ʱ�����ش����ݣ�
	 * ��һ����4S��һֱ���ӵ�64SΪֹ���μ�tcp_retransmit_timer��������
	 * ���������RTO*3.5=RTO*0.5+RTO*1+RTO*2,����RTO*0.5��
	 * ��һ�η���ACK��ʱ�䵽�Զ˵ĳ�ʱʱ�䣨ϵ
	 * �����ǳ���RTO��ֵ����RTO*1�ǶԶ˵�һ���ش�FIN
	 * ����ACK������Զ˵ĳ�ʱʱ�䣬RTO*2�ǶԶ˵�
	 * �����ش�FIN����ACK������Զ˵ĳ�ʱʱ�䡣ע��
	 * ���ش���ʱʱ���ָ���˱ܲ��������ǳ���2������
	 * �ش�֮��ִ�еģ����Ե�һ���ش��ĳ�ʱʱ���
	 * ��һ�η��͵ĳ�ʱʱ����ͬ��
	 */
	/* timeout := RTO * 3.5
	 *
	 * 3.5 = 1+2+0.5 to wait for two retransmits.
	 *
	 * RATIONALE: if FIN arrived and we entered TIME-WAIT state,
	 * our ACK acking that FIN can be lost. If N subsequent retransmitted
	 * FINs (or previous seqments) are lost (probability of such event
	 * is p^(N+1), where p is probability to lose single packet and
	 * time to detect the loss is about RTO*(2^N - 1) with exponential
	 * backoff). Normal timewait length is calculated so, that we
	 * waited at least for one retransmitted FIN (maximal RTO is 120sec).
	 * [ BTW Linux. following BSD, violates this requirement waiting
	 *   only for 60sec, we should wait at least for 240 secs.
	 *   Well, 240 consumes too much of resources 8)
	 * ]
	 * This interval is not reduced to catch old duplicate and
	 * responses to our wandering segments living for two MSLs.
	 * However, if we use PAWS to detect
	 * old duplicates, we can reduce the interval to bounds required
	 * by RTO, rather than MSL. So, if peer understands PAWS, we
	 * kill tw bucket after 3.5*RTO (it is important that this number
	 * is greater than TS tick!) and detect old duplicates with help
	 * of PAWS.
	 */
	/*
	 * TIME_WAIT��ʱʱ�����INET_TWDR_RECYCLE_TICK��
	 * ����ȡ���������жϽ���timewait���ƿ����
	 * ��cells����twcal_rowɢ�б��С�
	 * ����õ�ֵ���ڻ����INET_TWDR_RECYCLE_SLOTS��
	 * ������ӵ�cellsɢ�б��У�������ӵ�
	 * twcal_rowɢ�б���
	 */
	slot = (timeo + (1 << INET_TWDR_RECYCLE_TICK) - 1) >> INET_TWDR_RECYCLE_TICK;

	spin_lock(&twdr->death_lock);

	/* Unlink it, if it was scheduled */
	/*
	 * �����timewait���ƿ��Ѿ������ȣ���ɢ�б���ժ����
	 * ����Ҫ�ݼ���ǰϵͳ�д���TIME_wAIT״̬���׽�����
	 */
	if (inet_twsk_del_dead_node(tw)) 
		twdr->tw_count--; //�ڸú���inet_twsk_schedule�����inet_twsk_put���Ƿ�timewait�ռ�
	else
		atomic_inc(&tw->tw_refcnt);

	if (slot >= INET_TWDR_RECYCLE_SLOTS) { //�����slot�Ǹ��ݶ�ʱ����ʱʱ��timeo���ģ����Ծ��൱�ڸ��ݳ�ʱʱ���timewaitɢ�е�cells���С�
	/*
	 * ׼����ӵ�cellsɢ�б��С�����timewait���ƿ�
	 * ��ʱɾ��ʱ�䣬��������ӵ�cellsɢ�б��
	 * �ĸ�Ͱ��
	 */
		/* Schedule to slow timer */
		if (timeo >= timewait_len) {
			slot = INET_TWDR_TWKILL_SLOTS - 1;
		} else {
			slot = DIV_ROUND_UP(timeo, twdr->period);//����൱���Ǽ���timeo��twdr->period�ļ�����Ҳ���Ǽ���TCP_TIMEWAIT_LEN / INET_TWDR_TWKILL_SLOTS
			if (slot >= INET_TWDR_TWKILL_SLOTS) //�����Ϳ��԰���ʱ��ɢ�е�cell�У���ʾ�ж��ٸ�slot��TCP_TIMEWAIT_LEN / INET_TWDR_TWKILL_SLOTS
				slot = INET_TWDR_TWKILL_SLOTS - 1;
		}
		tw->tw_ttd = jiffies + timeo;
		slot = (twdr->slot + slot) & (INET_TWDR_TWKILL_SLOTS - 1);
		list = &twdr->cells[slot];
	} else {
		/*
		 * ׼����ӵ�twcal_rowɢ�б��С����twcal_row
		 * ɢ�б�Ϊ�գ����������´γ�ʱʱ�����
		 * Ͱ��Ȼ�����ó�ʱʱ���������ʱ����
		 * ���twcal_rowɢ�б�Ϊ�գ��ұ��γ�ʱʱ��
		 * �����ö�ʱ���ĳ�ʱʱ�䣬������������
		 * ��ʱ���ĳ�ʱʱ�䡣
		 * ����ȡ��ӵ�twcal_rowɢ�б���ĸ�Ͱ
		 */
		tw->tw_ttd = jiffies + (slot << INET_TWDR_RECYCLE_TICK);

		if (twdr->twcal_hand < 0) {
			twdr->twcal_hand = 0;
			twdr->twcal_jiffie = jiffies;
			twdr->twcal_timer.expires = twdr->twcal_jiffie +
					      (slot << INET_TWDR_RECYCLE_TICK);
			add_timer(&twdr->twcal_timer);
		} else {
			if (time_after(twdr->twcal_timer.expires,
				       jiffies + (slot << INET_TWDR_RECYCLE_TICK)))
				mod_timer(&twdr->twcal_timer,
					  jiffies + (slot << INET_TWDR_RECYCLE_TICK));
			slot = (twdr->twcal_hand + slot) & (INET_TWDR_RECYCLE_SLOTS - 1);
		}
		list = &twdr->twcal_row[slot];
	}

	/*
	 * ��timewait���ƿ���ӵ���Ӧ��ɢ�б��С�
	 */
	hlist_add_head(&tw->tw_death_node, list);

	/*
	 * ���ϵͳ֮ǰ������timewait���ƿ飬
	 * �����趨tw_timer��ʱ����
	 */ //inet_twdr_hangman 
	if (twdr->tw_count++ == 0)  //������timewait���ƿ���������ɾ��        tw_timer��ʱ���ص�������inet_twdr_hangman
		mod_timer(&twdr->tw_timer, jiffies + twdr->period);//ע�⣬�����tcp_death_row����Ķ�ʱ������Ҫһֱ������,ע���inet_csk_init_xmit_timers�м��ֶ�ʱ��������
	spin_unlock(&twdr->death_lock);
}

void inet_twsk_schedule1(struct inet_timewait_sock *tw,
		       struct inet_timewait_death_row *twdr,
		       const int timeo, const int timewait_len)
{
	struct hlist_head *list;
	int slot;

	/* timeout := RTO * 3.5
	 *
	 * 3.5 = 1+2+0.5 to wait for two retransmits.
	 *
	 * RATIONALE: if FIN arrived and we entered TIME-WAIT state,
	 * our ACK acking that FIN can be lost. If N subsequent retransmitted
	 * FINs (or previous seqments) are lost (probability of such event
	 * is p^(N+1), where p is probability to lose single packet and
	 * time to detect the loss is about RTO*(2^N - 1) with exponential
	 * backoff). Normal timewait length is calculated so, that we
	 * waited at least for one retransmitted FIN (maximal RTO is 120sec).
	 * [ BTW Linux. following BSD, violates this requirement waiting
	 *   only for 60sec, we should wait at least for 240 secs.
	 *   Well, 240 consumes too much of resources 8)
	 * ]
	 * This interval is not reduced to catch old duplicate and
	 * responces to our wandering segments living for two MSLs.
	 * However, if we use PAWS to detect
	 * old duplicates, we can reduce the interval to bounds required
	 * by RTO, rather than MSL. So, if peer understands PAWS, we
	 * kill tw bucket after 3.5*RTO (it is important that this number
	 * is greater than TS tick!) and detect old duplicates with help
	 * of PAWS.
	 */
	slot = (timeo + (1 << INET_TWDR_RECYCLE_TICK) - 1) >> INET_TWDR_RECYCLE_TICK;

	spin_lock(&twdr->death_lock);

	/* Unlink it, if it was scheduled */
	if (inet_twsk_del_dead_node(tw))
		twdr->tw_count--;
	else
		atomic_inc(&tw->tw_refcnt);

	if (slot >= INET_TWDR_RECYCLE_SLOTS) {
		/* Schedule to slow timer */
		if (timeo >= timewait_len) {
			slot = INET_TWDR_TWKILL_SLOTS - 1;
		} else {
			slot = DIV_ROUND_UP(timeo, twdr->period);
			if (slot >= INET_TWDR_TWKILL_SLOTS)
				slot = INET_TWDR_TWKILL_SLOTS - 1;
		}
		tw->tw_ttd = jiffies + timeo;
		slot = (twdr->slot + slot) & (INET_TWDR_TWKILL_SLOTS - 1);
		list = &twdr->cells[slot];
	} else {
		tw->tw_ttd = jiffies + (slot << INET_TWDR_RECYCLE_TICK);

		if (twdr->twcal_hand < 0) {
			twdr->twcal_hand = 0;
			twdr->twcal_jiffie = jiffies;
			twdr->twcal_timer.expires = twdr->twcal_jiffie +
					      (slot << INET_TWDR_RECYCLE_TICK);
			add_timer(&twdr->twcal_timer);
		} else {
			if (time_after(twdr->twcal_timer.expires,
				       jiffies + (slot << INET_TWDR_RECYCLE_TICK)))
				mod_timer(&twdr->twcal_timer,
					  jiffies + (slot << INET_TWDR_RECYCLE_TICK));
			slot = (twdr->twcal_hand + slot) & (INET_TWDR_RECYCLE_SLOTS - 1);
		}
		list = &twdr->twcal_row[slot];
	}

	hlist_add_head(&tw->tw_death_node, list);

	if (twdr->tw_count++ == 0)
		mod_timer(&twdr->tw_timer, jiffies + twdr->period);
	spin_unlock(&twdr->death_lock);
}
EXPORT_SYMBOL_GPL(inet_twsk_schedule);

/*
 * twcal_timer��ʱ�������̣��ö�ʱ����ʱ��
 * �����twcal_rowɢ�б���������ѳ�ʱ
 * ��timewait���ƿ�
 */
void inet_twdr_twcal_tick(unsigned long data)
{
	struct inet_timewait_death_row *twdr;
	int n, slot;
	unsigned long j;
	unsigned long now = jiffies;
	int killed = 0;
	int adv = 0;

	twdr = (struct inet_timewait_death_row *)data;

	spin_lock(&twdr->death_lock);
	/*
	 * twcal_handС��0����ʾtwcal_rowɢ�б��в�����
	 * timewait���ƿ飬����ڱ���֮ǰ����У�顣
	 */
	if (twdr->twcal_hand < 0)
		goto out;

	/*
	 * ��ȡ���α���twcal_rowɢ�б����ڡ�ͬʱ
	 * ȡ�ø�ɢ�б���ڶ����ϵĳ�ʱʱ�䣬
	 * ���ڼ��timewait���ƿ��Ƿ��ѳ�ʱ
	 */
	slot = twdr->twcal_hand;
	j = twdr->twcal_jiffie;

	/*
	 * ����twcal_rowɢ�б�ɾ���ѳ�ʱ��timewait���ƿ�
	 */
	for (n = 0; n < INET_TWDR_RECYCLE_SLOTS; n++) {
		/*
		 * �����ǰ��������е�timewait���ƿ��ѳ�ʱ��
		 * �����twcal_row�Լ�bhash��ehashɢ�б���ɾ����
		 * Ȼ�����ͷţ����ͳ�Ʊ���ɾ���ͷŵ�
		 * timewait���ƿ�����
		 */
		if (time_before_eq(j, now)) {
			struct hlist_node *node, *safe;
			struct inet_timewait_sock *tw;

			inet_twsk_for_each_inmate_safe(tw, node, safe,
						       &twdr->twcal_row[slot]) {
				__inet_twsk_del_dead_node(tw);
				__inet_twsk_kill(tw, twdr->hashinfo);
#ifdef CONFIG_NET_NS
				NET_INC_STATS_BH(twsk_net(tw), LINUX_MIB_TIMEWAITKILLED);
#endif
				inet_twsk_put(tw);
				killed++;
			}
		} else {
			/*
			 * ����������ʱʱ��С�ڵ�ǰ��ʱʱ���ڣ�˵��
			 * ��ʱ��timewait���ƿ���ȫ��������ɣ���������
			 * ��һ�γ�ʱ��twcal_jiffie�����twcal_hand��Ȼ����ʣ��
			 * ��ɢ�б��в����Ƿ���δ��ʱ��timewait���ƿ飬
			 * ��������������ó�ʱʱ��󷵻أ�����˵��
			 * ���е�timewait���ƿ鶼��������轫twcal_hand����Ϊ-1.
			 */
			if (!adv) {
				adv = 1;
				twdr->twcal_jiffie = j;
				twdr->twcal_hand = slot;
			}

			if (!hlist_empty(&twdr->twcal_row[slot])) {
				mod_timer(&twdr->twcal_timer, j);
				goto out;
			}
		}
		/*
		 * �ڱ���timewait���ƿ�ʱ����Ҫ��ڵĹؼ���
		 * ����������ϵĳ�ʱʱ�䡣
		 */
		j += 1 << INET_TWDR_RECYCLE_TICK;
		slot = (slot + 1) & (INET_TWDR_RECYCLE_SLOTS - 1);
	}
	twdr->twcal_hand = -1;

/*
 * �����ǰϵͳtimewait���ƿ���Ϊ�㣬��
 * ֹͣtw_timer��ʱ����
 */
out:
	if ((twdr->tw_count -= killed) == 0)
		del_timer(&twdr->tw_timer);
#ifndef CONFIG_NET_NS
	NET_ADD_STATS_BH(&init_net, LINUX_MIB_TIMEWAITKILLED, killed);
#endif
	spin_unlock(&twdr->death_lock);
}

void inet_twdr_twcal_tick1(unsigned long data)
{
	struct inet_timewait_death_row *twdr;
	int n, slot;
	unsigned long j;
	unsigned long now = jiffies;
	int killed = 0;
	int adv = 0;

	twdr = (struct inet_timewait_death_row *)data;

	spin_lock(&twdr->death_lock);
	if (twdr->twcal_hand < 0)
		goto out;

	slot = twdr->twcal_hand;
	j = twdr->twcal_jiffie;

	for (n = 0; n < INET_TWDR_RECYCLE_SLOTS; n++) {
		if (time_before_eq(j, now)) {
			struct hlist_node *node, *safe;
			struct inet_timewait_sock *tw;

			inet_twsk_for_each_inmate_safe(tw, node, safe,
						       &twdr->twcal_row[slot]) {
				__inet_twsk_del_dead_node(tw);
				__inet_twsk_kill(tw, twdr->hashinfo);
#ifdef CONFIG_NET_NS
				NET_INC_STATS_BH(twsk_net(tw), LINUX_MIB_TIMEWAITKILLED);
#endif
				inet_twsk_put(tw);
				killed++;
			}
		} else {
			if (!adv) {
				adv = 1;
				twdr->twcal_jiffie = j;
				twdr->twcal_hand = slot;
			}

			if (!hlist_empty(&twdr->twcal_row[slot])) {
				mod_timer(&twdr->twcal_timer, j);
				goto out;
			}
		}
		j += 1 << INET_TWDR_RECYCLE_TICK;
		slot = (slot + 1) & (INET_TWDR_RECYCLE_SLOTS - 1);
	}
	twdr->twcal_hand = -1;

out:
	if ((twdr->tw_count -= killed) == 0)
		del_timer(&twdr->tw_timer);
#ifndef CONFIG_NET_NS
	NET_ADD_STATS_BH(&init_net, LINUX_MIB_TIMEWAITKILLED, killed);
#endif
	spin_unlock(&twdr->death_lock);
}
EXPORT_SYMBOL_GPL(inet_twdr_twcal_tick);

void inet_twsk_purge(struct inet_hashinfo *hashinfo,
		     struct inet_timewait_death_row *twdr, int family)
{
	struct inet_timewait_sock *tw;
	struct sock *sk;
	struct hlist_nulls_node *node;
	unsigned int slot;

	for (slot = 0; slot <= hashinfo->ehash_mask; slot++) {
		struct inet_ehash_bucket *head = &hashinfo->ehash[slot];
restart_rcu:
		rcu_read_lock();
restart:
		sk_nulls_for_each_rcu(sk, node, &head->twchain) {
			tw = inet_twsk(sk);
			if ((tw->tw_family != family) ||
				atomic_read(&twsk_net(tw)->count))
				continue;

			if (unlikely(!atomic_inc_not_zero(&tw->tw_refcnt)))
				continue;

			if (unlikely((tw->tw_family != family) ||
				     atomic_read(&twsk_net(tw)->count))) {
				inet_twsk_put(tw);
				goto restart;
			}

			rcu_read_unlock();
			inet_twsk_deschedule(tw, twdr);
			inet_twsk_put(tw);
			goto restart_rcu;
		}
		/* If the nulls value we got at the end of this lookup is
		 * not the expected one, we must restart lookup.
		 * We probably met an item that was moved to another chain.
		 */
		if (get_nulls_value(node) != slot)
			goto restart;
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL_GPL(inet_twsk_purge);
