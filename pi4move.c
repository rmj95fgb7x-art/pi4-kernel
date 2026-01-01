#include "pi4.h"
#include <string.h>

static uint8_t current_epoch(void)
{
    extern uint8_t global_epoch;
    return global_epoch;
}

int pi4_move(id_t dst, id_t src, uint32_t off, uint32_t len)
{
    if (!dst || !src || len > CELL_SIZE) return -1;

    /* temporal safety check: epoch delta must be < 128 */
    uint8_t now = current_epoch();
    if ((now - id_epoch(src)) & 0x80) return -2; // expired
    if ((now - id_epoch(dst)) & 0x80) return -2;

    /* same-ring fast path (cache-local) */
    if (id_ring(dst) == id_ring(src)) {
        uint8_t *s = (uint8_t *)(uintptr_t)(src & ((1ULL<<48)-1));
        uint8_t *d = (uint8_t *)(uintptr_t)(dst & ((1ULL<<48)-1));
        memcpy(d, s + off, len);
        return 0;
    }

    /* cross-ring: hardware primitive (stubbed as memcpy for now) */
    /* TODO: custom insn 'ringmv' when RTL ready */
    uint8_t *s = (uint8_t *)(uintptr_t)(src & ((1ULL<<48)-1));
    uint8_t *d = (uint8_t *)(uintptr_t)(dst & ((1ULL<<48)-1));
    memcpy(d, s + off, len);
    return 0;
}
