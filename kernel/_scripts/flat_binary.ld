OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x8200;

    .text : {
        _build/context.o (.text)
        _build/main.o (.text)
        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    .data : {
        *(.data)
        *(.data.*)
    }
}
