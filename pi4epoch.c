#include "pi4.h"
#include "uart.h"

uint8_t global_epoch = 0;

/* called by timer interrupt (not implemented yet) */
void pi4_tick(void)
{
    ++global_epoch;
}

void pi4_wait(uint8_t target)
{
    while ((int8_t)(target - global_epoch) > 0)
        asm volatile("wfi");
}

/* quick sanity check */
void test_move(void)
{
    uart_puts("Running test_move... ");

    id_t a = pi4_alloc(1<<2, 64);  /* ring 2 */
    id_t b = pi4_alloc(1<<2, 64);  /* same ring */
    if (!a || !b) { uart_puts("alloc fail\r\n"); return; }

    /* write pattern */
    char *p = (char *)(uintptr_t)(a & ((1ULL<<48)-1));
    for (int i = 0; i < 16; ++i) p[i] = 'A' + i;

    /* move */
    if (pi4_move(b, a, 0, 16) != 0) { uart_puts("move fail\r\n"); return; }

    /* verify */
    char *q = (char *)(uintptr_t)(b & ((1ULL<<48)-1));
    for (int i = 0; i < 16; ++i)
        if (q[i] != 'A' + i) { uart_puts("verify fail\r\n"); return; }

    uart_puts("PASS\r\n");
}
