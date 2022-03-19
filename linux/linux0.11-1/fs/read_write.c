/* passed
* linux/fs/read_write.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

#include <sys/stat.h>		// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����
#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <sys/types.h>		// ����ͷ�ļ��������˻�����ϵͳ�������͡�

#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

// �ַ��豸��д������
extern int rw_char (int rw, int dev, char *buf, int count, off_t * pos);
// ���ܵ�����������
extern int read_pipe (struct m_inode *inode, char *buf, int count);
// д�ܵ�����������
extern int write_pipe (struct m_inode *inode, char *buf, int count);
// ���豸������������
extern int block_read (int dev, off_t * pos, char *buf, int count);
// ���豸д����������
extern int block_write (int dev, off_t * pos, char *buf, int count);
// ���ļ�����������
extern int file_read (struct m_inode *inode, struct file *filp,
		      char *buf, int count);
// д�ļ�����������
extern int file_write (struct m_inode *inode, struct file *filp,
		       char *buf, int count);

//// �ض�λ�ļ���дָ��ϵͳ���ú�����
// ����fd ���ļ������offset ���µ��ļ���дָ��ƫ��ֵ��origin ��ƫ�Ƶ���ʼλ�ã���SEEK_SET
// (0�����ļ���ʼ��)��SEEK_CUR(1���ӵ�ǰ��дλ��)��SEEK_END(2�����ļ�β��)����֮һ��
int sys_lseek (unsigned int fd, off_t offset, int origin)
{
	struct file *file;
	int tmp;

// ����ļ����ֵ���ڳ��������ļ���NR_OPEN(20)�����߸þ�����ļ��ṹָ��Ϊ�գ�����
// ��Ӧ�ļ��ṹ��i �ڵ��ֶ�Ϊ�գ�����ָ���豸�ļ�ָ���ǲ��ɶ�λ�ģ��򷵻س����벢�˳���
	if (fd >= NR_OPEN || !(file = current->filp[fd]) || !(file->f_inode)
			|| !IS_SEEKABLE (MAJOR (file->f_inode->i_dev)))
		return -EBADF;
// ����ļ���Ӧ��i �ڵ��ǹܵ��ڵ㣬�򷵻س����룬�˳����ܵ�ͷβָ�벻�������ƶ���
	if (file->f_inode->i_pipe)
		return -ESPIPE;
// �������õĶ�λ��־���ֱ����¶�λ�ļ���дָ�롣
	switch (origin)
	{
// origin = SEEK_SET��Ҫ�����ļ���ʼ����Ϊԭ�������ļ���дָ�롣��ƫ��ֵС���㣬�����
// �ش����롣���������ļ���дָ�����offset��
	case 0:
		if (offset < 0)
			return -EINVAL;
		file->f_pos = offset;
		break;
// origin = SEEK_CUR��Ҫ�����ļ���ǰ��дָ�봦��Ϊԭ���ض�λ��дָ�롣����ļ���ǰָ���
// ��ƫ��ֵС��0���򷵻س������˳��������ڵ�ǰ��дָ���ϼ���ƫ��ֵ��
	case 1:
		if (file->f_pos + offset < 0)
			return -EINVAL;
		file->f_pos += offset;
		break;
// origin = SEEK_END��Ҫ�����ļ�ĩβ��Ϊԭ���ض�λ��дָ�롣��ʱ���ļ���С����ƫ��ֵС����
// �򷵻س������˳��������ض�λ��дָ��Ϊ�ļ����ȼ���ƫ��ֵ��
	case 2:
		if ((tmp = file->f_inode->i_size + offset) < 0)
			return -EINVAL;
		file->f_pos = tmp;
		break;
// origin ���ó������س������˳���
	default:
		return -EINVAL;
	}
	return file->f_pos;		// �����ض�λ����ļ���дָ��ֵ��
}

//// ���ļ�ϵͳ���ú�����
// ����fd ���ļ������buf �ǻ�������count �������ֽ�����
int sys_read (unsigned int fd, char *buf, int count)
{
	struct file *file;
	struct m_inode *inode;

// ����ļ����ֵ���ڳ��������ļ���NR_OPEN��������Ҫ��ȡ���ֽڼ���ֵС��0�����߸þ��
// ���ļ��ṹָ��Ϊ�գ��򷵻س����벢�˳���
	if (fd >= NR_OPEN || count < 0 || !(file = current->filp[fd]))
		return -EINVAL;
// �����ȡ���ֽ���count ����0���򷵻�0���˳�
	if (!count)
		return 0;
// ��֤������ݵĻ������ڴ����ơ�
	verify_area (buf, count);
// ȡ�ļ���Ӧ��i �ڵ㡣���ǹܵ��ļ��������Ƕ��ܵ��ļ�ģʽ������ж��ܵ����������ɹ��򷵻�
// ��ȡ���ֽ��������򷵻س����룬�˳���
	inode = file->f_inode;
	if (inode->i_pipe)
		return (file->f_mode & 1) ? read_pipe (inode, buf, count) : -EIO;
// ������ַ����ļ�������ж��ַ��豸���������ض�ȡ���ַ�����
	if (S_ISCHR (inode->i_mode))
		return rw_char (READ, inode->i_zone[0], buf, count, &file->f_pos);
// ����ǿ��豸�ļ�����ִ�п��豸�������������ض�ȡ���ֽ�����
	if (S_ISBLK (inode->i_mode))
		return block_read (inode->i_zone[0], &file->f_pos, buf, count);
// �����Ŀ¼�ļ������ǳ����ļ�����������֤��ȡ��count ����Ч�Բ����е���������ȡ�ֽ�������
// �ļ���ǰ��дָ��ֵ�����ļ���С�����������ö�ȡ�ֽ���Ϊ�ļ�����-��ǰ��дָ��ֵ������ȡ��
// ����0���򷵻�0 �˳�����Ȼ��ִ���ļ������������ض�ȡ���ֽ������˳���
	if (S_ISDIR (inode->i_mode) || S_ISREG (inode->i_mode))
	{
		if (count + file->f_pos > inode->i_size)
			count = inode->i_size - file->f_pos;
		if (count <= 0)
			return 0;
		return file_read (inode, file, buf, count);
	}
// �����ӡ�ڵ��ļ����ԣ������س������˳���
	printk ("(Read)inode->i_mode=%06o\n\r", inode->i_mode);
	return -EINVAL;
}

int sys_write (unsigned int fd, char *buf, int count)
{
	struct file *file;
	struct m_inode *inode;

// ����ļ����ֵ���ڳ��������ļ���NR_OPEN��������Ҫд����ֽڼ���С��0�����߸þ��
// ���ļ��ṹָ��Ϊ�գ��򷵻س����벢�˳���
	if (fd >= NR_OPEN || count < 0 || !(file = current->filp[fd]))
		return -EINVAL;
// �����ȡ���ֽ���count ����0���򷵻�0���˳�
	if (!count)
		return 0;
// ȡ�ļ���Ӧ��i �ڵ㡣���ǹܵ��ļ���������д�ܵ��ļ�ģʽ�������д�ܵ����������ɹ��򷵻�
// д����ֽ��������򷵻س����룬�˳���
	inode = file->f_inode;
	if (inode->i_pipe)
		return (file->f_mode & 2) ? write_pipe (inode, buf, count) : -EIO;
// ������ַ����ļ��������д�ַ��豸����������д����ַ������˳���
	if (S_ISCHR (inode->i_mode))
		return rw_char (WRITE, inode->i_zone[0], buf, count, &file->f_pos);
// ����ǿ��豸�ļ�������п��豸д������������д����ֽ������˳���
	if (S_ISBLK (inode->i_mode))
		return block_write (inode->i_zone[0], &file->f_pos, buf, count);
// ���ǳ����ļ�����ִ���ļ�д������������д����ֽ������˳���
	if (S_ISREG (inode->i_mode))
		return file_write (inode, file, buf, count);
// ������ʾ��Ӧ�ڵ���ļ�ģʽ�����س����룬�˳���
	printk ("(Write)inode->i_mode=%06o\n\r", inode->i_mode);
	return -EINVAL;
}
