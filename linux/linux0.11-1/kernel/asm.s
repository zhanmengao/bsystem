;/* passed
;* linux/kernel/asm.s
;*
;* (C) 1991 Linus Torvalds
;*/
.386p
.model flat

;/*
;* asm.s contains the low-level code for most hardware faults.
;* page_exception is handled by the mm, so that isn't here. This
;* file also handles (hopefully) fpu-exceptions due to TS-bit, as
;* the fpu must be properly saved/resored. This hasn't been tested.
;* eax = -1
;* ϵͳ�жϵ���(eax=���ú�)
;* ebx,ecx,edx �з��е��ò���
;* ���úų���Χ?
;* �жϷ���
;* �Ĵ�����ջ
;* ds,es ָ���ں˴����
;* fs ָ��ֲ����ݶ�(�û�����)
;* ���ö�Ӧ��C ������
;* ����״̬?
;* ����schedule() ʱ��Ƭ=0��
;* ��ʼ����
;* ������ջ�ļĴ���
;* �����û�����?
;* �û���ջ?
;* ���ݽ����ź�λͼȡ���̵���
;* С�ź���������do signal()
;*/
;/*
;* asm.s �����а����󲿷ֵ�Ӳ�����ϣ����������ĵײ�δ��롣ҳ�쳣�����ڴ�������
;* mm ����ģ����Բ�������˳��򻹴���ϣ��������������TS-λ����ɵ�fpu �쳣��
;* ��Ϊfpu ������ȷ�ؽ��б���/�ָ�������Щ��û�в��Թ���
;*/

;// �������ļ���Ҫ�漰��Intel �������ж�int0--int16 �Ĵ���int17-int31 �������ʹ�ã���
;// ������һЩȫ�ֺ���������������ԭ����traps.c ��˵����
extrn _do_divide_error:far, _do_int3:far, _do_nmi:far, _do_overflow:far
extrn _do_bounds:far, _do_invalid_op:far, _do_coprocessor_segment_overrun:far
extrn _do_reserved:far, _coprocessor_error:far ptr, _do_double_fault:far
extrn _do_invalid_TSS:far, _do_segment_not_present:far
extrn _do_stack_segment:far, _do_general_protection:far

public _divide_error,_debug,_nmi,_int3,_overflow,_bounds,_invalid_op
public _double_fault,_coprocessor_segment_overrun
public _invalid_TSS,_segment_not_present,_stack_segment
public _general_protection,_irq13,_reserved

;// int0 -- ��������δ���ĺ���μ�ͼ4.1(a)����
;// �����Ǳ��������(divide_error)������롣���'_divide_error'ʵ������C ���Ժ�
;// ��divide_error()�����������ģ���ж�Ӧ�����ơ�'_do_divide_error'������traps.c �С�
.code
_divide_error:
	push dword ptr _do_divide_error ;// ���Ȱѽ�Ҫ���õĺ�����ַ��ջ����γ���ĳ����Ϊ0��
no_error_code: ;// �������޳���Ŵ������ڴ����������55 �еȡ�
	xchg [esp],eax ;// _do_divide_error �ĵ�ַ -> eax��eax ��������ջ��
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push ebp
	push ds ;// ����16 λ�ĶμĴ�����ջ��ҲҪռ��4 ���ֽڡ�
	push es
	push fs
	push 0 ;// "error code" ;// ����������ջ��
	lea edx,[esp+44] ;// ȡԭ���÷��ص�ַ����ջָ��λ�ã���ѹ���ջ��
	push edx
	mov edx,10h ;// �ں˴������ݶ�ѡ�����
	mov ds,dx
	mov es,dx
	mov fs,dx
	call eax ;// ����C ����do_divide_error()��
	add esp,8 ;// �ö�ջָ������ָ��Ĵ���fs ��ջ����
	pop fs
	pop es
	pop ds
	pop ebp
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax ;// ����ԭ��eax �е����ݡ�
	iretd

;// int1 -- debug �����ж���ڵ㡣�������ͬ�ϡ�
_debug:
	push _do_int3 ;// _do_debug C ����ָ����ջ������ͬ��
	jmp no_error_code

;// int2 -- �������жϵ�����ڵ㡣
_nmi:
	push _do_nmi
	jmp no_error_code

;// int3 -- ͬ_debug��
_int3:
	push _do_int3
	jmp no_error_code

;// int4 -- ����������ж���ڵ㡣
_overflow:
	push _do_overflow
	jmp no_error_code

;// int5 -- �߽�������ж���ڵ㡣
_bounds:
	push _do_bounds
	jmp no_error_code

;// int6 -- ��Ч����ָ������ж���ڵ㡣
_invalid_op:
	push _do_invalid_op
	jmp no_error_code

;// int9 -- Э�������γ��������ж���ڵ㡣
_coprocessor_segment_overrun:
	push _do_coprocessor_segment_overrun
	jmp no_error_code

;// int15 �C ������
_reserved:
	push _do_reserved
	jmp no_error_code

;// int45 -- ( = 0x20 + 13 ) ��ѧЭ��������Coprocessor���������жϡ�
;// ��Э������ִ����һ������ʱ�ͻᷢ��IRQ13 �ж��źţ���֪ͨCPU ������ɡ�
_irq13:
	push eax
	xor al,al ;// 80387 ��ִ�м���ʱ��CPU ��ȴ����������ɡ�
	out 0F0h,al ;// ͨ��д0xF0 �˿ڣ����жϽ�����CPU ��BUSY �����źţ�������
;// ����80387 �Ĵ�������չ��������PEREQ���ò�����Ҫ��Ϊ��ȷ��
;// �ڼ���ִ��80387 ���κ�ָ��֮ǰ����Ӧ���жϡ�
	mov al,20h
	out 20h,al ;// ��8259 ���жϿ���оƬ����EOI���жϽ������źš�
	jmp l1 ;// ��������תָ������ʱ���á�
l1: jmp l2
l2: out 0A0h,al ;// ����8259 ���жϿ���оƬ����EOI���жϽ������źš�
	pop eax
	jmp _coprocessor_error ;// _coprocessor_error ԭ���ڱ��ļ��У������Ѿ��ŵ�
							;// ��kernel/system_call.s, 131��

;// �����ж��ڵ���ʱ�����жϷ��ص�ַ֮�󽫳����ѹ���ջ����˷���ʱҲ��Ҫ������ŵ�����
;// int8 -- ˫������ϡ���������δ���ĺ���μ�ͼ4.1(b)����
_double_fault:
	push _do_double_fault ;// C ������ַ��ջ��
error_code:
	xchg [esp+4],eax ;// error code <-> %eax��eax ԭ����ֵ�������ڶ�ջ�ϡ�
	xchg [esp],ebx ;// &function <-> %ebx��ebx ԭ����ֵ�������ڶ�ջ�ϡ�
	push ecx
	push edx
	push edi
	push esi
	push ebp
	push ds
	push es
	push fs
	push eax ;// error code ;// �������ջ��
	lea eax,[esp+44] ;// offset ;// ���򷵻ص�ַ����ջָ��λ��ֵ��ջ��
	push eax
	mov eax,10h ;// ���ں����ݶ�ѡ�����
	mov ds,ax
	mov es,ax
	mov fs,ax
	call ebx ;// ������Ӧ��C ���������������ջ��
	add esp,8 ;// ��ջָ������ָ��ջ�з���fs ���ݵ�λ�á�
	pop fs
	pop es
	pop ds
	pop ebp
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	iretd

;// int10 -- ��Ч������״̬��(TSS)��
_invalid_TSS:
	push _do_invalid_TSS
	jmp error_code

;// int11 -- �β����ڡ�
_segment_not_present:
	push _do_segment_not_present
	jmp error_code

;// int12 -- ��ջ�δ���
_stack_segment:
	push _do_stack_segment
	jmp error_code

;// int13 -- һ�㱣���Գ���
_general_protection:
	push _do_general_protection
	jmp error_code

;// int7 -- �豸������(_device_not_available)��(kernel/system_call.s,148)
;// int14 -- ҳ����(_page_fault)��(mm/page.s,14)
;// int16 -- Э����������(_coprocessor_error)��(kernel/system_call.s,131)
;// ʱ���ж�int 0x20 (_timer_interrupt)��(kernel/system_call.s,176)
;// ϵͳ����int 0x80 (_system_call)�ڣ�kernel/system_call.s,80��
end
