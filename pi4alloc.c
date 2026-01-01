#include "pi4.h"

/* one cyclic queue per ring */
typedef struct {
    uint32_t head, tail;
    uint32_t cells[512];          // 512 slots per ring
} ringq_t;

static ringq_t rings[RING_MAX];
static uint8_t global_epoch;

/* ---------- internal ---------- */
static bool thermal_ok(uint8_t ring)
{
    /* simple square model: ring² ≤ budget */
    uint32_t budget = 1000;       /* mW, will be Kconfig later */
    return ring * ring <= budget;
}

/* ---------- API ---------- */
void pi4_init(void)
{
    for (int r = 0; r < RING_MAX; ++r) {
        rings[r].head = rings[r].tail = 0;
        /* pre-fill queue with cell indices */
        for (int c = 0; c < 512; ++c)
            rings[r].cells[c] = c * CELL_SIZE + (r << CELL_SHIFT);
    }
    global_epoch = 0;
}

id_t pi4_alloc(uint32_t ring_mask, uint32_t bytes)
{
    if (!bytes || bytes > CELL_SIZE) return 0;

    for (uint8_t r = 0; r < RING_MAX; ++r) {
        if (!(ring_mask & (1u << r))) continue;
        if (!thermal_ok(r)) continue;

        ringq_t *q = &rings[r];
        uint32_t head = q->head;
        uint32_t next = (head + 1) & 511;
        if (next == q->tail) continue;          // ring full

        uint32_t off = q->cells[head];
        q->head = next;

        /* pack id: ring(8) | epoch(8) | offset(48) */
        return ((uint64_t)r << 56) |
               ((uint64_t)global_epoch << 48) |
               off;
    }
    return 0;   // fail
}
