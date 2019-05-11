GLOBAL _mutex_acquire

section .text

_mutex_acquire:
	push rbp
	mov rbp, rsp

	mov eax, 1
	xchg eax, [rdi]

	mov rsp, rbp
	pop rbp