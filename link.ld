ENTRY(_start)

MEMORY
{
    ram (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS
{
    .text : {
        *(.text.start)
        *(.text*)
    } > ram

    .rodata : ALIGN(16) {
        *(.rodata*)
    } > ram

    .data : ALIGN(16) {
        _data_start = .;
        *(.data*)
        _data_end = .;
    } > ram

    .bss : ALIGN(16) {
        _bss_start = .;
        *(.bss*)
        *(COMMON*)
        _bss_end = .;
    } > ram

    /* 1 MiB stack just below kernel */
    _stack_bottom = ORIGIN(ram) + 0x100000;
    _stack_top    = _stack_bottom + 0x100000;
}
