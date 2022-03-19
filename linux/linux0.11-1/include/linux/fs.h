/*
* This file has definitions for some important file table
* structures etc.
*/
/*
* ���ļ�����ĳЩ��Ҫ�ļ���ṹ�Ķ���ȡ�
*/

#ifndef _FS_H
#define _FS_H

#include <sys/types.h>		// ����ͷ�ļ��������˻�����ϵͳ�������͡�

/* devices are as follows: (same as minix, so we can use the minix
* file system. These are major numbers.)
*
* 0 - unused (nodev)
* 1 - /dev/mem
* 2 - /dev/fd
* 3 - /dev/hd
* 4 - /dev/ttyx
* 5 - /dev/tty
* 6 - /dev/lp
* 7 - unnamed pipes
*/
/*
* ϵͳ�������豸���£�����minix ϵͳ��һ�����������ǿ���ʹ��minix ��
* �ļ�ϵͳ��������Щ�����豸�š���
*
* 0 - û���õ���nodev��
* 1 - /dev/mem �ڴ��豸��
* 2 - /dev/fd �����豸��
* 3 - /dev/hd Ӳ���豸��
* 4 - /dev/ttyx tty �����ն��豸��
* 5 - /dev/tty tty �ն��豸��
* 6 - /dev/lp ��ӡ�豸��
* 7 - unnamed pipes û�������Ĺܵ���
*/

#define IS_SEEKABLE(x) ((x)>=1 && (x)<=3)	// �Ƿ��ǿ���Ѱ�Ҷ�λ���豸��

#define READ 0
#define WRITE 1
#define READA 2			/* read-ahead - don't pause */
#define WRITEA 3		/* "write-ahead" - silly, but somewhat useful */

void buffer_init (long buffer_end);

#define MAJOR(a) (((unsigned)(a))>>8)	// ȡ���ֽڣ����豸�ţ���
#define MINOR(a) ((a)&0xff)	// ȡ���ֽڣ����豸�ţ���

#define NAME_LEN 14		// ���ֳ���ֵ��
#define ROOT_INO 1		// ��i �ڵ㡣

#define I_MAP_SLOTS 8		// i �ڵ�λͼ������
#define Z_MAP_SLOTS 8		// �߼��飨���ο飩λͼ������
#define SUPER_MAGIC 0x137F	// �ļ�ϵͳħ����

#define NR_OPEN 20		// ���ļ�����
#define NR_INODE 32
#define NR_FILE 64
#define NR_SUPER 8
#define NR_HASH 307
#define NR_BUFFERS nr_buffers
#define BLOCK_SIZE 1024		// ���ݿ鳤�ȡ�
#define BLOCK_SIZE_BITS 10	// ���ݿ鳤����ռ����λ����
#ifndef NULL
#define NULL 0
#endif

// ÿ���߼���ɴ�ŵ�i �ڵ�����
#define INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct d_inode)))
// ÿ���߼���ɴ�ŵ�Ŀ¼������
#define DIR_ENTRIES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct dir_entry)))

// �ܵ�ͷ���ܵ�β���ܵ���С���ܵ��գ����ܵ��������ܵ�ͷָ�������
#define PIPE_HEAD(inode) ((inode).i_zone[0])
#define PIPE_TAIL(inode) ((inode).i_zone[1])
#define PIPE_SIZE(inode) ((PIPE_HEAD(inode)-PIPE_TAIL(inode))&(PAGE_SIZE-1))
#define PIPE_EMPTY(inode) (PIPE_HEAD(inode)==PIPE_TAIL(inode))
#define PIPE_FULL(inode) (PIPE_SIZE(inode)==(PAGE_SIZE-1))
//#define INC_PIPE(head) \
//__asm__( "incl %0\n\tandl $4095,%0":: "m" (head))
#define INC_PIPE(head) _INC_PIPE(&(head))
extern _inline void _INC_PIPE(unsigned long *head) {
	_asm mov ebx,head
	_asm inc dword ptr [ebx]
	_asm and dword ptr [ebx],4095
}

typedef char buffer_block[BLOCK_SIZE];	// �黺������

// ������ͷ���ݽṹ������Ϊ��Ҫ��������
// �ڳ����г���bh ����ʾbuffer_head ���͵���д��
struct buffer_head
{
  char *b_data;			/* pointer to data block (1024 bytes) *///ָ�롣
  unsigned long b_blocknr;	/* block number */// ��š�
  unsigned short b_dev;		/* device (0 = free) */// ����Դ���豸�š�
  unsigned char b_uptodate;	// ���±�־����ʾ�����Ƿ��Ѹ��¡�
  unsigned char b_dirt;		/* 0-clean,1-dirty *///�޸ı�־:0 δ�޸�,1 ���޸�.
  unsigned char b_count;	/* users using this block */// ʹ�õ��û�����
  unsigned char b_lock;		/* 0 - ok, 1 -locked */// �������Ƿ�������
  struct task_struct *b_wait;	// ָ��ȴ��û���������������
  struct buffer_head *b_prev;	// hash ������ǰһ�飨���ĸ�ָ�����ڻ������Ĺ�����
  struct buffer_head *b_next;	// hash ��������һ�顣
  struct buffer_head *b_prev_free;	// ���б���ǰһ�顣
  struct buffer_head *b_next_free;	// ���б�����һ�顣
};

// �����ϵ������ڵ�(i �ڵ�)���ݽṹ��
struct d_inode
{
  unsigned short i_mode;	// �ļ����ͺ�����(rwx λ)��
  unsigned short i_uid;		// �û�id���ļ�ӵ���߱�ʶ������
  unsigned long i_size;		// �ļ���С���ֽ�������
  unsigned long i_time;		// �޸�ʱ�䣨��1970.1.1:0 �����룩��
  unsigned char i_gid;		// ��id(�ļ�ӵ�������ڵ���)��
  unsigned char i_nlinks;	// �����������ٸ��ļ�Ŀ¼��ָ���i �ڵ㣩��
  unsigned short i_zone[9];	// ֱ��(0-6)�����(7)��˫�ؼ��(8)�߼���š�
// zone ��������˼����������Σ����߼��顣
};

// �������ڴ��е�i �ڵ�ṹ��ǰ7 ����d_inode ��ȫһ����
struct m_inode
{
  unsigned short i_mode;	// �ļ����ͺ�����(rwx λ)��
  unsigned short i_uid;		// �û�id���ļ�ӵ���߱�ʶ������
  unsigned long i_size;		// �ļ���С���ֽ�������
  unsigned long i_mtime;	// �޸�ʱ�䣨��1970.1.1:0 �����룩��
  unsigned char i_gid;		// ��id(�ļ�ӵ�������ڵ���)��
  unsigned char i_nlinks;	// �ļ�Ŀ¼����������
  unsigned short i_zone[9];	// ֱ��(0-6)�����(7)��˫�ؼ��(8)�߼���š�
/* these are in memory also */
  struct task_struct *i_wait;	// �ȴ���i �ڵ�Ľ��̡�
  unsigned long i_atime;	// ������ʱ�䡣
  unsigned long i_ctime;	// i �ڵ������޸�ʱ�䡣
  unsigned short i_dev;		// i �ڵ����ڵ��豸�š�
  unsigned short i_num;		// i �ڵ�š�
  unsigned short i_count;	// i �ڵ㱻ʹ�õĴ�����0 ��ʾ��i �ڵ���С�
  unsigned char i_lock;		// ������־��
  unsigned char i_dirt;		// ���޸�(��)��־��
  unsigned char i_pipe;		// �ܵ���־��
  unsigned char i_mount;	// ��װ��־��
  unsigned char i_seek;		// ��Ѱ��־(lseek ʱ)��
  unsigned char i_update;	// ���±�־��
};

// �ļ��ṹ���������ļ������i �ڵ�֮�佨����ϵ��
struct file
{
  unsigned short f_mode;	// �ļ�����ģʽ��RW λ��
  unsigned short f_flags;	// �ļ��򿪺Ϳ��Ƶı�־��
  unsigned short f_count;	// ��Ӧ�ļ�������ļ�������������
  struct m_inode *f_inode;	// ָ���Ӧi �ڵ㡣
  off_t f_pos;			// �ļ�λ�ã���дƫ��ֵ����
};

// �ڴ��д��̳�����ṹ��
struct super_block
{
  unsigned short s_ninodes;	// �ڵ�����
  unsigned short s_nzones;	// �߼�������
  unsigned short s_imap_blocks;	// i �ڵ�λͼ��ռ�õ����ݿ�����
  unsigned short s_zmap_blocks;	// �߼���λͼ��ռ�õ����ݿ�����
  unsigned short s_firstdatazone;	// ��һ�������߼���š�
  unsigned short s_log_zone_size;	// log(���ݿ���/�߼���)������2 Ϊ�ף���
  unsigned long s_max_size;	// �ļ���󳤶ȡ�
  unsigned short s_magic;	// �ļ�ϵͳħ����
/* These are only in memory */
  struct buffer_head *s_imap[8];	// i �ڵ�λͼ�����ָ������(ռ��8 �飬�ɱ�ʾ64M)��
  struct buffer_head *s_zmap[8];	// �߼���λͼ�����ָ�����飨ռ��8 �飩��
  unsigned short s_dev;		// ���������ڵ��豸�š�
  struct m_inode *s_isup;	// ����װ���ļ�ϵͳ��Ŀ¼��i �ڵ㡣(isup-super i)
  struct m_inode *s_imount;	// ����װ����i �ڵ㡣
  unsigned long s_time;		// �޸�ʱ�䡣
  struct task_struct *s_wait;	// �ȴ��ó�����Ľ��̡�
  unsigned char s_lock;		// ��������־��
  unsigned char s_rd_only;	// ֻ����־��
  unsigned char s_dirt;		// ���޸�(��)��־��
};

// �����ϳ�����ṹ������125-132 ����ȫһ����
struct d_super_block
{
  unsigned short s_ninodes;	// �ڵ�����
  unsigned short s_nzones;	// �߼�������
  unsigned short s_imap_blocks;	// i �ڵ�λͼ��ռ�õ����ݿ�����
  unsigned short s_zmap_blocks;	// �߼���λͼ��ռ�õ����ݿ�����
  unsigned short s_firstdatazone;	// ��һ�������߼��顣
  unsigned short s_log_zone_size;	// log(���ݿ���/�߼���)������2 Ϊ�ף���
  unsigned long s_max_size;	// �ļ���󳤶ȡ�
  unsigned short s_magic;	// �ļ�ϵͳħ����
};

// �ļ�Ŀ¼��ṹ��
struct dir_entry
{
  unsigned short inode;		// i �ڵ㡣
  char name[NAME_LEN];		// �ļ�����
};

extern struct m_inode inode_table[NR_INODE];	// ����i �ڵ�����飨32 ���
extern struct file file_table[NR_FILE];	// �ļ������飨64 ���
extern struct super_block super_block[NR_SUPER];	// ���������飨8 ���
extern struct buffer_head *start_buffer;	// ��������ʼ�ڴ�λ�á�
extern int nr_buffers;		// ���������

//// ���̲�������ԭ�͡�
// ����������������Ƿ�ı䡣
extern void check_disk_change (int dev);
// ���ָ�����������̸��������������̸������򷵻�1�����򷵻�0��
extern int floppy_change (unsigned int nr);
// ��������ָ������������ȴ���ʱ�䣨���õȴ���ʱ������
extern int ticks_to_floppy_on (unsigned int dev);
// ����ָ����������
extern void floppy_on (unsigned int dev);
// �ر�ָ����������������
extern void floppy_off (unsigned int dev);
//// �������ļ�ϵͳ���������õĺ���ԭ�͡�
// ��i �ڵ�ָ�����ļ���Ϊ0��
extern void truncate (struct m_inode *inode);
// ˢ��i �ڵ���Ϣ��
extern void sync_inodes (void);
// �ȴ�ָ����i �ڵ㡣
extern void wait_on (struct m_inode *inode);
// �߼���(���Σ����̿�)λͼ������ȡ���ݿ�block ���豸�϶�Ӧ���߼���š�
extern int bmap (struct m_inode *inode, int block);
// �������ݿ�block ���豸�϶�Ӧ���߼��飬���������豸�ϵ��߼���š�
extern int create_block (struct m_inode *inode, int block);
// ��ȡָ��·������i �ڵ�š�
extern struct m_inode *namei (const char *pathname);
// ����·����Ϊ���ļ�������׼����
extern int open_namei (const char *pathname, int flag, int mode,
		       struct m_inode **res_inode);
// �ͷ�һ��i �ڵ�(��д���豸)��
extern void iput (struct m_inode *inode);
// ���豸��ȡָ���ڵ�ŵ�һ��i �ڵ㡣
extern struct m_inode *iget (int dev, int nr);
// ��i �ڵ��(inode_table)�л�ȡһ������i �ڵ��
extern struct m_inode *get_empty_inode (void);
// ��ȡ������һ���ܵ��ڵ㡣����Ϊi �ڵ�ָ�루�����NULL ��ʧ�ܣ���
extern struct m_inode *get_pipe_inode (void);
// �ڹ�ϣ���в���ָ�������ݿ顣�����ҵ���Ļ���ͷָ�롣
extern struct buffer_head *get_hash_table (int dev, int block);
// ���豸��ȡָ���飨���Ȼ���hash ���в��ң���
extern struct buffer_head *getblk (int dev, int block);
// ��/д���ݿ顣
extern void ll_rw_block (int rw, struct buffer_head *bh);
// �ͷ�ָ������顣
extern void brelse (struct buffer_head *buf);
// ��ȡָ�������ݿ顣
extern struct buffer_head *bread (int dev, int block);
// ��4 �黺������ָ����ַ���ڴ��С�
extern void bread_page (unsigned long addr, int dev, int b[4]);
// ��ȡͷһ��ָ�������ݿ飬����Ǻ�����Ҫ���Ŀ顣
extern struct buffer_head *breada (int dev, int block, ...);
// ���豸dev ����һ�����̿飨���Σ��߼��飩�������߼����
extern int new_block (int dev);
// �ͷ��豸�������е��߼���(���Σ����̿�)block����λָ���߼���block ���߼���λͼ����λ��
extern void free_block (int dev, int block);
// Ϊ�豸dev ����һ����i �ڵ㣬����i �ڵ�š�
extern struct m_inode *new_inode (int dev);
// �ͷ�һ��i �ڵ㣨ɾ���ļ�ʱ����
extern void free_inode (struct m_inode *inode);
// ˢ��ָ���豸��������
extern int sync_dev (int dev);
// ��ȡָ���豸�ĳ����顣
extern struct super_block *get_super (int dev);
extern int ROOT_DEV;

// ��װ���ļ�ϵͳ��
extern void mount_root (void);

#endif
