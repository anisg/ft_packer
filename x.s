SYS_WRITE equ 1
SYS_EXIT equ 60
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

	mov rax, SYS_EXIT
	mov rdi, 0
	syscall

	mov rax, 0x11111111
	jmp rax

