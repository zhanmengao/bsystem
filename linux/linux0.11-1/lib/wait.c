/*
 *  linux/lib/wait.c
 *
 *  (C) 1991  Linus Torvalds
 * 
 *     �ó����������waitpid()��wait()������������������̻�ȡ�����ӽ���֮һ��״̬��Ϣ��
 * ����ѡ�������ȡ�Ѿ���ֹ��ֹͣ���ӽ���״̬��Ϣ������������������������ӽ��̵�״̬
 * ��Ϣ���򱨸��˳���ǲ�ָ���ġ�
 *     wait()������ǰ���̣�ֱ�����ӽ���֮һ�˳�����ֹ���������յ�Ҫ����ֹ�ý��̵��źţ�
 * ��������Ҫ����һ���źž�����źŴ������
 *     waitpid()����ǰ���̣�ֱ��pid ָ�����ӽ����˳�����ֹ�������յ�Ҫ����ֹ�ý��̵�
 * �źţ���������Ҫ����һ���źž�����źŴ�����򣩡�
 *     ���pid= -1��options=0����waitpid()��������wait()����һ������������Ϊ����pid ��
 * options �����Ĳ�ͬ����ͬ�����μ�kernel/exit.c,142��
 */
#include <set_seg.h>

#define __LIBRARY__
// Linux ��׼ͷ�ļ��������˸��ַ��ų��������ͣ��������˸��ֺ�����
// �綨����__LIBRARY__���򻹰���ϵͳ���úź���Ƕ���_syscall0()�ȡ�
#include <unistd.h>
// �ȴ�����ͷ�ļ�������ϵͳ����wait()��waitpid()����س������š�
#include <sys/wait.h>

//// �ȴ�������ֹϵͳ���ú�����
// �������ṹ��Ӧ�ں�����pid_t waitpid(pid_t pid, int * wait_stat, int options)
//
// ������pid - �ȴ�����ֹ���̵Ľ���id������������ָ����������������ض���ֵ��
//       wait_stat - ���ڴ��״̬��Ϣ��options - WNOHANG ��WUNTRACED ����0��
_syscall3(pid_t,waitpid,pid_t,pid,int *,wait_stat,int,options)

//// wait()ϵͳ���á�ֱ�ӵ���waitpid()������
pid_t wait(int * wait_stat)
{
	return waitpid(-1,wait_stat,0);
}
