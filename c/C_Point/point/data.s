	.file	"data.c"
	.text
	.section	.rodata
.LC0:
	.string	"Hello World"
.LC1:
	.string	"str1 addr: %p \n"
.LC2:
	.string	"str2 addr: %p \n"
.LC3:
	.string	"str3 addr: %p \n"
.LC4:
	.string	"str4 addr: %p \n"
.LC5:
	.string	"str1 and str2 are not same"
.LC6:
	.string	"str3 and str4 are same"
.LC7:
	.string	"str3 and str4 are not same"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$8022916924116329800, %rax
	movq	%rax, -32(%rbp)
	movl	$6581362, -24(%rbp)
	movq	%rax, -20(%rbp)
	movl	$6581362, -12(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -48(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -40(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-20(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-40(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
	movq	-48(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jne	.L2
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	jmp	.L3
.L2:
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
.L3:
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
