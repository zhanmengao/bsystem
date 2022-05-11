.data
DBaseIndex qword 0
DFeInfo qword 0
DFeInfo2 qword 0
DCPUBaseInfo qword 0
DFeIndex qword 0
DCPUExInfo qword 0
.code

get_cpuid_info  PROC 
;DBaseIndex:qword, cCom:qword, DCPUBaseInfo:qword, DFeInfo:qword, DFeInfo2:qword, DFeIndex:qword, DCPUExInfo:qword, cProStr:qword
	push	rbp
	mov		rbp, rsp

	mov rsi, rcx	; cCom
	mov rdi, rdx	; cProStr
	
	xor rax, rax 
	cpuid 
	mov DBaseIndex, rax 

	mov qword ptr [rsi], rbx 
	mov qword ptr [rsi+8], rdx			;AMD CPU要把rcx改为rdx 
	mov qword ptr [rsi+16], rcx		;AMD CPU要把rdx改为rcx 

	mov rax, 1 
	cpuid 
	mov DCPUBaseInfo, rax 
	mov DFeInfo, rbx 
	mov DFeInfo2, rdx 

	mov rax, 80000000h
	cpuid 
	mov DFeIndex, rax 

	mov rax, 80000001h
	cpuid 
	mov DCPUExInfo, rax 

	mov rax, 80000002h
	cpuid 
	mov [rdi], rax 
	mov [rdi + 8]  , rbx 
	mov [rdi + 16]  , rcx 
	mov [rdi + 24]  ,rdx 

	mov rax, 80000003h
	cpuid 
	mov [rdi + 32]  , rax 
	mov [rdi + 40]  , rbx 
	mov [rdi + 48]  , rcx 
	mov [rdi + 56]  , rdx 

	mov rax, 80000004h
	cpuid 
	mov [rdi + 64]  , rax 
	mov [rdi + 72]  , rbx 
	mov [rdi + 80]  , rcx 
	mov [rdi + 88]  , rdx 
	
	pop rbp
	ret
	
get_cpuid_info ENDP
end