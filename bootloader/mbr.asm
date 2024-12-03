[BITS 16]
[ORG 0x7A00]

    cli
    xor ax, ax         ;
    mov ss, ax         ;
    mov ds, ax         ; Setting up segment registers
    mov es, ax         ;
    mov sp, 0x7A00     ; Setting up stack

    mov cx, 0x200      ; MBR size is 512 byte

    mov si, 0x7C00     ; MBR will be copied from
    mov di, 0x7A00     ; MBR will be copied to

    cld                ; Copy MBR to <0x0 : 0x7A00>
    rep movsb          ;

    jmp 0x0 : start    ; Jump to <0x0 : 0x7A00 + start>

start:
    sti


vbr_jump:
    jmp 0x7C00

uid db "SqOS"           ; Unique disk ID (probably unused)
res dw 0x0              ; Reserved word

pt1 times 0x10 db 0x0   ;
pt2 times 0x10 db 0x0   ; Partition table entries
pt3 times 0x10 db 0x0   ; 1, 2, 3, 4
pt4 times 0x10 db 0x0   ;

dw 0xAA55               ; Magic number (means "valid bootsector")
