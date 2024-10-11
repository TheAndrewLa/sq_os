[BITS 16]

section .text

; Constant pool (he-he)

SECTORS_COUNT equ 18
HEADS_COUNT equ 2
CYLINDERS_COUNT equ 20

DATA_SEGMENT equ gdt_data - gdt_null
CODE_SEGMENT equ gdt_code64 - gdt_null

; Kernel prologue

        cli

        xor ax, ax
        mov ss, ax
        mov sp, 0x7C00

        mov ax, 0x7C0
        mov ds, ax

; Reading by 2 sectors (1kb)
; This variant won't give us intersection of 64 KiB address
; And also 18 % 2 == 0. So we can perform reading entire disk with fixed cylinder/head in 9 iterations

        mov ax, 0x2000
        mov es, ax

; Mode is "read sectors" (ah)
; Reading by 2 sectors (1kb) (al)

        xor bx, bx
        xor dh, dh

        mov cx, 0x1

sectors:
        mov ax, 0x0202                ; Set working mode

        int 0x13                      ; Performing read
        jc error_handling

        xor di, di                    ; Update attempts count

        mov bp, es
        add bp, 0x40                  ; Moving buffer address (segment selector)
        mov es, bp

        add cl, 0x2                   ; Increment sector index & check
        cmp cl, SECTORS_COUNT + 1

        jne sectors

heads:
        mov cl, 0x1           ; Set sector to initial sector

        inc dh                ; Increase index of head & check
        cmp dh, HEADS_COUNT

        jne sectors

cylinders:
        xor dh, dh                     ; Set head to initial head

        inc ch                         ; Inrease index of cylinder & check
        cmp ch, CYLINDERS_COUNT        ; 20

        jne sectors

        jmp long_mode                  ; SUCCESS! (all sectors were read, going to long mode)

error_handling:
        inc di               ; Incresing attempts count and try again
        cmp di, 0x5          ;
        jne sectors

fatal:
        mov ax, 0x0e00 + '?'
        int 0x10
        int 0x10
        int 0x10

        ; TODO: more informative message about an error (pass an error on BX register)

        jmp $

long_mode:
        lgdt [gdt_descriptor]

        ; Set up PDP, PD, PT (or make it preloaded)

        ; Enable PAE
        ; Set control registers
        
        ; Set segment registers

        jmp $

[BITS 64]

long_mode_enter:
        mov rax, DATA_SEGMENT
        mov ss, rax
        mov ds, rax
        mov es, rax
        mov fs, rax
        mov gs, rax

        ; Far jump here

; Descriptors of segments

gdt_null: dq 0x0

gdt_code32: dq 0x004F9A000000FFFF
gdt_code64: dq 0x0020980000000000

gdt_data: dq 0x004F92000000FFFF

; GDT Descriptor should be 80bit

gdt_descriptor:
        dw $ - gdt_null - 1
        dq gdt_null + 0x20000
        
times 510 - ($ - $$) db 0x0
dw 0xAA55
