ENTRY(kernel_main)
OUTPUT_FORMAT(binary)

SECTIONS { 
    . = 0x20200;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
    
    .rodata : {
        *(.rodata)
    }
}
