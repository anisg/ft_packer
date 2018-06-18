





default rel

global main


SECTION .text   

main:
        push    rbp
        mov     rbp, rsp
        mov     dword [rbp-4H], 2
        mov     eax, dword [rbp-4H]
        pop     rbp
        ret



SECTION .data   


SECTION .bss    


