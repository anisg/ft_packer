SYS_WRITE equ 1
STD_OUTPUT equ 1

section .text
global _start

_start:
jmp short MainCode
    msg: db `....WOODY....\n`
    msglen equ $-msg
    
MainCode:
    mov rax, SYS_WRITE
    mov rdi, STD_OUTPUT
    lea rsi, [rel msg]
    mov rdx, msglen
    syscall

    jmp [0x11111111]
