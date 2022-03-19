#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;	// ���ڶ���Ĵ�С�����ȣ���
#endif

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;		// ����ʱ�䣨����ƣ���
#endif

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef long ptrdiff_t;
#endif

#ifndef NULL
#define NULL 0
#endif

typedef int pid_t;		// ���ڽ��̺źͽ�����š�
typedef unsigned short uid_t;	// �����û��ţ��û���ʶ�ţ���
typedef unsigned char gid_t;	// ������š�
typedef unsigned short dev_t;	// �����豸�š�
typedef unsigned short ino_t;	// �����ļ����кš�
typedef unsigned short mode_t;	// ����ĳЩ�ļ����ԡ�
typedef unsigned short umode_t;	//
typedef unsigned char nlink_t;	// �������Ӽ�����
typedef int daddr_t;
typedef long off_t;		// �����ļ����ȣ���С����
typedef unsigned char u_char;	// �޷����ַ����͡�
typedef unsigned short ushort;	// �޷��Ŷ��������͡�

typedef struct
{
  int quot, rem;
}
div_t;				// ����DIV ������
typedef struct
{
  long quot, rem;
}
ldiv_t;				// ���ڳ�DIV ������

struct ustat
{
  daddr_t f_tfree;
  ino_t f_tinode;
  char f_fname[6];
  char f_fpack[6];
};

#endif
