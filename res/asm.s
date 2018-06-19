SYS_WRITE equ 1
STD_OUTPUT equ 1

section .text
global _start

_start:
jmp short MainCode
    msg: db `....WOODY....\n`
    msglen equ $-msg

Decrypt:
	.in:
	push rbp
	mov rbp, rsp

	mov rdi, 0x33333333
	mov rsi, 0x22222222
	mov rdx, 0

	.loop:
		.cond:
		cmp rdx, rsi
		jge .out
		.stmt:
		sub byte [rdi + rdx], 13
		inc rdx
		jmp .cond

	.out:
	pop rbp
	ret

MainCode:
    push rax
    push rdi
    push rsi
    push rdx

    mov rax, SYS_WRITE
    mov rdi, STD_OUTPUT
    lea rsi, [rel msg]
    mov rdx, msglen
    syscall

	call Decrypt

    pop rdx
    pop rsi
    pop rdi
    pop rax

    push 0x11111111
    ret
