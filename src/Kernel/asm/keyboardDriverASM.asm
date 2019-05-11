GLOBAL _getKeyPress

section .text

_getKeyPress:
push rbp
    mov rbp,rsp
    mov rax, 0
    in al, 64h
    test al, 01h
    jz return
    in al,60h
    return:
        pop rbp
        ret
