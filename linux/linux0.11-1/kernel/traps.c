/* passed
 *  linux/kernel/traps.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

/*
 * �ڳ���asm.s �б�����һЩ״̬�󣬱�������������Ӳ������͹��ϡ�Ŀǰ��Ҫ���ڵ���Ŀ�ģ�
 * �Ժ���չ����ɱ�����𻵵Ľ��̣���Ҫ��ͨ������һ���źţ��������ҪҲ��ֱ��ɱ������
 */
// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <string.h>

// head ͷ�ļ��������˶��������ļ򵥽ṹ���ͼ���ѡ���������
#include <linux/head.h>
// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/sched.h>
// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/kernel.h>
// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ
#include <asm/system.h>
// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����
#include <asm/segment.h>
// ����/���ͷ�ļ�������Ӳ���˿�����/���������䡣
#include <asm/io.h>

// ������䶨��������Ƕ��ʽ������亯�����й�Ƕ��ʽ���Ļ����﷨���б���μ���¼��
// ȡ��seg �е�ַaddr ����һ���ֽڡ�
//#define get_seg_byte(seg,addr) ({ \
//register char __res; \
//__asm__("push %%fs;mov %%ax,%%fs;movb %%fs:%2,%%al;pop %%fs" \
//	:"=a" (__res):"0" (seg),"m" (*(addr))); \
//__res;})
_inline char get_seg_byte(unsigned short segm, void *addr)
{
	register char __res;
	_asm{
	push fs
	mov ax,segm
	mov fs,ax
	mov ebx,addr
	mov al,byte ptr fs:[ebx]
	mov __res,al
	pop fs
	}
	return __res;
}

// ȡ��seg �е�ַaddr ����һ�����֣�4 �ֽڣ���
_inline long 
get_seg_long(unsigned short segm,long *addr) {
	register unsigned long __res;
	_asm{
		push fs
		mov ax,segm
		mov fs,ax
		mov ebx,addr
		mov eax,fs:[ebx]
		mov __res,eax
		pop fs
	}
	return __res;
}
//#define get_seg_long(seg,addr) ({ \
//register unsigned long __res; \
//__asm__("push %%fs;mov %%ax,%%fs;movl %%fs:%2,%%eax;pop %%fs" \
//	:"=a" (__res):"0" (seg),"m" (*(addr))); \
//__res;})

// ȡfs �μĴ�����ֵ��ѡ�������
_inline unsigned short _fs() {
	register unsigned short __res;
	_asm mov ax,fs
	_asm mov __res,ax
	return __res;
}
//#define _fs() ({ \
//register unsigned short __res; \
//__asm__("mov %%fs,%%ax":"=a" (__res):); \
//__res;})

// ���¶�����һЩ����ԭ�͡�
int do_exit(long code);// (kernel/exit.c,102)

void page_exception(void);// [??]

// ���¶�����һЩ�жϴ������ԭ�ͣ������ڣ�kernel/asm.s ��system_call.s���С�
void divide_error(void);// int0 (kernel/asm.s,19)��
void debug(void);// int1 (kernel/asm.s,53)��
void nmi(void);// int2 (kernel/asm.s,57)��
void int3(void);// int3 (kernel/asm.s,61)��
void overflow(void);// int4 (kernel/asm.s,65)��
void bounds(void);// int5 (kernel/asm.s,69)��
void invalid_op(void);// int6 (kernel/asm.s,73)��
void device_not_available(void);// int7 (kernel/system_call.s,148)��
void double_fault(void);// int8 (kernel/asm.s,97)��
void coprocessor_segment_overrun(void);// int9 (kernel/asm.s,77)��
void invalid_TSS(void);// int10 (kernel/asm.s,131)��
void segment_not_present(void);// int11 (kernel/asm.s,135)��
void stack_segment(void);// int12 (kernel/asm.s,139)��
void general_protection(void);// int13 (kernel/asm.s,143)��
void page_fault(void);// int14 (mm/page.s,14)��
void reserved(void);// int15 (kernel/asm.s,81)��
void coprocessor_error(void);// int16 (kernel/system_call.s,131)��
void parallel_interrupt(void);// int39 (kernel/system_call.s,280)��
void irq13(void);// int45 Э�������жϴ���(kernel/asm.s,85)��

// ���ӳ���������ӡ�����жϵ����ơ�����š����ó����EIP��EFLAGS��ESP��fs �μĴ���ֵ��
// �εĻ�ַ���εĳ��ȡ����̺�pid������š�10 �ֽ�ָ���롣�����ջ���û����ݶΣ���
// ��ӡ16 �ֽڵĶ�ջ���ݡ�
static void die(char * str,long esp_ptr,long nr)
{
	long * esp = (long *) esp_ptr;
	int i;

	printk("%s: %04x\n\r",str,nr&0xffff);
	printk("EIP:\t%04x:%p\nEFLAGS:\t%p\nESP:\t%04x:%p\n",
		esp[1],esp[0],esp[2],esp[4],esp[3]);
	printk("fs: %04x\n",_fs());
	printk("base: %p, limit: %p\n",get_base(current->ldt[1]),get_limit(0x17));
	if (esp[4] == 0x17) {
		printk("Stack: ");
		for (i=0;i<4;i++)
			printk("%p ",get_seg_long(0x17,i+(long *)esp[3]));
		printk("\n");
	}
	str(i);
	printk("Pid: %d, process nr: %d\n\r",current->pid,0xffff & i);
	for(i=0;i<10;i++)
		printk("%02x ",0xff & get_seg_byte(esp[1],(i+(char *)esp[0])));
	printk("\n\r");
	do_exit(11);		/* play segment exception */
}

// ������Щ��do_��ͷ�ĺ����Ƕ�Ӧ�����жϴ��������õ�C ������
void do_double_fault(long esp, long error_code)
{
	die("double fault",esp,error_code);
}

void do_general_protection(long esp, long error_code)
{
	die("general protection",esp,error_code);
}

void do_divide_error(long esp, long error_code)
{
	die("divide error",esp,error_code);
}

void do_int3(long * esp, long error_code,
		long fs,long es,long ds,
		long ebp,long esi,long edi,
		long edx,long ecx,long ebx,long eax)
{
	int tr;

//	__asm__("str %%ax":"=a" (tr):"0" (0));
	_asm xor eax,eax
	_asm str ax
	_asm mov tr,eax
	printk("eax\t\tebx\t\tecx\t\tedx\n\r%8x\t%8x\t%8x\t%8x\n\r",
		eax,ebx,ecx,edx);
	printk("esi\t\tedi\t\tebp\t\tesp\n\r%8x\t%8x\t%8x\t%8x\n\r",
		esi,edi,ebp,(long) esp);
	printk("\n\rds\tes\tfs\ttr\n\r%4x\t%4x\t%4x\t%4x\n\r",
		ds,es,fs,tr);
	printk("EIP: %8x   CS: %4x  EFLAGS: %8x\n\r",esp[0],esp[1],esp[2]);
}

void do_nmi(long esp, long error_code)
{
	die("nmi",esp,error_code);
}

void do_debug(long esp, long error_code)
{
	die("debug",esp,error_code);
}

void do_overflow(long esp, long error_code)
{
	die("overflow",esp,error_code);
}

void do_bounds(long esp, long error_code)
{
	die("bounds",esp,error_code);
}

void do_invalid_op(long esp, long error_code)
{
	die("invalid operand",esp,error_code);
}

void do_device_not_available(long esp, long error_code)
{
	die("device not available",esp,error_code);
}

void do_coprocessor_segment_overrun(long esp, long error_code)
{
	die("coprocessor segment overrun",esp,error_code);
}

void do_invalid_TSS(long esp,long error_code)
{
	die("invalid TSS",esp,error_code);
}

void do_segment_not_present(long esp,long error_code)
{
	die("segment not present",esp,error_code);
}

void do_stack_segment(long esp,long error_code)
{
	die("stack segment",esp,error_code);
}

void do_coprocessor_error(long esp, long error_code)
{
	if (last_task_used_math != current)
		return;
	die("coprocessor error",esp,error_code);
}

void do_reserved(long esp, long error_code)
{
	die("reserved (15,17-47) error",esp,error_code);
}

// �������쳣�����壩�жϳ����ʼ���ӳ����������ǵ��жϵ����ţ��ж���������
// set_trap_gate()��set_system_gate()����Ҫ��������ǰ�����õ���Ȩ��Ϊ0��������3�����
// �ϵ������ж�int3������ж�overflow �ͱ߽�����ж�bounds �������κγ��������
// ��������������Ƕ��ʽ�������(include/asm/system.h,��36 �С�39 ��)��
void trap_init(void)
{
	int i;

	set_trap_gate(0,&divide_error);// ���ó�����������ж�����ֵ��������ͬ��
	set_trap_gate(1,&debug);
	set_trap_gate(2,&nmi);
	set_system_gate(3,&int3);	/* int3-5 can be called from all */
	set_system_gate(4,&overflow);
	set_system_gate(5,&bounds);
	set_trap_gate(6,&invalid_op);
	set_trap_gate(7,&device_not_available);
	set_trap_gate(8,&double_fault);
	set_trap_gate(9,&coprocessor_segment_overrun);
	set_trap_gate(10,&invalid_TSS);
	set_trap_gate(11,&segment_not_present);
	set_trap_gate(12,&stack_segment);
	set_trap_gate(13,&general_protection);
	set_trap_gate(14,&page_fault);
	set_trap_gate(15,&reserved);
	set_trap_gate(16,&coprocessor_error);
// ���潫int17-48 ���������Ⱦ�����Ϊreserved���Ժ�ÿ��Ӳ����ʼ��ʱ�����������Լ��������š�
	for (i=17;i<48;i++)
		set_trap_gate(i,&reserved);
	set_trap_gate(45,&irq13);// ����Э�������������š�
	outb_p(inb_p(0x21)&0xfb,0x21);// ������8259A оƬ��IRQ2 �ж�����
	outb(inb_p(0xA1)&0xdf,0xA1);// �����8259A оƬ��IRQ13 �ж�����
	set_trap_gate(39,&parallel_interrupt);// ���ò��пڵ������š�
}
