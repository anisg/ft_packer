SYS_WRITE equ 1
STD_OUTPUT equ 1

section .text
global _start

_start:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push rsp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	pushf
	jmp MainCode

    msg: db `....WOODY....\n`
    msglen equ $-msg
    key: db `0000000000000000`

Decrypt_block:
	;v -> rdi, k -> rsi
	.in:
	push rbp
	mov rbp, rsp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13

	;init data
	mov r8, 0xC6EF3720 ;sum
	mov r9, 0x9e3779b9 ;delta

	mov rdx, 0
	.loop:
		.cond:
			cmp rdx, 32
			jge .out
		.stmt:
			;v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
			;v0
			mov r10d,[rdi]
			;(v0<<4) + k2    #p1
			mov r11, r10
			shl r11, 4
			add r11d, [rsi + 8]
			;(v0 + sum)      #p2
			mov r12, r10
			add r12, r8
			;(v0>>5) + k3    #p3
			mov r13, r10
			shr r13, 5
			add r13d, [rsi + 12]
			;p1 ^ p2 ^ p3
			xor r11, r12
			xor r11, r13

			sub [rdi + 4], r11d

			;v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
			;v1
			mov r10d, [rdi+4]
			;(v1<<4) + k0    #p1
			mov r11, r10
			shl r11, 4
			add r11d, [rsi]
			;(v1 + sum)      #p2
			mov r12, r10
			add r12, r8
			;(v1>>5) + k1    #p3
			mov r13, r10
			shr r13, 5
			add r13d, [rsi + 4]
			;p1 ^ p2 ^ p3
			xor r11, r12
			xor r11, r13

			sub [rdi], r11d

			;sum -= delta
			sub r8, r9

		.increment:
			inc rdx
			jmp .cond
	.out:
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	ret

Decrypt:
	.in:
	push rbp
	mov rbp, rsp
	push r8

	;s -> rdi, n -> rsi, key -> r8
	mov rdi, 0x22222222
	mov rsi, 0x33333333
	lea r8, [rel key]

	mov rdx, 0
	.loop:
		.cond:
			cmp rdx,rsi
			jge .out
		.stmt:
			add rdx, 7
			cmp rdx, rsi
			jg .out
			sub rdx, 7

			push rdi
			push rsi
			push rdx

			add rdi, rdx
			mov rsi, r8
			call Decrypt_block

			pop rdx
			pop rsi
			pop rdi

		.increment:
			add rdx,8
			jmp .cond
	.out:
	pop r8
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

	popf
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsp
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
    ;jump
    push 0x11111111
    ret
