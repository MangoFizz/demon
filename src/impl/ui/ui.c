#include <stdint.h>

void check_for_updates(void *param) {
    // stubbed out - this function is broken anyway and halo trial will never get an update
    *(uint8_t *)(0x708C74) = 1;
    *(uint8_t *)(0x708C75) = 1;
    *(uint32_t *)(param + 0x18) = 0;
    *(uint16_t *)(param + 0x1C) = 1;
    *(uint16_t *)(param + 0x20) = 1;
    *(float *)(param + 0x24) = 0.0;
    *(uint8_t *)(0x68A911) = 0;
}
