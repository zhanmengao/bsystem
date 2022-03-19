//// ��ȡfs ����ָ����ַ�����ֽڡ�
// ������addr - ָ�����ڴ��ַ��
// %0 - (���ص��ֽ�_v)��%1 - (�ڴ��ַaddr)��
// ���أ������ڴ�fs:[addr]�����ֽڡ�
extern _inline unsigned char
get_fs_byte (const char *addr)
{
//  unsigned register char _v;

// __asm__ ("movb %%fs:%1,%0": "=r" (_v):"m" (*addr));
  _asm mov ebx,addr
  _asm mov al,byte ptr fs:[ebx];
//  _asm mov _v,al;
//  return _v;
}

//// ��ȡfs ����ָ����ַ�����֡�
// ������addr - ָ�����ڴ��ַ��
// %0 - (���ص���_v)��%1 - (�ڴ��ַaddr)��
// ���أ������ڴ�fs:[addr]�����֡�
extern _inline unsigned short
get_fs_word (const unsigned short *addr)
{
//  unsigned short _v;

//__asm__ ("movw %%fs:%1,%0": "=r" (_v):"m" (*addr));
  _asm mov ebx,addr
  _asm mov ax,word ptr fs:[ebx];
//  _asm mov _v,ax;
//  return _v;
}

//// ��ȡfs ����ָ����ַ���ĳ���(4 �ֽ�)��
// ������addr - ָ�����ڴ��ַ��
// %0 - (���صĳ���_v)��%1 - (�ڴ��ַaddr)��
// ���أ������ڴ�fs:[addr]���ĳ��֡�
extern _inline unsigned long
get_fs_long (const unsigned long *addr)
{
//  unsigned long _v;

//__asm__ ("movl %%fs:%1,%0": "=r" (_v):"m" (*addr));
  _asm mov ebx,addr
  _asm mov eax,dword ptr fs:[ebx];
//  _asm mov _v,eax;
//  return _v;
}

//// ��һ�ֽڴ����fs ����ָ���ڴ��ַ����
// ������val - �ֽ�ֵ��addr - �ڴ��ַ��
// %0 - �Ĵ���(�ֽ�ֵval)��%1 - (�ڴ��ַaddr)��
extern _inline void
put_fs_byte (char val, char *addr)//passed
{
//  __asm__ ("movb %0,%%fs:%1"::"r" (val), "m" (*addr));
	_asm mov ebx,addr
	_asm mov al,val;
	_asm mov byte ptr fs:[ebx],al;
}

//// ��һ�ִ����fs ����ָ���ڴ��ַ����
// ������val - ��ֵ��addr - �ڴ��ַ��
// %0 - �Ĵ���(��ֵval)��%1 - (�ڴ��ַaddr)��
extern _inline void
put_fs_word (short val, short *addr)
{
//  __asm__ ("movw %0,%%fs:%1"::"r" (val), "m" (*addr));
	_asm mov ebx,addr
	_asm mov ax,val;
	_asm mov word ptr fs:[ebx],ax;
}

//// ��һ���ִ����fs ����ָ���ڴ��ַ����
// ������val - ����ֵ��addr - �ڴ��ַ��
// %0 - �Ĵ���(����ֵval)��%1 - (�ڴ��ַaddr)��
extern _inline void
put_fs_long (long val, unsigned long *addr)
{
//  __asm__ ("movl %0,%%fs:%1"::"r" (val), "m" (*addr));
	_asm mov ebx,addr
	_asm mov eax,val;
	_asm mov dword ptr fs:[ebx],eax;
}

/*
* ���Ҹ���GNU ������Ӧ����ϸ�������Ĵ��롣��Щ������ʹ�ã����Ҳ�֪���Ƿ�
* ����һЩС����
* --- TYT��1991 ��11 ��24 ��
* [ ��Щ����û�д���Linus ]
*/

//// ȡfs �μĴ���ֵ(ѡ���)��
// ���أ�fs �μĴ���ֵ��
extern _inline unsigned short
get_fs ()
{
//  unsigned short _v;
//__asm__ ("mov %%fs,%%ax": "=a" (_v):);
  _asm mov ax,fs;
//  _asm mov _v,ax;
//  return _v;
}

//// ȡds �μĴ���ֵ��
// ���أ�ds �μĴ���ֵ��
extern _inline unsigned short
get_ds ()
{
//  unsigned short _v;
//__asm__ ("mov %%ds,%%ax": "=a" (_v):);
  _asm mov ax,fs;
//  _asm mov _v,ax;
//  return _v;
}

//// ����fs �μĴ�����
// ������val - ��ֵ��ѡ�������
extern _inline void
set_fs (unsigned long val)
{
//  __asm__ ("mov %0,%%fs"::"a" ((unsigned short) val));
	_asm mov eax,val;
	_asm mov fs,ax;
}
