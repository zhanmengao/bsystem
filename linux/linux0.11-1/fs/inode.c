/* passed
 *  linux/fs/inode.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#include <string.h>// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <sys/stat.h>// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
						// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/mm.h>// �ڴ����ͷ�ļ�������ҳ���С�����һЩҳ���ͷź���ԭ�͡�
#include <asm/system.h>// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ

struct m_inode inode_table[NR_INODE]={0};// �ڴ���i �ڵ��NR_INODE=32 ���

static void read_inode(struct m_inode * inode);
static void write_inode(struct m_inode * inode);

//// �ȴ�ָ����i �ڵ���á�
// ���i �ڵ��ѱ��������򽫵�ǰ������Ϊ�����жϵĵȴ�״̬��ֱ����i �ڵ������
static _inline void wait_on_inode(struct m_inode * inode)
{
	cli();
	while (inode->i_lock)
		sleep_on(&inode->i_wait);
	sti();
}

//// ��ָ����i �ڵ�����������ָ����i �ڵ㣩��
// ���i �ڵ��ѱ��������򽫵�ǰ������Ϊ�����жϵĵȴ�״̬��
// ֱ����i �ڵ������Ȼ�����������
static _inline void lock_inode(struct m_inode * inode)
{
	cli();
	while (inode->i_lock)
		sleep_on(&inode->i_wait);
	inode->i_lock=1;	// ��������־��
	sti();
}

//// ��ָ����i �ڵ������
// ��λi �ڵ��������־������ȷ�ػ��ѵȴ���i �ڵ�Ľ��̡�
static _inline void unlock_inode(struct m_inode * inode)
{
	inode->i_lock=0;
	wake_up(&inode->i_wait);
}

//// �ͷ��ڴ����豸dev ������i �ڵ㡣
// ɨ���ڴ��е�i �ڵ�����飬�����ָ���豸ʹ�õ�i �ڵ���ͷ�֮��
void invalidate_inodes(int dev)
{
	int i;
	struct m_inode * inode;

	inode = 0+inode_table;		// ��ָ������ָ��i �ڵ��ָ���������
	for(i=0 ; i<NR_INODE ; i++,inode++) {	// ɨ��i �ڵ��ָ�������е�����i �ڵ㡣
		wait_on_inode(inode);				// �ȴ���i �ڵ���ã���������
		if (inode->i_dev == dev) {			// �����ָ���豸��i �ڵ㣬��
			if (inode->i_count)				// �������������Ϊ0������ʾ�����棻
				printk("inode in use on removed disk\n\r");
			inode->i_dev = inode->i_dirt = 0;	// �ͷŸ�i �ڵ�(���豸��Ϊ0 ��)��
		}
	}
}

//// ͬ������i �ڵ㡣
// ͬ���ڴ����豸�ϵ�����i �ڵ���Ϣ��
void sync_inodes(void)
{
	int i;
	struct m_inode * inode;

	inode = 0+inode_table;		// ��ָ������ָ��i �ڵ��ָ���������
	for(i=0 ; i<NR_INODE ; i++,inode++) {	// ɨ��i �ڵ��ָ�����顣
		wait_on_inode(inode);				// �ȴ���i �ڵ���ã���������
		if (inode->i_dirt && !inode->i_pipe)	// �����i �ڵ����޸��Ҳ��ǹܵ��ڵ㣬
			write_inode(inode);					// ��д�̡�
	}
}

//// �ļ����ݿ�ӳ�䵽�̿�Ĵ��������(block λͼ��������bmap - block map)
// ������inode �C �ļ���i �ڵ㣻block �C �ļ��е����ݿ�ţ�create - ������־��
// ���������־��λ�����ڶ�Ӧ�߼��鲻����ʱ�������´��̿顣
// ����block ���ݿ��Ӧ���豸�ϵ��߼���ţ��̿�ţ���
static int _bmap(struct m_inode * inode,int block,int create)
{
	struct buffer_head * bh;
	int i;

// ������С��0����������
	if (block<0)
		panic("_bmap: block<0");
// �����Ŵ���ֱ�ӿ���+ ��ӿ���+ ���μ�ӿ����������ļ�ϵͳ��ʾ��Χ����������
	if (block >= 7+512+512*512)
		panic("_bmap: block>big");
// ����ÿ��С��7����ʹ��ֱ�ӿ��ʾ��
	if (block<7) {
// ���������־��λ������i �ڵ��ж�Ӧ�ÿ���߼��飨���Σ��ֶ�Ϊ0��������Ӧ�豸����һ����
// �飨�߼��飬���飩�����������߼���ţ��̿�ţ������߼����ֶ��С�Ȼ������i �ڵ��޸�ʱ�䣬
// ��i �ڵ����޸ı�־����󷵻��߼���š�
		if (create && !inode->i_zone[block])
			if (inode->i_zone[block]=new_block(inode->i_dev)) {
				inode->i_ctime=CURRENT_TIME;
				inode->i_dirt=1;
			}
		return inode->i_zone[block];
	}
// ����ÿ��>=7������С��7+512����˵����һ�μ�ӿ顣�����һ�μ�ӿ���д���
	block -= 7;
	if (block<512) {
// ����Ǵ��������Ҹ�i �ڵ��ж�Ӧ��ӿ��ֶ�Ϊ0�������ļ����״�ʹ�ü�ӿ飬��������
// һ���̿����ڴ�ż�ӿ���Ϣ��������ʵ�ʴ��̿�������ӿ��ֶ��С�Ȼ������i �ڵ�
// ���޸ı�־���޸�ʱ�䡣
		if (create && !inode->i_zone[7])
			if (inode->i_zone[7]=new_block(inode->i_dev)) {
				inode->i_dirt=1;
				inode->i_ctime=CURRENT_TIME;
			}
// ����ʱi �ڵ��ӿ��ֶ���Ϊ0������������̿�ʧ�ܣ�����0 �˳���
		if (!inode->i_zone[7])
			return 0;
// ��ȡ�豸�ϵ�һ�μ�ӿ顣
		if (!(bh = bread(inode->i_dev,inode->i_zone[7])))
			return 0;
// ȡ�ü�ӿ��ϵ�block ���е��߼���ţ��̿�ţ���
		i = ((unsigned short *) (bh->b_data))[block];
// ����Ǵ������Ҽ�ӿ�ĵ�block ���е��߼����Ϊ0 �Ļ���������һ���̿飨�߼��飩������
// ��ӿ��еĵ�block ����ڸ����߼����š�Ȼ����λ��ӿ�����޸ı�־��
		if (create && !i)
			if (i=new_block(inode->i_dev)) {
				((unsigned short *) (bh->b_data))[block]=i;
				bh->b_dirt=1;
			}
// ����ͷŸü�ӿ飬���ش�����������Ķ�Ӧblock ���߼���Ŀ�š�
		brelse(bh);
		return i;
	}
// �������е��ˣ��������ݿ��Ƕ��μ�ӿ飬���������һ�μ�ӿ����ơ������ǶԶ��μ�ӿ�Ĵ���
// ��block �ټ�ȥ��ӿ������ɵĿ���(512)��
	block -= 512;
// ������´�������i �ڵ�Ķ��μ�ӿ��ֶ�Ϊ0����������һ���̿����ڴ�Ŷ��μ�ӿ��һ����
// ��Ϣ��������ʵ�ʴ��̿��������μ�ӿ��ֶ��С�֮����i �ڵ����޸ı��ƺ��޸�ʱ�䡣
	if (create && !inode->i_zone[8])
		if (inode->i_zone[8]=new_block(inode->i_dev)) {
			inode->i_dirt=1;
			inode->i_ctime=CURRENT_TIME;
		}
// ����ʱi �ڵ���μ�ӿ��ֶ�Ϊ0������������̿�ʧ�ܣ�����0 �˳���
	if (!inode->i_zone[8])
		return 0;
// ��ȡ�ö��μ�ӿ��һ���顣
	if (!(bh=bread(inode->i_dev,inode->i_zone[8])))
		return 0;
// ȡ�ö��μ�ӿ��һ�����ϵ�(block/512)���е��߼���š�
	i = ((unsigned short *)bh->b_data)[block>>9];
// ����Ǵ������Ҷ��μ�ӿ��һ�����ϵ�(block/512)���е��߼����Ϊ0 �Ļ�����������һ����
// �飨�߼��飩��Ϊ���μ�ӿ�Ķ����飬���ö��μ�ӿ��һ�����е�(block/512)����ڸö���
// ��Ŀ�š�Ȼ����λ���μ�ӿ��һ�������޸ı�־�����ͷŶ��μ�ӿ��һ���顣
	if (create && !i)
		if (i=new_block(inode->i_dev)) {
			((unsigned short *) (bh->b_data))[block>>9]=i;
			bh->b_dirt=1;
		}
	brelse(bh);
// ������μ�ӿ�Ķ�������Ϊ0����ʾ������̿�ʧ�ܣ�����0 �˳���
	if (!i)
		return 0;
// ��ȡ���μ�ӿ�Ķ����顣
	if (!(bh=bread(inode->i_dev,i)))
		return 0;
// ȡ�ö������ϵ�block ���е��߼���š�(����511 ��Ϊ���޶�block ֵ������511)
	i = ((unsigned short *)bh->b_data)[block&511];
// ����Ǵ������Ҷ�����ĵ�block ���е��߼����Ϊ0 �Ļ���������һ���̿飨�߼��飩����Ϊ
// ���մ��������Ϣ�Ŀ顣���ö������еĵ�block ����ڸ����߼�����(i)��Ȼ����λ�������
// ���޸ı�־��
	if (create && !i)
		if (i=new_block(inode->i_dev)) {
			((unsigned short *) (bh->b_data))[block&511]=i;
			bh->b_dirt=1;
		}
// ����ͷŸö��μ�ӿ�Ķ����飬���ش�����������Ķ�Ӧblock ���߼���Ŀ�š�
	brelse(bh);
	return i;
}

//// ����i �ڵ���Ϣȡ�ļ����ݿ�block ���豸�϶�Ӧ���߼���š�
int bmap(struct m_inode * inode,int block)
{
	return _bmap(inode,block,0);
}

//// �����ļ����ݿ�block ���豸�϶�Ӧ���߼��飬�������豸�϶�Ӧ���߼���š�
int create_block(struct m_inode * inode, int block)
{
	return _bmap(inode,block,1);
}

//// �ͷ�һ��i �ڵ�(��д���豸)��
void iput(struct m_inode * inode)
{
	if (!inode)
		return;
	wait_on_inode(inode);	// �ȴ�inode �ڵ����(����������Ļ�)��
	if (!inode->i_count)
		panic("iput: trying to free free inode");
// ����ǹܵ�i �ڵ㣬���ѵȴ��ùܵ��Ľ��̣����ô�����1��������������򷵻ء������ͷ�
// �ܵ�ռ�õ��ڴ�ҳ�棬����λ�ýڵ�����ü���ֵ�����޸ı�־�͹ܵ���־�������ء�
// ����pipe �ڵ㣬inode->i_size ����������ڴ�ҳ��ַ���μ�get_pipe_inode()��228��234 �С�
	if (inode->i_pipe) {
		wake_up(&inode->i_wait);
		if (--inode->i_count)
			return;
		free_page(inode->i_size);
		inode->i_count=0;
		inode->i_dirt=0;
		inode->i_pipe=0;
		return;
	}
// ���i �ڵ��Ӧ���豸��=0���򽫴˽ڵ�����ü����ݼ�1�����ء�
	if (!inode->i_dev) {
		inode->i_count--;
		return;
	}
// ����ǿ��豸�ļ���i �ڵ㣬��ʱ�߼����ֶ�0 �����豸�ţ���ˢ�¸��豸�����ȴ�i �ڵ������
	if (S_ISBLK(inode->i_mode)) {
		sync_dev(inode->i_zone[0]);
		wait_on_inode(inode);
	}
repeat:
// ���i �ڵ�����ü�������1����ݼ�1��
	if (inode->i_count>1) {
		inode->i_count--;
		return;
	}
// ���i �ڵ��������Ϊ0�����ͷŸ�i �ڵ�������߼��飬���ͷŸ�i �ڵ㡣
	if (!inode->i_nlinks) {
		truncate(inode);
		free_inode(inode);
		return;
	}
// �����i �ڵ��������޸ģ�����¸�i �ڵ㣬���ȴ���i �ڵ������
	if (inode->i_dirt) {
		write_inode(inode);	/* we can sleep - so do again */
		wait_on_inode(inode);
		goto repeat;
	}
// i �ڵ����ü����ݼ�1��
	inode->i_count--;
	return;
}

//// ��i �ڵ��(inode_table)�л�ȡһ������i �ڵ��
// Ѱ�����ü���count Ϊ0 ��i �ڵ㣬������д�̺����㣬������ָ�롣
struct m_inode * get_empty_inode(void)
{
	struct m_inode * inode;
	static struct m_inode * last_inode = inode_table;// last_inode ָ��i �ڵ���һ�
	int i;

	do {
		// ɨ��i �ڵ��
		inode = NULL;
		for (i = NR_INODE; i ; i--) {
// ���last_inode �Ѿ�ָ��i �ڵ������1 ��֮������������ָ��i �ڵ��ʼ����
			if (++last_inode >= inode_table + NR_INODE)
				last_inode = inode_table;
// ���last_inode ��ָ���i �ڵ�ļ���ֵΪ0����˵�������ҵ�����i �ڵ����inode ָ��
// ��i �ڵ㡣�����i �ڵ�����޸ı�־��������־��Ϊ0�������ǿ���ʹ�ø�i �ڵ㣬�����˳�ѭ����
			if (!last_inode->i_count) {
				inode = last_inode;
				if (!inode->i_dirt && !inode->i_lock)
					break;
			}
		}
// ���û���ҵ�����i �ڵ�(inode=NULL)��������i �ڵ���ӡ����������ʹ�ã���������
		if (!inode) {
			for (i=0 ; i<NR_INODE ; i++)
				printk("%04x: %6d\t",inode_table[i].i_dev,
					inode_table[i].i_num);
			panic("No free inodes in mem");
		}
// �ȴ���i �ڵ����������ֱ������Ļ�����
		wait_on_inode(inode);
// �����i �ڵ����޸ı�־����λ�Ļ����򽫸�i �ڵ�ˢ�£����ȴ���i �ڵ������
		while (inode->i_dirt) {
			write_inode(inode);
			wait_on_inode(inode);
		}
	} while (inode->i_count);// ���i �ڵ��ֱ�����ռ�õĻ���������Ѱ�ҿ���i �ڵ㡣
// ���ҵ�����i �ڵ���򽫸�i �ڵ����������㣬�������ñ�־Ϊ1�����ظ�i �ڵ�ָ�롣
	memset(inode,0,sizeof(*inode));
	inode->i_count = 1;
	return inode;
}

//// ��ȡ�ܵ��ڵ㡣����Ϊi �ڵ�ָ�루�����NULL ��ʧ�ܣ���
// ����ɨ��i �ڵ��Ѱ��һ������i �ڵ��Ȼ��ȡ��һҳ�����ڴ湩�ܵ�ʹ�á�
// Ȼ�󽫵õ���i �ڵ�����ü�����Ϊ2(���ߺ�д��)����ʼ���ܵ�ͷ��β����i �ڵ�Ĺܵ����ͱ�ʾ��
struct m_inode * get_pipe_inode(void)
{
	struct m_inode * inode;

	if (!(inode = get_empty_inode()))	// ����Ҳ�������i �ڵ��򷵻�NULL��
		return NULL;
	if (!(inode->i_size=get_free_page())) {// �ڵ��i_size �ֶ�ָ�򻺳�����
		inode->i_count = 0;					// �����û�п����ڴ棬��
		return NULL;						// �ͷŸ�i �ڵ㣬������NULL��
	}
	inode->i_count = 2;	/* ��/д�����ܼ� */
	PIPE_HEAD(*inode) = PIPE_TAIL(*inode) = 0;// ��λ�ܵ�ͷβָ�롣
	inode->i_pipe = 1;			// �ýڵ�Ϊ�ܵ�ʹ�õı�־��
	return inode;		// ����i �ڵ�ָ�롣
}

//// ���豸�϶�ȡָ���ڵ�ŵ�i �ڵ㡣
// nr - i �ڵ�š�
struct m_inode * iget(int dev,int nr)
{
	struct m_inode * inode, * empty;

	if (!dev)
		panic("iget with dev==0");
// ��i �ڵ����ȡһ������i �ڵ㡣
	empty = get_empty_inode();
// ɨ��i �ڵ��Ѱ��ָ���ڵ�ŵ�i �ڵ㡣�������ýڵ�����ô�����
	inode = inode_table;
	while (inode < NR_INODE+inode_table) {
// �����ǰɨ���i �ڵ���豸�Ų�����ָ�����豸�Ż��߽ڵ�Ų�����ָ���Ľڵ�ţ������ɨ�衣
		if (inode->i_dev != dev || inode->i_num != nr) {
			inode++;
			continue;
		}
// �ҵ�ָ���豸�źͽڵ�ŵ�i �ڵ㣬�ȴ��ýڵ����������������Ļ�����
		wait_on_inode(inode);
// �ڵȴ��ýڵ�����Ľ׶Σ��ڵ����ܻᷢ���仯�������ٴ��жϣ���������˱仯�����ٴ�����
// ɨ������i �ڵ��
		if (inode->i_dev != dev || inode->i_num != nr) {
			inode = inode_table;
			continue;
		}
// ����i �ڵ����ü�����1��
		inode->i_count++;
		if (inode->i_mount) {
			int i;

// �����i �ڵ��������ļ�ϵͳ�İ�װ�㣬���ڳ����������Ѱ��װ�ڴ�i �ڵ�ĳ����顣���û��
// �ҵ�������ʾ������Ϣ�����ͷź�����ʼ��ȡ�Ŀ��нڵ㣬���ظ�i �ڵ�ָ�롣
			for (i = 0 ; i<NR_SUPER ; i++)
				if (super_block[i].s_imount==inode)
					break;
			if (i >= NR_SUPER) {
				printk("Mounted inode hasn't got sb\n");
				if (empty)
					iput(empty);
				return inode;
			}
// ����i �ڵ�д�̡��Ӱ�װ�ڴ�i �ڵ��ļ�ϵͳ�ĳ�������ȡ�豸�ţ�����i �ڵ��Ϊ1��Ȼ������
// ɨ������i �ڵ��ȡ�ñ���װ�ļ�ϵͳ�ĸ��ڵ㡣
			iput(inode);
			dev = super_block[i].s_dev;
			nr = ROOT_INO;
			inode = inode_table;
			continue;
		}
// �Ѿ��ҵ���Ӧ��i �ڵ㣬��˷�����ʱ����Ŀ��нڵ㣬���ظ��ҵ���i �ڵ㡣
		if (empty)
			iput(empty);
		return inode;
	}
// �����i �ڵ����û���ҵ�ָ����i �ڵ㣬������ǰ������Ŀ���i �ڵ���i �ڵ���н����ýڵ㡣
// ������Ӧ�豸�϶�ȡ��i �ڵ���Ϣ�����ظ�i �ڵ㡣
	if (!empty)
		return (NULL);
	inode=empty;
	inode->i_dev = dev;
	inode->i_num = nr;
	read_inode(inode);
	return inode;
}

//// ���豸�϶�ȡָ��i �ڵ����Ϣ���ڴ��У��������У���
static void read_inode(struct m_inode * inode)
{
	struct super_block * sb;
	struct buffer_head * bh;
	int block;

// ����������i �ڵ㣬ȡ�ýڵ������豸�ĳ����顣
	lock_inode(inode);
	if (!(sb=get_super(inode->i_dev)))
		panic("trying to read inode without dev");
// ��i �ڵ����ڵ��߼����= (������+������) + i �ڵ�λͼռ�õĿ���+ �߼���λͼռ�õĿ���+
// (i �ڵ��-1)/ÿ�麬�е�i �ڵ�����
	block = 2 + sb->s_imap_blocks + sb->s_zmap_blocks +
		(inode->i_num-1)/INODES_PER_BLOCK;
// ���豸�϶�ȡ��i �ڵ����ڵ��߼��飬������inode ָ��ָ���Ӧi �ڵ���Ϣ��
	if (!(bh=bread(inode->i_dev,block)))
		panic("unable to read i-node block");
	*(struct d_inode *)inode =
		((struct d_inode *)bh->b_data)
			[(inode->i_num-1)%INODES_PER_BLOCK];
// ����ͷŶ���Ļ���������������i �ڵ㡣
	brelse(bh);
	unlock_inode(inode);
}

//// ��ָ��i �ڵ���Ϣд���豸��д�뻺������Ӧ�Ļ�����У���������ˢ��ʱ��д�����У���
static void write_inode(struct m_inode * inode)
{
	struct super_block * sb;
	struct buffer_head * bh;
	int block;

// ����������i �ڵ㣬�����i �ڵ�û�б��޸Ĺ����߸�i �ڵ���豸�ŵ����㣬�������i �ڵ㣬
// ���˳���
	lock_inode(inode);
	if (!inode->i_dirt || !inode->i_dev) {
		unlock_inode(inode);
		return;
	}
// ��ȡ��i �ڵ�ĳ����顣
	if (!(sb=get_super(inode->i_dev)))
		panic("trying to write inode without device");
// ��i �ڵ����ڵ��߼����= (������+������) + i �ڵ�λͼռ�õĿ���+ �߼���λͼռ�õĿ���+
// (i �ڵ��-1)/ÿ�麬�е�i �ڵ�����
	block = 2 + sb->s_imap_blocks + sb->s_zmap_blocks +
		(inode->i_num-1)/INODES_PER_BLOCK;
// ���豸�϶�ȡ��i �ڵ����ڵ��߼��顣
	if (!(bh=bread(inode->i_dev,block)))
		panic("unable to read i-node block");
// ����i �ڵ���Ϣ���Ƶ��߼����Ӧ��i �ڵ�����С�
	((struct d_inode *)bh->b_data)
		[(inode->i_num-1)%INODES_PER_BLOCK] =
			*(struct d_inode *)inode;
// �û��������޸ı�־����i �ڵ��޸ı�־���㡣Ȼ���ͷŸú���i �ڵ�Ļ���������������i �ڵ㡣
	bh->b_dirt=1;
	inode->i_dirt=0;
	brelse(bh);
	unlock_inode(inode);
}
