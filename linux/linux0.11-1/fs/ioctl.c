/* passed
 *  linux/fs/ioctl.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#include <string.h>// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <errno.h>// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <sys/stat.h>// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
									// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣

// �ն�ioctl(chr_drv/tty_ioctl.c, 115)��
extern int tty_ioctl(int dev, int cmd, int arg);

// ���������������(ioctl)����ָ�롣
typedef int (*ioctl_ptr)(int dev,int cmd,int arg);

// ����ϵͳ���豸������
#define NRDEVS ((sizeof (ioctl_table))/(sizeof (ioctl_ptr)))

// ioctl ��������ָ���
static ioctl_ptr ioctl_table[]={
	NULL,		/* nodev */
	NULL,		/* /dev/mem */
	NULL,		/* /dev/fd */
	NULL,		/* /dev/hd */
	tty_ioctl,	/* /dev/ttyx */
	tty_ioctl,	/* /dev/tty */
	NULL,		/* /dev/lp */
	NULL		/* named pipes */
};	

//// ϵͳ���ú���- ����������ƺ�����
// ������fd - �ļ���������cmd - �����룻arg - ������
// ���أ��ɹ��򷵻�0�����򷵻س����롣
int sys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg)
{	
	struct file * filp;
	int dev,mode;

// ����ļ������������ɴ򿪵��ļ��������߶�Ӧ���������ļ��ṹָ��Ϊ�գ��򷵻س����룬�˳���
	if (fd >= NR_OPEN || !(filp = current->filp[fd]))
		return -EBADF;
// ȡ��Ӧ�ļ������ԡ�������ļ������ַ��ļ���Ҳ���ǿ��豸�ļ����򷵻س����룬�˳���
	mode=filp->f_inode->i_mode;
	if (!S_ISCHR(mode) && !S_ISBLK(mode))
		return -EINVAL;
// ���ַ�����豸�ļ���i �ڵ���ȡ�豸�š�����豸�Ŵ���ϵͳ���е��豸�����򷵻س���š�
	dev = filp->f_inode->i_zone[0];
	if (MAJOR(dev) >= NRDEVS)
		return -ENODEV;
// ������豸��ioctl ����ָ�����û�ж�Ӧ�������򷵻س����롣
	if (!ioctl_table[MAJOR(dev)])
		return -ENOTTY;
// ���򷵻�ʵ��ioctl ���������룬�ɹ��򷵻�0�����򷵻س����롣
	return ioctl_table[MAJOR(dev)](dev,cmd,arg);
}
