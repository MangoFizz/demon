#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#include <windows.h>

#include "../misc_types.h"
#include "../init/init.h"

static char *console_text = (char *)(0x6AE378);
static const ColorARGB *default_prompt_colors = (ColorARGB *)(0x68E274);
static uint16_t *command_history_next_index = (uint16_t *)(0x6AEC7E);
static uint16_t *command_history_length = (uint16_t *)(0x6AEC7C);
static uint16_t *command_history_selected_index = (uint16_t *)(0x6AEC80);
static uint8_t *console_enabled = (uint8_t *)(0x6AE2C1);
static ColorARGB *console_prompt_color = (ColorARGB *)(0x6AE348);

static bool devmode = false;

void set_console_prompt_display_params(void) {
    // set prompt to these colors
    *console_prompt_color = *default_prompt_colors;

    // room for 32 characters
    char *prompt_text = (char *)(0x6AE358);
    strcpy(prompt_text, "halo( ");

    // initialize these
    *console_text = 0;
    *command_history_next_index = 0xFFFF;
    *command_history_length = 0;
    *command_history_selected_index = 0xFFFF;

    // check if we should enable the console or have devmode (basically backported from Halo Custom Edition)
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
extern uint8_t (*unknown_function_00481b70)(uint32_t a, uint32_t b);
uint32_t *unknown_006a8854 = (uint32_t *)(0x6A8854);

bool command_is_allowed(uint8_t a) {
    // Enable all commands if devmode
    if(devmode) {
        return true;
    }

    uint32_t flags = *unknown_006a8854;

    if((uint16_t)(flags) == 0) {
        return true;
    }

    if((flags & 1) && !(a & 1)) {
        return false;
    }

    if(((flags & 0x100) != 0) && !(a & 1)) {
        return false;
    }

    return unknown_function_00481b70(a, 1)
        && unknown_function_00481b70(a, 2)
        && unknown_function_00481b70(a, 3)
        && unknown_function_00481b70(a, 4)
        && unknown_function_00481b70(a, 6)
        && unknown_function_00481b70(a, 5);
}
