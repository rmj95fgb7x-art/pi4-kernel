#include "pi4.h"          // we will create this header next
#include "uart.h"         // we will create this after pi4.h

void kernel_main(void)
{
    uart_init();                       // early console
    uart_puts("\r\nπ/4 micro-kernel starting...\r\n");

    pi4_init();                        // init lattice allocator + epoch
    test_move();                       // quick sanity check

    uart_puts("All tests OK.  Halted.\r\n");
    while (1) { asm volatile("wfi"); } // power-down loop
}

/* build-time assertions */
_Static_assert(CELL_SIZE == 512, "CELL_SIZE must match lattice");
_Static_assert(RING_MAX < 32, "fits in 32-bit id mask");
