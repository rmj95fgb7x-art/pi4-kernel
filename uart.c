#include "pi4.h"

/* QEMU virt machine UART 0 */
#define UART_BASE 0x10000000

static inline void mmio_write(uint64_t addr, uint8_t val)
{
    *(volatile uint8_t *)addr = val;
}

static inline uint8_t mmio_read(uint64_t addr)
{
    return *(volatile uint8_t *)addr;
}

void uart_init(void)
{
    /* 8-N-1, no IRQ, 115200 baud */
    mmio_write(UART_BASE + 3, 0x80);          // DLAB on
    mmio_write(UART_BASE + 0, 12);            // 38400*12 = 115200
    mmio_write(UART_BASE + 1, 0);
    mmio_write(UART_BASE + 3, 0x03);          // 8-bit, DLAB off
}

void uart_putc(char c)
{
    while (!(mmio_read(UART_BASE + 5) & 0x20)) { asm volatile("nop"); }
    mmio_write(UART_BASE, c);
}

void uart_puts(const char *s)
{
    while (*s) uart_putc(*s++);
}
