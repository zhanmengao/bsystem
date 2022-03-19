/* passed
 *  linux/fs/open.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#include <string.h>// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <errno.h>// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <fcntl.h>// �ļ�����ͷ�ļ��������ļ������������Ĳ������Ƴ������ŵĶ��塣
#include <sys/types.h>// ����ͷ�ļ��������˻�����ϵͳ�������͡�
#include <utime.h>// �û�ʱ��ͷ�ļ��������˷��ʺ��޸�ʱ��ṹ�Լ�utime()ԭ�͡�
#include <sys/stat.h>// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
						// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/tty.h>// tty ͷ�ļ����������й�tty_io������ͨ�ŷ���Ĳ�����������
#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

// ȡ�ļ�ϵͳ��Ϣϵͳ���ú�����
int sys_ustat(int dev, struct ustat * ubuf)
{
	return -ENOSYS;
}

//// �����ļ����ʺ��޸�ʱ�䡣
// ����filename ���ļ�����times �Ƿ��ʺ��޸�ʱ��ṹָ�롣
// ���times ָ�벻ΪNULL����ȡutimbuf �ṹ�е�ʱ����Ϣ�������ļ��ķ��ʺ��޸�ʱ�䡣���
// times ָ����NULL����ȡϵͳ��ǰʱ��������ָ���ļ��ķ��ʺ��޸�ʱ����
int sys_utime(char * filename, struct utimbuf * times)
{
	struct m_inode * inode;
	long actime,modtime;

// �����ļ���Ѱ�Ҷ�Ӧ��i �ڵ㣬���û���ҵ����򷵻س����롣
	if (!(inode=namei(filename)))
		return -ENOENT;
// ������ʺ��޸�ʱ�����ݽṹָ�벻ΪNULL����ӽṹ�ж�ȡ�û����õ�ʱ��ֵ��
	if (times) {
		actime = get_fs_long((unsigned long *) &times->actime);
		modtime = get_fs_long((unsigned long *) &times->modtime);
// ���򽫷��ʺ��޸�ʱ����Ϊ��ǰʱ�䡣
	} else
		actime = modtime = CURRENT_TIME;
// �޸�i �ڵ��еķ���ʱ���ֶκ��޸�ʱ���ֶΡ�
	inode->i_atime = actime;
	inode->i_mtime = modtime;
// ��i �ڵ����޸ı�־���ͷŸýڵ㣬������0��
	inode->i_dirt = 1;
	iput(inode);
	return 0;
}

/*
 * �ļ�����XXX�����Ǹ�����ʵ�û�id ������Ч�û�id��BSD ϵͳʹ������ʵ�û�id��
 * ��ʹ�õ��ÿ��Թ�setuid ����ʹ�á���ע��POSIX ��׼����ʹ����ʵ�û�ID��
 */
//// �����ļ��ķ���Ȩ�ޡ�
// ����filename ���ļ�����mode �������룬��R_OK(4)��W_OK(2)��X_OK(1)��F_OK(0)��ɡ�
// ��������������Ļ����򷵻�0�����򷵻س����롣
int sys_access(const char * filename,int mode)
{
	struct m_inode * inode;
	int res, i_mode;

// �������ɵ�3 λ��ɣ����������и߱���λ��
	mode &= 0007;
// ����ļ�����Ӧ��i �ڵ㲻���ڣ��򷵻س����롣
	if (!(inode=namei(filename)))
		return -EACCES;
// ȡ�ļ��������룬���ͷŸ�i �ڵ㡣
	i_mode = res = inode->i_mode & 0777;
	iput(inode);
// �����ǰ�����Ǹ��ļ�����������ȡ�ļ��������ԡ�
	if (current->uid == inode->i_uid)
		res >>= 6;
// ���������ǰ����������ļ�ͬ��һ�飬��ȡ�ļ������ԡ�
	else if (current->gid == inode->i_gid)
		res >>= 6;
// ����ļ����Ծ��в�ѯ������λ���������ɣ�����0��
	if ((res & 0007 & mode) == mode)
		return 0;
/*
 * XXX ��������������Ĳ��ԣ���Ϊ����ʵ������Ҫ������Ч�û�id ��
 * ��ʵ�û�id����ʱ�أ���Ȼ��ŵ���suser()�������������ȷʵҪ����
 * suser()����������Ҫ���ű����á� 
 */
// �����ǰ�û�id Ϊ0�������û�������������ִ��λ��0 ���ļ����Ա��κ��˷��ʣ��򷵻�0��
	if ((!current->uid) &&
	    (!(mode & 1) || (i_mode & 0111)))
		return 0;
	// ���򷵻س����롣
	return -EACCES;
}

//// �ı䵱ǰ����Ŀ¼ϵͳ���ú�����
// ����filename ��Ŀ¼����
// �����ɹ��򷵻�0�����򷵻س����롣
int sys_chdir(const char * filename)
{
	struct m_inode * inode;

// ����ļ�����Ӧ��i �ڵ㲻���ڣ��򷵻س����롣
	if (!(inode = namei(filename)))
		return -ENOENT;
// �����i �ڵ㲻��Ŀ¼��i �ڵ㣬���ͷŸýڵ㣬���س����롣
	if (!S_ISDIR(inode->i_mode)) {
		iput(inode);
		return -ENOTDIR;
	}
// �ͷŵ�ǰ����ԭ����Ŀ¼i �ڵ㣬��ָ������õĹ���Ŀ¼i �ڵ㡣����0��
	iput(current->pwd);
	current->pwd = inode;
	return (0);
}

//// �ı��Ŀ¼ϵͳ���ú�����
// ��ָ����·������Ϊ��Ŀ¼'/'��
// ��������ɹ��򷵻�0�����򷵻س����롣
int sys_chroot(const char * filename)
{
	struct m_inode * inode;

// ����ļ�����Ӧ��i �ڵ㲻���ڣ��򷵻س����롣
	if (!(inode=namei(filename)))
		return -ENOENT;
// �����i �ڵ㲻��Ŀ¼��i �ڵ㣬���ͷŸýڵ㣬���س����롣
	if (!S_ISDIR(inode->i_mode)) {
		iput(inode);
		return -ENOTDIR;
	}
// �ͷŵ�ǰ���̵ĸ�Ŀ¼i �ڵ㣬������Ϊ����ָ��Ŀ¼����i �ڵ㣬����0��
	iput(current->root);
	current->root = inode;
	return (0);
}

//// �޸��ļ�����ϵͳ���ú�����
// ����filename ���ļ�����mode ���µ��ļ����ԡ�
// �������ɹ��򷵻�0�����򷵻س����롣
int sys_chmod(const char * filename,int mode)
{
	struct m_inode * inode;

// ����ļ�����Ӧ��i �ڵ㲻���ڣ��򷵻س����롣
	if (!(inode=namei(filename)))
		return -ENOENT;
// �����ǰ���̵���Ч�û�id �������ļ�i �ڵ���û�id�����ҵ�ǰ���̲��ǳ����û������ͷŸ�
// �ļ�i �ڵ㣬���س����롣
	if ((current->euid != inode->i_uid) && !suser()) {
		iput(inode);
		return -EACCES;
	}
// ��������i �ڵ���ļ����ԣ����ø�i �ڵ����޸ı�־���ͷŸ�i �ڵ㣬����0��
	inode->i_mode = (mode & 07777) | (inode->i_mode & ~07777);
	inode->i_dirt = 1;
	iput(inode);
	return 0;
}

//// �޸��ļ�����ϵͳ���ú�����
// ����filename ���ļ�����uid ���û���ʶ��(�û�id)��gid ����id��
// �������ɹ��򷵻�0�����򷵻س����롣
int sys_chown(const char * filename,int uid,int gid)
{
	struct m_inode * inode;

// ����ļ�����Ӧ��i �ڵ㲻���ڣ��򷵻س����롣
	if (!(inode=namei(filename)))
		return -ENOENT;
// ����ǰ���̲��ǳ����û������ͷŸ�i �ڵ㣬���س����롣
	if (!suser()) {
		iput(inode);
		return -EACCES;
	}
// �����ļ���Ӧi �ڵ���û�id ����id������i �ڵ��Ѿ��޸ı�־���ͷŸ�i �ڵ㣬����0��
	inode->i_uid=uid;
	inode->i_gid=gid;
	inode->i_dirt=1;
	iput(inode);
	return 0;
}

//// �򿪣��򴴽����ļ�ϵͳ���ú�����
// ����filename ���ļ�����flag �Ǵ��ļ���־��ֻ��O_RDONLY��ֻдO_WRONLY ���дO_RDWR��
// �Լ�O_CREAT��O_EXCL��O_APPEND ������һЩ��־����ϣ���������������һ�����ļ�����mode
// ����ָ��ʹ���ļ���������ԣ���Щ������S_IRWXU(�ļ��������ж���д��ִ��Ȩ��)��S_IRUSR
// (�û����ж��ļ�Ȩ��)��S_IRWXG(���Ա���ж���д��ִ��Ȩ��)�ȵȡ������´������ļ�����Щ
// ����ֻӦ���ڽ������ļ��ķ��ʣ�������ֻ���ļ��Ĵ򿪵���Ҳ������һ���ɶ�д���ļ������
// �������ɹ��򷵻��ļ����(�ļ�������)�����򷵻س����롣(�μ�sys/stat.h, fcntl.h)
int sys_open(const char * filename,int flag,int mode)
{
	struct m_inode * inode;
	struct file * f;
	int i,fd;

// ���û����õ�ģʽ����̵�ģʽ���������룬������ɵ��ļ�ģʽ��
	mode &= 0777 & ~current->umask;
// �������̽ṹ���ļ��ṹָ�����飬����һ����������Ѿ�û�п�����򷵻س����롣
	for(fd=0 ; fd<NR_OPEN ; fd++)
		if (!current->filp[fd])
			break;
	if (fd>=NR_OPEN)
		return -EINVAL;
// ����ִ��ʱ�ر��ļ����λͼ����λ��Ӧ����λ��
	current->close_on_exec &= ~(1<<fd);
// ��f ָ���ļ������鿪ʼ�������������ļ��ṹ��(������ü���Ϊ0 ����)�����Ѿ�û�п���
// �ļ���ṹ��򷵻س����롣
	f=0+file_table;
	for (i=0 ; i<NR_FILE ; i++,f++)
		if (!f->f_count) break;
	if (i>=NR_FILE)
		return -EINVAL;
// �ý��̵Ķ�Ӧ�ļ�������ļ��ṹָ��ָ�����������ļ��ṹ�����������ü�������1��
	(current->filp[fd]=f)->f_count++;
// ���ú���ִ�д򿪲�����������ֵС��0����˵�������ͷŸ����뵽���ļ��ṹ�����س����롣
	if ((i=open_namei(filename,flag,mode,&inode))<0) {
		current->filp[fd]=NULL;
		f->f_count=0;
		return i;
	}
/* ttys ��Щ���⣨ttyxx ����==4��tty ����==5��*/
// ������ַ��豸�ļ�����ô����豸����4 �Ļ��������õ�ǰ���̵�tty ��Ϊ��i �ڵ�����豸�š�
// �����õ�ǰ����tty ��Ӧ��tty ����ĸ�������ŵ��ڽ��̵ĸ�������š�
	if (S_ISCHR(inode->i_mode))
		if (MAJOR(inode->i_zone[0])==4) {
			if (current->leader && current->tty<0) {
				current->tty = MINOR(inode->i_zone[0]);
				tty_table[current->tty].pgrp = current->pgrp;
			}
// ����������ַ��ļ��豸����5 �Ļ�������ǰ����û��tty����˵�������ͷ�i �ڵ�����뵽��
// �ļ��ṹ�����س����롣
		} else if (MAJOR(inode->i_zone[0])==5)
			if (current->tty<0) {
				iput(inode);
				current->filp[fd]=NULL;
				f->f_count=0;
				return -EPERM;
			}
/* ͬ�����ڿ��豸�ļ�����Ҫ�����Ƭ�Ƿ񱻸��� */
// ����򿪵��ǿ��豸�ļ���������Ƭ�Ƿ����������������Ҫ�Ǹ��ٻ����ж�Ӧ���豸������
// �����ʧЧ��
	if (S_ISBLK(inode->i_mode))
		check_disk_change(inode->i_zone[0]);
// ��ʼ���ļ��ṹ�����ļ��ṹ���Ժͱ�־���þ�����ü���Ϊ1������i �ڵ��ֶΣ��ļ���дָ��
// ��ʼ��Ϊ0�������ļ������
	f->f_mode = inode->i_mode;
	f->f_flags = flag;
	f->f_count = 1;
	f->f_inode = inode;
	f->f_pos = 0;
	return (fd);
}

//// �����ļ�ϵͳ���ú�����
// ����pathname ��·������mode �������sys_open()������ͬ��
// �ɹ��򷵻��ļ���������򷵻س����롣
int sys_creat(const char * pathname, int mode)
{
	return sys_open(pathname, O_CREAT | O_TRUNC, mode);
}

// �ر��ļ�ϵͳ���ú�����
// ����fd ���ļ������
// �ɹ��򷵻�0�����򷵻س����롣
int sys_close(unsigned int fd)
{	
	struct file * filp;

// ���ļ����ֵ���ڳ���ͬʱ�ܴ򿪵��ļ������򷵻س����롣
	if (fd >= NR_OPEN)
		return -EINVAL;
// ��λ���̵�ִ��ʱ�ر��ļ����λͼ��Ӧλ��
	current->close_on_exec &= ~(1<<fd);
// �����ļ������Ӧ���ļ��ṹָ����NULL���򷵻س����롣
	if (!(filp = current->filp[fd]))
		return -EINVAL;
// �ø��ļ�������ļ��ṹָ��ΪNULL��
	current->filp[fd] = NULL;
// ���ڹر��ļ�֮ǰ����Ӧ�ļ��ṹ�еľ�����ü����Ѿ�Ϊ0����˵���ں˳���������
	if (filp->f_count == 0)
		panic("Close: file count is 0");
// ���򽫶�Ӧ�ļ��ṹ�ľ�����ü�����1���������Ϊ0���򷵻�0���ɹ��������ѵ���0��˵����
// �ļ��Ѿ�û�о�����ã����ͷŸ��ļ�i �ڵ㣬����0��
	if (--filp->f_count)
		return (0);
	iput(filp->f_inode);
	return (0);
}
