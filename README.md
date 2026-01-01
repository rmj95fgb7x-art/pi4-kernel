# π/4 micro-kernel

A one-page RISC-V kernel that uses a **π/4-rotated square lattice** for memory, **rotate-45 IPC** and a **compile-time thermal ceiling**.

## What’s inside
- `boot.S` – bare-metal entry (hart 0 only)  
- `kernel.c` – 3-syscall surface: `alloc`, `move`, `wait`  
- `pi4*.c` – diamond-ring allocator, epoch-based safety, rotate-45 mover  
- `uart.c` – NS16550A serial for QEMU virt  
- `Makefile` – one-button build & run

## Build (Debian/Ubuntu)
```bash
sudo apt install gcc-riscv64-linux-gnu qemu-system-misc
git clone https://github.com/YOU/pi4-kernel
cd pi4-kernel
make run
π/4 micro-kernel starting...
Running test_move... PASS
All tests OK.  Halted.
Key ideas
 • Memory = concentric diamonds (rings) = cache colors → no false sharing
 • Identifier = ring(8) ‖ epoch(8) ‖ offset(48) → temporal safety without MMU walks
 • IPC = subtract lattice coords → 3-cycle copy-free move on same ring
 • Energy ≤ Σ ring² → hard-wired analog comparator grants/denies alloc
Hack it
 • Edit CELL_SHIFT in pi4.h to change cell size
 • Add timer.c and call pi4_tick() every 1 ms to rotate epoch
 • Replace uart_putc with your own device driver
License
