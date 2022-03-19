;/* passed
;* linux/kernel/system_call.s
;*
;* (C) 1991 Linus Torvalds
;*/
.386p
.model flat

;/*
;* system_call.s contains the system-call low-level handling routines.
;* This also contains the timer-interrupt handler, as some of the code is
;* the same. The hd- and flopppy-interrupts are also here.
;*
;* NOTE: This code handles signal-recognition, which happens every time
;* after a timer-interrupt and after each system call. Ordinary interrupts
;* don't handle signal-recognition, as that would clutter them up totally
;* unnecessarily.
;*
;* Stack layout in 'ret_from_system_call':
;*
;* 0(%esp) - %eax
;* 4(%esp) - %ebx
;* 8(%esp) - %ecx
;* C(%esp) - %edx
;* 10(%esp) - %fs
;* 14(%esp) - %es
;* 18(%esp) - %ds
;* 1C(%esp) - %eip
;* 20(%esp) - %cs
;* 24(%esp) - %eflags
;* 28(%esp) - %oldesp
;* 2C(%esp) - %oldss
;*/
;/*
;* system_call.s �ļ�����ϵͳ����(system-call)�ײ㴦���ӳ���������Щ����Ƚ����ƣ�����
;* ͬʱҲ����ʱ���жϴ���(timer-interrupt)�����Ӳ�̺����̵��жϴ������Ҳ�����
;*
;* ע�⣺��δ��봦���ź�(signal)ʶ����ÿ��ʱ���жϺ�ϵͳ����֮�󶼻����ʶ��һ��
;* �ж��źŲ��������ź�ʶ����Ϊ���ϵͳ��ɻ��ҡ�
;*
;* ��ϵͳ���÷��أ�'ret_from_system_call'��ʱ��ջ�����ݼ�����19-30 �С�
;*/
SIG_CHLD = 17 ;// ����SIG_CHLD �źţ��ӽ���ֹͣ���������

R_EAX = 00h ;// ��ջ�и����Ĵ�����ƫ��λ�á�
R_EBX = 04h
R_ECX = 08h
R_EDX = 0Ch
R_FS = 10h
R_ES = 14h
R_DS = 18h
R_EIP = 1Ch
R_CS = 20h
EFLAGS = 24h
OLDR_ESP = 28h ;// ������Ȩ���仯ʱ��
OLR_DSS = 2Ch

;// ������Щ������ṹ(task_struct)�б�����ƫ��ֵ���μ�include/linux/sched.h��77 �п�ʼ��
state = 0 ;// these are offsets into the task-struct. ;// ����״̬��
counter = 4 ;// ��������ʱ�����(�ݼ�)���δ�����������ʱ��Ƭ��
priority = 8 ;// ����������������ʼ����ʱcounter=priority��Խ��������ʱ��Խ����
signal = 12 ;// ���ź�λͼ��ÿ������λ����һ���źţ��ź�ֵ=λƫ��ֵ+1��
sigaction = 16 ;// MUST be 16 (=len of sigaction) // sigaction �ṹ���ȱ�����16 �ֽڡ�
;// �ź�ִ�����Խṹ�����ƫ��ֵ����Ӧ�źŽ�Ҫִ�еĲ����ͱ�־��Ϣ��
blocked = (33*16) ;// �������ź�λͼ��ƫ������

;// ���¶�����sigaction �ṹ�е�ƫ�������μ�include/signal.h����48 �п�ʼ��
;// offsets within sigaction
sa_handler = 0 ;// �źŴ�����̵ľ��������������
sa_mask = 4 ;// �ź���������
sa_flags = 8 ;// �źż���
sa_restorer = 12 ;// ���ػָ�ִ�еĵ�ַλ�á�

nr_system_calls = 72 ;// Linux 0.11 ���ں��е�ϵͳ����������

;/*
;* Ok, I get parallel printer interrupts while using the floppy for some
;* strange reason. Urgel. Now I just ignore them.
;*/
;/*
;* ���ˣ���ʹ������ʱ���յ��˲��д�ӡ���жϣ�����֡��ǣ����ڲ�������
;*/
;// ������ڵ㡣
extrn _schedule:proc,_do_signal:proc,_math_error:proc
extrn _math_state_restore:proc,_math_emulate:proc,_jiffies:proc
extrn _do_timer:proc,_do_execve:proc
extrn _find_empty_process:proc,_copy_process:proc
extrn _do_floppy:proc,_unexpected_floppy_interrupt:proc
extrn _do_hd:proc,_unexpected_hd_interrupt:proc

extrn _current:dword,_task:dword,_sys_call_table:dword

public _system_call,_sys_fork,_timer_interrupt,_sys_execve
public _hd_interrupt,_floppy_interrupt,_parallel_interrupt
public _device_not_available, _coprocessor_error

.code

;// �����ϵͳ���úš�
align 4 ;// �ڴ�4 �ֽڶ��롣
bad_sys_call:
	mov eax,-1 ;// eax ����-1���˳��жϡ�
	iretd
;// ����ִ�е��ȳ�����ڡ����ȳ���schedule ��(kernel/sched.c,104)��
align 4
reschedule:
	push ret_from_sys_call ;// ��ret_from_sys_call �ĵ�ַ��ջ��101 �У���
	jmp _schedule
;//// int 0x80 --linux ϵͳ������ڵ�(�����ж�int 0x80��eax ���ǵ��ú�)��
align 4
_system_call:
	cmp eax,nr_system_calls-1 ;// ���ú����������Χ�Ļ�����eax ����-1 ���˳���
	ja bad_sys_call
	push ds ;// ����ԭ�μĴ���ֵ��
	push es
	push fs
	push edx ;// ebx,ecx,edx �з���ϵͳ������Ӧ��C ���Ժ����ĵ��ò�����
	push ecx ;// push %ebx,%ecx,%edx as parameters
	push ebx ;// to the system call
	mov edx,10h ;// set up ds,es to kernel space
	mov ds,dx ;// ds,es ָ���ں����ݶ�(ȫ���������������ݶ�������)��
	mov es,dx
	mov edx,17h ;// fs points to local data space
	mov fs,dx ;// fs ָ��ֲ����ݶ�(�ֲ��������������ݶ�������)��
;// �������������ĺ����ǣ����õ�ַ = _sys_call_table + %eax * 4���μ��б���˵����
;// ��Ӧ��C �����е�sys_call_table ��include/linux/sys.h �У����ж�����һ������72 ��
;// ϵͳ����C �������ĵ�ַ�����
	call [_sys_call_table+eax*4]
	push eax ;// ��ϵͳ���ú���ջ��
	mov eax,_current ;// ȡ��ǰ���񣨽��̣����ݽṹ��ַ??eax��
;// ����97-100 �в鿴��ǰ���������״̬��������ھ���״̬(state ������0)��ȥִ�е��ȳ���
;// ����������ھ���״̬��counter[??]ֵ����0����Ҳȥִ�е��ȳ���
	cmp dword ptr [state+eax],0 ;// state
	jne reschedule
	cmp dword ptr [counter+eax],0 ;// counter
	je reschedule
;// ������δ���ִ�д�ϵͳ����C �������غ󣬶��ź�������ʶ����
ret_from_sys_call:
;// �����б�ǰ�����Ƿ��ǳ�ʼ����task0��������򲻱ض�������ź�������Ĵ���ֱ�ӷ��ء�
;// 103 ���ϵ�_task ��ӦC �����е�task[]���飬ֱ������task �൱������task[0]��
	mov eax,_current ;// task[0] cannot have signals
	cmp eax,_task
	je l1 ;// ��ǰ(forward)��ת�����l1��
;// ͨ����ԭ���ó������ѡ����ļ�����жϵ��ó����Ƿ��ǳ����û�������ǳ����û���ֱ��
;// �˳��жϣ�����������ź����Ĵ�������Ƚ�ѡ����Ƿ�Ϊ��ͨ�û�����ε�ѡ���0x000f
;// (RPL=3���ֲ�����1 ����(�����))�������������ת�˳��жϳ���
	cmp word ptr [R_CS+esp],0fh ;// was old code segment supervisor ?
	jne l1
;// ���ԭ��ջ��ѡ�����Ϊ0x17��Ҳ��ԭ��ջ�����û����ݶ��У�����Ҳ�˳���
	cmp word ptr [OLR_DSS+esp],17h ;// was stack segment = 0x17 ?
	jne l1
;// ������δ��루109-120������;������ȡ��ǰ����ṹ�е��ź�λͼ(32 λ��ÿλ����1 ���ź�)��
;// Ȼ��������ṹ�е��ź����������Σ��룬������������ź�λ��ȡ����ֵ��С���ź�ֵ���ٰ�
;// ԭ�ź�λͼ�и��źŶ�Ӧ��λ��λ����0������󽫸��ź�ֵ��Ϊ����֮һ����do_signal()��
;// do_signal()�ڣ�kernel/signal.c,82���У����������13 ����ջ����Ϣ��
	mov ebx,[signal+eax] ;// ȡ�ź�λͼ??ebx��ÿ1 λ����1 ���źţ���32 ���źš�
	mov ecx,[blocked+eax] ;// ȡ���������Σ��ź�λͼ??ecx��
	not ecx ;// ÿλȡ����
	and ecx,ebx ;// �����ɵ��ź�λͼ��
	bsf ecx,ecx ;// �ӵ�λ��λ0����ʼɨ��λͼ�����Ƿ���1 ��λ��
;// ���У���ecx ������λ��ƫ��ֵ�����ڼ�λ0-31����
	je l1 ;// ���û���ź�����ǰ��ת�˳���
	btr ebx,ecx ;// ��λ���źţ�ebx ����ԭsignal λͼ����
	mov dword ptr [signal+eax],ebx ;// ���±���signal λͼ��Ϣ??current->signal��
	inc ecx ;// ���źŵ���Ϊ��1 ��ʼ����(1-32)��
	push ecx ;// �ź�ֵ��ջ��Ϊ����do_signal �Ĳ���֮һ��
	call _do_signal ;// ����C �����źŴ������(kernel/signal.c,82)
	pop eax ;// �����ź�ֵ��
l1: pop eax
	pop ebx
	pop ecx
	pop edx
	pop fs
	pop es
	pop ds
	iretd

;//// int16 -- ������δ��봦��Э�����������ĳ����źš���תִ��C ����math_error()
;// (kernel/math/math_emulate.c,82)�����غ���ת��ret_from_sys_call ������ִ�С�
align 4
_coprocessor_error:
	push ds
	push es
	push fs
	push edx
	push ecx
	push ebx
	push eax
	mov eax,10h ;// ds,es ��Ϊָ���ں����ݶΡ�
	mov ds,ax
	mov es,ax
	mov eax,17h ;// fs ��Ϊָ��ֲ����ݶΣ������������ݶΣ���
	mov fs,ax
	push ret_from_sys_call ;// ��������÷��صĵ�ַ��ջ��
	jmp _math_error ;// ִ��C ����math_error()(kernel/math/math_emulate.c,37)

;//// int7 -- �豸�����ڻ�Э������������(Coprocessor not available)��
;// ������ƼĴ���CR0 ��EM ��־��λ����CPU ִ��һ��R_ESC ת��ָ��ʱ�ͻ��������жϣ�������
;// �����л���������жϴ������ģ��R_ESC ת��ָ�169 �У���
;// CR0 ��TS ��־����CPU ִ������ת��ʱ���õġ�TS ��������ȷ��ʲôʱ��Э�������е����ݣ������ģ�
;// ��CPU ����ִ�е�����ƥ���ˡ���CPU ������һ��ת��ָ��ʱ����TS ��λ�ˣ��ͻ��������жϡ�
;// ��ʱ��Ӧ�ûָ��������Э������ִ��״̬��165 �У����μ�(kernel/sched.c,77)�е�˵����
;// ���ж����ת�Ƶ����ret_from_sys_call ��ִ����ȥ����Ⲣ�����źţ���
align 4
_device_not_available:
	push ds
	push es
	push fs
	push edx
	push ecx
	push ebx
	push eax
	mov eax,10h ;// ds,es ��Ϊָ���ں����ݶΡ�
	mov ds,ax
	mov es,ax
	mov eax,17h ;// fs ��Ϊָ��ֲ����ݶΣ������������ݶΣ���
	mov fs,ax
	push ret_from_sys_call ;// ��������ת����õķ��ص�ַ��ջ��
	clts ;// clear TS so that we can use math
	mov eax,cr0
	test eax,4h ;// EM (math emulation bit)
;// �������EM ������жϣ���ָ�������Э������״̬��
	je  goto_math_state_restore;// ִ��C ����math_state_restore()(kernel/sched.c,77)��
	push ebp
	push esi
	push edi
	call _math_emulate ;// ����C ����math_emulate(kernel/math/math_emulate.c,18)��
	pop edi
	pop esi
	pop ebp
	ret ;// �����ret ����ת��ret_from_sys_call(101 ��)��
goto_math_state_restore:
	jmp _math_state_restore

;//// int32 -- (int 0x20) ʱ���жϴ�������ж�Ƶ�ʱ�����Ϊ100Hz(include/linux/sched.h,5)��
;// ��ʱоƬ8253/8254 ����(kernel/sched.c,406)����ʼ���ġ��������jiffies ÿ10 �����1��
;// ��δ��뽫jiffies ��1�����ͽ����ж�ָ���8259 ��������Ȼ���õ�ǰ��Ȩ����Ϊ��������
;// C ����do_timer(long CPL)�������÷���ʱתȥ��Ⲣ�����źš�
align 4
_timer_interrupt:
	push ds ;// save ds,es and put kernel data space
	push es ;// into them. %fs is used by _system_call
	push fs
	push edx ;// we save %eax,%ecx,%edx as gcc doesn't
	push ecx ;// save those across function calls. %ebx
	push ebx ;// is saved as we use that in ret_sys_call
	push eax
	mov eax,10h ;// ds,es ��Ϊָ���ں����ݶΡ�
	mov ds,ax
	mov es,ax
	mov eax,17h ;// fs ��Ϊָ��ֲ����ݶΣ������������ݶΣ���
	mov fs,ax
	inc dword ptr _jiffies
;// ���ڳ�ʼ���жϿ���оƬʱû�в����Զ�EOI������������Ҫ��ָ�������Ӳ���жϡ�
	mov al,20h ;// EOI to interrupt controller ;//1
	out 20h,al ;// ����������OCW2 ��0x20 �˿ڡ�
;// ����3 ���ѡ�����ȡ����ǰ��Ȩ����(0 ��3)��ѹ���ջ����Ϊdo_timer �Ĳ�����
	mov eax,dword ptr [R_CS+esp]
	and eax,3 ;// %eax is CPL (0 or 3, 0=supervisor)
	push eax
;// do_timer(CPL)ִ�������л�����ʱ�ȹ�������kernel/shched.c,305 ��ʵ�֡�
	call _do_timer ;// 'do_timer(long CPL)' does everything from
	add esp,4 ;// task switching to accounting ...
	jmp ret_from_sys_call

;//// ����sys_execve()ϵͳ���á�ȡ�жϵ��ó���Ĵ���ָ����Ϊ��������C ����do_execve()��
;// do_execve()��(fs/exec.c,182)��
align 4
_sys_execve:
	lea eax,[R_EIP+esp]
	push eax
	call _do_execve
	add esp,4 ;// ��������ʱѹ��ջ��R_EIP ֵ��
	ret

;//// sys_fork()���ã����ڴ����ӽ��̣���system_call ����2��ԭ����include/linux/sys.h �С�
;// ���ȵ���C ����find_empty_process()��ȡ��һ�����̺�pid�������ظ�����˵��Ŀǰ��������
;// ������Ȼ�����copy_process()���ƽ��̡�
align 4
_sys_fork:
	call _find_empty_process ;// ����find_empty_process()(kernel/fork.c,135)��
	test eax,eax
	js l2
	push gs
	push esi
	push edi
	push ebp
	push eax
	call _copy_process ;// ����C ����copy_process()(kernel/fork.c,68)��
	add esp,20 ;// ������������ѹջ���ݡ�
l2: ret

;//// int 46 -- (int 0x2E) Ӳ���жϴ��������ӦӲ���ж�����IRQ14��
;// ��Ӳ�̲�����ɻ����ͻᷢ�����ж��źš�(�μ�kernel/blk_drv/hd.c)��
;// ������8259A �жϿ��ƴ�оƬ���ͽ���Ӳ���ж�ָ��(EOI)��Ȼ��ȡ����do_hd �еĺ���ָ�����edx
;// �Ĵ����У�����do_hd ΪNULL�������ж�edx ����ָ���Ƿ�Ϊ�ա����Ϊ�գ����edx ��ֵָ��
;// unexpected_hd_interrupt()��������ʾ������Ϣ�������8259A ��оƬ��EOI ָ�������edx ��
;// ָ��ָ��ĺ���: read_intr()��write_intr()��unexpected_hd_interrupt()��
_hd_interrupt:
	push eax
	push ecx
	push edx
	push ds
	push es
	push fs
	mov eax,10h ;// ds,es ��Ϊ�ں����ݶΡ�
	mov ds,ax
	mov es,ax
	mov eax,17h ;// fs ��Ϊ���ó���ľֲ����ݶΡ�
	mov fs,ax
;// ���ڳ�ʼ���жϿ���оƬʱû�в����Զ�EOI������������Ҫ��ָ�������Ӳ���жϡ�
	mov al,20h
	out 0A0h,al ;// EOI to interrupt controller ;//1 ;// �ʹ�8259A��
	jmp l3 ;// give port chance to breathe
l3: jmp l4 ;// ��ʱ���á�
l4: xor edx,edx
	xchg edx,dword ptr _do_hd ;// do_hd ����Ϊһ������ָ�룬������ֵread_intr()��
;// write_intr()������ַ��(kernel/blk_drv/hd.c)
;// �ŵ�edx �Ĵ�����ͽ�do_hd ָ�������ΪNULL��
	test edx,edx ;// ���Ժ���ָ���Ƿ�ΪNull��
	jne l5 ;// ���գ���ʹָ��ָ��C ����unexpected_hd_interrupt()��
	mov edx,dword ptr _unexpected_hd_interrupt ;// (kernel/blk_drv/hdc,237)��
l5: out 20h,al ;// ����8259A �жϿ�����EOI ָ�����Ӳ���жϣ���
	call edx ;// "interesting" way of handling intr.
	pop fs ;// �Ͼ����do_hd ָ���C ������
	pop es
	pop ds
	pop edx
	pop ecx
	pop eax
	iretd

;//// int38 -- (int 0x26) �����������жϴ��������ӦӲ���ж�����IRQ6��
;// �䴦������������Ӳ�̵Ĵ������һ����(kernel/blk_drv/floppy.c)��
;// ������8259A �жϿ�������оƬ����EOI ָ�Ȼ��ȡ����do_floppy �еĺ���ָ�����eax
;// �Ĵ����У�����do_floppy ΪNULL�������ж�eax ����ָ���Ƿ�Ϊ�ա���Ϊ�գ����eax ��ֵָ��
;// unexpected_floppy_interrupt ()��������ʾ������Ϣ��������eax ָ��ĺ���: rw_interrupt,
;// seek_interrupt,recal_interrupt,reset_interrupt ��unexpected_floppy_interrupt��
_floppy_interrupt:
	push eax
	push ecx
	push edx
	push ds
	push es
	push fs
	mov eax,10h ;// ds,es ��Ϊ�ں����ݶΡ�
	mov ds,ax
	mov es,ax
	mov eax,17h ;// fs ��Ϊ���ó���ľֲ����ݶΡ�
	mov fs,ax
	mov al,20h ;// ����8259A �жϿ�����EOI ָ�����Ӳ���жϣ���
	out 20h,al ;// EOI to interrupt controller ;//1
	xor eax,eax
	xchg eax,dword ptr _do_floppy ;// do_floppy Ϊһ����ָ�룬������ֵʵ�ʴ���C ��������
;// �ŵ�eax �Ĵ�����ͽ�do_floppy ָ������ÿա�
	test eax,eax ;// ���Ժ���ָ���Ƿ�=NULL?
	jne l6 ;// ���գ���ʹָ��ָ��C ����unexpected_floppy_interrupt()��
	mov eax,dword ptr _unexpected_floppy_interrupt
l6: call eax ;// "interesting" way of handling intr.
	pop fs ;// �Ͼ����do_floppy ָ��ĺ�����
	pop es
	pop ds
	pop edx
	pop ecx
	pop eax
	iretd

;//// int 39 -- (int 0x27) ���п��жϴ�����򣬶�ӦӲ���ж������ź�IRQ7��
;// ���汾�ں˻�δʵ�֡�����ֻ�Ƿ���EOI ָ�
_parallel_interrupt:
	push eax
	mov al,20h
	out 20h,al
	pop eax
	iretd
end