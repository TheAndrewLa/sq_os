; registers.asm

[BITS 32]

global read_cr0
global read_cr3
global read_cr4

global read_cr2

global write_cr0
global write_cr3
global write_cr4

read_cr0:
    mov eax, cr0
    ret

read_cr3:
    mov eax, cr3
    ret

read_cr2:
    mov eax, cr2
    ret

read_cr4:
    mov eax, cr4
    ret

write_cr0:
    mov eax, [esp + 4]
    mov cr0, eax
    ret

write_cr3:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

write_cr4:
    mov eax, [esp + 4]
    mov cr4, eax
    ret
