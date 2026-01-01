.section .text.start
.global _start

# _start is loaded at 0x8000_0000 by QEMU virt machine
_start:
    # pick hart 0 only
    csrr    t0, mhartid
    bnez    t0, halt

    # bare-metal stack (1 MiB below kernel load address)
    la      sp, _stack_top

    # clear BSS
    la      t0, _bss_start
    la      t1, _bss_end
bss_zero:
    sd      zero, (t0)
    addi    t0, t0, 8
    bltu    t0, t1, bss_zero

    # jump to C kernel
    call    kernel_main

halt:
    wfi
    j       halt
