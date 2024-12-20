; ports.asm

[BITS 32]

global outb
global outw

global inb
global inw

global __lidt
global __lgdt

outb:
    mov dx, [esp + 4]
    mov al, byte [esp + 8]
    out dx, al
    ret


outw:
    mov dx, [esp + 4]
    mov ax, word [esp + 8]
    out dx, ax
    ret

inb:
	mov dx, [esp + 4]
	in al, dx
    ret

inw:
	mov dx, [esp + 4]
	in ax, dx
    ret

__lidt:
    lidt [esp + 4]
    ret

__lgdt:
    lgdt [esp + 4]
    ret
