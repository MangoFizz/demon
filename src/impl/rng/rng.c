#include "rng.h"

uint32_t advance_rng(void) {
    uint32_t *halo_rng = (uint32_t *)(0x00709618);
    *halo_rng = *halo_rng * 0x19660D + 0x3C6EF35F;
    return *halo_rng;
}

int16_t random_i16(int16_t max) {
    return (int16_t)(((advance_rng() >> 16) * max) >> 16);
}
