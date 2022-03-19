/* passed
* linux/fs/super.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

/*
* super.c contains code to handle the super-block tables.
*/
#include <linux/config.h>	// �ں�����ͷ�ļ�������������Ժ�Ӳ�����ͣ�HD_TYPE����ѡ�
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/system.h>		// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ

#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <sys/stat.h>		// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

int sync_dev (int dev);		// ��ָ���豸ִ�и��ٻ������豸�����ݵ�ͬ��������(fs/buffer.c,59)
void wait_for_keypress (void);	// �ȴ�������(kernel/chr_drv/tty_io.c, 140)

/* set_bit()ʹ����setb ָ���Ϊ��������gas ����ʶ��ָ��setc */
//// ����ָ��λƫ�ƴ�����λ��ֵ(0 ��1)�������ظñ���λֵ��(Ӧ��ȡ��Ϊtest_bit()������)
// Ƕ��ʽ���ꡣ����bitnr �Ǳ���λƫ��ֵ��addr �ǲ��Ա���λ��������ʼ��ַ��
// %0 - ax(__res)��%1 - 0��%2 - bitnr��%3 - addr
/*#define set_bit(bitnr,addr) ({ \
register int __res __asm__( "ax"); \
__asm__( "bt %2,%3;setb %%al": "=a" (__res): "a" (0), "r" (bitnr), "m" (*(addr))); \
__res; })*/
extern _inline int set_bit(int bitnr,char* addr)
{
//	register int __res;
	_asm{
		xor eax,eax
		mov ebx,bitnr
		mov edx,addr
		bt [edx],ebx
		setb al
//		mov __res,eax
	}
//	return __res;
}

struct super_block super_block[NR_SUPER] = {0};	// ������ṹ���飨��8 ���
/* this is initialized in init/main.c */
/* ROOT_DEV ����init/main.c �б���ʼ�� */
int ROOT_DEV;

//// ����ָ���ĳ����顣
static void
lock_super (struct super_block *sb)
{
	cli ();			// ���жϡ�
	while (sb->s_lock)		// ����ó������Ѿ���������˯�ߵȴ���
		sleep_on (&(sb->s_wait));
	sb->s_lock = 1;		// ���ó������������������־����
	sti ();			// ���жϡ�
}

//// ��ָ������������������ʹ��ulock_super ������������������
static void
free_super (struct super_block *sb)
{
	cli ();			// ���жϡ�
	sb->s_lock = 0;		// ��λ������־��
	wake_up (&(sb->s_wait));	// ���ѵȴ��ó�����Ľ��̡�
	sti ();			// ���жϡ�
}

//// ˯�ߵȴ������������
static void
wait_on_super (struct super_block *sb)
{
	cli ();			// ���жϡ�
	while (sb->s_lock)		// ����������Ѿ���������˯�ߵȴ���
		sleep_on (&(sb->s_wait));
	sti ();			// ���жϡ�
}

//// ȡָ���豸�ĳ����顣���ظó�����ṹָ�롣
struct super_block *
get_super (int dev)
{
	struct super_block *s;

// ���û��ָ���豸���򷵻ؿ�ָ�롣
	if (!dev)
		return NULL;
// s ָ�򳬼������鿪ʼ���������������������飬Ѱ��ָ���豸�ĳ����顣
	s = 0 + super_block;
	while (s < NR_SUPER + super_block)
// �����ǰ��������ָ���豸�ĳ����飬�����ȵȴ��ó�������������Ѿ����������������Ļ�����
// �ڵȴ��ڼ䣬�ó������п��ܱ������豸ʹ�ã���˴�ʱ�����ж�һ���Ƿ���ָ���豸�ĳ����飬
// ������򷵻ظó������ָ�롣��������¶Գ���������������һ�飬���s ����ָ�򳬼�������
// ��ʼ����
	if (s->s_dev == dev)
	{
		wait_on_super (s);
		if (s->s_dev == dev)
			return s;
		s = 0 + super_block;
// �����ǰ������ǣ�������һ����û���ҵ�ָ���ĳ����飬�򷵻ؿ�ָ�롣
	}
	else
		s++;
	return NULL;
}

//// �ͷ�ָ���豸�ĳ����顣
// �ͷ��豸��ʹ�õĳ������������s_dev=0�������ͷŸ��豸i �ڵ�λͼ���߼���λͼ��ռ��
// �ĸ��ٻ���顣����������Ӧ���ļ�ϵͳ�Ǹ��ļ�ϵͳ��������i �ڵ����Ѿ���װ���������ļ�
// ϵͳ�������ͷŸó����顣
void
put_super (int dev)
{
	struct super_block *sb;
//  struct m_inode *inode;
	int i;

// ���ָ���豸�Ǹ��ļ�ϵͳ�豸������ʾ������Ϣ����ϵͳ�̸ı��ˣ�׼��������ս�ɡ��������ء�
	if (dev == ROOT_DEV)
	{
		printk ("root diskette changed: prepare for armageddon\n\r");
		return;
	}
// ����Ҳ���ָ���豸�ĳ����飬�򷵻ء�
	if (!(sb = get_super (dev)))
		return;
// ����ó�����ָ�����ļ�ϵͳi �ڵ��ϰ�װ���������ļ�ϵͳ������ʾ������Ϣ�����ء�
	if (sb->s_imount)
	{
		printk ("Mounted disk changed - tssk, tssk\n\r");
		return;
	}
// �ҵ�ָ���豸�ĳ���������������ó����飬Ȼ���øó������Ӧ���豸���ֶ�Ϊ0��Ҳ������
// �����ó����顣
	lock_super (sb);
	sb->s_dev = 0;
// Ȼ���ͷŸ��豸i �ڵ�λͼ���߼���λͼ�ڻ���������ռ�õĻ���顣
	for (i = 0; i < I_MAP_SLOTS; i++)
		brelse (sb->s_imap[i]);
	for (i = 0; i < Z_MAP_SLOTS; i++)
		brelse (sb->s_zmap[i]);
// ���Ըó���������������ء�
	free_super (sb);
	return;
}

//// ���豸�϶�ȡ�����鵽�������С�
// ������豸�ĳ������Ѿ��ڸ��ٻ����в�����Ч����ֱ�ӷ��ظó������ָ�롣
static struct super_block *
read_super (int dev)
{
	struct super_block *s;
	struct buffer_head *bh;
	int i, block;

// ���û��ָ���豸���򷵻ؿ�ָ�롣
	if (!dev)
		return NULL;
// ���ȼ����豸�Ƿ�ɸ�������Ƭ��Ҳ���Ƿ��������豸��������������̣�����ٻ������йظ�
// �豸�����л�����ʧЧ����Ҫ����ʧЧ�����ͷ�ԭ�����ص��ļ�ϵͳ����
	check_disk_change (dev);
// ������豸�ĳ������Ѿ��ڸ��ٻ����У���ֱ�ӷ��ظó������ָ�롣
	if (s = get_super (dev))
		return s;
// ���������ڳ������������ҳ�һ������(Ҳ����s_dev=0 ����)����������Ѿ�ռ���򷵻ؿ�ָ�롣
	for (s = 0 + super_block;; s++)
	{
		if (s >= NR_SUPER + super_block)
			return NULL;
		if (!s->s_dev)
			break;
	}
// �ҵ����������󣬾ͽ��ó���������ָ���豸���Ըó�������ڴ�����в��ֳ�ʼ����
	s->s_dev = dev;
	s->s_isup = NULL;
	s->s_imount = NULL;
	s->s_time = 0;
	s->s_rd_only = 0;
	s->s_dirt = 0;
// Ȼ�������ó����飬�����豸�϶�ȡ��������Ϣ��bh ָ��Ļ������С���������������ʧ�ܣ�
// ���ͷ�����ѡ���ĳ����������е��������������ؿ�ָ���˳���
	lock_super (s);
	if (!(bh = bread (dev, 1)))
	{
		s->s_dev = 0;
		free_super (s);
		return NULL;
	}
// ���豸�϶�ȡ�ĳ�������Ϣ���Ƶ�������������Ӧ��ṹ�С����ͷŴ�Ŷ�ȡ��Ϣ�ĸ��ٻ���顣
	*((struct d_super_block *) s) = *((struct d_super_block *) bh->b_data);
	brelse (bh);
// �����ȡ�ĳ�������ļ�ϵͳħ���ֶ����ݲ��ԣ�˵���豸�ϲ�����ȷ���ļ�ϵͳ�����ͬ����
// һ�����ͷ�����ѡ���ĳ����������е��������������ؿ�ָ���˳���
// ���ڸð�linux �ںˣ�ֻ֧��minix �ļ�ϵͳ�汾1.0����ħ����0x137f��
	if (s->s_magic != SUPER_MAGIC)
	{
		s->s_dev = 0;
		free_super (s);
		return NULL;
	}
// ���濪ʼ��ȡ�豸��i �ڵ�λͼ���߼���λͼ���ݡ����ȳ�ʼ���ڴ泬����ṹ��λͼ�ռ䡣
	for (i = 0; i < I_MAP_SLOTS; i++)
		s->s_imap[i] = NULL;
	for (i = 0; i < Z_MAP_SLOTS; i++)
		s->s_zmap[i] = NULL;
// Ȼ����豸�϶�ȡi �ڵ�λͼ���߼���λͼ��Ϣ��������ڳ������Ӧ�ֶ��С�
	block = 2;
	for (i = 0; i < s->s_imap_blocks; i++)
		if (s->s_imap[i] = bread (dev, block))
			block++;
		else
			break;
	for (i = 0; i < s->s_zmap_blocks; i++)
		if (s->s_zmap[i] = bread (dev, block))
			block++;
		else
			break;
// ���������λͼ�߼�����������λͼӦ��ռ�е��߼�������˵���ļ�ϵͳλͼ��Ϣ�����⣬������
// ��ʼ��ʧ�ܡ����ֻ���ͷ�ǰ�������������Դ�����ؿ�ָ�벢�˳���
	if (block != 2 + s->s_imap_blocks + s->s_zmap_blocks)
	{
// �ͷ�i �ڵ�λͼ���߼���λͼռ�õĸ��ٻ�������
		for (i = 0; i < I_MAP_SLOTS; i++)
			brelse (s->s_imap[i]);
		for (i = 0; i < Z_MAP_SLOTS; i++)
			brelse (s->s_zmap[i]);
//�ͷ�����ѡ���ĳ����������е���������ó���������ؿ�ָ���˳���
		s->s_dev = 0;
		free_super (s);
		return NULL;
	}
// ����һ�гɹ��������������i �ڵ�ĺ�������������豸�����е�i �ڵ��Ѿ�ȫ��ʹ�ã������
// �����᷵��0 ֵ�����0 ��i �ڵ��ǲ����õģ��������ｫλͼ�е����λ����Ϊ1���Է�ֹ�ļ�
// ϵͳ����0 ��i �ڵ㡣ͬ���ĵ���Ҳ���߼���λͼ�����λ����Ϊ1��
	s->s_imap[0]->b_data[0] |= 1;
	s->s_zmap[0]->b_data[0] |= 1;
// �����ó����飬�����س�����ָ�롣
	free_super (s);
	return s;
}

//// ж���ļ�ϵͳ��ϵͳ���ú�����
// ����dev_name ���豸�ļ�����
int
sys_umount (char *dev_name)
{
	struct m_inode *inode;
	struct super_block *sb;
	int dev;

// ���ȸ����豸�ļ����ҵ���Ӧ��i �ڵ㣬��ȡ���е��豸�š�
	if (!(inode = namei (dev_name)))
		return -ENOENT;
	dev = inode->i_zone[0];
// ������ǿ��豸�ļ������ͷŸ������i �ڵ�dev_i�����س����롣
	if (!S_ISBLK (inode->i_mode))
	{
		iput (inode);
		return -ENOTBLK;
	}
// �ͷ��豸�ļ�����i �ڵ㡣
	iput (inode);
// ����豸�Ǹ��ļ�ϵͳ�����ܱ�ж�أ����س���š�
	if (dev == ROOT_DEV)
		return -EBUSY;
// ���ȡ�豸�ĳ�����ʧ�ܣ����߸��豸�ļ�ϵͳû�а�װ�����򷵻س����롣
	if (!(sb = get_super (dev)) || !(sb->s_imount))
		return -ENOENT;
// �����������ָ���ı���װ����i �ڵ�û����λ�䰲װ��־������ʾ������Ϣ��
	if (!sb->s_imount->i_mount)
		printk ("Mounted inode has i_mount=0\n");
// ����i �ڵ�����Ƿ��н�����ʹ�ø��豸�ϵ��ļ���������򷵻�æ�����롣
	for (inode = inode_table + 0; inode < inode_table + NR_INODE; inode++)
		if (inode->i_dev == dev && inode->i_count)
			return -EBUSY;
// ��λ����װ����i �ڵ�İ�װ��־���ͷŸ�i �ڵ㡣
	sb->s_imount->i_mount = 0;
	iput (sb->s_imount);
// �ó������б���װi �ڵ��ֶ�Ϊ�գ����ͷ��豸�ļ�ϵͳ�ĸ�i �ڵ㣬�ó������б���װϵͳ
// ��i �ڵ�ָ��Ϊ�ա�
	sb->s_imount = NULL;
	iput (sb->s_isup);
	sb->s_isup = NULL;
// �ͷŸ��豸�ĳ������Լ�λͼռ�õĻ���飬���Ը��豸ִ�и��ٻ������豸�����ݵ�ͬ��������
	put_super (dev);
	sync_dev (dev);
	return 0;
}

//// ��װ�ļ�ϵͳ���ú�����
// ����dev_name ���豸�ļ�����dir_name �ǰ�װ����Ŀ¼����rw_flag ����װ�ļ��Ķ�д��־��
// �������صĵط�������һ��Ŀ¼�������Ҷ�Ӧ��i �ڵ�û�б���������ռ�á�
int
sys_mount (char *dev_name, char *dir_name, int rw_flag)
{
	struct m_inode *dev_i, *dir_i;
	struct super_block *sb;
	int dev;

// ���ȸ����豸�ļ����ҵ���Ӧ��i �ڵ㣬��ȡ���е��豸�š�
// ���ڿ������豸�ļ����豸����i �ڵ��i_zone[0]�С�
	if (!(dev_i = namei (dev_name)))
		return -ENOENT;
	dev = dev_i->i_zone[0];
// ������ǿ��豸�ļ������ͷŸ�ȡ�õ�i �ڵ�dev_i�����س����롣
	if (!S_ISBLK (dev_i->i_mode))
	{
		iput (dev_i);
		return -EPERM;
	}
// �ͷŸ��豸�ļ���i �ڵ�dev_i��
	iput (dev_i);
// ���ݸ�����Ŀ¼�ļ����ҵ���Ӧ��i �ڵ�dir_i��
	if (!(dir_i = namei (dir_name)))
		return -ENOENT;
// �����i �ڵ�����ü�����Ϊ1�������������ã������߸�i �ڵ�Ľڵ���Ǹ��ļ�ϵͳ�Ľڵ�
// ��1�����ͷŸ�i �ڵ㣬���س����롣
	if (dir_i->i_count != 1 || dir_i->i_num == ROOT_INO)
	{
		iput (dir_i);
		return -EBUSY;
	}
// ����ýڵ㲻��һ��Ŀ¼�ļ��ڵ㣬��Ҳ�ͷŸ�i �ڵ㣬���س����롣
	if (!S_ISDIR (dir_i->i_mode))
	{
		iput (dir_i);
		return -EPERM;
	}
// ��ȡ����װ�ļ�ϵͳ�ĳ����飬���ʧ����Ҳ�ͷŸ�i �ڵ㣬���س����롣
	if (!(sb = read_super (dev)))
	{
		iput (dir_i);
		return -EBUSY;
	}
// �����Ҫ����װ���ļ�ϵͳ�Ѿ���װ�������ط������ͷŸ�i �ڵ㣬���س����롣
	if (sb->s_imount)
	{
		iput (dir_i);
		return -EBUSY;
	}
// �����Ҫ��װ����i �ڵ��Ѿ���װ���ļ�ϵͳ(��װ��־�Ѿ���λ)�����ͷŸ�i �ڵ㣬���س����롣
	if (dir_i->i_mount)
	{
		iput (dir_i);
		return -EPERM;
	}
// ����װ�ļ�ϵͳ������ġ�����װ��i �ڵ㡱�ֶ�ָ��װ����Ŀ¼����i �ڵ㡣
	sb->s_imount = dir_i;
// ���ð�װλ��i �ڵ�İ�װ��־�ͽڵ����޸ı�־��/* ע�⣡����û��iput(dir_i) */
	dir_i->i_mount = 1;		/* �⽫��umount �ڲ��� */
	dir_i->i_dirt = 1;		/* NOTE! we don't iput(dir_i) */
	return 0;			/* we do that in umount */
}

//// ��װ���ļ�ϵͳ��
// �ú�������ϵͳ������ʼ������ʱ(sys_setup())���õġ�( kernel/blk_drv/hd.c, 157 )
void
mount_root (void)
{
	int i, free;
	struct super_block *p;
	struct m_inode *mi;

// �������i �ڵ�ṹ����32 ���ֽڣ���������������ж������ڷ�ֹ�޸�Դ����ʱ�Ĳ�һ���ԡ�
	if (32 != sizeof (struct d_inode))
		panic ("bad i-node size");
// ��ʼ���ļ������飨��64 �Ҳ��ϵͳͬʱֻ�ܴ�64 ���ļ������������ļ��ṹ�е����ü���
// ����Ϊ0��[??Ϊʲô���������ʼ����]
	for (i = 0; i < NR_FILE; i++)
		file_table[i].f_count = 0;
// ������ļ�ϵͳ�����豸�����̵Ļ�������ʾ��������ļ�ϵͳ�̣������س����������ȴ�������
	if (MAJOR (ROOT_DEV) == 2)
	{
		printk ("Insert root floppy and press ENTER");
		wait_for_keypress ();
	}
// ��ʼ�����������飨��8 ���
	for (p = &super_block[0]; p < &super_block[NR_SUPER]; p++)
	{
		p->s_dev = 0;
		p->s_lock = 0;
		p->s_wait = NULL;
	}
// ��������豸�ϳ�����ʧ�ܣ�����ʾ��Ϣ����������
	if (!(p = read_super (ROOT_DEV)))
		panic ("Unable to mount root");
//���豸�϶�ȡ�ļ�ϵͳ�ĸ�i �ڵ�(1)�����ʧ������ʾ������Ϣ��������
	if (!(mi = iget (ROOT_DEV, ROOT_INO)))
		panic ("Unable to read root i-node");
// ��i �ڵ����ô�������3 �Ρ���Ϊ����266-268 ����Ҳ�����˸�i �ڵ㡣
	mi->i_count += 3;		/* NOTE! it is logically used 4 times, not 1 */
/* ע�⣡���߼��Ͻ������ѱ�������4 �Σ�������1 �� */
// �øó�����ı���װ�ļ�ϵͳi �ڵ�ͱ���װ����i �ڵ�Ϊ��i �ڵ㡣
	p->s_isup = p->s_imount = mi;
// ���õ�ǰ���̵ĵ�ǰ����Ŀ¼�͸�Ŀ¼i �ڵ㡣��ʱ��ǰ������1 �Ž��̡�
	current->pwd = mi;
	current->root = mi;
// ͳ�Ƹ��豸�Ͽ��п�����������i ���ڳ������б������豸�߼���������
	free = 0;
	i = p->s_nzones;
// Ȼ������߼���λͼ����Ӧ����λ��ռ�����ͳ�Ƴ����п���������꺯��set_bit()ֻ���ڲ���
// ����λ���������ñ���λ��"i&8191"����ȡ��i �ڵ���ڵ�ǰ���е�ƫ��ֵ��"i>>13"�ǽ�i ����
// 8192��Ҳ����һ�����̿�����ı���λ����
	while (--i >= 0)
		if (!set_bit (i & 8191, p->s_zmap[i >> 13]->b_data))
			free++;
// ��ʾ�豸�Ͽ����߼�����/�߼���������
	printk ("%d/%d free blocks\n\r", free, p->s_nzones);
// ͳ���豸�Ͽ���i �ڵ�����������i ���ڳ������б������豸��i �ڵ�����+1����1 �ǽ�0 �ڵ�
// Ҳͳ�ƽ�ȥ��
	free = 0;
	i = p->s_ninodes + 1;
// Ȼ�����i �ڵ�λͼ����Ӧ����λ��ռ��������������i �ڵ�����
	while (--i >= 0)
		if (!set_bit (i & 8191, p->s_imap[i >> 13]->b_data))
			free++;
// ��ʾ�豸�Ͽ��õĿ���i �ڵ���/i �ڵ�������
	printk ("%d/%d free inodes\n\r", free, p->s_ninodes);
}
