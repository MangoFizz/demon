#ifndef DEMON__IMPL_STRINGS_STRINGS_H
#define DEMON__IMPL_STRINGS_STRINGS_H

#include <uchar.h>

#include "../memory/table.h"

/**
 * Load a unicode string
 *
 * @param unicode_string_list_id id to read
 * @param index                  index to read
 */
const char16_t *get_unicode_string(TableID unicode_string_list_id, uint16_t index);

/**
 * Convert an 8-bit C string to a 16-bit string.
 *
 * @param output_capacity_bytes length in bytes of the output buffer
 * @param output                pointer to output
 * @param input                 pointer to input
 */
void convert_8bit_string_to_16bit(uint32_t output_capacity_bytes, char16_t *output, const char *input);

#endif
