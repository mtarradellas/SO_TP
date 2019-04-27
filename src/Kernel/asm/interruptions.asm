GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler

GLOBAL _exception0Handler
GLOBAL _exceptionInvalidOpcodeHandler

GLOBAL _syscall_handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
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
%endmacro

%macro irqHandlerMaster 1

	pushState

	mov rdi, %1 ; first parameter
	
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	;mov rax, 27d		for testing exception handler
	;mov r10, 10d


	pushState

	mov rdi, %1 ; first parameter
	mov rsi, rsp ; second parameter (stackpointer)
	call exceptionDispatcher

	popState

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	;mov rsp, rbp   ;  Libera el stack utilizado por la funcion anterior que 
	;add rsp, 8		;  no llego a liberarse al tener una exception, no estaba 
				   	;  completamente seguro de como lograr que funcione 

	mov qword [rsp],  400000h  ; set return to shell initialize

	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h, al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  
    out	0A1h, al
    pop     rbp
    retn


; Timer Tick
_irq00Handler:
	irqHandlerMaster 0

; Keyboard
_irq01Handler:
	irqHandlerMaster 1

; Zero Division Exception
_exception0Handler:
	exceptionHandler 0

; Invalid Opcode Exception
_exceptionInvalidOpcodeHandler:
	exceptionHandler 1

; System Call
_syscall_handler:
	
	push rbp
	mov rbp, rsp

	pushState

	call syscallDispatcher

	popState
	pop rbp

	iretq

haltcpu:
	cli
	hlt
	ret


SECTION .bss
	aux resq 1