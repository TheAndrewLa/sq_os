[BITS 16]

section .text

; Some sort of kernel prologue (setting up SS & DS registers)

cli

xor ax, ax
mov ss, ax
mov sp, 0x7C00

mov ax, 0x7C0
mov ds, ax

sti

; Here's a code that performs printing 'hello world'

mov ah, 0xE
xor di, di

print_loop:
        mov al, byte [greeting + di]

        test al, al
        jz main_loop

        int 0x10 

        inc di
        jmp print_loop

; Infinite loop (protection from UB)

main_loop:
        jmp main_loop

; Just info about size of program

%assign CODE_SIZE $ - $$
%warning Size of code is CODE_SIZE

greeting: db "Hello World!", 0x0

times 510 - ($ - $$) db 0x0
db 0x55, 0xAA

; "Comp-time" assertion that our file is not more than 512 bytes

%if $ - $$ != 512
        %fatal Size of boot sector is  more than 512
%endif

