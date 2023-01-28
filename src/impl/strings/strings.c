#include "strings.h"
#include "../tag.h"
#include "ringhopper/unicode_string_list.h"

const char16_t *missing_string_utf16 = L"<missing string>";
const char16_t *invalid_string_utf16 = L"<invalid string>";

const char16_t *get_unicode_string(TableID unicode_string_list_id, uint16_t index) {
    if(ID_IS_NULL(unicode_string_list_id)) {
        return missing_string_utf16;
    }

    UnicodeStringList *string_list = get_tag_data(unicode_string_list_id);
    if(index >= string_list->strings.count) {
        return missing_string_utf16;
    }

    UnicodeStringListString *string = &string_list->strings.elements[index];
    size_t string_length = string->string.size;
    size_t string_chars = string_length / sizeof(char16_t);
    const char16_t *string_data = (const char16_t *)(string->string.pointer);

    // If the string has an invalid length or it's not null terminated, the string cannot be trusted and is invalid, so we return this.
    //
    // The original game will simply "fix" it by editing the actual tag data to null terminate it if it isn't null terminated, even
    // though no tools that make unicode_string_list tags would ever produce tags that need this to work.
    //
    // Allowing this means maps can essentially write memory to any arbitrary addres in RAM. This is not desirable and can likely be
    // exploited, so we handle this properly instead.
    if(string_data == NULL || string_length == 0 || string_length != string_chars * sizeof(char16_t) || string_data[string_chars - 1] != 0) {
        return invalid_string_utf16;
    }

    return string_data;
}

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
