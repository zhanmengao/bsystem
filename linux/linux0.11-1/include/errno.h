#ifndef _ERRNO_H
#define _ERRNO_H

/*
* ok, as I hadn't got any other source of information about
* possible error numbers, I was forced to use the same numbers
* as minix.
* Hopefully these are posix or something. I wouldn't know (and posix
* isn't telling me - they want $$$ for their f***ing standard).
*
* We don't use the _SIGN cludge of minix, so kernel returns must
* see to the sign by themselves.
*
* NOTE! Remember to change strerror() if you change this file!
*/
/*
* ok��������û�еõ��κ������йس���ŵ����ϣ���ֻ��ʹ����minix ϵͳ
* ��ͬ�ĳ�����ˡ�
* ϣ����Щ��POSIX ���ݵĻ�����һ���̶����������ģ��Ҳ�֪��������POSIX
* û�и����� - Ҫ������ǵĻ쵰��׼��Ҫ��Ǯ����
*
* ����û��ʹ��minix ������_SIGN �أ������ں˵ķ���ֵ�����Լ���������š�
*
* ע�⣡�����ı���ļ��Ļ�������ҲҪ�޸�strerror()������
*/

extern int errno;

#define ERROR 99		// һ�����
#define EPERM 1			// ����û����ɡ�
#define ENOENT 2		// �ļ���Ŀ¼�����ڡ�
#define ESRCH 3			// ָ���Ľ��̲����ڡ�
#define EINTR 4			// �жϵĺ������á�
#define EIO 5			// ����/�����
#define ENXIO 6			// ָ���豸���ַ�����ڡ�
#define E2BIG 7			// �����б�̫����
#define ENOEXEC 8		// ִ�г����ʽ����
#define EBADF 9			// �ļ����(������)����
#define ECHILD 10		// �ӽ��̲����ڡ�
#define EAGAIN 11		// ��Դ��ʱ�����á�
#define ENOMEM 12		// �ڴ治�㡣
#define EACCES 13		// û�����Ȩ�ޡ�
#define EFAULT 14		// ��ַ��
#define ENOTBLK 15		// ���ǿ��豸�ļ���
#define EBUSY 16		// ��Դ��æ��
#define EEXIST 17		// �ļ��Ѵ��ڡ�
#define EXDEV 18		// �Ƿ����ӡ�
#define ENODEV 19		// �豸�����ڡ�
#define ENOTDIR 20		// ����Ŀ¼�ļ���
#define EISDIR 21		// ��Ŀ¼�ļ���
#define EINVAL 22		// ������Ч��
#define ENFILE 23		// ϵͳ���ļ���̫�ࡣ
#define EMFILE 24		// ���ļ���̫�ࡣ
#define ENOTTY 25		// ��ǡ����IO ���Ʋ���(û��tty �ն�)��
#define ETXTBSY 26		// ����ʹ�á�
#define EFBIG 27		// �ļ�̫��
#define ENOSPC 28		// �豸�������豸�Ѿ�û�пռ䣩��
#define ESPIPE 29		// ��Ч���ļ�ָ���ض�λ��
#define EROFS 30		// �ļ�ϵͳֻ����
#define EMLINK 31		// ����̫�ࡣ
#define EPIPE 32		// �ܵ���
#define EDOM 33			// ��(domain)����
#define ERANGE 34		// ���̫��
#define EDEADLK 35		// ������Դ������
#define ENAMETOOLONG 36		// �ļ���̫����
#define ENOLCK 37		// û���������á�
#define ENOSYS 38		// ���ܻ�û��ʵ�֡�
#define ENOTEMPTY 39		// Ŀ¼���ա�

#endif
