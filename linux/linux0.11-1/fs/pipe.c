/* passed
 *  linux/fs/pipe.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#include <signal.h>// �ź�ͷ�ļ��������źŷ��ų������źŽṹ�Լ��źŲ�������ԭ�͡�

#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
									// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/mm.h>	/* ʹ�����е�get_free_page */
									// �ڴ����ͷ�ļ�������ҳ���С�����һЩҳ���ͷź���ԭ�͡�
#include <asm/segment.h>// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

//// �ܵ�������������
// ����inode �ǹܵ���Ӧ��i �ڵ㣬buf �����ݻ�����ָ�룬count �Ƕ�ȡ���ֽ�����
int read_pipe(struct m_inode * inode, char * buf, int count)
{
	int chars, size, read = 0;

// ������ȡ���ֽڼ���ֵcount ����0����ѭ��ִ�����²�����
	while (count>0) {
// ����ǰ�ܵ���û������(size=0)�����ѵȴ��ýڵ�Ľ��̣������û��д�ܵ��ߣ��򷵻��Ѷ�
// �ֽ������˳��������ڸ�i �ڵ���˯�ߣ��ȴ���Ϣ��
		while (!(size=PIPE_SIZE(*inode))) {
			wake_up(&inode->i_wait);
			if (inode->i_count != 2) /* are there any writers? */
				return read;
			sleep_on(&inode->i_wait);
		}
// ȡ�ܵ�β��������ĩ�˵��ֽ���chars���������ڻ���Ҫ��ȡ���ֽ���count�����������count��
// ���chars ���ڵ�ǰ�ܵ��к������ݵĳ���size�����������size��
		chars = PAGE_SIZE-PIPE_TAIL(*inode);
		if (chars > count)
			chars = count;
		if (chars > size)
			chars = size;
// ���ֽڼ�����ȥ�˴οɶ����ֽ���chars�����ۼ��Ѷ��ֽ�����
		count -= chars;
		read += chars;
// ��size ָ��ܵ�β����������ǰ�ܵ�βָ�루ǰ��chars �ֽڣ���
		size = PIPE_TAIL(*inode);
		PIPE_TAIL(*inode) += chars;
		PIPE_TAIL(*inode) &= (PAGE_SIZE-1);
// ���ܵ��е����ݸ��Ƶ��û��������С����ڹܵ�i �ڵ㣬��i_size �ֶ����ǹܵ������ָ�롣
		while (chars-->0)
			put_fs_byte(((char *)inode->i_size)[size++],buf++);
	}
// ���ѵȴ��ùܵ�i �ڵ�Ľ��̣������ض�ȡ���ֽ�����
	wake_up(&inode->i_wait);
	return read;
}
	
//// �ܵ�д����������
// ����inode �ǹܵ���Ӧ��i �ڵ㣬buf �����ݻ�����ָ�룬count �ǽ�д��ܵ����ֽ�����
int write_pipe(struct m_inode * inode, char * buf, int count)
{
	int chars, size, written = 0;

// ����д����ֽڼ���ֵcount ������0����ѭ��ִ�����²�����
	while (count>0) {
// ����ǰ�ܵ���û���Ѿ�����(size=0)�����ѵȴ��ýڵ�Ľ��̣������û�ж��ܵ��ߣ��������
// ����SIGPIPE �źţ���������д����ֽ������˳�����д��0 �ֽڣ��򷵻�-1�������ڸ�i �ڵ���
// ˯�ߣ��ȴ��ܵ��ڳ��ռ䡣
		while (!(size=(PAGE_SIZE-1)-PIPE_SIZE(*inode))) {
			wake_up(&inode->i_wait);
			if (inode->i_count != 2) { /* no readers */
				current->signal |= (1<<(SIGPIPE-1));
				return written?written:-1;
			}
			sleep_on(&inode->i_wait);
		}
// ȡ�ܵ�ͷ����������ĩ�˿ռ��ֽ���chars���������ڻ���Ҫд����ֽ���count�����������
// count�����chars ���ڵ�ǰ�ܵ��п��пռ䳤��size�����������size��
		chars = PAGE_SIZE-PIPE_HEAD(*inode);
		if (chars > count)
			chars = count;
		if (chars > size)
			chars = size;
// д���ֽڼ�����ȥ�˴ο�д����ֽ���chars�����ۼ���д�ֽ�����written��
		count -= chars;
		written += chars;
// ��size ָ��ܵ�����ͷ����������ǰ�ܵ�����ͷ��ָ�루ǰ��chars �ֽڣ���
		size = PIPE_HEAD(*inode);
		PIPE_HEAD(*inode) += chars;
		PIPE_HEAD(*inode) &= (PAGE_SIZE-1);
// ���û�����������chars ���ֽڵ��ܵ��С����ڹܵ�i �ڵ㣬��i_size �ֶ����ǹܵ������ָ�롣
		while (chars-->0)
			((char *)inode->i_size)[size++]=get_fs_byte(buf++);
	}
// ���ѵȴ���i �ڵ�Ľ��̣�������д����ֽ������˳���
	wake_up(&inode->i_wait);
	return written;
}

//// �����ܵ�ϵͳ���ú�����
// ��fildes ��ָ�������д���һ���ļ����(������)������ļ����ָ��һ�ܵ�i �ڵ㡣fildes[0]
// ���ڶ��ܵ������ݣ�fildes[1]������ܵ���д�����ݡ�
// �ɹ�ʱ����0������ʱ����-1��
int sys_pipe(unsigned long * fildes)
{
	struct m_inode * inode;
	struct file * f[2];
	int fd[2];
	int i,j;

// ��ϵͳ�ļ�����ȡ������������ü����ֶ�Ϊ0 ��������ֱ��������ü���Ϊ1��
	j=0;
	for(i=0;j<2 && i<NR_FILE;i++)
		if (!file_table[i].f_count)
			(f[j++]=i+file_table)->f_count++;
// ���ֻ��һ����������ͷŸ���(���ü�����λ)��
	if (j==1)
		f[0]->f_count=0;
// ���û���ҵ�����������򷵻�-1��
	if (j<2)
		return -1;
// �������ȡ�õ������ļ��ṹ��ֱ����һ�ļ��������ʹ���̵��ļ��ṹָ��ֱ�ָ��������
// �ļ��ṹ��
	j=0;
	for(i=0;j<2 && i<NR_OPEN;i++)
		if (!current->filp[i]) {
			current->filp[ fd[j]=i ] = f[j];
			j++;
		}
// ���ֻ��һ�������ļ���������ͷŸþ����
	if (j==1)
		current->filp[fd[0]]=NULL;
// ���û���ҵ��������о�������ͷ������ȡ�������ļ��ṹ���λ���ü���ֵ����������-1��
	if (j<2) {
		f[0]->f_count=f[1]->f_count=0;
		return -1;
	}
// ����ܵ�i �ڵ㣬��Ϊ�ܵ����仺������1 ҳ�ڴ棩��������ɹ�������Ӧ�ͷ������ļ��������
// ���ṹ�������-1��
	if (!(inode=get_pipe_inode())) {
		current->filp[fd[0]] =
			current->filp[fd[1]] = NULL;
		f[0]->f_count = f[1]->f_count = 0;
		return -1;
	}
// ��ʼ�������ļ��ṹ����ָ��ͬһ��i �ڵ㣬��дָ�붼���㡣��1 ���ļ��ṹ���ļ�ģʽ��Ϊ����
// ��2 ���ļ��ṹ���ļ�ģʽ��Ϊд��
	f[0]->f_inode = f[1]->f_inode = inode;
	f[0]->f_pos = f[1]->f_pos = 0;
	f[0]->f_mode = 1;		/* read */
	f[1]->f_mode = 2;		/* write */
// ���ļ�������鸴�Ƶ���Ӧ���û������У�������0���˳���
	put_fs_long(fd[0],0+fildes);
	put_fs_long(fd[1],1+fildes);
	return 0;
}
