// ��֪��Ϊʲô�����ﶨ��memcpy��string.h�ļ����Ѿ���һ���ˡ�
/*
* ע��!!!memcpy(dest,src,n)����μĴ���ds=es=ͨ�����ݶΡ����ں���ʹ�õ�
* ���к��������ڸü��裨ds=es=�ں˿ռ䣬fs=�ֲ����ݿռ䣬gs=null��,��������
* ��Ϊ��Ӧ�ó���Ҳ��������ds=es=�û����ݿռ䣩������κ��û���������Ķ���
* es �Ĵ����������򲢲�������ϵͳ���������ɵġ�
*/
//// �ڴ�鸴�ơ���Դ��ַsrc ����ʼ����n ���ֽڵ�Ŀ�ĵ�ַdest ����
// ������dest - ���Ƶ�Ŀ�ĵ�ַ��src - ���Ƶ�Դ��ַ��n - �����ֽ�����
// %0 - edi(Ŀ�ĵ�ַdest)��%1 - esi(Դ��ַsrc)��%2 - ecx(�ֽ���n)��
/*extern _inline void* memcpy(void * dest, const void * src, int n)
{
	_asm{
		pushf
		mov esi,src
		mov edi,dest
		mov ecx,n	// ������ecx(n)�ֽڡ�
		cld
		rep movsb // ��ds:[esi]���Ƶ�es:[edi]������esi++��edi++��
		popf
	}
	return dest;
}
#define memcpy(dest,src,n) ({ \
void * _res = dest; \
__asm__ ( "cld;rep;movsb" \
:: "D" ((long)(_res)), "S" ((long)(src)), "c" ((long) (n)) \
: "di", "si", "cx"); \
_res; \
})*/

