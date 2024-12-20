OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x8200;

    .text : {
        _build/collect_context.o (.text)
        _build/main.o (.text)
        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }
}
