/* passed
 *  linux/fs/fcntl.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#include <string.h>// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <errno.h>// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
									// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

#include <fcntl.h>// �ļ�����ͷ�ļ��������ļ������������Ĳ������Ƴ������ŵĶ��塣
#include <sys/stat.h>// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

extern int sys_close(int fd);// �ر��ļ�ϵͳ���á�(fs/open.c, 192)

//// �����ļ����(������)��
// ����fd �������Ƶ��ļ������arg ָ�����ļ��������С��ֵ��
// �������ļ����������롣
static int dupfd(unsigned int fd, unsigned int arg)
{
// ����ļ����ֵ����һ�����������ļ���NR_OPEN�����߸þ�����ļ��ṹ�����ڣ������
// ���س����벢�˳���
	if (fd >= NR_OPEN || !current->filp[fd])
		return -EBADF;
// ���ָ�����¾��ֵarg ���������ļ�������������س����벢�˳���
	if (arg >= NR_OPEN)
		return -EINVAL;
// �ڵ�ǰ���̵��ļ��ṹָ��������Ѱ�������Ŵ��ڵ���arg ����û��ʹ�õ��
	while (arg < NR_OPEN)
		if (current->filp[arg])
			arg++;
		else
			break;
// ����ҵ����¾��ֵarg ���������ļ�������������س����벢�˳���
	if (arg >= NR_OPEN)
		return -EMFILE;
// ��ִ��ʱ�رձ�־λͼ�и�λ�þ��λ��Ҳ��������exec()�ຯ��ʱ���رոþ����
	current->close_on_exec &= ~(1<<arg);
// ����ļ��ṹָ�����ԭ���fd ��ָ�룬�����ļ����ü�����1��
	(current->filp[arg] = current->filp[fd])->f_count++;
	return arg;		// �����µ��ļ������
}

//// �����ļ����ϵͳ���ú�����
// ����ָ���ļ����oldfd���¾��ֵ����newfd�����newfd �Ѿ��򿪣������ȹر�֮��
int sys_dup2(unsigned int oldfd, unsigned int newfd)
{
	sys_close(newfd);			// �����newfd �Ѿ��򿪣������ȹر�֮��
	return dupfd(oldfd,newfd);	// ���Ʋ������¾����
}

//// �����ļ����ϵͳ���ú�����
// ����ָ���ļ����oldfd���¾����ֵ�ǵ�ǰ��С��δ�þ����
int sys_dup(unsigned int fildes)
{
	return dupfd(fildes,0);
}

//// �ļ�����ϵͳ���ú�����
// ����fd ���ļ������cmd �ǲ�������(�μ�include/fcntl.h��23-30 ��)��
int sys_fcntl(unsigned int fd, unsigned int cmd, unsigned long arg)
{	
	struct file * filp;

// ����ļ����ֵ����һ�����������ļ���NR_OPEN�����߸þ�����ļ��ṹָ��Ϊ�գ������
// ���س����벢�˳���
	if (fd >= NR_OPEN || !(filp = current->filp[fd]))
		return -EBADF;
// ���ݲ�ͬ����cmd ���зֱ���
	switch (cmd) {
		case F_DUPFD:	// �����ļ������
			return dupfd(fd,arg);
		case F_GETFD:	// ȡ�ļ������ִ��ʱ�رձ�־��
			return (current->close_on_exec>>fd)&1;
		case F_SETFD:	// ���þ��ִ��ʱ�رձ�־��arg λ0 ��λ�����ã�����رա�
			if (arg&1)
				current->close_on_exec |= (1<<fd);
			else
				current->close_on_exec &= ~(1<<fd);
			return 0;
		case F_GETFL:	// ȡ�ļ�״̬��־�ͷ���ģʽ��
			return filp->f_flags;
		case F_SETFL:	// �����ļ�״̬�ͷ���ģʽ(����arg ������ӡ���������־)��
			filp->f_flags &= ~(O_APPEND | O_NONBLOCK);
			filp->f_flags |= arg & (O_APPEND | O_NONBLOCK);
			return 0;
		case F_GETLK:	case F_SETLK:	case F_SETLKW:	// δʵ�֡�
			return -1;
		default:
			return -1;
	}
}
