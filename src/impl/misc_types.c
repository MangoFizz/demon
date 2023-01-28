#include "misc_types.h"

void convert_8bit_string_to_16bit(uint32_t output_capacity_bytes, char16_t *output, const char *input) {
    size_t chars_left = output_capacity_bytes / sizeof(*output) - 1;
    char16_t *output_start = output;

    while(*input != 0 && chars_left > 0) {
        *(output++) = *(input++);
        chars_left--;
    }

    // Null terminate
    *output = 0;
}
