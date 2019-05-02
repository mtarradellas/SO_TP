GLOBAL _runProcess


section .text

_runProcess:
	mov rsp, [rsp + 8]
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
	