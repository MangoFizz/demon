#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#include <windows.h>

#include "../types/types.h"
#include "../init/init.h"

static char *console_text = (char *)(0x6AE378);
static uint16_t *command_history_next_index = (uint16_t *)(0x6AEC7E);
static uint16_t *command_history_length = (uint16_t *)(0x6AEC7C);
static uint16_t *command_history_selected_index = (uint16_t *)(0x6AEC80);
static uint8_t *console_enabled = (uint8_t *)(0x6AE2C1);
static ColorARGB *console_prompt_color = (ColorARGB *)(0x6AE348);

static bool devmode = false;

void set_console_prompt_display_params(void) {
    // set prompt to these colors
    console_prompt_color->a = 1.0;
    console_prompt_color->r = 1.0;
    console_prompt_color->g = 0.3;
    console_prompt_color->b = 1.0;

    // room for 32 characters
    char *prompt_text = (char *)(0x6AE358);
    strcpy(prompt_text, "halo( ");

    // initialize these
    *console_text = 0;
    *command_history_next_index = 0xFFFF;
    *command_history_length = 0;
    *command_history_selected_index = 0xFFFF;

    // -console and -devmode - backported from Halo Custom Edition
    *console_enabled = get_exe_argument_value("-console", NULL);
    devmode = get_exe_argument_value("-devmode", NULL);
}

extern void (*console_printf_in)(const ColorARGB *color, const char *fmt, ...);

void console_printf(const ColorARGB *color, const char *fmt, ...) {
    char passed_text[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(passed_text, sizeof(passed_text), fmt, args);
    va_end(args);
    console_printf_in(color, "%s", passed_text);
}

// TODO: The exact meanings of these is not known. This should be figured out later.
static uint16_t *command_restrictions = (uint16_t *)(0x6A8854);

bool command_is_allowed(uint8_t a) {
    // Enable all commands if devmode
    if(devmode) {
        return true;
    }

    uint16_t restrictions = *command_restrictions;

    if(restrictions == 0) {
        return true;
    }

    if((restrictions & 1) && !(a & 1)) {
        return false;
    }

    if(((restrictions & 0x100) != 0) && !(a & 1)) {
        return false;
    }

    for(int i = 0; i <= 6; i++) {
        uint32_t flag = 1 << i;
        uint32_t antiflag = flag << 8;

        bool flag_set = a & flag;

        if((restrictions & flag) && !flag_set) {
            return false;
        }

        if((restrictions & antiflag) && flag_set) {
            return false;
        }
    }

    return true;
}

void load_names_from_reflexive(const GenericReflexive *reflexive, uint32_t name_offset, uint32_t element_size) {
    uint16_t *capacity = (uint16_t *)(0x6A8848);
    uint16_t *count = (uint16_t *)(0x6A8858);
    const char ***names = (const char ***)(0x6A885C);

    const void *current_name = reflexive->pointer + name_offset;
    const char *filter = *(const char **)(0x6A884C);
    size_t filter_len = strlen(filter);

    size_t index = 0;
    while(index < reflexive->count && *count < *capacity) {
        // Check if the item starts with this
        if(strncmp(filter, current_name, filter_len) == 0) {
            (*names)[*count] = current_name;
            (*count)++;
        }
        index++;
        current_name += element_size;
    }
}
