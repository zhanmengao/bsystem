/* passed
* linux/kernel/exit.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)
#include <signal.h>		// �ź�ͷ�ļ��������źŷ��ų������źŽṹ�Լ��źŲ�������ԭ�͡�
#include <sys/wait.h>		// �ȴ�����ͷ�ļ�������ϵͳ����wait()��waitpid()����س������š�

#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/tty.h>		// tty ͷ�ļ����������й�tty_io������ͨ�ŷ���Ĳ�����������
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

int sys_pause (void);
int sys_close (int fd);

//// �ͷ�ָ������(����)��
void release (struct task_struct *p)
{
  int i;

  if (!p)
    return;
  for (i = 1; i < NR_TASKS; i++)	// ɨ���������飬Ѱ��ָ������
    if (task[i] == p)
	{
		task[i] = NULL;		// �ÿո�������ͷ�����ڴ�ҳ��
		free_page ((long) p);
		schedule ();		// ���µ��ȡ�
		return;
	}
  panic ("trying to release non-existent task");	// ָ����������������������
}

//// ��ָ������(*p)�����ź�(sig)��Ȩ��Ϊpriv��
static _inline int
send_sig (long sig, struct task_struct *p, int priv)
{
// ���źŲ���ȷ������ָ��Ϊ��������˳���
  if (!p || sig < 1 || sig > 32)
    return -EINVAL;
// ����Ȩ�������Ч�û���ʶ��(euid)����ָ�����̵�euid �����ǳ����û������ڽ���λͼ�����
// ���źţ���������˳�������suser()����Ϊ(current->euid==0)�������ж��Ƿ񳬼��û���
  if (priv || (current->euid == p->euid) || suser ())
    p->signal |= (1 << (sig - 1));
  else
    return -EPERM;
  return 0;
}

//// ��ֹ�Ự(session)��
static void kill_session (void)
{
  struct task_struct **p = NR_TASKS + task;	// ָ��*p ����ָ������������ĩ�ˡ�

// �������е����񣨳�����0 ���⣩�������Ự���ڵ�ǰ���̵ĻỰ���������͹ҶϽ����źš�
  while (--p > &FIRST_TASK)
  {
    if (*p && (*p)->session == current->session)
	  (*p)->signal |= 1 << (SIGHUP - 1);	// ���͹ҶϽ����źš�
  }
}

/*
* Ϊ���������ȷ����źţ�XXX ��Ҫ�����ɡ�kill()����ɻ��Ʒǳ�����!
*/
//// kill()ϵͳ���ÿ��������κν��̻�����鷢���κ��źš�
// ���pid ֵ>0�����źű����͸�pid��
// ���pid=0����ô�źžͻᱻ���͸���ǰ���̵Ľ������е����н��̡�
// ���pid=-1�����ź�sig �ͻᷢ�͸�����һ������������н��̡�
// ���pid < -1�����ź�sig �����͸�������-pid �����н��̡�
// ����ź�sig Ϊ0���򲻷����źţ����Ի���д����顣����ɹ��򷵻�0��
int sys_kill (int pid, int sig)
{
  struct task_struct **p = NR_TASKS + task;
  int err, retval = 0;

  if (!pid)
    while (--p > &FIRST_TASK)
	{
		if (*p && (*p)->pgrp == current->pid)
		  if (err = send_sig (sig, *p, 1))
			retval = err;
	}
  else if (pid > 0)
    while (--p > &FIRST_TASK)
    {
	if (*p && (*p)->pid == pid)
	  if (err = send_sig (sig, *p, 0))
	    retval = err;
    }
  else if (pid == -1)
    while (--p > &FIRST_TASK)
      if (err = send_sig (sig, *p, 0))
		retval = err;
      else
		while (--p > &FIRST_TASK)
		  if (*p && (*p)->pgrp == -pid)
			if (err = send_sig (sig, *p, 0))
			  retval = err;
  return retval;
}

//// ֪ͨ������ -- �����pid �����ź�SIGCHLD���ӽ��̽�ֹͣ����ֹ��
// ���û���ҵ������̣����Լ��ͷš�
static void tell_father (int pid)
{
  int i;

  if (pid)
    for (i = 0; i < NR_TASKS; i++)
    {
		if (!task[i])
		  continue;
		if (task[i]->pid != pid)
		  continue;
		task[i]->signal |= (1 << (SIGCHLD - 1));
		return;
    }
/* if we don't find any fathers, we just release ourselves */
/* This is not really OK. Must change it to make father 1 */
  printk ("BAD BAD - no father found\n\r");
  release (current);		// ���û���ҵ������̣����Լ��ͷš�
}

//// �����˳����������ϵͳ���õ��жϴ�������б����á�
int do_exit (long code)		// code �Ǵ����롣
{
  int i;

// �ͷŵ�ǰ���̴���κ����ݶ���ռ���ڴ�ҳ(free_page_tables()��mm/memory.c,105 ��)��
  free_page_tables (get_base (current->ldt[1]), get_limit (0x0f));
  free_page_tables (get_base (current->ldt[2]), get_limit (0x17));
// �����ǰ�������ӽ��̣��ͽ��ӽ��̵�father ��Ϊ1(�丸���̸�Ϊ����1)��������ӽ����Ѿ�
// ���ڽ���(ZOMBIE)״̬���������1 �����ӽ�����ֹ�ź�SIGCHLD��
  for (i = 0; i < NR_TASKS; i++)
    if (task[i] && task[i]->father == current->pid)
      {
	task[i]->father = 1;
	if (task[i]->state == TASK_ZOMBIE)
/* assumption task[1] is always init */
	  (void) send_sig (SIGCHLD, task[1], 1);
      }
// �رյ�ǰ���̴��ŵ������ļ���
  for (i = 0; i < NR_OPEN; i++)
    if (current->filp[i])
      sys_close (i);
// �Ե�ǰ���̹���Ŀ¼pwd����Ŀ¼root �Լ����г����i �ڵ����ͬ�����������ֱ��ÿա�
  iput (current->pwd);
  current->pwd = NULL;
  iput (current->root);
  current->root = NULL;
  iput (current->executable);
  current->executable = NULL;
// �����ǰ��������ͷ(leader)���̲������п��Ƶ��նˣ����ͷŸ��նˡ�
  if (current->leader && current->tty >= 0)
    tty_table[current->tty].pgrp = 0;
// �����ǰ�����ϴ�ʹ�ù�Э����������last_task_used_math �ÿա�
  if (last_task_used_math == current)
    last_task_used_math = NULL;
// �����ǰ������leader ���̣�����ֹ������ؽ��̡�
  if (current->leader)
    kill_session ();
// �ѵ�ǰ������Ϊ����״̬���������˳��롣
  current->state = TASK_ZOMBIE;
  current->exit_code = code;
// ֪ͨ�����̣�Ҳ���򸸽��̷����ź�SIGCHLD -- �ӽ��̽�ֹͣ����ֹ��
  tell_father (current->father);
  schedule ();			// ���µ��Ƚ��̵����С�
  return (-1);			/* just to suppress warnings */
}

//// ϵͳ����exit()����ֹ���̡�
int sys_exit (int error_code)
{
  return do_exit ((error_code & 0xff) << 8);
}

//// ϵͳ����waitpid()������ǰ���̣�ֱ��pid ָ�����ӽ����˳�����ֹ�������յ�Ҫ����ֹ
// �ý��̵��źţ���������Ҫ����һ���źž�����źŴ�����򣩡����pid ��ָ���ӽ�������
// �˳����ѳ���ν�Ľ������̣����򱾵��ý����̷��ء��ӽ���ʹ�õ�������Դ���ͷš�
// ���pid > 0, ��ʾ�ȴ����̺ŵ���pid ���ӽ��̡�
// ���pid = 0, ��ʾ�ȴ�������ŵ��ڵ�ǰ���̵��κ��ӽ��̡�
// ���pid < -1, ��ʾ�ȴ�������ŵ���pid ����ֵ���κ��ӽ��̡�
// [ ���pid = -1, ��ʾ�ȴ��κ��ӽ��̡�]
// ��options = WUNTRACED����ʾ����ӽ�����ֹͣ�ģ�Ҳ���Ϸ��ء�
// ��options = WNOHANG����ʾ���û���ӽ����˳�����ֹ�����Ϸ��ء�
// ���stat_addr ��Ϊ�գ���ͽ�״̬��Ϣ���浽���
int sys_waitpid (pid_t pid, unsigned long *stat_addr, int options)
{
  int flag, code;
  struct task_struct **p;

  verify_area (stat_addr, 4);
repeat:
  flag = 0;
  for (p = &LAST_TASK; p > &FIRST_TASK; --p)
  {				// ����������ĩ�˿�ʼɨ����������
      if (!*p || *p == current)	// ��������ͱ������
		continue;
      if ((*p)->father != current->pid)	// ������ǵ�ǰ���̵��ӽ�����������
		continue;
      if (pid > 0)
		{			// ���ָ����pid>0����ɨ��Ľ���pid
		  if ((*p)->pid != pid)	// ��֮���ȣ���������
			continue;
		}
      else if (!pid)
		{			// ���ָ����pid=0����ɨ��Ľ������
		  if ((*p)->pgrp != current->pgrp)	// �뵱ǰ���̵���Ų��ȣ���������
			continue;
		}
      else if (pid != -1)
		{			// ���ָ����pid<-1����ɨ��Ľ������
		  if ((*p)->pgrp != -pid)	// �������ֵ���ȣ���������
			continue;
		}
    switch ((*p)->state)
	{
	case TASK_STOPPED:
	  if (!(options & WUNTRACED))
	    continue;
	  put_fs_long (0x7f, stat_addr);	// ��״̬��ϢΪ0x7f��
	  return (*p)->pid;	// �˳��������ӽ��̵Ľ��̺š�
	case TASK_ZOMBIE:
	  current->cutime += (*p)->utime;	// ���µ�ǰ���̵��ӽ����û�
	  current->cstime += (*p)->stime;	// ̬�ͺ���̬����ʱ�䡣
	  flag = (*p)->pid;
	  code = (*p)->exit_code;	// ȡ�ӽ��̵��˳��롣
	  release (*p);		// �ͷŸ��ӽ��̡�
	  put_fs_long (code, stat_addr);	// ��״̬��ϢΪ�˳���ֵ��
	  return flag;		// �˳��������ӽ��̵�pid.
	default:
	  flag = 1;		// ����ӽ��̲���ֹͣ����״̬����flag=1��
	  continue;
	}
  }
  if (flag)
  {				// ����ӽ���û�д����˳�����״̬��
	  if (options & WNOHANG)	// ����options = WNOHANG�������̷��ء�
		return 0;
	  current->state = TASK_INTERRUPTIBLE;	// �õ�ǰ����Ϊ���жϵȴ�״̬��
	  schedule ();		// ���µ��ȡ�
	  if (!(current->signal &= ~(1 << (SIGCHLD - 1))))	// �ֿ�ʼִ�б�����ʱ��
		goto repeat;		// �������û���յ���SIGCHLD ���źţ������ظ�����
	  else
		return -EINTR;		// �˳������س����롣
  }
  return -ECHILD;
}
