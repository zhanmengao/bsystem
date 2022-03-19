;/* passed
;* linux/kernel/rs_io.s
;*
;* (C) 1991 Linus Torvalds
;*/
.386p
.model flat
;/*
;* rs_io.s
;*
;* This module implements the rs232 io interrupts.
;*/
;/*
;* �ó���ģ��ʵ��rs232 ��������жϴ������
;*/

;// size �Ƕ�д���л��������ֽڳ��ȡ�
bsize = 1024 ;/* must be power of two ! ������2 �Ĵη�������
			 ; and must match the value ��tty_io.c �е�ֵƥ��!
			 ; in tty_io.c!!! */

;/* these are the offsets into the read/write buffer structures */
;/* ������Щ�Ƕ�д����ṹ�е�ƫ���� */
;// ��Ӧ������include/linux/tty.h �ļ���tty_queue �ṹ�и�������ƫ������
rs_addr = 0 ;// ���ж˿ں��ֶ�ƫ�ƣ��˿ں���0x3f8 ��0x2f8����
head = 4 ;// ��������ͷָ���ֶ�ƫ�ơ�
tail = 8 ;// ��������βָ���ֶ�ƫ�ơ�
proc_list = 12 ;// �ȴ��û���Ľ����ֶ�ƫ�ơ�
buf = 16 ;// �������ֶ�ƫ�ơ�

startup = 256 ;/* chars left in write queue when we restart it */
;/* ��д�����ﻹʣ256 ���ַ��ռ�(WAKEUP_CHARS)ʱ�����ǾͿ���д */

;/*
;* These are the actual interrupt routines. They look where
;* the interrupt is coming from, and take appropriate action.
;*/
;/*
;* ��Щ��ʵ�ʵ��жϳ��򡣳������ȼ���жϵ���Դ��Ȼ��ִ����Ӧ
;* �Ĵ���
;*/
extrn _table_list:dword, _do_tty_interrupt:proc
public _rs1_interrupt,_rs2_interrupt

.code
align 4
;// ���ж˿�1 �жϴ��������ڵ㡣
_rs1_interrupt:
	push _table_list+8 ;// tty ���ж�Ӧ����1 �Ķ�д����ָ��ĵ�ַ��ջ(tty_io.c��99)��
	jmp rs_int
align 4
;// ���ж˿�2 �жϴ��������ڵ㡣
_rs2_interrupt:
	push _table_list+16 ;// tty ���ж�Ӧ����2 �Ķ�д�������ָ��ĵ�ַ��ջ��
rs_int:
	push edx
	push ecx
	push ebx
	push eax
	push es
	push ds ;/* as this is an interrupt, we cannot */
	push 10h ;/* know that bs is ok. Load it */
	pop ds ;/* ��������һ���жϳ������ǲ�֪��ds �Ƿ���ȷ��*/
	push 10h ;/* ���Լ�������(��ds��es ָ���ں����ݶ� */
	pop es
	mov edx,24[esp] ;// ���������ָ���ַ����edx �Ĵ�����
;// Ҳ��35 ��39 ��������ѹ���ջ�ĵ�ַ��
	mov edx,[edx] ;// ȡ������ָ��(��ַ)->edx��
	mov edx,rs_addr[edx] ;// ȡ����1 �Ķ˿ں�??edx��
	add edx,2 ;/* interrupt ident. reg */ /* edx ָ���жϱ�ʶ�Ĵ��� */
rep_int: ;// �жϱ�ʶ�Ĵ����˿���0x3fa��0x2fa�����μ��Ͻ��б����Ϣ��
	xor eax,eax ;// eax ���㡣
	in al,dx ;// ȡ�жϱ�ʶ�ֽڣ������ж��ж���Դ(��4 ���ж����)��
	test al,1 ;// �����ж����޴�������ж�(λ0=1 ���жϣ�=0 ���ж�)��
	jne end1 ;// ���޴������жϣ�����ת���˳�����end��
	cmp al,6 ;/* this shouldn't happen, but ... */ /* �ⲻ�ᷢ�������ǡ�*/
	ja end1 ;// al ֵ>6? ������ת��end��û������״̬����
	mov ecx,24[esp] ;// ��ȡ�������ָ���ַ??ecx��
	push edx ;// ���˿ں�0x3fa(0x2fa)��ջ��
	sub edx,2 ;// 0x3f8(0x2f8)��
	call jmp_table[eax*2] ;/* NOTE! not ;*4, bit0 is 0 already */ /* ����4��λ0 ����0*/
;// ���������ָ�����д������ж�ʱ��al ��λ0=0��λ2-1 ���ж����ͣ�����൱���Ѿ����ж�����
;// ����2�������ٳ�2���õ���ת���Ӧ���ж����͵�ַ������ת������ȥ����Ӧ����
	pop edx ;// �����жϱ�ʶ�Ĵ����˿ں�0x3fa����0x2fa����
	jmp rep_int ;// ��ת�������ж����޴������жϲ���������
end1:
	mov al,20h ;// ���жϿ��������ͽ����ж�ָ��EOI��
	out 20h,al ;/* EOI */
	pop ds
	pop es
	pop eax
	pop ebx
	pop ecx
	pop edx
	add esp,4 ;// jump over _table_list entry # �����������ָ���ַ��
	iretd

;// ���ж����ʹ�������ַ��ת������4 ���ж���Դ��
;// modem ״̬�仯�жϣ�д�ַ��жϣ����ַ��жϣ���·״̬�������жϡ�
jmp_table:
	dd modem_status,write_char,read_char,line_status

align 4
modem_status:
	add edx,6 ;/* clear intr by reading modem status reg */
	in al,dx ;/* ͨ����modem ״̬�Ĵ������и�λ(0x3fe) */
	ret

align 4
line_status:
	add edx,5 ;/* clear intr by reading line status reg. */
	in al,dx ;/* ͨ������·״̬�Ĵ������и�λ(0x3fd) */
	ret

align 4
read_char:
	in al,dx ;// ��ȡ�ַ�->al��
	mov edx,ecx ;// ��ǰ���ڻ������ָ���ַ??edx��
	sub edx,_table_list ;// �������ָ�����ַ - ��ǰ���ڶ���ָ���ַ??edx��
	shr edx,3 ;// ��ֵ/8�����ڴ���1 ��1�����ڴ���2 ��2��
	mov ecx,[ecx] ;// read-queue # ȡ��������нṹ��ַ??ecx��
	mov ebx,head[ecx] ;// ȡ�������л���ͷָ��??ebx��
	mov buf[ebx+ecx],al ;// ���ַ����ڻ�������ͷָ����ָ��λ�á�
	inc ebx ;// ��ͷָ��ǰ��һ�ֽڡ�
	and ebx,bsize-1 ;// �û�������С��ͷָ�����ģ������ָ�벻�ܳ�����������С��
	cmp ebx,tail[ecx] ;// ������ͷָ����βָ��Ƚϡ�
	je l1 ;// ����ȣ���ʾ������������ת�����1 ����
	mov head[ecx],ebx ;// �����޸Ĺ���ͷָ�롣
l1: push edx ;// �����ں�ѹ���ջ(1- ����1��2 - ����2)����Ϊ������
	call _do_tty_interrupt ;// ����tty �жϴ���C ��������
	add esp,4 ;// ������ջ�����������ء�
	ret

align 4
write_char:
	mov ecx,[4+ecx] ;// write-queue # ȡд������нṹ��ַ??ecx��
	mov ebx,head[ecx] ;// ȡд����ͷָ��??ebx��
	sub ebx,tail[ecx] ;// ͷָ�� - βָ�� = �������ַ�����
	and ebx,bsize-1 ;// nr chars in queue # ��ָ��ȡģ���㡣
	je write_buffer_empty ;// ���ͷָ�� = βָ�룬˵��д�������ַ�����ת����
	cmp ebx,startup ;// �������ַ�������256 ����
	ja l2 ;// ����������ת����
	mov ebx,proc_list[ecx] ;// wake up sleeping process # ���ѵȴ��Ľ��̡�
;// ȡ�ȴ��ö��еĽ��̵�ָ�룬���ж��Ƿ�Ϊ�ա�
	test ebx,ebx ;// is there any? # �еȴ��Ľ�����
	je l2 ;// �ǿյģ�����ǰ��ת�����1 ����
	mov dword ptr [ebx],0 ;// ���򽫽�����Ϊ������״̬(���ѽ���)����
l2: mov ebx,tail[ecx] ;// ȡβָ�롣
	mov al,buf[ebx+ecx] ;// �ӻ�����βָ�봦ȡһ�ַ�??al��
	out dx,al ;// ��˿�0x3f8(0x2f8)�ͳ������ּĴ����С�
	inc ebx ;// βָ��ǰ�ơ�
	and ebx,bsize-1 ;// βָ������������ĩ�ˣ����ۻء�
	mov tail[ecx],ebx ;// �������޸Ĺ���βָ�롣
	cmp ebx,head[ecx] ;// βָ����ͷָ��Ƚϣ�
	je write_buffer_empty ;// ����ȣ���ʾ�����ѿգ�����ת��
	ret
align 4
write_buffer_empty:
	mov ebx,proc_list[ecx] ;// wake up sleeping process # ���ѵȴ��Ľ��̡�
;// ȡ�ȴ��ö��еĽ��̵�ָ�룬���ж��Ƿ�Ϊ�ա�
	test ebx,ebx ;// is there any? # �еȴ��Ľ�����
	je l3 ;// �ޣ�����ǰ��ת�����1 ����
	mov dword ptr [ebx],0 ;// ���򽫽�����Ϊ������״̬(���ѽ���)��
l3: inc edx ;// ָ��˿�0x3f9(0x2f9)��
	in al,dx ;// ��ȡ�ж�����Ĵ�����
	jmp l4 ;// �����ӳ١�
l4: jmp l5
l5: and al,0dh ;/* disable transmit interrupt */
;/* ���η��ͱ��ּĴ������жϣ�λ1�� */
	out dx,al ;// д��0x3f9(0x2f9)��
	ret
end