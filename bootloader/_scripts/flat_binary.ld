OUTPUT_FORMAT(binary)
ENTRY(loader_main)

SECTIONS {
    . = 0x20200;

    .text : {
        build/main.o (.text)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }
}
