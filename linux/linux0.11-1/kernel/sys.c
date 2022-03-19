/* passed
* linux/kernel/sys.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��

#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/tty.h>		// tty ͷ�ļ����������й�tty_io������ͨ�ŷ���Ĳ�����������
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����
#include <sys/times.h>		// �����˽���������ʱ��Ľṹtms �Լ�times()����ԭ�͡�
#include <sys/utsname.h>	// ϵͳ���ƽṹͷ�ļ���

// �������ں�ʱ�䡣
int sys_ftime ()
{
	return -ENOSYS;
}

//
int sys_break ()
{
	return -ENOSYS;
}

// ���ڵ�ǰ���̶��ӽ��̽��е���(degugging)��
int sys_ptrace ()
{
	return -ENOSYS;
}

// �ı䲢��ӡ�ն������á�
int sys_stty ()
{
	return -ENOSYS;
}

// ȡ�ն���������Ϣ��
int sys_gtty ()
{
	return -ENOSYS;
}

// �޸��ļ�����
int sys_rename ()
{
	return -ENOSYS;
}

//
int sys_prof ()
{
	return -ENOSYS;
}

// ���õ�ǰ�����ʵ���Լ�/������Ч��ID��gid�����������û�г����û���Ȩ��
// ��ôֻ�ܻ�����ʵ����ID ����Ч��ID�����������г����û���Ȩ����������������Ч�ĺ�ʵ��
// ����ID��������gid��saved gid�������ó�����Чgid ֵͬ��
int sys_setregid (int rgid, int egid)
{
	if (rgid > 0)
	{
		if ((current->gid == rgid) || suser ())
			current->gid = rgid;
		else
			return (-EPERM);
	}
	if (egid > 0)
	{
		if ((current->gid == egid) || (current->egid == egid) || 
								(current->sgid == egid) || suser ())
			current->egid = egid;
		else
			return (-EPERM);
    }
	return 0;
}

// ���ý������(gid)���������û�г����û���Ȩ��������ʹ��setgid()������Чgid
// ��effective gid������Ϊ���䱣��gid(saved gid)����ʵ��gid(real gid)�����������
// �����û���Ȩ����ʵ��gid����Чgid �ͱ���gid �������óɲ���ָ����gid��
int sys_setgid (int gid)
{
	return (sys_setregid (gid, gid));
}

// �򿪻�رս��̼��ʹ��ܡ�
int sys_acct ()
{
	return -ENOSYS;
}

// ӳ�����������ڴ浽���̵������ַ�ռ䡣
int sys_phys ()
{
	return -ENOSYS;
}

int sys_lock ()
{
	return -ENOSYS;
}

int sys_mpx ()
{
	return -ENOSYS;
}

int sys_ulimit ()
{
	return -ENOSYS;
}

// ���ش�1970 ��1 ��1 ��00:00:00 GMT ��ʼ��ʱ��ʱ��ֵ���룩�����tloc ��Ϊnull����ʱ��ֵ
// Ҳ�洢�����
int sys_time (long *tloc)
{
	int i;

	i = CURRENT_TIME;
	if (tloc)
	{
		verify_area (tloc, 4);	// ��֤�ڴ������Ƿ񹻣�������4 �ֽڣ���
		put_fs_long (i, (unsigned long *) tloc);	// Ҳ�����û����ݶ�tloc ����
	}
	return i;
}

/*
* Unprivileged users may change the real user id to the effective uid
* or vice versa.
*/
/*
* ����Ȩ���û����Լ�ʵ���û���ʶ��(real uid)�ĳ���Ч�û���ʶ��(effective uid)����֮ҲȻ��
*/
// ���������ʵ���Լ�/������Ч�û�ID��uid�����������û�г����û���Ȩ����ôֻ�ܻ�����
// ʵ���û�ID ����Ч�û�ID�����������г����û���Ȩ����������������Ч�ĺ�ʵ�ʵ��û�ID��
// ������uid��saved uid�������ó�����Чuid ֵͬ��
int sys_setreuid (int ruid, int euid)
{
	int old_ruid = current->uid;

	if (ruid > 0)
	{
		if ((current->euid == ruid) || (old_ruid == ruid) || suser ())
			current->uid = ruid;
		else
			return (-EPERM);
	}
	if (euid > 0)
	{
		if ((old_ruid == euid) || (current->euid == euid) || suser ())
			current->euid = euid;
		else
		{
			current->uid = old_ruid;
			return (-EPERM);
		}
	}
	return 0;
}

// ���������û���(uid)���������û�г����û���Ȩ��������ʹ��setuid()������Чuid
// ��effective uid�����ó��䱣��uid(saved uid)����ʵ��uid(real uid)�����������
// �����û���Ȩ����ʵ��uid����Чuid �ͱ���uid �������óɲ���ָ����uid��
int sys_setuid (int uid)
{
	return (sys_setreuid (uid, uid));
}

// ����ϵͳʱ������ڡ�����tptr �Ǵ�1970 ��1 ��1 ��00:00:00 GMT ��ʼ��ʱ��ʱ��ֵ���룩��
// ���ý��̱�����г����û�Ȩ�ޡ�
int sys_stime (long *tptr)
{
	if (!suser ())		// ������ǳ����û�������أ���ɣ���
		return -EPERM;
	startup_time = get_fs_long ((unsigned long *) tptr) - jiffies / HZ;
	return 0;
}

// ��ȡ��ǰ����ʱ�䡣tms �ṹ�а����û�ʱ�䡢ϵͳʱ�䡢�ӽ����û�ʱ�䡢�ӽ���ϵͳʱ�䡣
int sys_times (struct tms *tbuf)
{
	if (tbuf)
	{
		verify_area (tbuf, sizeof *tbuf);
		put_fs_long (current->utime, (unsigned long *) &tbuf->tms_utime);
		put_fs_long (current->stime, (unsigned long *) &tbuf->tms_stime);
		put_fs_long (current->cutime, (unsigned long *) &tbuf->tms_cutime);
		put_fs_long (current->cstime, (unsigned long *) &tbuf->tms_cstime);
	}
	return jiffies;
}

// ������end_data_seg ��ֵ��������ϵͳȷʵ���㹻���ڴ棬���ҽ���û�г�Խ��������ݶδ�С
// ʱ���ú����������ݶ�ĩβΪend_data_seg ָ����ֵ����ֵ������ڴ����β����ҪС�ڶ�ջ
// ��β16KB������ֵ�����ݶε��½�βֵ���������ֵ��Ҫ��ֵ��ͬ��������д�������
// �ú����������û�ֱ�ӵ��ã�����libc �⺯�����а�װ�����ҷ���ֵҲ��һ����
int sys_brk (unsigned long end_data_seg)
{
	if (end_data_seg >= current->end_code &&	// �������>�����β������
	end_data_seg < current->start_stack - 16384)	// С�ڶ�ջ-16KB��
		current->brk = end_data_seg;	// �����������ݶν�βֵ��
	return current->brk;		// ���ؽ��̵�ǰ�����ݶν�βֵ��
}

/*
* This needs some heave checking ...
* I just haven't get the stomach for it. I also don't fully
* understand sessions/pgrp etc. Let somebody who does explain it.
*/
/*
* ���������ҪĳЩ�ϸ�ļ�顭
* ��ֻ��û��θ��������Щ����Ҳ����ȫ����sessions/pgrp �ȡ��������˽����ǵ��������ɡ�
*/
// ������pid ָ�����̵Ľ�����ID ���ó�pgid���������pid=0����ʹ�õ�ǰ���̺š����
// pgid Ϊ0����ʹ�ò���pid ָ���Ľ��̵���ID ��Ϊpgid������ú������ڽ����̴�һ��
// �������Ƶ���һ�������飬���������������������ͬһ���Ự(session)������������£�
// ����pgid ָ����Ҫ��������н�����ID����ʱ����ĻỰID �����뽫Ҫ������̵���ͬ(193 ��)��
int sys_setpgid (int pid, int pgid)
{
	int i;

	if (!pid)			// �������pid=0����ʹ�õ�ǰ���̺š�
		pid = current->pid;
	if (!pgid)			// ���pgid Ϊ0����ʹ�õ�ǰ����pid ��Ϊpgid��
		pgid = current->pid;	// [??������POSIX �������г���]
	for (i = 0; i < NR_TASKS; i++)	// ɨ���������飬����ָ�����̺ŵ�����
	if (task[i] && task[i]->pid == pid)
	{
		if (task[i]->leader)	// ����������Ѿ������죬������ء�
			return -EPERM;
		if (task[i]->session != current->session)	// ���������ĻỰID
			return -EPERM;	// �뵱ǰ���̵Ĳ�ͬ��������ء�
		task[i]->pgrp = pgid;	// ���ø������pgrp��
		return 0;
	}
	return -ESRCH;
}

// ���ص�ǰ���̵���š���getpgid(0)��ͬ��
int sys_getpgrp (void)
{
	return current->pgrp;
}

// ����һ���Ự(session)����������leader=1��������������Ự=�����=����̺š�
int sys_setsid (void)
{
	if (current->leader && !suser ())	// �����ǰ�������ǻỰ���첢�Ҳ��ǳ����û�
		return -EPERM;		// ������ء�
	current->leader = 1;		// ���õ�ǰ����Ϊ�»Ự���졣
	current->session = current->pgrp = current->pid;	// ���ñ�����session = pid��
	current->tty = -1;		// ��ʾ��ǰ����û�п����նˡ�
	return current->pgrp;		// ���ػỰID��
}

// ��ȡϵͳ��Ϣ������utsname �ṹ����5 ���ֶΣ��ֱ��ǣ����汾����ϵͳ�����ơ�����ڵ����ơ�
// ��ǰ���м��𡢰汾�����Ӳ���������ơ�
int sys_uname (struct utsname *name)
{
	static struct utsname thisname = {	// ��������˽ṹ�е���Ϣ�����ֱ���϶���ı䡣
		"linux .0", "nodename", "release ", "version ", "machine "
	};
	int i;

	if (!name)
		return -ERROR;		// ��������Ϣ�Ļ�����ָ��Ϊ��������ء�
	verify_area (name, sizeof *name);	// ��֤��������С�Ƿ��ޣ������ѷ�����ڴ�ȣ���
	for (i = 0; i < sizeof *name; i++)	// ��utsname �е���Ϣ���ֽڸ��Ƶ��û��������С�
		put_fs_byte (((char *) &thisname)[i], i + (char *) name);
	return 0;
}

// ���õ�ǰ���̴����ļ�����������Ϊmask & 0777��������ԭ�����롣
int sys_umask (int mask)
{
	int old = current->umask;

	current->umask = mask & 0777;
	return (old);
}
