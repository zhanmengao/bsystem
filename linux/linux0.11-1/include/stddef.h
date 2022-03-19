#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef long ptrdiff_t;		// ����ָ�������������͡�
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;	// sizeof ���ص����͡�
#endif

#undef NULL
#define NULL 0	// ��ָ�롣

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)	// ��Ա�������е�ƫ��λ�á�

#endif
