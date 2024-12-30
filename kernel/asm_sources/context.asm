[BITS 32]

[EXTERN get_context]
[EXTERN restore_context]

KERNEL_CODE_SEG equ 0x8
KERNEL_DATA_SEG equ 0x10

USER_CODE_SEG equ 0x18
USER_DATA_SEG equ 0x20

section .text

; Saving context


collect_context:

    cli

    push ds
    push es
    push fs
    push gs
    pusha

    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    push esp

    call get_context
    ret

restore_context:
    add esp, 0x4
    mov esp, [esp]

    popa
    pop gs
    pop fs
    pop es
    pop ds

    add esp, 0x8
    iret

times 0x100 - ($ - $$) db 0x0
