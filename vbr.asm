[BITS 16]

section .text

; Constant pool (he-he)

SECTORS_COUNT equ 18
HEADS_COUNT equ 2
CYLINDERS_COUNT equ 20

DATA_SEGMENT equ gdt_data - gdt_null
CODE_SEGMENT equ gdt_code - gdt_null

; Kernel prologue

        cli

        xor ax, ax
        mov ss, ax
        mov sp, 0x7c00

        mov ax, 0x7c0
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
        xor dh, dh                  ; Set head to initial head

        inc ch                      ; Inrease index of cylinder & check
        cmp ch, CYLINDERS_COUNT     ; 20

        jne sectors

        jmp protected_mode          ; SUCCESS! (all sectors were read)

error_handling:
        inc di               ; Incresing attempts count and try again
        cmp di, 0x5          ;
        jne sectors

        mov ax, 0x0e00 + '?'
        int 0x10
        int 0x10
        int 0x10

        jmp $

protected_mode:

        lgdt [gdt_descriptor]

        mov eax, cr0            ;
        or al, 0x1              ; Setting up control register
        mov cr0, eax            ;

        jmp CODE_SEGMENT : protected_mode_jump + 0x7c00

[BITS 32]

protected_mode_jump:
        mov eax, DATA_SEGMENT     ;
        mov ss, eax               ;
        mov ds, eax               ; Setting up stack segment, data segment, extra segments
        mov es, eax               ;
        mov fs, eax               ;
        mov gs, eax               ;

        mov esp, 0x20000          ; Setting up stack pointer

        jmp CODE_SEGMENT : 0x20200

; Descriptors of segments

gdt_null: dq 0x0
gdt_code: dq 0x004F9A000000FFFF
gdt_data: dq 0x004F92000000FFFF

; GDT Descriptor should be 80bit

gdt_descriptor:
        dw $ - gdt_null - 1
        dd gdt_null + 0x20000

times 510 - ($ - $$) db 0x0
dw 0xAA55

; (c) by andrew.la
