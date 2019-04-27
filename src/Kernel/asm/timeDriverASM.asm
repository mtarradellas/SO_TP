GLOBAL _getHour
GLOBAL _getMinute
GLOBAL _getSecond

section .text

get:
	out 70h, al
	in al, 71h
	ret

_getHour:
    push rbp
    mov rbp, rsp

    call statusRegisterB

    mov rax, 0h
    mov al, 4h
    call get

    mov rsp, rbp
    pop rbp
    ret

_getMinute:
	push rbp
  	mov rbp, rsp

  	call statusRegisterB

	mov rax, 0h
    mov al, 2h
	call get

	mov rsp, rbp
	pop rbp
	ret

_getSecond:
 	push rbp
  	mov rbp, rsp

  	call statusRegisterB

	mov rax, 0h
	call get

	mov rsp, rbp
  	pop rbp
	ret

statusRegisterB:
	push rbp
  	mov rbp, rsp
	mov al,0x0B ; RTC Status Register B
	out 70h,al
	in al, 71h
	or al, 0x04
	out 71h, al	; 24 hour format time
	mov rsp, rbp
  	pop rbp
	ret
