#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/types.h>		// ����ͷ�ļ��������˻�����ϵͳ�������͡�

/* open/fcntl - NOCTTY, NDELAY isn't implemented yet */
/* open/fcntl - NOCTTY ��NDELAY ���ڻ�û��ʵ�� */
#define O_ACCMODE 00003		// �ļ�����ģʽ�����롣
// ���ļ�open()���ļ�����fcntl()����ʹ�õ��ļ�����ģʽ��ͬʱֻ��ʹ������֮һ��
#define O_RDONLY 00		// ��ֻ����ʽ���ļ���
#define O_WRONLY 01		// ��ֻд��ʽ���ļ���
#define O_RDWR 02		// �Զ�д��ʽ���ļ���
// �������ļ�������־������open()�������������ģʽ��'λ��'�ķ�ʽһ��ʹ�á�
#define O_CREAT 00100 /* not fcntl */	// ����ļ������ھʹ�����
#define O_EXCL 00200 /* not fcntl */	// ��ռʹ���ļ���־��
#define O_NOCTTY 00400 /* not fcntl */	// ����������նˡ�
#define O_TRUNC 01000 /* not fcntl */	// ���ļ��Ѵ�������д�������򳤶Ƚ�Ϊ0��
#define O_APPEND 02000		// ����ӷ�ʽ�򿪣��ļ�ָ����Ϊ�ļ�β��
#define O_NONBLOCK 04000 /* not fcntl */	// ��������ʽ�򿪺Ͳ����ļ���
#define O_NDELAY O_NONBLOCK	// ��������ʽ�򿪺Ͳ����ļ���

/* Defines for fcntl-commands. Note that currently
* locking isn't supported, and other things aren't really
* tested.
*/
/* ���涨����fcntl �����ע��Ŀǰ�������û��֧�֣�������
* ����ʵ���ϻ�û�в��Թ���
*/
// �ļ����(������)��������fcntl()�����
#define F_DUPFD 0 /* dup */	// �����ļ����Ϊ��С��ֵ�ľ����
#define F_GETFD 1 /* get f_flags */	// ȡ�ļ������־��
#define F_SETFD 2 /* set f_flags */	// �����ļ������־��
#define F_GETFL 3 /* more flags (cloexec) */	// ȡ�ļ�״̬��־�ͷ���ģʽ��
#define F_SETFL 4		// �����ļ�״̬��־�ͷ���ģʽ��
// �������ļ��������fcntl()�ĵ���������lock ��ָ��flock �ṹ��ָ�롣
#define F_GETLK 5 /* not implemented */	// ������ֹ������flock �ṹ��
#define F_SETLK 6		// ����(F_RDLCK ��F_WRLCK)�����(F_UNLCK)������
#define F_SETLKW 7		// �ȴ����û����������

/* for F_[GET|SET]FL */
/* ���� F_GETFL ��F_SETFL */
// ��ִ��exec()�غ���ʱ�ر��ļ������(ִ��ʱ�ر� - Close On EXECution)
#define FD_CLOEXEC 1		/* actually anything with low bit set goes */
/* ʵ����ֻҪ��λΪ1 ���� */

/* Ok, these are locking features, and aren't implemented at any
* level. POSIX wants them.
*/
/* OK���������������ͣ��κκ����ж���û��ʵ�֡�POSIX ��׼Ҫ����Щ���͡�
*/
#define F_RDLCK 0		// �������ļ�������
#define F_WRLCK 1		// ��ռ��д�ļ�������
#define F_UNLCK 2		// �ļ�������

/* Once again - not implemented, but ... */
/* ͬ�� - Ҳ��û��ʵ�֣�����... */
// �ļ������������ݽṹ����������Ӱ���ļ��ε�����(l_type)����ʼƫ��(l_whence)��
// ���ƫ��(l_start)����������(l_len)��ʵʩ�����Ľ���id��
struct flock
{
  short l_type;			// �������ͣ�F_RDLCK��F_WRLCK��F_UNLCK����
  short l_whence;		// ��ʼƫ��(SEEK_SET��SEEK_CUR ��SEEK_END)��
  off_t l_start;		// ���������Ŀ�ʼ�������ƫ�ƣ��ֽ�������
  off_t l_len;			// ���������Ĵ�С�������0 ��Ϊ���ļ�ĩβ��
  pid_t l_pid;			// �����Ľ���id��
};

// ������ʹ��������־������ĺ���ԭ�͡�
// �������ļ�����дһ���Ѵ����ļ���
// ����filename ���������ļ����ļ�����mode �Ǵ����ļ������ԣ��μ�include/sys/stat.h����
extern int creat (const char *filename, mode_t mode);
// �ļ������������Ӱ���ļ��Ĵ򿪡�
// ����fildes ���ļ������cmd �ǲ������������23-30 �С�
extern int fcntl (int fildes, int cmd, ...);
// ���ļ������ļ����ļ����֮�佨����ϵ��
// ����filename �������ļ����ļ�����flags ������7-17 ���ϵı�־����ϡ�
extern int open (const char *filename, int flags, ...);

#endif
