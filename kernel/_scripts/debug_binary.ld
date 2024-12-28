OUTPUT_FORMAT("elf32-i386")

SECTIONS {
    . = 0x8200;

    .text : {
        _build/context.o (.text)
        _build/main.o (.text)
        *(.text)
        *(.text.*)
    }

    .data : {
        *(.data)
    }

    .note : {
        *(.note)
    }

    .stab : {
        *(.stab)
    }

    .stabstr : {
        *(.stabstr)
    }

    .rodata : {
        *(.rodata)
    }
    
    .comment : {
        *(.comment)
    }
}
