CROSS    = riscv64-linux-gnu-
CC       = $(CROSS)gcc
LD       = $(CROSS)ld
OBJDUMP  = $(CROSS)objdump
QEMU     = qemu-system-riscv64

CFLAGS   = -Wall -O2 -march=rv64im -mabi=lp64 -ffreestanding -nostdlib -I.
LDFLAGS  = -T link.ld -nostdlib

SRCS = boot.S kernel.c uart.c pi4alloc.c pi4move.c pi4epoch.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.S=.o)

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

run: kernel.elf
	$(QEMU) -machine virt -bios none -kernel kernel.elf -nographic -serial mon:stdio

clean:
	rm -f *.o kernel.elf

.PHONY: run clean
