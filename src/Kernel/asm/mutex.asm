GLOBAL _mutexAcquire

section .text

_mutexAcquire:
	push rbp
	mov rbp, rsp

	mov eax, 1
	xchg eax, [rdi]

	mov rsp, rbp
	pop rbp
  ret