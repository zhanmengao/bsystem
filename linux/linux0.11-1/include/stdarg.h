#ifndef _STDARG_H
#define _STDARG_H

typedef char *va_list;		// ����va_list ��һ���ַ�ָ�����͡�

/* Amount of space required in an argument list for an arg of type TYPE.
TYPE may alternatively be an expression whose type is used. */
/* �������������ΪTYPE ��arg �����б���Ҫ��Ŀռ�������
TYPE Ҳ������ʹ�ø����͵�һ�����ʽ */

// ������䶨����ȡ�����TYPE ���͵��ֽڳ���ֵ����int ����(4)�ı�����
#define __va_rounded_size(TYPE) \
(((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

// ��������������ú�ʵ�֣�ʹAP ָ�򴫸������Ŀɱ������ĵ�һ��������
// �ڵ�һ�ε���va_arg ��va_end ֮ǰ���������ȵ��øú�����
// 17 ���ϵ�__builtin_saveregs()����gcc �Ŀ����libgcc2.c �ж���ģ����ڱ���Ĵ�����
// ����˵���ɲμ�gcc �ֲ��½ڡ�Target Description Macros���е�
// ��Implementing the Varargs Macros��С�ڡ�
#ifndef __sparc__
#define va_start(AP, LASTARG) \
(AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
#else
#define va_start(AP, LASTARG) \
(__builtin_saveregs (), \
AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
#endif

// ����ú����ڱ����ú������һ���������ء�va_end �����޸�AP ʹ�������µ���
// va_start ֮ǰ���ܱ�ʹ�á�va_end ������va_arg �������еĲ������ٱ����á�
void va_end (va_list);		/* Defined in gnulib *//* ��gnulib �ж��� */
#define va_end(AP)

// ����ú�������չ���ʽʹ������һ�������ݲ���������ͬ�����ͺ�ֵ��
// ����ȱʡֵ��va_arg �������ַ����޷����ַ��͸������͡�
// �ڵ�һ��ʹ��va_arg ʱ�������ر��еĵ�һ��������������ÿ�ε��ö������ر��е�
// ��һ������������ͨ���ȷ���AP��Ȼ�����������ָ����һ����ʵ�ֵġ�
// va_arg ʹ��TYPE ����ɷ��ʺͶ�λ��һ�ÿ����һ��va_arg�������޸�AP ��ָʾ
// ���е���һ������
#define va_arg(AP, TYPE) \
(AP += __va_rounded_size (TYPE), \
*((TYPE *) (AP - __va_rounded_size (TYPE))))

#endif /* _STDARG_H */
