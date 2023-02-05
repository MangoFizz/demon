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
