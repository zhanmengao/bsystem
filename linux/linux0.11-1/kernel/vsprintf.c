/* passed
* linux/kernel/vsprintf.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
* Wirzenius wrote this portably, Torvalds fucked it up :-)
*/

#include <stdarg.h>		// ��׼����ͷ�ļ����Ժ����ʽ������������б���Ҫ˵����-��
// ����(va_list)��������(va_start, va_arg ��va_end)������
// vsprintf��vprintf��vfprintf ������
#include <string.h>		// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����

/* we use this so that we can do without the ctype library */
/* ����ʹ������Ķ��壬�������ǾͿ��Բ�ʹ��ctype ���� */
#define is_digit(c) ((c) >= '0' && (c) <= '9')	// �ж��ַ��Ƿ������ַ���

// �ú������ַ����ִ�ת�������������������ִ�ָ���ָ�룬�����ǽ����ֵ������ָ�뽫ǰ�ơ�
static int skip_atoi (const char **s)
{
	int i = 0;

	while (is_digit (**s))
		i = i * 10 + *((*s)++) - '0';
	return i;
}

// ���ﶨ��ת�����͵ĸ��ַ��ų�����
#define ZEROPAD 1 /* pad with zero */	/* ����� */
#define SIGN 2 /* unsigned/signed long */	/* �޷���/���ų����� */
#define PLUS 4 /* show plus */	/* ��ʾ�� */
#define SPACE 8 /* space if plus */	/* ���Ǽӣ����ÿո� */
#define LEFT 16 /* left justified */	/* ����� */
#define SPECIAL 32 /* 0x */	/* 0x */
#define SMALL 64 /* use 'abcdef' instead of 'ABCDEF' */	/* ʹ��Сд��ĸ */

// �����������룺n Ϊ��������base Ϊ�����������n Ϊ�̣���������ֵΪ������
// �μ�4.5.3 ���й�Ƕ�������Ϣ��
#define do_div(n,base) _do_div(&(n),base)
extern _inline int _do_div(int *n,int base)
{
	int __res;
	_asm mov ecx,n
	_asm mov eax,[ecx]
	_asm xor edx,edx
	_asm mov ebx,base
	_asm div ebx
	_asm mov [ecx],eax
	_asm mov __res,edx
	return __res;
}
/*#define do_div(n,base) ({ \
int __res; \
__asm__( "divl %4": "=a" (n), "=d" (__res): "" (n), "1" (0), "r" (base)); \
__res; })*/

// ������ת��Ϊָ�����Ƶ��ַ�����
// ���룺num-������base-���ƣ�size-�ַ������ȣ�precision-���ֳ���(����)��type-����ѡ�
// �����str �ַ���ָ�롣
static char *
number (char *str, int num, int base, int size, int precision, int type)
{
	char c, sign, tmp[36];
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

// �������type ָ����Сд��ĸ������Сд��ĸ����
// �������ָ��Ҫ�����������߽磩�������������е������־��
// ������ƻ���С��2 �����36�����˳�����Ҳ��������ֻ�ܴ��������2-32 ֮�������
	if (type & SMALL)
		digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
// �������ָ��Ҫ���㣬�����ַ�����c='0'��Ҳ��''��������c ���ڿո��ַ���
// �������ָ���Ǵ�������������ֵnum С��0�����÷��ű���sign=���ţ���ʹnum ȡ����ֵ��
// �����������ָ���ǼӺţ�����sign=�Ӻţ����������ʹ��ո��־��sign=�ո񣬷�����0��
	c = (type & ZEROPAD) ? '0' : ' ';
	if (type & SIGN && num < 0)
	{
		sign = '-';
		num = -num;
	}
	else
		sign = (type & PLUS) ? '+' : ((type & SPACE) ? ' ' : 0);
// �������ţ�����ֵ��1��������ָ��������ת���������ʮ�����ƿ���ټ���2 λ(����0x)��
// ���ڰ˽��ƿ�ȼ�1�����ڰ˽���ת�����ǰ��һ���㣩��
	if (sign)
		size--;
	if (type & SPECIAL)
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
// �����ֵnum Ϊ0������ʱ�ַ���='0'��������ݸ����Ļ�������ֵnum ת�����ַ���ʽ��
	i = 0;
	if (num == 0)
		tmp[i++] = '0';
	else
		while (num != 0)
			tmp[i++] = digits[do_div (num, base)];
// ����ֵ�ַ��������ھ���ֵ���򾫶�ֵ��չΪ���ָ���ֵ��
// ���ֵsize ��ȥ���ڴ����ֵ�ַ��ĸ�����
	if (i > precision)
		precision = i;
	size -= precision;
// ������������ʼ�γ�����Ҫ��ת�����������ʱ�����ַ���str �С�
// ��������û������(ZEROPAD)�����루���������־������str ������
// ���ʣ����ֵָ���Ŀո��������������λ���������š�
	if (!(type & (ZEROPAD + LEFT)))
		while (size-- > 0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
// ������ָ��������ת��������ڰ˽���ת�����ͷһλ����һ��'0'��������ʮ����������'0x'��
	if (type & SPECIAL)
		if (base == 8)
			*str++ = '0';
		else if (base == 16)
		{
			*str++ = '0';
			*str++ = digits[33];	// 'X'��'x'
		}
// ��������û������������룩��־������ʣ�����д��c �ַ���'0'��ո񣩣���51 �С�
	if (!(type & LEFT))
		while (size-- > 0)
			*str++ = c;
// ��ʱi ������ֵnum �����ָ����������ָ���С�ھ���ֵ����str �з��루����ֵ-i����'0'��
	while (i < precision--)
		*str++ = '0';
// ��ת��ֵ���õ������ַ�����str �С���i ����
	while (i-- > 0)
		*str++ = tmp[i];
// �����ֵ�Դ����㣬���ʾ���ͱ�־���������־��־������ʣ�����з���ո�
	while (size-- > 0)
		*str++ = ' ';
	return str;			// ����ת���õ��ַ�����
}

// ���溯�����͸�ʽ��������ַ����С�
// Ϊ�������ں���ʹ�ø�ʽ���������Linus ���ں�ʵ���˸�C ��׼������
// ���в���fmt �Ǹ�ʽ�ַ�����args �Ǹ����仯��ֵ��buf ������ַ�����������
// ��μ��������б����йظ�ʽת���ַ��Ľ��ܡ�
int
vsprintf (char *buf, const char *fmt, va_list args)
{
	int len;
	int i;
	char *str;			// ���ڴ��ת�������е��ַ�����
	char *s;
	int *ip;

	int flags;			/* flags to number() */
/* number()����ʹ�õı�־ */
	int field_width;		/* width of output field */
/* ����ֶο��*/
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
/* min. �������ָ�����max. �ַ������ַ����� */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
/* 'h', 'l',��'L'���������ֶ� */
// ���Ƚ��ַ�ָ��ָ��buf��Ȼ��ɨ���ʽ�ַ������Ը�����ʽת��ָʾ������Ӧ�Ĵ���
	for (str = buf; *fmt; ++fmt)
	{
// ��ʽת��ָʾ�ַ�������'%'��ʼ�������fmt ��ʽ�ַ�����ɨ��'%'��Ѱ�Ҹ�ʽת���ַ����Ŀ�ʼ��
// ���Ǹ�ʽָʾ��һ���ַ��������δ���str��
		if (*fmt != '%')
		{
			*str++ = *fmt;
			continue;
		}

// ����ȡ�ø�ʽָʾ�ַ����еı�־�򣬲�����־��������flags �����С�
/* process flags */
		flags = 0;
repeat:
		++fmt;			/* this also skips first '%' */
		switch (*fmt)
		{
		case '-':
			flags |= LEFT;
			goto repeat;		// ���������
		case '+':
			flags |= PLUS;
			goto repeat;		// �żӺš�
		case ' ':
			flags |= SPACE;
			goto repeat;		// �ſո�
		case '#':
			flags |= SPECIAL;
			goto repeat;		// ������ת����
		case '0':
			flags |= ZEROPAD;
			goto repeat;		// Ҫ����(��'0')��
		}

// ȡ��ǰ�����ֶο����ֵ������field_width �����С���������������ֵ��ֱ��ȡ��Ϊ���ֵ��
// �������������ַ�'*'����ʾ��һ������ָ����ȡ���˵���va_arg ȡ���ֵ������ʱ���ֵ
// С��0����ø�����ʾ����б�־��'-'��־�����룩����˻����ڱ�־����������ñ�־����
// ���ֶο��ֵȡΪ�����ֵ��
/* get field width */
		field_width = -1;
		if (is_digit (*fmt))
			field_width = skip_atoi (&fmt);
		else if (*fmt == '*')
		{
/* it's the next argument */
			field_width = va_arg (args, int);
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

// ������δ��룬ȡ��ʽת�����ľ����򣬲�����precision �����С�������ʼ�ı�־��'.'��
// �䴦�������������������ơ����������������ֵ��ֱ��ȡ��Ϊ����ֵ���������������
// �ַ�'*'����ʾ��һ������ָ�����ȡ���˵���va_arg ȡ����ֵ������ʱ���ֵС��0����
// ���ֶξ���ֵȡΪ�����ֵ��
/* get the precision */
		precision = -1;
		if (*fmt == '.')
		{
			++fmt;
			if (is_digit (*fmt))
				precision = skip_atoi (&fmt);
			else if (*fmt == '*')
			{
/* it's the next argument */
				precision = va_arg (args, int);
			}
			if (precision < 0)
			precision = 0;
		}

// ������δ�������������η������������qualifer ��������h,l,L �ĺ���μ��б���˵������
/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
		{
			qualifier = *fmt;
			++fmt;
		}

// �������ת��ָʾ����
		switch (*fmt)
		{
// ���ת��ָʾ����'c'�����ʾ��Ӧ����Ӧ���ַ�����ʱ�����־������������룬����ֶ�ǰ��
// ��������ֵ-1 ���ո��ַ���Ȼ���ٷ�������ַ����������򻹴���0�����ʾΪ���룬����
// �����ַ�������ӿ��ֵ-1 ���ո��ַ���
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg (args, int);
			while (--field_width > 0)
				*str++ = ' ';
			break;

// ���ת��ָʾ����'s'�����ʾ��Ӧ�������ַ���������ȡ�����ַ����ĳ��ȣ����䳬���˾�����ֵ��
// ����չ������=�ַ������ȡ���ʱ�����־������������룬����ֶ�ǰ����(���ֵ-�ַ�������)
// ���ո��ַ���Ȼ���ٷ�������ַ������������򻹴���0�����ʾΪ���룬���ڲ����ַ�������
// ���(���ֵ-�ַ�������)���ո��ַ���
		case 's':
			s = va_arg (args, char *);
			len = strlen (s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

// �����ʽת������'o'����ʾ�轫��Ӧ�Ĳ���ת���ɰ˽��������ַ���������number()��������
		case 'o':
			str = number (str, va_arg (args, unsigned long), 8,
									field_width, precision, flags);
			break;

// �����ʽת������'p'����ʾ��Ӧ������һ��ָ�����͡���ʱ���ò���û�����ÿ������Ĭ�Ͽ��
// Ϊ8��������Ҫ���㡣Ȼ�����number()�������д���
		case 'p':
			if (field_width == -1)
			{
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number (str, (unsigned long) va_arg (args, void *), 16,
							field_width, precision, flags);
			break;

// ����ʽת��ָʾ��'x'��'X'�����ʾ��Ӧ������Ҫ��ӡ��ʮ�������������'x'��ʾ��Сд��ĸ��ʾ��
		case 'x':
			flags |= SMALL;
		case 'X':
			str = number (str, va_arg (args, unsigned long), 16,
									field_width, precision, flags);
			break;

// �����ʽת���ַ���'d','i'��'u'�����ʾ��Ӧ������������'d', 'i'������������������Ҫ����
// �����ű�־��'u'�����޷���������
		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number (str, va_arg (args, unsigned long), 10,
									field_width, precision, flags);
			break;

// ����ʽת��ָʾ����'n'�����ʾҪ�ѵ�ĿǰΪֹת��������ַ������浽��Ӧ����ָ��ָ����λ���С�
// ��������va_arg()ȡ�øò���ָ�룬Ȼ���Ѿ�ת���õ��ַ��������ָ����ָ��λ�á�
		case 'n':
			ip = va_arg (args, int *);
			*ip = (str - buf);
			break;

// ����ʽת��������'%'�����ʾ��ʽ�ַ����д�ֱ�ӽ�һ��'%'д��������С�
// �����ʽת������λ�ô������ַ�����Ҳֱ�ӽ����ַ�д��������У������ص�107 �м�������
// ��ʽ�ַ����������ʾ�Ѿ�������ʽ�ַ����Ľ�β�������˳�ѭ����
		default:
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';			// �����ת���õ��ַ�����β������null��
	return str - buf;		// ����ת���õ��ַ�������ֵ��
}
