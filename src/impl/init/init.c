#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static const char ***parameters = (const char ***)(0x7116C0);
static const uint32_t *parameter_count = (uint32_t *)(0x7116C4);

bool get_exe_argument_value(const char *argument, const char **value) {
    // If value is NULL, change it to point to this temporary value instead (removes further checks)
    const char *value_temp;
    if(value == NULL) {
        value = &value_temp;
    }
    *value = NULL;

    // Search
    for(size_t i = 0; i < *parameter_count; i++) {
        const char **parameter_tuple = *parameters + i;
        const char *parameter = parameter_tuple[0];

        if(strcmp(parameter, argument) != 0) {
            continue;
        }

        // If there is a next parameter and it does not start with a '-', return it as well.
        const char **parameter_next = parameter_tuple + 1;
        if(i + 1 < *parameter_count && (*parameter_next)[0] != '-') {
            *value = (*parameter_next);
        }
        return true;
    }
    return false;
}

// Stubbed because it's slow and buggy (Halo does not like specs that overflow 32-bit int like 4+ GB VRAM)
void query_system_specs_stub(void) {
    // Current specs
    *(uint32_t *)(0x6E7090) = 1024 * 1024 * 1024; // VRAM
    *(uint32_t *)(0x7123D8) = 1024 * 1024 * 1024; // VRAM (again)
    *(uint32_t *)(0x7123D0) = 1024; // RAM (MB)
    *(uint32_t *)(0x7123D4) = 1000; // CPU speed (MHz)
}

// Stubbed because we do not want to support config.txt - you should not need it on a PC made after 2005.
const char *read_config_stub(void) {
    // Minimum requirements
    *(uint32_t *)(0x6E7268) = 1; // VRAM
    *(uint32_t *)(0x6E726C) = 16; // RAM
    *(uint32_t *)(0x6E7274) = 1; // Disk space
    *(uint32_t *)(0x6E7278) = 1; // D3D version
    *(uint32_t *)(0x6E7280) = 1; // CPU speed
    *(uint32_t *)(0x712384) = 1; // disable buffering - the only setting worth turning on
    return NULL; // do not show a message dialog
}
