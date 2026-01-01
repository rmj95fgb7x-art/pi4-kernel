#ifndef PI4_H
#define PI4_H

#include <stdint.h>
#include <stdbool.h>

/* ---------- compile-time knobs ---------- */
#define CELL_SHIFT   9          // 512-byte cells
#define CELL_SIZE    (1u<<CELL_SHIFT)
#define RING_MAX     16         // 0..15
#define EPOCH_WRAP   256        // 8-bit epoch field
#define QUANTUM_MS   1          // 1 ms tick (QEMU)

/* ---------- opaque types ---------- */
typedef uint64_t id_t;          // [ring:8|epoch:8|offset:48]

/* ---------- core API ---------- */
void     pi4_init(void);
id_t     pi4_alloc(uint32_t ring_mask, uint32_t bytes);
int      pi4_move(id_t dst, id_t src, uint32_t off, uint32_t len);
void     pi4_wait(uint8_t epoch);

/* ---------- inlines ---------- */
static inline uint8_t id_ring(id_t id)  { return id>>56; }
static inline uint8_t id_epoch(id_t id) { return (id>>48)&0xFF; }

#endif
