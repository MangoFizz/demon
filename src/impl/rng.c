#include "rng.h"

uint32_t advance_rng(void) {
    uint32_t *halo_rng = (uint32_t *)(0x00709618);
    *halo_rng = *halo_rng * 0x19660D + 0x3C6EF35F;
    return *halo_rng;
}
