/* passed
 *  linux/mm/memory.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

/*
 * ��������Ǵ�01.12.91 ��ʼ��д��- �ڳ�����Ʊ����ƺ�������Ҫ�ĳ���
 * ����Ӧ���Ǻ����ױ��Ƶ�- linus
 */

/*
 * OK����������ǱȽ����ױ�д�ģ�������ҳ��ȴ��Ҫ�е㼼�ɡ�����ҳ�������
 * 02.12.91 ��ʼ��д�ģ������ܹ�����- Linus��
 *
 * ͨ��ִ�д�Լ30 ��/bin/sh �Թ�����������˲��ԣ������ں˵�����Ҫռ�ö���
 * 6M ���ڴ棬��Ŀǰȴ���á����ڿ��������úܺá�
 *
 * ��"invalidate()"����Ҳ����������- ���ⷽ���һ����Ĳ�����
 */

// �ź�ͷ�ļ��������źŷ��ų������źŽṹ�Լ��źŲ�������ԭ�͡�
#include <signal.h>

// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ
#include <asm/system.h>

// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/sched.h>
// head ͷ�ļ��������˶��������ļ򵥽ṹ���ͼ���ѡ���������
#include <linux/head.h>
// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/kernel.h>

void do_exit(long code);// �����˳�����������kernel/exit.c��

//// ��ʾ�ڴ������������Ϣ�����˳���
static _inline void oom(void)
{
	printk("out of memory\n\r");
	do_exit(SIGSEGV);// do_exit()Ӧ��ʹ���˳����룬���������ź�ֵSIGSEGV(11)
						// ��ֵͬ�ĳ����뺬���ǡ���Դ��ʱ�����á�������ͬ�塣
}

// ˢ��ҳ�任���ٻ���꺯����
// Ϊ����ߵ�ַת����Ч�ʣ�CPU �����ʹ�õ�ҳ�����ݴ����оƬ�и��ٻ����С�
// ���޸Ĺ�ҳ����Ϣ֮�󣬾���Ҫˢ�¸û�����������ʹ�����¼���ҳĿ¼��ַ
// �Ĵ���cr3 �ķ���������ˢ�¡�����eax = 0����ҳĿ¼�Ļ�ַ��
//#define invalidate() \
//__asm__("movl %%eax,%%cr3"::"a" (0))
#define invalidate() \
_asm{_asm xor eax,eax _asm mov cr3,eax}

/* ���涨������Ҫ�Ķ�������Ҫ��head.s ���ļ��е������Ϣһ��ı� */
// linux 0.11 �ں�Ĭ��֧�ֵ�����ڴ�������16M�������޸���Щ�������ʺϸ�����ڴ档
#define LOW_MEM 0x100000	// �ڴ�Ͷˣ�1MB����
#define PAGING_MEMORY (15*1024*1024)// ��ҳ�ڴ�15MB�����ڴ������15M��
#define PAGING_PAGES (PAGING_MEMORY>>12)// ��ҳ��������ڴ�ҳ����
#define MAP_NR(addr) (((addr)-LOW_MEM)>>12)// ָ�������ڴ��ַӳ��Ϊҳ�š�
#define USED 100// ҳ�汻ռ�ñ�־��

// �ú������жϸ�����ַ�Ƿ�λ�ڵ�ǰ���̵Ĵ�����С�
#define CODE_SPACE(addr) \
((((addr)+4095)&~4095) < current->start_code + current->end_code)

static unsigned long HIGH_MEMORY = 0;// ȫ�ֱ��������ʵ�������ڴ���߶˵�ַ��

// ����1 ҳ�ڴ棨4K �ֽڣ���
//#define copy_page(from,to) \
//__asm__("cld ; rep ; movsl"::"S" (from),"D" (to),"c" (1024):"cx","di","si")
#define copy_page(from,to) _copy_page((void *)(from),(void *)(to))
_inline void _copy_page(void *from, void *to)
{_asm{
	pushf
	mov ecx,1024 
	mov esi,from 
	mov edi,to 
	cld 
	rep movsd 
	popf
}}

// �ڴ�ӳ���ֽ�ͼ(1 �ֽڴ���1 ҳ�ڴ�)��ÿ��ҳ���Ӧ
// ���ֽ����ڱ�־ҳ�浱ǰ�����ã�ռ�ã�������
static unsigned char mem_map [ PAGING_PAGES ] = {0,};

/*
 * ��ȡ�׸�(ʵ���������1 ��:-)�������ҳ�棬�����Ϊ��ʹ�á����û�п���ҳ�棬
 * �ͷ���0��
 */
//// ȡ�������ҳ�档����Ѿ�û�п����ڴ��ˣ��򷵻�0��
// ���룺%1(ax=0) - 0��%2(LOW_MEM)��%3(cx=PAGING PAGES)��%4(edi=mem_map+PAGING_PAGES-1)��
// ���������%0(ax=ҳ����ʼ��ַ)��
// ����%4 �Ĵ���ʵ��ָ��mem_map[]�ڴ��ֽ�ͼ�����һ���ֽڡ����������ֽ�ͼĩ�˿�ʼ��ǰɨ��
// ����ҳ���־��ҳ������ΪPAGING_PAGES��������ҳ����У����ڴ�ӳ���ֽ�Ϊ0���򷵻�ҳ���ַ��
// ע�⣡������ֻ��ָ�������ڴ�����һҳ����ҳ�棬����û��ӳ�䵽ĳ�����̵����Ե�ַȥ������
// ��put_page()��������������ӳ��ġ�
unsigned long get_free_page(void)
{
//	unsigned long __res = mem_map+PAGING_PAGES-1;
	__asm {
		pushf
		xor eax, eax
		mov ecx,PAGING_PAGES
//		mov edi,__res 
		mov edi,offset mem_map + PAGING_PAGES - 1
		std
		repne scasb		// ����λ��λ����al(0)���Ӧ(di)ÿ��ҳ������ݱȽϣ�
		jne l1		// ���û�е���0 ���ֽڣ�����ת����������0����
		mov byte ptr [edi+1],1	// ����Ӧҳ����ڴ�ӳ��λ��1��
		sal ecx,12	// ҳ����*4K = ���ҳ����ʼ��ַ��
		add ecx,LOW_MEM	// �ټ��ϵͶ��ڴ��ַ�������ҳ��ʵ��������ʼ��ַ��
		mov edx,ecx	// ��ҳ��ʵ����ʼ��ַ -> edx �Ĵ�����
		mov ecx,1024	// �Ĵ���ecx �ü���ֵ1024��
		lea edi,[edx+4092]// ��4092+edx ��λ�� -> edi(��ҳ���ĩ��)��
		rep stosd	// ��edi ��ָ�ڴ����㣨������Ҳ������ҳ�����㣩��
//		mov __res,edx	// ��ҳ����ʼ��ַ -> __res������ֵ����
		mov eax,edx
	l1:	popf
	}
//	return __res;// ���ؿ���ҳ���ַ������޿���Ҳ�򷵻�0����
}
//unsigned long get_free_page(void)
//{
//register unsigned long __res asm("ax");
//
//__asm__("std ; repne ; scasb\n\t"	// ����λ��λ����al(0)���Ӧÿ��ҳ���(di)���ݱȽϣ�
//	"jne 1f\n\t"	// ���û�е���0 ���ֽڣ�����ת����������0����
//	"movb $1,1(%%edi)\n\t"// ����Ӧҳ����ڴ�ӳ��λ��1��
//	"sall $12,%%ecx\n\t"// ҳ����*4K = ���ҳ����ʼ��ַ��
//	"addl %2,%%ecx\n\t"// �ټ��ϵͶ��ڴ��ַ�������ҳ��ʵ��������ʼ��ַ��
//	"movl %%ecx,%%edx\n\t"// ��ҳ��ʵ����ʼ��ַ -> edx �Ĵ�����
//	"movl $1024,%%ecx\n\t"// �Ĵ���ecx �ü���ֵ1024��
//	"leal 4092(%%edx),%%edi\n\t"// ��4092+edx ��λ�� -> edi(��ҳ���ĩ��)��
///	"rep ; stosl\n\t"// ��edi ��ָ�ڴ����㣨������Ҳ������ҳ�����㣩��
//	"movl %%edx,%%eax\n"// ��ҳ����ʼ��ַ -> eax������ֵ����
//	"1:"
//	:"=a" (__res)
//	:"0" (0),"i" (LOW_MEM),"c" (PAGING_PAGES),
//	"D" (mem_map+PAGING_PAGES-1)
//	:"di","cx","dx");
//return __res;// ���ؿ���ҳ���ַ������޿���Ҳ�򷵻�0����
//}

/*
 * �ͷ������ַ'addr'��ʼ��һҳ�ڴ档���ں���'free_page_tables()'��
 */
//// �ͷ������ַaddr ��ʼ��һҳ���ڴ档
// 1MB ���µ��ڴ�ռ������ں˳���ͻ��壬����Ϊ����ҳ����ڴ�ռ䡣
void free_page(unsigned long addr)
{
	if (addr < LOW_MEM) return;// ��������ַaddr С���ڴ�Ͷˣ�1MB�����򷵻ء�
	if (addr >= HIGH_MEMORY)// ��������ַaddr>=�ڴ���߶ˣ�����ʾ������Ϣ��
		panic("trying to free nonexistent page");
	addr -= LOW_MEM;// �����ַ��ȥ�Ͷ��ڴ�λ�ã��ٳ���4KB����ҳ��š�
	addr >>= 12;
	if (mem_map[addr]--) return;// �����Ӧ�ڴ�ҳ��ӳ���ֽڲ�����0�����1 ���ء�
	mem_map[addr]=0;// �����ö�Ӧҳ��ӳ���ֽ�Ϊ0������ʾ������Ϣ��������
	panic("trying to free free page");
}

/*
 * ���溯���ͷ�ҳ���������ڴ�飬'exit()'��Ҫ�ú�������copy_page_tables()
 * ���ƣ��ú���������4Mb ���ڴ�顣
 */
//// ����ָ�������Ե�ַ���޳���ҳ����������ͷŶ�Ӧ�ڴ�ҳ����ָ�����ڴ�鲢�ñ�����С�
// ҳĿ¼λ�������ַ0 ��ʼ������1024 �ռ4K �ֽڡ�ÿ��Ŀ¼��ָ��һ��ҳ��
// ҳ��������ַ0x1000 ����ʼ��������Ŀ¼�ռ䣩��ÿ��ҳ����1024 �Ҳռ4K �ڴ档
// ÿ��ҳ�����Ӧһҳ�����ڴ棨4K����Ŀ¼���ҳ����Ĵ�С��Ϊ4 ���ֽڡ�
// ������from - ��ʼ����ַ��size - �ͷŵĳ��ȡ�
int free_page_tables(unsigned long from,unsigned long size)
{
	unsigned long *pg_table;
	unsigned long * dir, nr;

	if (from & 0x3fffff)// Ҫ�ͷ��ڴ��ĵ�ַ����4M Ϊ�߽硣
		panic("free_page_tables called with wrong alignment");
	if (!from)// ������ͼ�ͷ��ں˺ͻ�����ռ�ռ䡣
		panic("Trying to free up swapper memory space");
// ������ռҳĿ¼����(4M �Ľ�λ������)��Ҳ����ռҳ������
	size = (size + 0x3fffff) >> 22;
// ����һ�������ʼĿ¼���Ӧ��Ŀ¼���=from>>22����ÿ��ռ4 �ֽڣ���������ҳĿ¼�Ǵ�
// �����ַ0 ��ʼ�����ʵ�ʵ�Ŀ¼��ָ��=Ŀ¼���<<2��Ҳ��(from>>20)������0xffc ȷ��
// Ŀ¼��ָ�뷶Χ��Ч��
	dir = (unsigned long *) ((from>>20) & 0xffc); /* _pg_dir = 0 */
	for ( ; size-->0 ; dir++) {// size ��������Ҫ���ͷ��ڴ��Ŀ¼������
		if (!(1 & *dir))// �����Ŀ¼����Ч(P λ=0)���������
			continue;// Ŀ¼���λ0(P λ)��ʾ��Ӧҳ���Ƿ���ڡ�
		pg_table = (unsigned long *) (0xfffff000 & *dir);// ȡĿ¼����ҳ���ַ��
		for (nr=0 ; nr<1024 ; nr++) {// ÿ��ҳ����1024 ��ҳ�
			if (1 & *pg_table)// ����ҳ������Ч(P λ=1)�����ͷŶ�Ӧ�ڴ�ҳ��
				free_page(0xfffff000 & *pg_table);
			*pg_table = 0;// ��ҳ�����������㡣
			pg_table++;// ָ��ҳ������һ�
		}
		free_page(0xfffff000 & *dir);// �ͷŸ�ҳ����ռ�ڴ�ҳ�档������ҳ����
										// �����ַ1M ���ڣ��������ʲô��������
		*dir = 0;// ����Ӧҳ���Ŀ¼�����㡣
	}
	invalidate();// ˢ��ҳ�任���ٻ��塣
	return 0;
}

/*
 * ���ˣ��������ڴ����mm ����Ϊ���ӵĳ���֮һ����ͨ��ֻ�����ڴ�ҳ��
 * ������һ����Χ�����Ե�ַ�е����ݡ�ϣ��������û�д�����Ϊ�Ҳ���
 * �ٵ����������� :-)
 *
 * ע�⣡���ǲ����ǽ������κ��ڴ��- �ڴ��ĵ�ַ��Ҫ��4Mb �ı���������
 * һ��ҳĿ¼���Ӧ���ڴ��С������Ϊ���������ʹ�����ܼ򵥡�����������
 * ������fork()ʹ�ã�fork.c��
 *
 * ע��!!��from==0 ʱ������Ϊ��һ��fork()���ø����ں˿ռ䡣��ʱ����
 * ���븴������ҳĿ¼���Ӧ���ڴ棬��Ϊ�������ᵼ���ڴ����ص��˷�- ����
 * ֻ����ͷ160 ��ҳ��- ��Ӧ640kB����ʹ�Ǹ�����Щҳ��Ҳ�Ѿ��������ǵ�����
 * ���ⲻ��ռ�ø�����ڴ�- �ڵ�1Mb �ڴ淶Χ�����ǲ�ִ��дʱ���Ʋ���������
 * ��Щҳ��������ں˹����������nr=xxxx �����������nr �ڳ�����ָҳ��������
 */
//// ����ָ�����Ե�ַ�ͳ��ȣ�ҳ��������ڴ��Ӧ��ҳĿ¼���ҳ���Ӷ������Ƶ�ҳĿ¼��
//// ҳ���Ӧ��ԭ�����ڴ���������ʹ�á�
// ����ָ����ַ�ͳ��ȵ��ڴ��Ӧ��ҳĿ¼���ҳ���������ҳ���������ҳ��ԭ�ڴ���������
// �˺��������̽������ڴ�����ֱ����һ������ִ��д����ʱ���ŷ����µ��ڴ�ҳ��дʱ���ƻ��ƣ���
int copy_page_tables(unsigned long from,unsigned long to,long size)
{
	unsigned long * from_page_table;
	unsigned long * to_page_table;
	unsigned long this_page;
	unsigned long * from_dir, * to_dir;
	unsigned long nr;

	// Դ��ַ��Ŀ�ĵ�ַ����Ҫ����4Mb ���ڴ�߽��ַ�ϡ��������������
	if ((from&0x3fffff) || (to&0x3fffff))
		panic("copy_page_tables called with wrong alignment");
	// ȡ��Դ��ַ��Ŀ�ĵ�ַ��Ŀ¼��(from_dir ��to_dir)���μ���115 ���ע�͡�
	from_dir = (unsigned long *) ((from>>20) & 0xffc); /* _pg_dir = 0 */
	to_dir = (unsigned long *) ((to>>20) & 0xffc);
	// ����Ҫ���Ƶ��ڴ��ռ�õ�ҳ������Ҳ��Ŀ¼��������
	size = ((unsigned) (size+0x3fffff)) >> 22;
	// ���濪ʼ��ÿ��ռ�õ�ҳ�����ν��и��Ʋ�����
	for( ; size-->0 ; from_dir++,to_dir++) {
		if (1 & *to_dir)// ���Ŀ��Ŀ¼��ָ����ҳ���Ѿ�����(P=1)�������������
			panic("copy_page_tables: already exist");
		if (!(1 & *from_dir))// �����ԴĿ¼��δ��ʹ�ã����ø��ƶ�Ӧҳ��������
			continue;
		// ȡ��ǰԴĿ¼����ҳ��ĵ�ַ -> from_page_table��
		from_page_table = (unsigned long *) (0xfffff000 & *from_dir);
// ΪĿ��ҳ��ȡһҳ�����ڴ棬���������0 ��˵��û�����뵽�����ڴ�ҳ�档����ֵ=-1���˳���
		if (!(to_page_table = (unsigned long *) get_free_page()))
			return -1;	/* Out of memory, see freeing */
		// ����Ŀ��Ŀ¼����Ϣ��7 �Ǳ�־��Ϣ����ʾ(Usr, R/W, Present)��
		*to_dir = ((unsigned long) to_page_table) | 7;
		// ��Ե�ǰ�����ҳ�������踴�Ƶ�ҳ��������������ں˿ռ䣬����踴��ͷ160 ҳ��
		// ������Ҫ����1 ��ҳ���е�����1024 ҳ�档
		nr = (from==0)?0xA0:1024;
		// ���ڵ�ǰҳ����ʼ����ָ����Ŀnr ���ڴ�ҳ�档
		for ( ; nr-- > 0 ; from_page_table++,to_page_table++) {
			this_page = *from_page_table;// ȡԴҳ�������ݡ�
			if (!(1 & this_page))// �����ǰԴҳ��û��ʹ�ã����ø��ơ�
				continue;
// ��λҳ������R/W ��־(��0)��(���U/S λ��0����R/W ��û�����á����U/S ��1����R/W ��0��
// ��ô�������û���Ĵ����ֻ�ܶ�ҳ�档���U/S ��R/W ����λ�������д��Ȩ�ޡ�)
			this_page &= ~2;
			*to_page_table = this_page;// ����ҳ����Ƶ�Ŀ��ҳ���С�
// �����ҳ������ָҳ��ĵ�ַ��1M ���ϣ�����Ҫ�����ڴ�ҳ��ӳ������mem_map[]�����Ǽ���
// ҳ��ţ�������Ϊ������ҳ��ӳ��������Ӧ�����������ô�����
			if (this_page > LOW_MEM) {
// �������ĺ�������Դҳ������ָ�ڴ�ҳҲΪֻ������Ϊ���ڿ�ʼ���������̹����ڴ����ˡ�
// ������һ���ڴ���Ҫ����д�����������ͨ��ҳ�쳣��д��������Ϊִ��д�����Ľ��̷���
// һҳ�µĿ���ҳ�棬Ҳ������дʱ���ƵĲ�����
				*from_page_table = this_page;// ��Դҳ����Ҳֻ����
				this_page -= LOW_MEM;
				this_page >>= 12;
				mem_map[this_page]++;
			}
		}
	}
	invalidate();// ˢ��ҳ�任���ٻ��塣
	return 0;
}

/*
 * ���溯����һ�ڴ�ҳ�������ָ����ַ����������ҳ��������ַ�����
 * �ڴ治��(�ڷ���ҳ���ҳ��ʱ)���򷵻�0��
 */
//// ��һ�����ڴ�ҳ��ӳ�䵽ָ�������Ե�ַ����
// ��Ҫ��������ҳĿ¼��ҳ��������ָ��ҳ�����Ϣ�����ɹ��򷵻�ҳ���ַ��
unsigned long put_page(unsigned long page,unsigned long address)
{
	unsigned long tmp, *page_table;

/* ע��!!!����ʹ����ҳĿ¼��ַ_pg_dir=0 ������ */

// ��������ҳ��λ�õ���LOW_MEM(1Mb)�򳬳�ϵͳʵ�ʺ����ڴ�߶�HIGH_MEMORY���򷢳����档
	if (page < LOW_MEM || page >= HIGH_MEMORY)
		printk("Trying to put page %p at %p\n",page,address);
	// ��������ҳ�����ڴ�ҳ��ӳ���ֽ�ͼ��û����λ������ʾ������Ϣ��
	if (mem_map[(page-LOW_MEM)>>12] != 1)
		printk("mem_map disagrees with %p at %p\n",page,address);
	// ����ָ����ַ��ҳĿ¼���ж�Ӧ��Ŀ¼��ָ�롣
	page_table = (unsigned long *) ((address>>20) & 0xffc);
// �����Ŀ¼����Ч(P=1)(Ҳ��ָ����ҳ�����ڴ���)�������ȡ��ָ��ҳ��ĵ�ַ -> page_table��
	if ((*page_table)&1)
		page_table = (unsigned long *) (0xfffff000 & *page_table);
	else {
// �����������ҳ���ҳ��ʹ�ã����ڶ�ӦĿ¼��������Ӧ��־7��User, U/S, R/W����Ȼ��
// ��ҳ��ĵ�ַ -> page_table��
		if (!(tmp=get_free_page()))
			return 0;
		*page_table = tmp|7;
		page_table = (unsigned long *) tmp;
	}
	// ��ҳ��������ָ����ַ�������ڴ�ҳ���ҳ�������ݡ�ÿ��ҳ������1024 ��(0x3ff)��
	page_table[(address>>12) & 0x3ff] = page | 7;
/* ����Ҫˢ��ҳ�任���ٻ��� */
	return page;// ����ҳ���ַ��
}

//// ȡ��д����ҳ�溯��������ҳ�쳣�жϹ�����д�����쳣�Ĵ���дʱ���ƣ���
// �������Ϊҳ����ָ�롣
// [ un_wp_page ��˼��ȡ��ҳ���д������Un-Write Protected��]
void un_wp_page(unsigned long * table_entry)
{
	unsigned long old_page,new_page;

	old_page = 0xfffff000 & *table_entry;// ȡԭҳ���Ӧ��Ŀ¼��š�
// ���ԭҳ���ַ�����ڴ�Ͷ�LOW_MEM(1Mb)����������ҳ��ӳ���ֽ�ͼ������ֵΪ1����ʾ��
// ������1 �Σ�ҳ��û�б����������ڸ�ҳ���ҳ��������R/W ��־����д������ˢ��ҳ�任
// ���ٻ��壬Ȼ�󷵻ء�
	if (old_page >= LOW_MEM && mem_map[MAP_NR(old_page)]==1) {
		*table_entry |= 2;
		invalidate();
		return;
	}
	// ���������ڴ���������һҳ����ҳ�档
	if (!(new_page=get_free_page()))
		oom();// Out of Memory���ڴ治������
// ���ԭҳ������ڴ�Ͷˣ�����ζ��mem_map[]>1��ҳ���ǹ���ģ�����ԭҳ���ҳ��ӳ��
// ����ֵ�ݼ�1��Ȼ��ָ��ҳ�������ݸ���Ϊ��ҳ��ĵ�ַ�����ÿɶ�д�ȱ�־(U/S, R/W, P)��
// ˢ��ҳ�任���ٻ��塣���ԭҳ�����ݸ��Ƶ���ҳ�档
	if (old_page >= LOW_MEM)
		mem_map[MAP_NR(old_page)]--;
	*table_entry = new_page | 7;
	invalidate();
	copy_page(old_page,new_page);
}	

/*
 * ���û���ͼ��һ������ҳ����дʱ���ú��������Ѵ��ڵ��ڴ�ҳ�棬��дʱ���ƣ�
 * ����ͨ����ҳ�渴�Ƶ�һ���µ�ַ�ϲ��ݼ�ԭҳ��Ĺ���ҳ�����ֵʵ�ֵġ�
 *
 * ������ڴ���ռ䣬���Ǿ��Զδ�����Ϣ�˳���
 */
//// ҳ�쳣�жϴ�����õ�C ������д����ҳ�洦��������page.s �����б����á�
// ����error_code ����CPU �Զ�������address ��ҳ�����Ե�ַ��
// д����ҳ��ʱ���踴��ҳ�棨дʱ���ƣ���
void do_wp_page(unsigned long error_code,unsigned long address)
{
#if 0
/* �������ڻ���������������Ϊestdio ����ڴ���ռ�ִ��д���� */
/* ����̫�޴��ˡ��������GNU �õ�libc.a �⡣ */
	if (CODE_SPACE(address))	// �����ַλ�ڴ���ռ䣬����ִֹ�г���
		do_exit(SIGSEGV);
#endif
// ����ȡ��ҳ�汣��������ָ��ҳ����ҳ���е�ҳ����ָ�룬����㷽���ǣ�
// ((address>>10) & 0xffc)������ָ����ַ��ҳ����ҳ���е�ƫ�Ƶ�ַ��
// (0xfffff000 &((address>>20) &0xffc))��ȡĿ¼����ҳ��ĵ�ֵַ��
// ����((address>>20) &0xffc)����ҳ������ҳ���Ŀ¼��ָ�룻
// ������Ӽ���ָ����ַ��Ӧҳ���ҳ����ָ�롣����Թ����ҳ����и��ơ�
	un_wp_page(
		(unsigned long *)(((address>>10) & 0xffc) + 
		(0xfffff000 & *((unsigned long *) ((address>>20) &0xffc))))
	);

}

//// дҳ����֤��
// ��ҳ�治��д������ҳ�档��fork.c ��34 �б����á�
void write_verify(unsigned long address)
{
	unsigned long page;

// �ж�ָ����ַ����ӦҳĿ¼���ҳ���Ƿ����(P)����������(P=0)�򷵻ء�
	if (!( (page = *((unsigned long *) ((address>>20) & 0xffc)) )&1))
		return;
// ȡҳ��ĵ�ַ������ָ����ַ��ҳ����ҳ���е�ҳ����ƫ��ֵ���ö�Ӧ����ҳ���ҳ����ָ�롣
	page &= 0xfffff000;
	page += ((address>>10) & 0xffc);
// �����ҳ�治��д(��־R/W û����λ)����ִ�й������͸���ҳ�������дʱ���ƣ���
	if ((3 & *(unsigned long *) page) == 1)  /* non-writeable, present */
		un_wp_page((unsigned long *) page);
	return;
}

//// ȡ��һҳ�����ڴ沢ӳ�䵽ָ�����Ե�ַ����
// ��get_free_page()��ͬ��get_free_page()��������ȡ�������ڴ�����һҳ�����ڴ档���ú���
// �����ǻ�ȡ��һҳ�����ڴ�ҳ�棬����һ������put_page()��������ҳ��ӳ�䵽ָ�������Ե�ַ
// ����
void get_empty_page(unsigned long address)
{
	unsigned long tmp;

// ������ȡ��һ����ҳ�棬���߲��ܽ�ҳ����õ�ָ����ַ��������ʾ�ڴ治������Ϣ��
// 279 ����Ӣ��ע�͵ĺ����ǣ���ʹִ��get_free_page()����0 Ҳ����ν����Ϊput_page()
// �л���Դ�����ٴ������������ҳ��ģ���210 �С�
	if (!(tmp=get_free_page()) || !put_page(tmp,address)) {
		free_page(tmp);		/* 0 is ok - ignored */
		oom();
	}
}

/*
 * try_to_share()������"p"�м��λ�ڵ�ַ"address"����ҳ�棬��ҳ���Ƿ���ڣ��Ƿ�ɾ���
 * ����Ǹɾ��Ļ������뵱ǰ������
 *
 * ע�⣡���������Ѽٶ�p !=��ǰ���񣬲������ǹ���ͬһ��ִ�г���
 */
//// ���ԶԽ���ָ����ַ����ҳ����й��������
// ͬʱ����ָ֤���ĵ�ַ���Ƿ��Ѿ�������ҳ�棬���������������
// ����1-�ɹ���0-ʧ�ܡ�
static int try_to_share(unsigned long address, struct task_struct * p)
{
	unsigned long from;
	unsigned long to;
	unsigned long from_page;
	unsigned long to_page;
	unsigned long phys_addr;

// ��ָ���ڴ��ַ��ҳĿ¼�
	from_page = to_page = ((address>>20) & 0xffc);
// �������p �Ĵ�����ʼ��ַ����Ӧ��ҳĿ¼�
	from_page += ((p->start_code>>20) & 0xffc);
// ���㵱ǰ�����д�����ʼ��ַ����Ӧ��ҳĿ¼�
	to_page += ((current->start_code>>20) & 0xffc);
/* ��from ���Ƿ����ҳĿ¼�� */
// *** ��p ����ҳ����в�����
// ȡҳĿ¼�����ݡ������Ŀ¼����Ч(P=0)���򷵻ء�����ȡ��Ŀ¼���Ӧҳ���ַ-> from��
	from = *(unsigned long *) from_page;
	if (!(from & 1))
		return 0;
	from &= 0xfffff000;
// �����ַ��Ӧ��ҳ����ָ��ֵ����ȡ����ҳ�������� -> phys_addr��
	from_page = from + ((address>>10) & 0xffc);
	phys_addr = *(unsigned long *) from_page;
/* ҳ��ɾ����Ҵ����� */
// 0x41 ��Ӧҳ�����е�Dirty ��Present ��־�����ҳ�治�ɾ�����Ч�򷵻ء�
	if ((phys_addr & 0x41) != 0x01)
		return 0;
// ȡҳ��ĵ�ַ -> phys_addr�������ҳ���ַ�����ڻ�С���ڴ�Ͷ�(1M)Ҳ�����˳���
	phys_addr &= 0xfffff000;
	if (phys_addr >= HIGH_MEMORY || phys_addr < LOW_MEM)
		return 0;
// *** �Ե�ǰ����ҳ����в�����
// ȡҳĿ¼������ -> to�������Ŀ¼����Ч(P=0)����ȡ����ҳ�棬������to_page ��ָ��Ŀ¼�
	to = *(unsigned long *) to_page;
	if (!(to & 1))
		if (to = get_free_page())
			*(unsigned long *) to_page = to | 7;
		else
			oom();
// ȡ��Ӧҳ���ַ -> to��ҳ�����ַ to_page�������Ӧ��ҳ���Ѿ����ڣ������������
	to &= 0xfffff000;
	to_page = to + ((address>>10) & 0xffc);
	if (1 & *(unsigned long *) to_page)
		panic("try_to_share: to_page already exists");
/* �����ǽ��й�����д���� */
// ��p ������ҳ����д������־(��R/W=0 ֻ��)�����ҵ�ǰ�����еĶ�Ӧҳ����ָ������
	*(unsigned long *) from_page &= ~2;
	*(unsigned long *) to_page = *(unsigned long *) from_page;
	// ˢ��ҳ�任���ٻ��塣
	invalidate();
	// ����������ҳ���ҳ��ţ�������Ӧҳ��ӳ���������е����õ���1��
	phys_addr -= LOW_MEM;
	phys_addr >>= 12;
	mem_map[phys_addr]++;
	return 1;
}

/*
 * share_page()��ͼ�ҵ�һ�����̣��������뵱ǰ���̹���ҳ�档����address ��
 * ��ǰ���ݿռ������������ĳҳ���ַ��
 *
 * ��������ͨ�����executable->i_count ����֤�Ƿ���С���������������ѹ���
 * ��inode������Ӧ�ô���1��
 */
//// ����ҳ�档��ȱҳ����ʱ�����ܷ���ҳ��
// ����1 - �ɹ���0 - ʧ�ܡ���
static int share_page(unsigned long address)
{
	struct task_struct ** p;

// ����ǲ���ִ�еģ��򷵻ء�excutable ��ִ�н��̵��ڴ�i �ڵ�ṹ��
	if (!current->executable)
		return 0;
// ���ֻ�ܵ���ִ��(executable->i_count=1)��Ҳ�˳���
	if (current->executable->i_count < 2)
		return 0;
// ����������������������Ѱ���뵱ǰ���̿ɹ���ҳ��Ľ��̣�
// �����Զ�ָ����ַ��ҳ����й���
	for (p = &LAST_TASK ; p > &FIRST_TASK ; --p) {
		if (!*p)// �������������У������Ѱ�ҡ�
			continue;
		if (current == *p)// ������ǵ�ǰ����Ҳ����Ѱ�ҡ�
			continue;
		if ((*p)->executable != current->executable)// ���executable ���ȣ�Ҳ������
			continue;
		if (try_to_share(address,*p))// ���Թ���ҳ�档
			return 1;
	}
	return 0;
}

//// ҳ�쳣�жϴ�����õĺ���������ȱҳ�쳣�������page.s �����б����á�
// ����error_code ����CPU �Զ�������address ��ҳ�����Ե�ַ��
void do_no_page(unsigned long error_code,unsigned long address)
{
	int nr[4];
	unsigned long tmp;
	unsigned long page;
	int block,i;

	address &= 0xfffff000;// ҳ���ַ��
// �������ָ�����Ե�ַ�ڽ��̿ռ�������ڽ��̻�ַ��ƫ�Ƴ���ֵ��
	tmp = address - current->start_code;
// ����ǰ���̵�executable �գ�����ָ����ַ��������+���ݳ��ȣ�������һҳ�����ڴ棬��ӳ��
// Ӱ�䵽ָ�������Ե�ַ����executable �ǽ��̵�i �ڵ�ṹ����ֵΪ0���������̸տ�ʼ���ã�
// ��Ҫ�ڴ棻��ָ�������Ե�ַ������������ݳ��ȣ����������������µ��ڴ�ռ䣬Ҳ��Ҫ���衣
// ��˾�ֱ�ӵ���get_empty_page()����������һҳ�����ڴ沢ӳ�䵽ָ�����Ե�ַ�����ɡ�
// start_code �ǽ��̴���ε�ַ��end_data �Ǵ�������ݳ��ȡ�����linux �ںˣ����Ĵ���κ�
// ���ݶ�����ʼ��ַ����ͬ�ġ�
	if (!current->executable || tmp >= current->end_data) {
		get_empty_page(address);
		return;
	}
// ������Թ���ҳ��ɹ������˳���
	if (share_page(tmp))
		return;
// ȡ����ҳ�棬����ڴ治���ˣ�����ʾ�ڴ治������ֹ���̡�
	if (!(page = get_free_page()))
		oom();
/* ��ס��������ͷҪʹ��1 �����ݿ� */
// ���ȼ���ȱҳ���ڵ����ݿ��BLOCK_SIZE = 1024 �ֽڣ����һҳ�ڴ���Ҫ4 �����ݿ顣
	block = 1 + tmp/BLOCK_SIZE;
// ����i �ڵ���Ϣ��ȡ���ݿ����豸�ϵĶ�Ӧ���߼���š�
	for (i=0 ; i<4 ; block++,i++)
		nr[i] = bmap(current->executable,block);
// ���豸��һ��ҳ������ݣ�4 ���߼��飩��ָ�������ַpage ����
	bread_page(page,current->executable->i_dev,nr);
// ��������һҳ�ڴ�󣬸�ҳ�ڴ�Ĳ��ֿ��ܻᳬ�����̵�end_data λ�á������ѭ�����Ƕ�����
// ҳ�泬���Ĳ��ֽ������㴦��
	i = tmp + 4096 - current->end_data;
	tmp = page + 4096;
	while (i-- > 0) {
		tmp--;
		*(char *)tmp = 0;
	}
// ���������ҳ��ӳ�䵽ָ�����Ե�ַ�Ĳ����ɹ����ͷ��ء�������ͷ��ڴ�ҳ����ʾ�ڴ治����
	if (put_page(page,address))
		return;
	free_page(page);
	oom();
}

//// �����ڴ��ʼ����
// ������start_mem - ��������ҳ����������ڴ���ʼλ�ã���ȥ��RAMDISK ��ռ�ڴ�ռ�ȣ���
// end_mem - ʵ�������ڴ�����ַ��
// �ڸð��linux �ں��У������ʹ��16Mb ���ڴ棬����16Mb ���ڴ潫���ڿ��ǣ����ò��á�
// 0 - 1Mb �ڴ�ռ������ں�ϵͳ����ʵ��0-640Kb����
void mem_init(long start_mem, long end_mem)
{
	int i;

	HIGH_MEMORY = end_mem;// �����ڴ���߶ˡ�
	for (i=0 ; i<PAGING_PAGES ; i++)// ����������ҳ��Ϊ��ռ��(USED=100)״̬��
		mem_map[i] = USED;// ����ҳ��ӳ������ȫ�ó�USED��
	i = MAP_NR(start_mem);// Ȼ������ʹ����ʼ�ڴ��ҳ��š�
	end_mem -= start_mem;// �ټ���ɷ�ҳ������ڴ���С��
	end_mem >>= 12;// �Ӷ�����������ڷ�ҳ�����ҳ������
	while (end_mem-->0)// �����Щ����ҳ���Ӧ��ҳ��ӳ���������㡣
		mem_map[i++]=0;
}

// �����ڴ����ҳ��������ʾ��
void calc_mem(void)
{
	int i,j,k,free=0;
	long * pg_tbl;

// ɨ���ڴ�ҳ��ӳ������mem_map[]����ȡ����ҳ��������ʾ��
	for(i=0 ; i<PAGING_PAGES ; i++)
		if (!mem_map[i]) free++;
	printk("%d pages free (of %d)\n\r",free,PAGING_PAGES);
// ɨ������ҳĿ¼���0��1 ������ҳĿ¼����Ч����ͳ�ƶ�Ӧҳ������Чҳ����������ʾ��
	for(i=2 ; i<1024 ; i++) {
		if (1&pg_dir[i]) {
			pg_tbl=(long *) (0xfffff000 & pg_dir[i]);
			for(j=k=0 ; j<1024 ; j++)
				if (pg_tbl[j]&1)
					k++;
			printk("Pg-dir[%d] uses %d pages\n",i,k);
		}
	}
}

