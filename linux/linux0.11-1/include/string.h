#ifndef _STRING_H_
#define _STRING_H_
/*
  ��ʵvc�����������������Щ�������壬������ȫ���԰�string.h�ڵĺ�������
  ��ע�͵���Ȼ��Ϊÿ��.c�ļ�����ʱ����/Ox����ѡ��ɡ�
*/

#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

extern char *strerror (int errno);
/*
* ����ַ���ͷ�ļ�����Ƕ��������ʽ�����������ַ�������������ʹ��gcc ʱ��ͬʱ
* �ٶ���ds=es=���ݿռ䣬��Ӧ���ǳ���ġ���������ַ����������Ǿ��ֹ����д���
* �Ż��ģ������Ǻ���strtok��strstr��str[c]spn������Ӧ����������������ȴ������
* ô������⡣���еĲ��������϶���ʹ�üĴ���������ɵģ���ʹ�ú������������ࡣ
* ���еط���ʹ�����ַ���ָ�����ʹ�ô��롰��΢���������?
*
* (C) 1991 Linus Torvalds
*/

//// ��һ���ַ���(src)��������һ���ַ���(dest)��ֱ������NULL �ַ���ֹͣ��
// ������dest - Ŀ���ַ���ָ�룬src - Դ�ַ���ָ�롣
// %0 - esi(src)��%1 - edi(dest)��
extern _inline char *
strcpy (char *dest, const char *src)
{
	_asm{
		pushf
		mov esi,src
		mov edi,dest
		cld		// �巽��λ��
	l1:	lodsb	// ����DS:[esi]��1 �ֽ�->al��������esi��
		stosb		// �洢�ֽ�al->ES:[edi]��������edi��
		test al,al	// �մ洢���ֽ���0��
		jne l1	// ��������ת�����l1 �������������
		popf
	}
	return dest;			// ����Ŀ���ַ���ָ�롣
}
/*extern _inline char *
strcpy (char *dest, const char *src)
{
  __asm__ ("cld\n"		// �巽��λ��
	   "1:\tlodsb\n\t"	// ����DS:[esi]��1 �ֽ�??al��������esi��
	   "stosb\n\t"		// �洢�ֽ�al??ES:[edi]��������edi��
	   "testb %%al,%%al\n\t"	// �մ洢���ֽ���0��
	   "jne 1b"		// �����������ת�����1 �������������
::"S" (src), "D" (dest):"si", "di", "ax");
  return dest;			// ����Ŀ���ַ���ָ�롣
}*/

//// ����Դ�ַ���count ���ֽڵ�Ŀ���ַ�����
// ���Դ������С��count ���ֽڣ��͸��ӿ��ַ�(NULL)��Ŀ���ַ�����
// ������dest - Ŀ���ַ���ָ�룬src - Դ�ַ���ָ�룬count - �����ֽ�����
// %0 - esi(src)��%1 - edi(dest)��%2 - ecx(count)��
static _inline char *
strncpy (char *dest, const char *src, int count)
{
	_asm{
		pushf
		mov esi,src
		mov edi,dest
		mov ecx,count
		cld		// �巽��λ��
	l1:	dec ecx	// �Ĵ���ecx--��count--����
		js l2		// ���count<0 ����ǰ��ת�����l2��������
		lodsb		// ȡds:[esi]��1 �ֽ�->al������esi++��
		stosb		// �洢���ֽ�->es:[edi]������edi++��
		test al,al	// ���ֽ���0��
		jne l1		// ���ǣ�����ǰ��ת�����l1 ������������
		rep stosb		// ������Ŀ�Ĵ��д��ʣ������Ŀ��ַ���
	l2:	popf
	}
	return dest;			// ����Ŀ���ַ���ָ�롣
}
/*extern _inline char *
strncpy (char *dest, const char *src, int count)
{
  __asm__ ("cld\n"		// �巽��λ��
	   "1:\tdecl %2\n\t"	// �Ĵ���ecx--��count--����
	   "js 2f\n\t"		// ���count<0 ����ǰ��ת�����2��������
	   "lodsb\n\t"		// ȡds:[esi]��1 �ֽ�??al������esi++��
	   "stosb\n\t"		// �洢���ֽ�??es:[edi]������edi++��
	   "testb %%al,%%al\n\t"	// ���ֽ���0��
	   "jne 1b\n\t"		// ���ǣ�����ǰ��ת�����1 ������������
	   "rep\n\t"		// ������Ŀ�Ĵ��д��ʣ������Ŀ��ַ���
"stosb\n" "2:"::"S" (src), "D" (dest), "c" (count):"si", "di", "ax",
	   "cx");
  return dest;			// ����Ŀ���ַ���ָ�롣
}*/

//// ��Դ�ַ���������Ŀ���ַ�����ĩβ����
// ������dest - Ŀ���ַ���ָ�룬src - Դ�ַ���ָ�롣
// %0 - esi(src)��%1 - edi(dest)��%2 - eax(0)��%3 - ecx(-1)��
extern _inline char *
strcat (char *dest, const char *src)
{
	_asm {
		pushf
		mov esi,src
		mov edi,dest
		xor al,al
		mov ecx,0xffffffff
		cld		// �巽��λ��
		repne scasb		// �Ƚ�al ��es:[edi]�ֽڣ�������edi++��
						// ֱ���ҵ�Ŀ�Ĵ�����0 ���ֽڣ���ʱedi �Ѿ�ָ���1 �ֽڡ�
		dec edi		// ��es:[edi]ָ��0 ֵ�ֽڡ�
	l1: lodsb	// ȡԴ�ַ����ֽ�ds:[esi]->al����esi++��
		stosb		// �����ֽڴ浽es:[edi]����edi++��
		test al,al	// ���ֽ���0��
		jne l1	// ���ǣ��������ת�����1 ���������������������
		popf
	}
	return dest;			// ����Ŀ���ַ���ָ�롣
}
/*extern _inline char *
strcat (char *dest, const char *src)
{
  __asm__ ("cld\n\t"		// �巽��λ��
	   "repne\n\t"		// �Ƚ�al ��es:[edi]�ֽڣ�������edi++��
	   "scasb\n\t"		// ֱ���ҵ�Ŀ�Ĵ�����0 ���ֽڣ���ʱedi �Ѿ�ָ���1 �ֽڡ�
	   "decl %1\n"		// ��es:[edi]ָ��0 ֵ�ֽڡ�
	   "1:\tlodsb\n\t"	// ȡԴ�ַ����ֽ�ds:[esi]??al����esi++��
	   "stosb\n\t"		// �����ֽڴ浽es:[edi]����edi++��
	   "testb %%al,%%al\n\t"	// ���ֽ���0��
	   "jne 1b"		// ���ǣ��������ת�����1 ���������������������
::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff):"si", "di", "ax",
	   "cx");
  return dest;			// ����Ŀ���ַ���ָ�롣
}*/

//// ��Դ�ַ�����count ���ֽڸ��Ƶ�Ŀ���ַ�����ĩβ���������һ���ַ���
// ������dest - Ŀ���ַ�����src - Դ�ַ�����count - �����Ƶ��ֽ�����
// %0 - esi(src)��%1 - edi(dest)��%2 - eax(0)��%3 - ecx(-1)��%4 - (count)��
static _inline char *
strncat (char *dest, const char *src, int count)
{
	_asm {
		pushf
		mov esi,src
		mov edi,dest
		xor al,al
		mov ecx,0xffffffff
		cld		// �巽��λ��
		repne scasb		// �Ƚ�al ��es:[edi]�ֽڣ�edi++��ֱ���ҵ�Ŀ�Ĵ���ĩ��0 ֵ�ֽڡ�
		dec edi	// edi ָ���0 ֵ�ֽڡ�
		mov ecx,count	// �������ֽ���??ecx��
	l1: dec ecx	// ecx--����0 ��ʼ��������
		js l2		// ecx <0 ?��������ǰ��ת�����l2 ����
		lodsb		// ����ȡds:[esi]�����ֽ�->al��esi++��
		stosb		// �洢��es:[edi]����edi++��
		test al,al	// ���ֽ�ֵΪ0��
		jne l1		// �����������ת�����1 �����������ơ�
	l2: xor al,al	// ��al ���㡣
		stosb		// �浽es:[edi]����
		popf
	}
  return dest;			// ����Ŀ���ַ���ָ�롣
}
/*extern _inline char *
strncat (char *dest, const char *src, int count)
{
  __asm__ ("cld\n\t"		// �巽��λ��
	   "repne\n\t"		// �Ƚ�al ��es:[edi]�ֽڣ�edi++��
	   "scasb\n\t"		// ֱ���ҵ�Ŀ�Ĵ���ĩ��0 ֵ�ֽڡ�
	   "decl %1\n\t"	// edi ָ���0 ֵ�ֽڡ�
	   "movl %4,%3\n"	// �������ֽ���??ecx��
	   "1:\tdecl %3\n\t"	// ecx--����0 ��ʼ��������
	   "js 2f\n\t"		// ecx <0 ?��������ǰ��ת�����2 ����
	   "lodsb\n\t"		// ����ȡds:[esi]�����ֽ�??al��esi++��
	   "stosb\n\t"		// �洢��es:[edi]����edi++��
	   "testb %%al,%%al\n\t"	// ���ֽ�ֵΪ0��
	   "jne 1b\n"		// �����������ת�����1 �����������ơ�
	   "2:\txorl %2,%2\n\t"	// ��al ���㡣
	   "stosb"		// �浽es:[edi]����
::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff), "g" (count):"si", "di", "ax",
	   "cx");
  return dest;			// ����Ŀ���ַ���ָ�롣
}*/

//// ��һ���ַ�������һ���ַ������бȽϡ�
// ������csrc - �ַ���1��ct - �ַ���2��
// %0 - eax(__res)����ֵ��%1 - edi(csrc)�ַ���1 ָ�룬%2 - esi(ct)�ַ���2 ָ�롣
// ���أ������1 > ��2���򷵻�1����1 = ��2���򷵻�0����1 < ��2���򷵻�-1��
extern _inline int
strcmp (const char *csrc, const char *ct)
{
//  register int __res;	// __res �ǼĴ�������(eax)��
  _asm{
	  pushf
	  mov edi,csrc
	  mov esi,ct
	  cld		// �巽��λ��
  l1: lodsb	// ȡ�ַ���2 ���ֽ�ds:[esi]??al������esi++��
	  scasb		// al ���ַ���1 ���ֽ�es:[edi]���Ƚϣ�����edi++��
	  jne l2		// �������ȣ�����ǰ��ת�����2��
	  test al,al	// ���ֽ���0 ֵ�ֽ����ַ�����β����
	  jne l1		// ���ǣ��������ת�����1�������Ƚϡ�
	  xor eax,eax	// �ǣ��򷵻�ֵeax ���㣬
	  jmp l3		// ��ǰ��ת�����3��������
  l2: mov eax,1	// eax ����1��
	  jl l3		// ��ǰ��Ƚ��д�2 �ַ�<��1 �ַ����򷵻���ֵ��������
	  neg eax	// ����eax = -eax�����ظ�ֵ��������
//  l3: mov __res,eax
  l3: popf
  }
//  return __res;			// ���رȽϽ����
}
/*extern _inline int
strcmp (const char *csrc, const char *ct)
{
  register int __res __asm__ ("ax");	// __res �ǼĴ�������(eax)��
  __asm__ ("cld\n"		// �巽��λ��
	   "1:\tlodsb\n\t"	// ȡ�ַ���2 ���ֽ�ds:[esi]??al������esi++��
	   "scasb\n\t"		// al ���ַ���1 ���ֽ�es:[edi]���Ƚϣ�����edi++��
	   "jne 2f\n\t"		// �������ȣ�����ǰ��ת�����2��
	   "testb %%al,%%al\n\t"	// ���ֽ���0 ֵ�ֽ����ַ�����β����
	   "jne 1b\n\t"		// ���ǣ��������ת�����1�������Ƚϡ�
	   "xorl %%eax,%%eax\n\t"	// �ǣ��򷵻�ֵeax ���㣬
	   "jmp 3f\n"		// ��ǰ��ת�����3��������
	   "2:\tmovl $1,%%eax\n\t"	// eax ����1��
	   "jl 3f\n\t"		// ��ǰ��Ƚ��д�2 �ַ�<��1 �ַ����򷵻���ֵ��������
	   "negl %%eax\n"	// ����eax = -eax�����ظ�ֵ��������
"3:": "=a" (__res): "D" (csrc), "S" (ct):"si", "di");
  return __res;			// ���رȽϽ����
}*/

//// �ַ���1 ���ַ���2 ��ǰcount ���ַ����бȽϡ�
// ������csrc - �ַ���1��ct - �ַ���2��count - �Ƚϵ��ַ�����
// %0 - eax(__res)����ֵ��%1 - edi(csrc)��1 ָ�룬%2 - esi(ct)��2 ָ�룬%3 - ecx(count)��
// ���أ������1 > ��2���򷵻�1����1 = ��2���򷵻�0����1 < ��2���򷵻�-1��
static _inline int
strncmp (const char *csrc, const char *ct, int count)
{
//  register int __res;	// __res �ǼĴ�������(eax)��
  _asm{
	  pushf
      mov edi,csrc
	  mov esi,ct
	  mov ecx,count
	  cld		// �巽��λ��
  l1: dec ecx	// count--��
	  js l2		// ���count<0������ǰ��ת�����2��
	  lodsb		// ȡ��2 ���ַ�ds:[esi]??al������esi++��
	  scasb		// �Ƚ�al �봮1 ���ַ�es:[edi]������edi++��
	  jne l3		// �������ȣ�����ǰ��ת�����3��
	  test al,al	// ���ַ���NULL �ַ���
	  jne l1		// ���ǣ��������ת�����1�������Ƚϡ�
  l2: xor eax,eax	// ��NULL �ַ�����eax ���㣨����ֵ����
	  jmp l4		// ��ǰ��ת�����4��������
  l3: mov eax,1	// eax ����1��
	  jl l4		// ���ǰ��Ƚ��д�2 �ַ�<��2 �ַ����򷵻�1��������
	  neg eax	// ����eax = -eax�����ظ�ֵ��������
//  l4: mov __res,eax
  l4: popf
  }
//  return __res;			// ���رȽϽ����
}
/*extern _inline int
strncmp (const char *csrc, const char *ct, int count)
{
  register int __res __asm__ ("ax");	// __res �ǼĴ�������(eax)��
  __asm__ ("cld\n"		// �巽��λ��
	   "1:\tdecl %3\n\t"	// count--��
	   "js 2f\n\t"		// ���count<0������ǰ��ת�����2��
	   "lodsb\n\t"		// ȡ��2 ���ַ�ds:[esi]??al������esi++��
	   "scasb\n\t"		// �Ƚ�al �봮1 ���ַ�es:[edi]������edi++��
	   "jne 3f\n\t"		// �������ȣ�����ǰ��ת�����3��
	   "testb %%al,%%al\n\t"	// ���ַ���NULL �ַ���
	   "jne 1b\n"		// ���ǣ��������ת�����1�������Ƚϡ�
	   "2:\txorl %%eax,%%eax\n\t"	// ��NULL �ַ�����eax ���㣨����ֵ����
	   "jmp 4f\n"		// ��ǰ��ת�����4��������
	   "3:\tmovl $1,%%eax\n\t"	// eax ����1��
	   "jl 4f\n\t"		// ���ǰ��Ƚ��д�2 �ַ�<��2 �ַ����򷵻�1��������
	   "negl %%eax\n"	// ����eax = -eax�����ظ�ֵ��������
"4:": "=a" (__res): "D" (csrc), "S" (ct), "c" (count):"si", "di",
	   "cx");
  return __res;			// ���رȽϽ����
}*/

//// ���ַ�����Ѱ�ҵ�һ��ƥ����ַ���
// ������s - �ַ�����c - ��Ѱ�ҵ��ַ���
// %0 - eax(__res)��%1 - esi(�ַ���ָ��s)��%2 - eax(�ַ�c)��
// ���أ������ַ����е�һ�γ���ƥ���ַ���ָ�롣��û���ҵ�ƥ����ַ����򷵻ؿ�ָ�롣
static _inline char *
strchr (const char *s, char c)
{
//  register char *__res;	// __res �ǼĴ�������(eax)��
  _asm{
	  pushf
	  mov esi,s
	  mov ah,c
	  cld		// �巽��λ��
  l1: lodsb	// ȡ�ַ������ַ�ds:[esi]->al������esi++��
	  cmp al,ah	// �ַ������ַ�al ��ָ���ַ�ah ��Ƚϡ�
	  je l2		// ����ȣ�����ǰ��ת�����2 ����
	  test al,al	// al ���ַ���NULL �ַ��𣿣��ַ�����β����
	  jne l1		// �����ǣ��������ת�����1�������Ƚϡ�
	  mov esi,1	// �ǣ���˵��û���ҵ�ƥ���ַ���esi ��1��
  l2: mov eax,esi	// ��ָ��ƥ���ַ���һ���ֽڴ���ָ��ֵ����eax
	  dec eax		// ��ָ�����Ϊָ��ƥ����ַ���
//	  mov __res,eax
	  popf
  }
//  return __res;			// ����ָ�롣
}
/*extern _inline char *
strchr (const char *s, char c)
{
  register char *__res __asm__ ("ax");	// __res �ǼĴ�������(eax)��
  __asm__ ("cld\n\t"		// �巽��λ��
	   "movb %%al,%%ah\n"	// �����Ƚ��ַ��Ƶ�ah��
	   "1:\tlodsb\n\t"	// ȡ�ַ������ַ�ds:[esi]??al������esi++��
	   "cmpb %%ah,%%al\n\t"	// �ַ������ַ�al ��ָ���ַ�ah ��Ƚϡ�
	   "je 2f\n\t"		// ����ȣ�����ǰ��ת�����2 ����
	   "testb %%al,%%al\n\t"	// al ���ַ���NULL �ַ��𣿣��ַ�����β����
	   "jne 1b\n\t"		// �����ǣ��������ת�����1�������Ƚϡ�
	   "movl $1,%1\n"	// �ǣ���˵��û���ҵ�ƥ���ַ���esi ��1��
	   "2:\tmovl %1,%0\n\t"	// ��ָ��ƥ���ַ���һ���ֽڴ���ָ��ֵ����eax
	   "decl %0"		// ��ָ�����Ϊָ��ƥ����ַ���
: "=a" (__res): "S" (s), "" (c):"si");
  return __res;			// ����ָ�롣
}*/

//// Ѱ���ַ�����ָ���ַ����һ�γ��ֵĵط��������������ַ�����
// ������s - �ַ�����c - ��Ѱ�ҵ��ַ���
// %0 - edx(__res)��%1 - edx(0)��%2 - esi(�ַ���ָ��s)��%3 - eax(�ַ�c)��
// ���أ������ַ��������һ�γ���ƥ���ַ���ָ�롣��û���ҵ�ƥ����ַ����򷵻ؿ�ָ�롣
static _inline char *
strrchr (const char *s, char c)
{
//  register char *__res;	// __res �ǼĴ�������(edx)��
  _asm{
	  pushf
	  xor edx,edx
	  mov esi,s
	  mov ah,c
	  cld		// �巽��λ��
  l1: lodsb	// ȡ�ַ������ַ�ds:[esi]->al������esi++��
	  cmp al,ah	// �ַ������ַ�al ��ָ���ַ�ah ���Ƚϡ�
	  jne l2		// ������ȣ�����ǰ��ת�����2 ����
	  mov edx,esi	// ���ַ�ָ�뱣�浽edx �С�
	  dec edx		// ָ�����һλ��ָ���ַ�����ƥ���ַ�����
  l2: test al,al	// �Ƚϵ��ַ���0 �𣨵��ַ���β����
	  jne l1	// �����������ת�����1 ���������Ƚϡ�
//	  mov __res,edx
      mov eax,edx
	  popf
  }
//  return __res;			// ����ָ�롣
}
/*extern _inline char *
strrchr (const char *s, char c)
{
  register char *__res __asm__ ("dx");	// __res �ǼĴ�������(edx)��
  __asm__ ("cld\n\t"		// �巽��λ��
	   "movb %%al,%%ah\n"	// ����Ѱ�ҵ��ַ��Ƶ�ah��
	   "1:\tlodsb\n\t"	// ȡ�ַ������ַ�ds:[esi]??al������esi++��
	   "cmpb %%ah,%%al\n\t"	// �ַ������ַ�al ��ָ���ַ�ah ���Ƚϡ�
	   "jne 2f\n\t"		// ������ȣ�����ǰ��ת�����2 ����
	   "movl %%esi,%0\n\t"	// ���ַ�ָ�뱣�浽edx �С�
	   "decl %0\n"		// ָ�����һλ��ָ���ַ�����ƥ���ַ�����
	   "2:\ttestb %%al,%%al\n\t"	// �Ƚϵ��ַ���0 �𣨵��ַ���β����
	   "jne 1b"		// �����������ת�����1 ���������Ƚϡ�
: "=d" (__res): "" (0), "S" (s), "a" (c):"ax", "si");
  return __res;			// ����ָ�롣
}*/

//// ���ַ���1 ��Ѱ�ҵ�1 ���ַ����У����ַ������е��κ��ַ����������ַ���2 �С�
// ������csrc - �ַ���1 ָ�룬ct - �ַ���2 ָ�롣
// %0 - esi(__res)��%1 - eax(0)��%2 - ecx(-1)��%3 - esi(��1 ָ��csrc)��%4 - (��2 ָ��ct)��
// �����ַ���1 �а����ַ���2 ���κ��ַ����׸��ַ����еĳ���ֵ��
extern _inline int
strspn (const char *csrc, const char *ct)
{
  register char *__res;	// __res �ǼĴ�������(esi)��
  _asm{
	  pushf
	  xor al,al
	  mov ebx,ct
	  mov edi,ebx	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
	  mov ecx,0xffffffff
	  cld		// �巽��λ��
	  repne scasb// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++���������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
	  not ecx	// ecx ��ÿλȡ����
	  dec ecx	// ecx--���ô�2 �ĳ���ֵ��-> ecx
	  mov edx,ecx	// ����2 �ĳ���ֵ�ݷ���edx �С�
	  mov esi,csrc
  l1: lodsb	// ȡ��1 �ַ�ds:[esi]->al������esi++��
	  test al,al	// ���ַ�����0 ֵ�𣨴�1 ��β����
	  je l2		// ����ǣ�����ǰ��ת�����2 ����
	  mov edi,ebx	// ȡ��2 ͷָ�����edi �С�
	  mov ecx,edx	// �ٽ���2 �ĳ���ֵ����ecx �С�
	  repne scasb	// �Ƚ�al �봮2 ���ַ�es:[edi]������edi++���������Ⱦͼ����Ƚϡ�
	  je l1		// �����ȣ��������ת�����1 ����
  l2: dec esi	// esi--��ָ�����һ�������ڴ�2 �е��ַ���
	  mov __res,esi
	  popf
  }
  return __res - csrc;		// �����ַ����еĳ���ֵ��
}
/*extern _inline int
strspn (const char *csrc, const char *ct)
{
  register char *__res __asm__ ("si");	// __res �ǼĴ�������(esi)��
  __asm__ ("cld\n\t"		// �巽��λ��
	   "movl %4,%%edi\n\t"	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
	   "repne\n\t"		// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
	   "notl %%ecx\n\t"	// ecx ��ÿλȡ����
	   "decl %%ecx\n\t"	// ecx--���ô�2 �ĳ���ֵ��
	   "movl %%ecx,%%edx\n"	// ����2 �ĳ���ֵ�ݷ���edx �С�
	   "1:\tlodsb\n\t"	// ȡ��1 �ַ�ds:[esi]??al������esi++��
	   "testb %%al,%%al\n\t"	// ���ַ�����0 ֵ�𣨴�1 ��β����
	   "je 2f\n\t"		// ����ǣ�����ǰ��ת�����2 ����
	   "movl %4,%%edi\n\t"	// ȡ��2 ͷָ�����edi �С�
	   "movl %%edx,%%ecx\n\t"	// �ٽ���2 �ĳ���ֵ����ecx �С�
	   "repne\n\t"		// �Ƚ�al �봮2 ���ַ�es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚϡ�
	   "je 1b\n"		// �����ȣ��������ת�����1 ����
	   "2:\tdecl %0"	// esi--��ָ�����һ�������ڴ�2 �е��ַ���
: "=S" (__res): "a" (0), "c" (0xffffffff), "" (csrc), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res - csrc;		// �����ַ����еĳ���ֵ��
}*/

//// Ѱ���ַ���1 �в������ַ���2 ���κ��ַ����׸��ַ����С�
// ������csrc - �ַ���1 ָ�룬ct - �ַ���2 ָ�롣
// %0 - esi(__res)��%1 - eax(0)��%2 - ecx(-1)��%3 - esi(��1 ָ��csrc)��%4 - (��2 ָ��ct)��
// �����ַ���1 �в������ַ���2 ���κ��ַ����׸��ַ����еĳ���ֵ��
extern _inline int
strcsrcpn (const char *csrc, const char *ct)
{
  register char *__res;	// __res �ǼĴ�������(esi)��
	_asm{
		pushf
		xor al,al
		mov ecx,0xffffffff
		mov ebx,ct
		mov edi,ebx	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
		cld		// �巽��λ��
		repne scasb// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++���������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
		not ecx	// ecx ��ÿλȡ����
		dec ecx	// ecx--���ô�2 �ĳ���ֵ��
		mov edx,ecx	// ����2 �ĳ���ֵ�ݷ���edx �С�
		mov esi,csrc
	l1: lodsb	// ȡ��1 �ַ�ds:[esi]->al������esi++��
		test al,al	// ���ַ�����0 ֵ�𣨴�1 ��β����
		je l2		// ����ǣ�����ǰ��ת�����2 ����
		mov edi,ebx	// ȡ��2 ͷָ�����edi �С�
		mov ecx,edx 	// �ٽ���2 �ĳ���ֵ����ecx �С�
		repne scasb	// �Ƚ�al �봮2 ���ַ�es:[edi]������edi++���������Ⱦͼ����Ƚϡ�
		jne l1		// �������ȣ��������ת�����1 ����
	l2: dec esi
		mov __res,esi	// esi--��ָ�����һ�������ڴ�2 �е��ַ���
		popf
	}
  return __res - csrc;		// �����ַ����еĳ���ֵ��
}
/*extern _inline int
strcsrcpn (const char *csrc, const char *ct)
{
  register char *__res __asm__ ("si");	// __res �ǼĴ�������(esi)��
  __asm__ ("cld\n\t"		// �巽��λ��
	   "movl %4,%%edi\n\t"	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
	   "repne\n\t"		// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
	   "notl %%ecx\n\t"	// ecx ��ÿλȡ����
	   "decl %%ecx\n\t"	// ecx--���ô�2 �ĳ���ֵ��
	   "movl %%ecx,%%edx\n"	// ����2 �ĳ���ֵ�ݷ���edx �С�
	   "1:\tlodsb\n\t"	// ȡ��1 �ַ�ds:[esi]??al������esi++��
	   "testb %%al,%%al\n\t"	// ���ַ�����0 ֵ�𣨴�1 ��β����
	   "je 2f\n\t"		// ����ǣ�����ǰ��ת�����2 ����
	   "movl %4,%%edi\n\t"	// ȡ��2 ͷָ�����edi �С�
	   "movl %%edx,%%ecx\n\t"	// �ٽ���2 �ĳ���ֵ����ecx �С�
	   "repne\n\t"		// �Ƚ�al �봮2 ���ַ�es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚϡ�
	   "jne 1b\n"		// �������ȣ��������ת�����1 ����
	   "2:\tdecl %0"	// esi--��ָ�����һ�������ڴ�2 �е��ַ���
: "=S" (__res): "a" (0), "c" (0xffffffff), "" (csrc), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res - csrc;		// �����ַ����еĳ���ֵ��
}*/

//// ���ַ���1 ��Ѱ���׸��������ַ���2 �е��κ��ַ���
// ������csrc - �ַ���1 ��ָ�룬ct - �ַ���2 ��ָ�롣
// %0 -esi(__res)��%1 -eax(0)��%2 -ecx(0xffffffff)��%3 -esi(��1 ָ��csrc)��%4 -(��2 ָ��ct)��
// �����ַ���1 ���׸������ַ���2 ���ַ���ָ�롣
extern _inline char *
strpbrk (const char *csrc, const char *ct)
{
//  register char *__res;	// __res �ǼĴ�������(esi)��
	_asm{
		pushf
		xor al,al
		mov ebx,ct
		mov edi,ebx	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
		mov ecx,0xffffffff
		cld		// �巽��λ��
		repne scasb// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++���������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
		not ecx	// ecx ��ÿλȡ����
		dec ecx	// ecx--���ô�2 �ĳ���ֵ��
		mov edx,ecx	// ����2 �ĳ���ֵ�ݷ���edx �С�
		mov esi,csrc
	l1: lodsb	// ȡ��1 �ַ�ds:[esi]??al������esi++��
		test al,al	// ���ַ�����0 ֵ�𣨴�1 ��β����
		je l2		// ����ǣ�����ǰ��ת�����2 ����
		mov edi,ebx	// ȡ��2 ͷָ�����edi �С�
		mov ecx,edx	// �ٽ���2 �ĳ���ֵ����ecx �С�
		repne scasb		// �Ƚ�al �봮2 ���ַ�es:[edi]������edi++���������Ⱦͼ����Ƚϡ�
		jne l1	// �������ȣ��������ת�����1 ����
		dec esi	// esi--��ָ��һ�������ڴ�2 �е��ַ���
		jmp l3		// ��ǰ��ת�����3 ����
	l2: xor esi,esi	// û���ҵ����������ģ�������ֵΪNULL��
//	l3: mov __res,esi
	l3: mov eax,esi
		popf
	}
//  return __res;			// ����ָ��ֵ��
}
/*extern _inline char *
strpbrk (const char *csrc, const char *ct)
{
  register char *__res __asm__ ("si");	// __res �ǼĴ�������(esi)��
  __asm__ ("cld\n\t"		// �巽��λ��
	   "movl %4,%%edi\n\t"	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
	   "repne\n\t"		// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
	   "notl %%ecx\n\t"	// ecx ��ÿλȡ����
	   "decl %%ecx\n\t"	// ecx--���ô�2 �ĳ���ֵ��
	   "movl %%ecx,%%edx\n"	// ����2 �ĳ���ֵ�ݷ���edx �С�
	   "1:\tlodsb\n\t"	// ȡ��1 �ַ�ds:[esi]??al������esi++��
	   "testb %%al,%%al\n\t"	// ���ַ�����0 ֵ�𣨴�1 ��β����
	   "je 2f\n\t"		// ����ǣ�����ǰ��ת�����2 ����
	   "movl %4,%%edi\n\t"	// ȡ��2 ͷָ�����edi �С�
	   "movl %%edx,%%ecx\n\t"	// �ٽ���2 �ĳ���ֵ����ecx �С�
	   "repne\n\t"		// �Ƚ�al �봮2 ���ַ�es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚϡ�
	   "jne 1b\n\t"		// �������ȣ��������ת�����1 ����
	   "decl %0\n\t"	// esi--��ָ��һ�������ڴ�2 �е��ַ���
	   "jmp 3f\n"		// ��ǰ��ת�����3 ����
	   "2:\txorl %0,%0\n"	// û���ҵ����������ģ�������ֵΪNULL��
"3:": "=S" (__res): "a" (0), "c" (0xffffffff), "" (csrc), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res;			// ����ָ��ֵ��
}*/

//// ���ַ���1 ��Ѱ���׸�ƥ�������ַ���2 ���ַ�����
// ������csrc - �ַ���1 ��ָ�룬ct - �ַ���2 ��ָ�롣
// %0 -eax(__res)��%1 -eax(0)��%2 -ecx(0xffffffff)��%3 -esi(��1 ָ��csrc)��%4 -(��2 ָ��ct)��
// ���أ������ַ���1 ���׸�ƥ���ַ���2 ���ַ���ָ�롣
extern _inline char *
strstr (const char *csrc, const char *ct)
{
//  register char *__res;	// __res �ǼĴ�������(eax)��
	_asm {
		pushf
		mov ebx,ct
		mov edi,ebx	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
		mov ecx,0xffffffff
		xor al,al	// al = 0
		cld 		// �巽��λ��
		repne scasb// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++���������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
		not ecx	// ecx ��ÿλȡ����
		dec ecx
// ע�⣡���������Ϊ�գ�������Z ��־ // �ô�2 �ĳ���ֵ��
		mov edx,ecx	// ����2 �ĳ���ֵ�ݷ���edx �С�
		mov esi,csrc
	l1: mov edi,ebx	// ȡ��2 ͷָ�����edi �С�
		mov ecx,edx	// �ٽ���2 �ĳ���ֵ����ecx �С�
		mov eax,esi	// ����1 ��ָ�븴�Ƶ�eax �С�
		repe cmpsb// �Ƚϴ�1 �ʹ�2 �ַ�(ds:[esi],es:[edi])��esi++,edi++������Ӧ�ַ���Ⱦ�һֱ�Ƚ���ȥ��
		je l2
// �Կմ�ͬ����Ч�������� // ��ȫ��ȣ���ת�����2��
		xchg esi,eax	// ��1 ͷָ��->esi���ȽϽ���Ĵ�1 ָ��->eax��
		inc esi	// ��1 ͷָ��ָ����һ���ַ���
		cmp [eax-1],0	// ��1 ָ��(eax-1)��ָ�ֽ���0 ��
		jne l1	// ��������ת�����1�������Ӵ�1 �ĵ�2 ���ַ���ʼ�Ƚϡ�
		xor eax,eax	// ��eax����ʾû���ҵ�ƥ�䡣
//	l2: mov __res,eax
	l2:	popf
	}
//  return __res;			// ���رȽϽ����
}
/*extern _inline char *
strstr (const char *csrc, const char *ct)
{
  register char *__res __asm__ ("ax");	// __res �ǼĴ�������(eax)��
  __asm__ ("cld\n\t" \		// �巽��λ��
	   "movl %4,%%edi\n\t"	// ���ȼ��㴮2 �ĳ��ȡ���2 ָ�����edi �С�
	   "repne\n\t"		// �Ƚ�al(0)�봮2 �е��ַ���es:[edi]������edi++��
	   "scasb\n\t"		// �������Ⱦͼ����Ƚ�(ecx �𲽵ݼ�)��
	   "notl %%ecx\n\t"	// ecx ��ÿλȡ����
	   "decl %%ecx\n\t"	// NOTE! This also sets Z if searchstring=''
// ע�⣡���������Ϊ�գ�������Z ��־ // �ô�2 �ĳ���ֵ��
	   "movl %%ecx,%%edx\n"	// ����2 �ĳ���ֵ�ݷ���edx �С�
	   "1:\tmovl %4,%%edi\n\t"	// ȡ��2 ͷָ�����edi �С�
	   "movl %%esi,%%eax\n\t"	// ����1 ��ָ�븴�Ƶ�eax �С�
	   "movl %%edx,%%ecx\n\t"	// �ٽ���2 �ĳ���ֵ����ecx �С�
	   "repe\n\t"		// �Ƚϴ�1 �ʹ�2 �ַ�(ds:[esi],es:[edi])��esi++,edi++��
	   "cmpsb\n\t"		// ����Ӧ�ַ���Ⱦ�һֱ�Ƚ���ȥ��
	   "je 2f\n\t"		// also works for empty string, see above 
// �Կմ�ͬ����Ч�������� // ��ȫ��ȣ���ת�����2��
	   "xchgl %%eax,%%esi\n\t"	// ��1 ͷָ��??esi���ȽϽ���Ĵ�1 ָ��??eax��
	   "incl %%esi\n\t"	// ��1 ͷָ��ָ����һ���ַ���
	   "cmpb $0,-1(%%eax)\n\t"	// ��1 ָ��(eax-1)��ָ�ֽ���0 ��
	   "jne 1b\n\t"		// ��������ת�����1�������Ӵ�1 �ĵ�2 ���ַ���ʼ�Ƚϡ�
	   "xorl %%eax,%%eax\n\t"	// ��eax����ʾû���ҵ�ƥ�䡣
"2:": "=a" (__res): "" (0), "c" (0xffffffff), "S" (csrc), "g" (ct):"cx", "dx", "di",
	   "si");
  return __res;			// ���رȽϽ����
}*/

//// �����ַ������ȡ�
// ������s - �ַ�����
// %0 - ecx(__res)��%1 - edi(�ַ���ָ��s)��%2 - eax(0)��%3 - ecx(0xffffffff)��
// ���أ������ַ����ĳ��ȡ�
extern _inline int
strlen (const char *s)
{
//  register int __res;	// __res �ǼĴ�������(ecx)��
	_asm{
		pushf
		mov edi,s
		mov ecx,0xffffffff
		xor al,al
		cld		// �巽��λ��
		repne scasb		// al(0)���ַ������ַ�es:[edi]�Ƚϣ�������Ⱦ�һֱ�Ƚϡ�
		not ecx	// ecx ȡ����
		dec ecx		// ecx--�����ַ����ó���ֵ��
//		mov __res,ecx
		mov eax,ecx
		popf
	}
//  return __res;			// �����ַ�������ֵ��
}
/*extern _inline int
strlen (const char *s)
{
  register int __res __asm__ ("cx");	// __res �ǼĴ�������(ecx)��
  __asm__ ("cld\n\t"		// �巽��λ��
	   "repne\n\t"		// al(0)���ַ������ַ�es:[edi]�Ƚϣ�
	   "scasb\n\t"		// ������Ⱦ�һֱ�Ƚϡ�
	   "notl %0\n\t"	// ecx ȡ����
	   "decl %0"		// ecx--�����ַ����ó���ֵ��
: "=c" (__res): "D" (s), "a" (0), "" (0xffffffff):"di");
  return __res;			// �����ַ�������ֵ��
}*/

extern char *___strtok;		// ������ʱ���ָ�����汻�����ַ���1(s)��ָ�롣

//// �����ַ���2 �е��ַ����ַ���1 �ָ�ɱ��(tokern)���С�
// ����1 �����ǰ��������������(token)�����У����ɷָ���ַ���2 �е�һ�������ַ��ֿ���
// ��һ�ε���strtok()ʱ��������ָ���ַ���1 �е�1 ��token ���ַ���ָ�룬���ڷ���token ʱ��
// һnull �ַ�д���ָ����������ʹ��null ��Ϊ�ַ���1 �ĵ��ã��������ַ�������ɨ���ַ���1��
// ֱ��û��token Ϊֹ���ڲ�ͬ�ĵ��ù����У��ָ����2 ���Բ�ͬ��
// ������s - ��������ַ���1��ct - ���������ָ�����ַ���2��
// ��������%0 - ebx(__res)��%1 - esi(__strtok)��
// ������룺%2 - ebx(__strtok)��%3 - esi(�ַ���1 ָ��s)��%4 - ���ַ���2 ָ��ct����
// ���أ������ַ���s �е�1 ��token�����û���ҵ�token���򷵻�һ��null ָ�롣
// ����ʹ���ַ���s ָ��Ϊnull �ĵ��ã�����ԭ�ַ���s ��������һ��token��
extern _inline char *
strtok (char *s, register const char *ct)
{
//  register char *__res;
	_asm {
		pushf
		mov esi,s
		test esi,esi	// ���Ȳ���esi(�ַ���1 ָ��s)�Ƿ���NULL��
		jne l1		// ������ǣ���������״ε��ñ���������ת���1��
		mov ebx,strtok
		test ebx,ebx	// �����NULL�����ʾ�˴��Ǻ������ã���ebx(__strtok)��
		je l8		// ���ebx ָ����NULL�����ܴ�����ת������
		mov esi,ebx	// ��ebx ָ�븴�Ƶ�esi��
	l1: xor ebx,ebx	// ��ebx ָ�롣
		mov edi,ct	// �������ַ���2 �ĳ��ȡ�edi ָ���ַ���2��
		mov ecx,0xffffffff 	// ��ecx = 0xffffffff��
		xor eax,eax	// ����eax��
		cld
		repne scasb// ��al(0)��es:[edi]�Ƚϣ�����edi++��ֱ���ҵ��ַ���2 �Ľ���null �ַ��������ecx==0��
		not ecx	// ��ecx ȡ����
		dec ecx	// ecx--���õ��ַ���2 �ĳ���ֵ��
		je l7	// �ָ���ַ����� // ����2 ����Ϊ0����ת���7��
		mov edx,ecx	// ����2 �����ݴ���edx��
	l2: lodsb	// ȡ��1 ���ַ�ds:[esi]->al������esi++��
		test al,al	// ���ַ�Ϊ0 ֵ��(��1 ����)��
		je l7		// ����ǣ�����ת���7��
		mov edi,ct	// edi �ٴ�ָ��2 �ס�
		mov ecx,edx	// ȡ��2 �ĳ���ֵ���������ecx��
		repne scasb// ��al �д�1 ���ַ��봮2 �������ַ��Ƚϣ��жϸ��ַ��Ƿ�Ϊ�ָ����
		je l2		// �����ڴ�2 ���ҵ���ͬ�ַ����ָ����������ת���2��
		dec esi	// �����Ƿָ������1 ָ��esi ָ���ʱ�ĸ��ַ���
		cmp [esi],0	// ���ַ���NULL �ַ���
		je l7		// ���ǣ�����ת���7 ����
		mov ebx,esi	// �����ַ���ָ��esi �����ebx��
	l3: lodsb	// ȡ��1 ��һ���ַ�ds:[esi]->al������esi++��
		test al,al	// ���ַ���NULL �ַ���
		je l5		// ���ǣ���ʾ��1 ��������ת�����5��
		mov edi,ct	// edi �ٴ�ָ��2 �ס�
		mov ecx,edx	// ��2 ����ֵ���������ecx��
		repne scasb // ��al �д�1 ���ַ��봮2 ��ÿ���ַ��Ƚϣ�����al �ַ��Ƿ��Ƿָ����
		jne l3		// �����Ƿָ������ת���3����⴮1 ����һ���ַ���
		dec esi	// ���Ƿָ������esi--��ָ��÷ָ���ַ���
		cmp [esi],0	// �÷ָ����NULL �ַ���
		je l5		// ���ǣ�����ת�����5��
		mov [esi],0	// �����ǣ��򽫸÷ָ����NULL �ַ��滻����
		inc esi	// esi ָ��1 ����һ���ַ���Ҳ��ʣ�മ�ס�
		jmp l6		// ��ת���6 ����
	l5: xor esi,esi	// esi ���㡣
	l6: cmp [ebx],0	// ebx ָ��ָ��NULL �ַ���
		jne l7	// �����ǣ�����ת���7��
		xor ebx,ebx	// ���ǣ�����ebx=NULL��
	l7: test ebx,ebx	// ebx ָ��ΪNULL ��
		jne l8	// ����������ת8�����������롣
		mov esi,ebx	// ��esi ��ΪNULL��
//	l8: mov __res,esi
	l8: mov eax,esi
		popf
	}
//  return __res;			// ����ָ����token ��ָ�롣
}
/*extern _inline char *
strtok (char *s, const char *ct)
{
  register char *__res __asm__ ("si");
  __asm__ ("testl %1,%1\n\t"	// ���Ȳ���esi(�ַ���1 ָ��s)�Ƿ���NULL��
	   "jne 1f\n\t"		// ������ǣ���������״ε��ñ���������ת���1��
	   "testl %0,%0\n\t"	// �����NULL�����ʾ�˴��Ǻ������ã���ebx(__strtok)��
	   "je 8f\n\t"		// ���ebx ָ����NULL�����ܴ�����ת������
	   "movl %0,%1\n"	// ��ebx ָ�븴�Ƶ�esi��
	   "1:\txorl %0,%0\n\t"	// ��ebx ָ�롣
	   "movl $-1,%%ecx\n\t"	// ��ecx = 0xffffffff��
	   "xorl %%eax,%%eax\n\t"	// ����eax��
	   "cld\n\t"		// �巽��λ��
	   "movl %4,%%edi\n\t"	// �������ַ���2 �ĳ��ȡ�edi ָ���ַ���2��
	   "repne\n\t"		// ��al(0)��es:[edi]�Ƚϣ�����edi++��
	   "scasb\n\t"		// ֱ���ҵ��ַ���2 �Ľ���null �ַ��������ecx==0��
	   "notl %%ecx\n\t"	// ��ecx ȡ����
	   "decl %%ecx\n\t"	// ecx--���õ��ַ���2 �ĳ���ֵ��
	   "je 7f\n\t"		// empty delimeter-string 
// �ָ���ַ����� // ����2 ����Ϊ0����ת���7��
	   "movl %%ecx,%%edx\n"	// ����2 �����ݴ���edx��
	   "2:\tlodsb\n\t"	// ȡ��1 ���ַ�ds:[esi]??al������esi++��
	   "testb %%al,%%al\n\t"	// ���ַ�Ϊ0 ֵ��(��1 ����)��
	   "je 7f\n\t"		// ����ǣ�����ת���7��
	   "movl %4,%%edi\n\t"	// edi �ٴ�ָ��2 �ס�
	   "movl %%edx,%%ecx\n\t"	// ȡ��2 �ĳ���ֵ���������ecx��
	   "repne\n\t"		// ��al �д�1 ���ַ��봮2 �������ַ��Ƚϣ�
	   "scasb\n\t"		// �жϸ��ַ��Ƿ�Ϊ�ָ����
	   "je 2b\n\t"		// �����ڴ�2 ���ҵ���ͬ�ַ����ָ����������ת���2��
	   "decl %1\n\t"	// �����Ƿָ������1 ָ��esi ָ���ʱ�ĸ��ַ���
	   "cmpb $0,(%1)\n\t"	// ���ַ���NULL �ַ���
	   "je 7f\n\t"		// ���ǣ�����ת���7 ����
	   "movl %1,%0\n"	// �����ַ���ָ��esi �����ebx��
	   "3:\tlodsb\n\t"	// ȡ��1 ��һ���ַ�ds:[esi]??al������esi++��
	   "testb %%al,%%al\n\t"	// ���ַ���NULL �ַ���
	   "je 5f\n\t"		// ���ǣ���ʾ��1 ��������ת�����5��
	   "movl %4,%%edi\n\t"	// edi �ٴ�ָ��2 �ס�
	   "movl %%edx,%%ecx\n\t"	// ��2 ����ֵ���������ecx��
	   "repne\n\t"		// ��al �д�1 ���ַ��봮2 ��ÿ���ַ��Ƚϣ�
	   "scasb\n\t"		// ����al �ַ��Ƿ��Ƿָ����
	   "jne 3b\n\t"		// �����Ƿָ������ת���3����⴮1 ����һ���ַ���
	   "decl %1\n\t"	// ���Ƿָ������esi--��ָ��÷ָ���ַ���
	   "cmpb $0,(%1)\n\t"	// �÷ָ����NULL �ַ���
	   "je 5f\n\t"		// ���ǣ�����ת�����5��
	   "movb $0,(%1)\n\t"	// �����ǣ��򽫸÷ָ����NULL �ַ��滻����
	   "incl %1\n\t"	// esi ָ��1 ����һ���ַ���Ҳ��ʣ�മ�ס�
	   "jmp 6f\n"		// ��ת���6 ����
	   "5:\txorl %1,%1\n"	// esi ���㡣
	   "6:\tcmpb $0,(%0)\n\t"	// ebx ָ��ָ��NULL �ַ���
	   "jne 7f\n\t"		// �����ǣ�����ת���7��
	   "xorl %0,%0\n"	// ���ǣ�����ebx=NULL��
	   "7:\ttestl %0,%0\n\t"	// ebx ָ��ΪNULL ��
	   "jne 8f\n\t"		// ����������ת8�����������롣
	   "movl %0,%1\n"	// ��esi ��ΪNULL��
"8:": "=b" (__res), "=S" (___strtok): "" (___strtok), "1" (s), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res;			// ����ָ����token ��ָ�롣
}*/

//// �ڴ�鸴�ơ���Դ��ַsrc ����ʼ����n ���ֽڵ�Ŀ�ĵ�ַdest ����
// ������dest - ���Ƶ�Ŀ�ĵ�ַ��src - ���Ƶ�Դ��ַ��n - �����ֽ�����
// %0 - ecx(n)��%1 - esi(src)��%2 - edi(dest)��
extern _inline void *
memcpy (void *dest, const void *src, int n)
{
	_asm{
		pushf
		mov esi,src
		mov edi,dest
		mov ecx,n
		cld		// �巽��λ��
		rep movsb	// �ظ�ִ�и���ecx ���ֽڣ���ds:[esi]��es:[edi]��esi++��edi++��
		popf
	}
	return dest;			// ����Ŀ�ĵ�ַ��
}
/*extern _inline void *
memcpy (void *dest, const void *src, int n)
{
  __asm__ ("cld\n\t"		// �巽��λ��
	   "rep\n\t"		// �ظ�ִ�и���ecx ���ֽڣ�
	   "movsb"		// ��ds:[esi]��es:[edi]��esi++��edi++��
::"c" (n), "S" (src), "D" (dest):"cx", "si", "di");
  return dest;			// ����Ŀ�ĵ�ַ��
}*/

//// �ڴ���ƶ���ͬ�ڴ�鸴�ƣ��������ƶ��ķ���
// ������dest - ���Ƶ�Ŀ�ĵ�ַ��src - ���Ƶ�Դ��ַ��n - �����ֽ�����
// ��dest<src ��%0 - ecx(n)��%1 - esi(src)��%2 - edi(dest)��
// ����%0 - ecx(n)��%1 - esi(src+n-1)��%2 - edi(dest+n-1)��
// ����������Ϊ�˷�ֹ�ڸ���ʱ������ص����ǡ�
extern _inline void *
memmove (void *dest, const void *src, int n)
{
	if (dest < src) 
	{_asm {
		pushf
		mov esi,src
		mov edi,dest
		mov ecx,n
		cld		// �巽��λ��
		rep movsb// ��ds:[esi]��es:[edi]������esi++��edi++���ظ�ִ�и���ecx �ֽڡ�
		popf
	}}else{_asm {
		pushf
//		mov esi,src + n - 1
		mov esi,src
		add esi,n - 1
//		mov edi,dest + n - 1
		mov edi,dest
		add edi,n - 1
		mov ecx,n
		std		// �÷���λ����ĩ�˿�ʼ���ơ�
		rep movsb// ��ds:[esi]��es:[edi]������esi--��edi--������ecx ���ֽڡ�
		popf
	}}
  return dest;
}
/*extern _inline void *
memmove (void *dest, const void *src, int n)
{
  if (dest < src)
    __asm__ ("cld\n\t"		// �巽��λ��
	     "rep\n\t"		// ��ds:[esi]��es:[edi]������esi++��edi++��
	     "movsb"		// �ظ�ִ�и���ecx �ֽڡ�
  ::"c" (n), "S" (src), "D" (dest):"cx", "si", "di");
  else
    __asm__ ("std\n\t"		// �÷���λ����ĩ�˿�ʼ���ơ�
	     "rep\n\t"		// ��ds:[esi]��es:[edi]������esi--��edi--��
	     "movsb"		// ����ecx ���ֽڡ�
  ::"c" (n), "S" (src + n - 1), "D" (dest + n - 1):"cx", "si",
	     "di");
  return dest;
}*/

//// �Ƚ�n ���ֽڵ������ڴ棨�����ַ���������ʹ����NULL �ֽ�Ҳ��ֹͣ�Ƚϡ�
// ������csrc - �ڴ��1 ��ַ��ct - �ڴ��2 ��ַ��count - �Ƚϵ��ֽ�����
// %0 - eax(__res)��%1 - eax(0)��%2 - edi(�ڴ��1)��%3 - esi(�ڴ��2)��%4 - ecx(count)��
// ���أ�����1>��2 ����1����1<��2������-1����1==��2���򷵻�0��
extern _inline int
memcmp (const void *csrc, const void *ct, int count)
{
//  register int __res; //__asm__ ("ax")	 __res �ǼĴ���������
	_asm {
		pushf
		mov edi,csrc
		mov esi,ct
		xor eax,eax // eax = 0
		mov ecx,count
		cld		// �巽��λ��
		repe cmpsb// �Ƚ�ds:[esi]��es:[edi]�����ݣ�����esi++��edi++�����������ظ���
		je l1		// �������ͬ������ת�����1������0(eax)ֵ
		mov eax,1	// ����eax ��1��
		jl l1		// ���ڴ��2 ���ݵ�ֵ<�ڴ��1������ת���1��
		neg eax	// ����eax = -eax��
//	l1: mov __res,eax
	l1:	popf
	}
//  return __res;			// ���رȽϽ����
}
/*extern _inline int
memcmp (const void *csrc, const void *ct, int count)
{
  register int __res __asm__ ("ax");	// __res �ǼĴ���������
  __asm__ ("cld\n\t"		// �巽��λ��
	   "repe\n\t"		// ���������ظ���
	   "cmpsb\n\t"		// �Ƚ�ds:[esi]��es:[edi]�����ݣ�����esi++��edi++��
	   "je 1f\n\t"		// �������ͬ������ת�����1������0(eax)ֵ
	   "movl $1,%%eax\n\t"	// ����eax ��1��
	   "jl 1f\n\t"		// ���ڴ��2 ���ݵ�ֵ<�ڴ��1������ת���1��
	   "negl %%eax\n"	// ����eax = -eax��
"1:": "=a" (__res): "" (0), "D" (csrc), "S" (ct), "c" (count):"si", "di",
	   "cx");
  return __res;			// ���رȽϽ����
}*/

//// ��n �ֽڴ�С���ڴ��(�ַ���)��Ѱ��ָ���ַ���
// ������csrc - ָ���ڴ���ַ��c - ָ�����ַ���count - �ڴ�鳤�ȡ�
// %0 - edi(__res)��%1 - eax(�ַ�c)��%2 - edi(�ڴ���ַcsrc)��%3 - ecx(�ֽ���count)��
// ���ص�һ��ƥ���ַ���ָ�룬���û���ҵ����򷵻�NULL �ַ���
extern _inline void *
memchr (const void *csrc, char c, int count)
{
//  register void *__res;	// __res �ǼĴ���������
  if (!count)			// ����ڴ�鳤��==0���򷵻�NULL��û���ҵ���
    return NULL;
	_asm {
		pushf
		mov edi,csrc
		mov ecx,count
		mov al,c
		cld		// �巽��λ��
		repne scasb// al ���ַ���es:[edi]�ַ����Ƚϣ�����edi++�������������ظ�ִ��������䣬
		je l1		// ����������ǰ��ת�����1 ����
		mov edi,1	// ����edi ����1��
	l1: dec edi	// ��edi ָ���ҵ����ַ�������NULL����
//		mov __res,edi
		mov eax,edi
		popf
	}
//  return __res;			// �����ַ�ָ�롣
}
/*extern _inline void *
memchr (const void *csrc, char c, int count)
{
  register void *__res __asm__ ("di");	// __res �ǼĴ���������
  if (!count)			// ����ڴ�鳤��==0���򷵻�NULL��û���ҵ���
    return NULL;
  __asm__ ("cld\n\t"		// �巽��λ��
	   "repne\n\t"		// �����������ظ�ִ��������䣬
	   "scasb\n\t"		// al ���ַ���es:[edi]�ַ����Ƚϣ�����edi++��
	   "je 1f\n\t"		// ����������ǰ��ת�����1 ����
	   "movl $1,%0\n"	// ����edi ����1��
	   "1:\tdecl %0"	// ��edi ָ���ҵ����ַ�������NULL����
: "=D" (__res): "a" (c), "D" (csrc), "c" (count):"cx");
  return __res;			// �����ַ�ָ�롣
}*/

//// ���ַ���дָ�������ڴ�顣
// ���ַ�c ��дs ָ����ڴ����򣬹���count �ֽڡ�
// %0 - eax(�ַ�c)��%1 - edi(�ڴ��ַ)��%2 - ecx(�ֽ���count)��
extern _inline void *
memset (void *s, char c, int count)
{
	_asm {
		pushf
		mov edi,s
		mov ecx,count
		mov al,c
		cld		// �巽��λ��
		rep stosb// ��al ���ַ�����es:[edi]�У�����edi++���ظ�ecx ָ���Ĵ�����ִ��
		popf
	}
  return s;
}
/*extern _inline void *
memset (void *s, char c, int count)
{
  __asm__ ("cld\n\t"		// �巽��λ��
	   "rep\n\t"		// �ظ�ecx ָ���Ĵ�����ִ��
	   "stosb"		// ��al ���ַ�����es:[edi]�У�����edi++��
::"a" (c), "D" (s), "c" (count):"cx", "di");
  return s;
}*/

#endif
