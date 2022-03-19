/* passed
* linux/fs/stat.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <sys/stat.h>		// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

#include <linux/fs.h>		// �ļ�ϵͳͷ�ļ��������ļ���ṹ��file,buffer_head,m_inode �ȣ���
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

//// �����ļ�״̬��Ϣ��
// ����inode ���ļ���Ӧ��i �ڵ㣬statbuf ��stat �ļ�״̬�ṹָ�룬���ڴ��ȡ�õ�״̬��Ϣ��
static void
cp_stat (struct m_inode *inode, struct stat *statbuf)
{
	struct stat tmp;
	int i;

// ������֤(�����)������ݵ��ڴ�ռ䡣
	verify_area (statbuf, sizeof (*statbuf));
// Ȼ����ʱ������Ӧ�ڵ��ϵ���Ϣ��
	tmp.st_dev = inode->i_dev;	// �ļ����ڵ��豸�š�
	tmp.st_ino = inode->i_num;	// �ļ�i �ڵ�š�
	tmp.st_mode = inode->i_mode;	// �ļ����ԡ�
	tmp.st_nlink = inode->i_nlinks;	// �ļ�����������
	tmp.st_uid = inode->i_uid;	// �ļ����û�id��
	tmp.st_gid = inode->i_gid;	// �ļ�����id��
	tmp.st_rdev = inode->i_zone[0];	// �豸��(����ļ���������ַ��ļ�����ļ�)��
	tmp.st_size = inode->i_size;	// �ļ���С���ֽ�����������ļ��ǳ����ļ�����
	tmp.st_atime = inode->i_atime;	// ������ʱ�䡣
	tmp.st_mtime = inode->i_mtime;	// ����޸�ʱ�䡣
	tmp.st_ctime = inode->i_ctime;	// ���ڵ��޸�ʱ�䡣
// �����Щ״̬��Ϣ���Ƶ��û��������С�
	for (i = 0; i < sizeof (tmp); i++)
		put_fs_byte (((char *) &tmp)[i], &((char *) statbuf)[i]);
}

//// �ļ�״̬ϵͳ���ú��� - �����ļ�����ȡ�ļ�״̬��Ϣ��
// ����filename ��ָ�����ļ�����statbuf �Ǵ��״̬��Ϣ�Ļ�����ָ�롣
// ����0���������򷵻س����롣
int
sys_stat (char *filename, struct stat *statbuf)
{
	struct m_inode *inode;

// ���ȸ����ļ����ҳ���Ӧ��i �ڵ㣬�������򷵻ش����롣
	if (!(inode = namei (filename)))
		return -ENOENT;
// ��i �ڵ��ϵ��ļ�״̬��Ϣ���Ƶ��û��������У����ͷŸ�i �ڵ㡣
	cp_stat (inode, statbuf);
	iput (inode);
	return 0;
}

//// �ļ�״̬ϵͳ���� - �����ļ������ȡ�ļ�״̬��Ϣ��
// ����fd ��ָ���ļ��ľ��(������)��statbuf �Ǵ��״̬��Ϣ�Ļ�����ָ�롣
// ����0���������򷵻س����롣
int
sys_fstat (unsigned int fd, struct stat *statbuf)
{
	struct file *f;
	struct m_inode *inode;

// ����ļ����ֵ����һ�����������ļ���NR_OPEN�����߸þ�����ļ��ṹָ��Ϊ�գ�����
// ��Ӧ�ļ��ṹ��i �ڵ��ֶ�Ϊ�գ���������س����벢�˳���
	if (fd >= NR_OPEN || !(f = current->filp[fd]) || !(inode = f->f_inode))
		return -EBADF;
// ��i �ڵ��ϵ��ļ�״̬��Ϣ���Ƶ��û��������С�
	cp_stat (inode, statbuf);
	return 0;
}
