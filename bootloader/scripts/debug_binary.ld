OUTPUT_FORMAT("elf32-i386")
ENTRY(loader_main)

SECTIONS {
    . = 0x20200;

    .text : {
        build/main.o (.text)
        *(.text)
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
