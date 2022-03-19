	.file	"argvLower.c"
	.section	.rodata
.LC0:
	.string	"agrgc=%d,input para=%s\n"
.LC1:
	.string	"low"
.LC2:
	.string	"tolower,input=%c,output="
.LC3:
	.string	"upp"
.LC4:
	.string	"toupper,input=%c,output="
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rdx
	movl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$2, -20(%rbp)
	jle	.L2
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L2
	movq	-32(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movb	%al, -2(%rbp)
	movsbl	-2(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movsbl	-2(%rbp), %eax
	movl	%eax, %edi
	call	tolower@PLT
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L3
.L2:
	cmpl	$2, -20(%rbp)
	jle	.L3
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L3
	movq	-32(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movb	%al, -1(%rbp)
	movsbl	-1(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movsbl	-1(%rbp), %eax
	movl	%eax, %edi
	call	toupper@PLT
	movl	%eax, %edi
	call	putchar@PLT
.L3:
	movl	$9, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.2.0-8ubuntu3.2) 7.2.0"
	.section	.note.GNU-stack,"",@progbits
