#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct ColorARGB {
    float a;
    float r;
    float g;
    float b;
} ColorARGB;

static char *console_text = (char *)(0x6AE378);
static const ColorARGB *default_prompt_colors = (ColorARGB *)(0x68E274);
static uint16_t *command_history_next_index = (uint16_t *)(0x6AEC7E);
static uint16_t *command_history_length = (uint16_t *)(0x6AEC7C);
static uint16_t *command_history_selected_index = (uint16_t *)(0x6AEC80);
static uint8_t *console_enabled = (uint8_t *)(0x6AE2C1);
static ColorARGB *console_prompt_color = (ColorARGB *)(0x6AE348);

void set_console_prompt_display_params() {
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

    // if we have enhancements on, turn on the console
    #ifdef DEMON_ENABLE_ENHANCEMENTS
    *console_enabled = 1;
    #else
    // disable console by default
    *console_enabled = 0;
    #endif
}

bool command_allowed() {
    // stubbed: the actual function does a series of checks to see if the command is 'whitelisted' so it can be shown in a non-devmode environment
    return true;
}
