GLOBAL _runProcess
GLOBAL _initProcess

section .text

_runProcess:
	mov rsp, rdi
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
	iretq

_initProcess:
	mov rbp, rsp
	mov rsp, [rbp + 32]
	add rsp, 8	; align
	push 0	; SS
	push [rbp + 32]	 ; RSP
	push 0x202	; EEFLAGS
	push 0x008	; CS
	push [rbp + 24]


