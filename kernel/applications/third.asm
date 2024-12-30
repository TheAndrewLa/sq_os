[BITS 32]

org 0x700000

section .text

print_loop:
    mov eax, msg
    int 0x80
    jmp print_loop

msg:
    db "Hello from 3rd task ", 0x0
