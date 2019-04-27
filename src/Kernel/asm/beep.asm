; research from:
;
; http://www.cs.binghamton.edu/~reckert/220/8254_timer.html
;
;Command register format for port 43h:
;            ---------------------------------
;           |  7 | 6 | 5 | 4 | 3 | 2 | 1 | 0  |
;            ---------------------------------
;              |   |   |   |   |   |   |   |__ Count format (0=binary)
;              |   |   |   |   |___|___|_______ Mode of operation 
;              |   |   |   |                    (3 = 011B = square wave)
;              |   |   |__ |_______ How to access latch (11B = LSB then MSB)
;              |___|_______________ Timer/oscillator channel
;


GLOBAL beepon
GLOBAL beepoff


section .text

beepon:
    push rbp
    mov rbp, rsp

    mov al, 0xB6        ; 1011 0110 Command register for port 43h
    out 0x43, al
    mov rax, 0
    mov ax, 3A98h       ; frequency
    out 0x42, al
    mov al, ah
    out 0x42, al
    in al, 0x61
    mov al, 0x03
    out 61h, al

    mov rsp, rbp
    pop rbp
    ret

beepoff:

    in al, 0x61
    mov al, 0x00
    out 0x61, al
    ret