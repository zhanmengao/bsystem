#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/types.h>		// ����ͷ�ļ��������˻�����ϵͳ�������͡�

typedef int sig_atomic_t;	// �����ź�ԭ�Ӳ������͡�
typedef unsigned int sigset_t;	/* 32 bits */// �����źż����͡�

#define _NSIG 32		// �����ź����� -- 32 �֡�
#define NSIG _NSIG		// NSIG = _NSIG

// ������Щ��Linux 0.11 �ں��ж�����źš�
#define SIGHUP 1		// Hang Up -- �ҶϿ����ն˻���̡�
#define SIGINT 2		// Interrupt -- ���Լ��̵��жϡ�
#define SIGQUIT 3		// Quit -- ���Լ��̵��˳���
#define SIGILL 4		// Illeagle -- �Ƿ�ָ�
#define SIGTRAP 5		// Trap -- ���ٶϵ㡣
#define SIGABRT 6		// Abort -- �쳣������
#define SIGIOT 6		// IO Trap -- ͬ�ϡ�
#define SIGUNUSED 7		// Unused -- û��ʹ�á�
#define SIGFPE 8		// FPE -- Э����������
#define SIGKILL 9		// Kill -- ǿ�Ƚ�����ֹ��
#define SIGUSR1 10		// User1 -- �û��ź�1�����̿�ʹ�á�
#define SIGSEGV 11		// Segment Violation -- ��Ч�ڴ����á�
#define SIGUSR2 12		// User2 -- �û��ź�2�����̿�ʹ�á�
#define SIGPIPE 13		// Pipe -- �ܵ�д�����޶��ߡ�
#define SIGALRM 14		// Alarm -- ʵʱ��ʱ��������
#define SIGTERM 15		// Terminate -- ������ֹ��
#define SIGSTKFLT 16		// Stack Fault -- ջ����Э����������
#define SIGCHLD 17		// Child -- �ӽ���ֹͣ����ֹ��
#define SIGCONT 18		// Continue -- �ָ����̼���ִ�С�
#define SIGSTOP 19		// Stop -- ֹͣ���̵�ִ�С�
#define SIGTSTP 20		// TTY Stop -- tty ����ֹͣ���̣��ɺ��ԡ�
#define SIGTTIN 21		// TTY In -- ��̨�����������롣
#define SIGTTOU 22		// TTY Out -- ��̨�������������

/* Ok, I haven't implemented sigactions, but trying to keep headers POSIX */
/* OK���һ�û��ʵ��sigactions �ı��ƣ�����ͷ�ļ�����ϣ������POSIX ��׼ */
#define SA_NOCLDSTOP 1		// ���ӽ��̴���ֹͣ״̬���Ͳ���SIGCHLD ����
#define SA_NOMASK 0x40000000	// ����ֹ��ָ�����źŴ������(�źž��)�����յ����źš�
#define SA_ONESHOT 0x80000000	// �źž��һ�������ù��ͻָ���Ĭ�ϴ�������

// ���²�������sigprocmask()-- �ı������źż�(������)����Щ�������Ըı�ú�������Ϊ��
#define SIG_BLOCK 0		/* for blocking signals */
// �������źż��м��ϸ������źż���
#define SIG_UNBLOCK 1		/* for unblocking signals */
// �������źż���ɾ��ָ�����źż���
#define SIG_SETMASK 2		/* for setting the signal mask */
// ���������źż����ź������룩��

#define SIG_DFL ((void (*)(int))0)	/* default signal handling */
// Ĭ�ϵ��źŴ�������źž������
#define SIG_IGN ((void (*)(int))1)	/* ignore signal */
// �����źŵĴ������

// ������sigaction �����ݽṹ��
// sa_handler �Ƕ�Ӧĳ�ź�ָ��Ҫ��ȡ���ж��������������SIG_DFL��������SIG_IGN ������
// ���źţ�Ҳ������ָ������źź�����һ��ָ�롣
// sa_mask �����˶��źŵ������룬���źų���ִ��ʱ����������Щ�źŵĴ���
// sa_flags ָ���ı��źŴ�����̵��źż���������37-39 �е�λ��־����ġ�
// sa_restorer �ָ�����ָ�룬�����ڱ���ԭ���صĹ���ָ�롣
// ���⣬���𴥷��źŴ�����ź�Ҳ��������������ʹ����SA_NOMASK ��־��
struct sigaction
{
  void (*sa_handler) (int);
  sigset_t sa_mask;
  int sa_flags;
  void (*sa_restorer) (void);
};

// Ϊ�ź�_sig ��װһ���µ��źŴ�������źž��������sigaction()���ơ�
void (*signal (int _sig, void (*_func) (int))) (int);
// ��ǰ���̷���һ���źš������õȼ���kill(getpid(),sig)��
int raise (int sig);
// ���������κν��������̷����κ��źš�
int kill (pid_t pid, int sig);
// ���źż�������źš�
int sigaddset (sigset_t * mask, int signo);
// ���źż���ȥ��ָ�����źš�
int sigdelset (sigset_t * mask, int signo);
// ���źż������ָ���źż���
int sigemptyset (sigset_t * mask);
// ���źż������������źš�
int sigfillset (sigset_t * mask);
// �ж�һ���ź��Ƿ����źż��еġ�1 -- �ǣ� 0 -- ���ǣ� -1 -- ����
int sigismember (sigset_t * mask, int signo);	/* 1 - is, 0 - not, -1 error */
// ��set �е��źŽ��м�⣬���Ƿ��й�����źš�
int sigpending (sigset_t * set);
// �ı�Ŀǰ�ı������źż����ź������룩��
int sigprocmask (int how, sigset_t * set, sigset_t * oldset);
// ��sigmask ��ʱ�滻���̵��ź�������,Ȼ����ͣ�ý���ֱ���յ�һ���źš�
int sigsuspend (sigset_t * sigmask);
// ���ڸı�������յ�ָ���ź�ʱ����ȡ���ж���
int sigaction (int sig, struct sigaction *act, struct sigaction *oldact);

#endif /* _SIGNAL_H */
