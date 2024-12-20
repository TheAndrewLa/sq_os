[BITS 32]

[EXTERN handler]

section .text

; Saving context

collect_context:

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
    call handler

restore_context:

    add esp, 0x4

    popa
    pop gs
    pop fs
    pop es
    pop ds

    add esp, 0x8

    sti
    iret

times 0x100 - ($ - $$) db 0x0
