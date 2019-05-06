GLOBAL _runProcess
GLOBAL _initProcess
GLOBAL _interrupt

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

	; End Of Interrupt
	mov al, 20h
	out 20h, al
	
	pop rax

	iretq

_initProcess:

	push rbp
	mov rbp, rsp

	mov rsp, rdi ; stack pointer of new process
	mov rbx, rsp ; base stack pointer for new process
	; simulation for interruped stack
	push 0x0    ; Align
	push 0x0	; SS
	push rbx  ; RSP
	push 0x202	; FLAGS
	push 0x08	; CS
	push rsi  ; IP for main of new process
	; new registers for the new process to pop, numbered for testing
	push 1  ; rax
	push 2  ; rbx
	push 3  ; rcx
	push 4  ; rdx
	push rdi  ; rbp new base pointer
	push rdx  ; rdi (argv for main)
	push rcx  ; rsi (argc for main)
	push 8  ; r8
	push 9  ; r9
	push 9  ; r10
	push 11  ; r11
	push 12  ; r12
	push 13  ; r13
	push 14  ; r14
	push 15  ; r15

	mov rax, rsp

	mov rsp, rbp
	pop rbp
	ret

_interrupt:
	int 20h
	ret