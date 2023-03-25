#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

#include <windows.h>

#include "../types/types.h"
#include "../init/init.h"
#include "../script/global.h"
#include "../tag/tag.h"

#include "ringhopper/scenario.h"

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

static const char ***load_name_names = (const char ***)(0x6A885C);
static uint16_t *load_names_capacity = (uint16_t *)(0x6A8848);
static uint16_t *load_names_count = (uint16_t *)(0x6A8858);
static const char **load_names_filter = (const char **)(0x6A884C);

void load_names_from_reflexive(const GenericReflexive *reflexive, uint32_t name_offset, uint32_t element_size) {
    const void *current_name = reflexive->pointer + name_offset;
    const char *filter = *load_names_filter;
    size_t filter_len = strlen(filter);

    size_t index = 0;
    while(index < reflexive->count && *load_names_count < *load_names_capacity) {
        // Check if the item starts with this
        if(strncmp(filter, current_name, filter_len) == 0) {
            (*load_name_names)[*load_names_count] = current_name;
            (*load_names_count)++;
        }
        index++;
        current_name += element_size;
    }
}

void list_globals(void) {
    const char *filter = *load_names_filter;
    size_t filter_len = strlen(filter);

    size_t internal_global_count = get_internal_global_count();
    EngineGlobal **internal_globals = get_internal_globals();

    size_t index = 0;
    while(index < internal_global_count && *load_names_count < *load_names_capacity) {
        // Skip compiled-out globals
        if(internal_globals[index]->data == NULL) {
            goto skip_global;
        }

        // DevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmodeDevmode
        if(!command_is_allowed(internal_globals[index]->permission)) {
            goto skip_global;
        }

        // Check if the item starts with this
        const char *name = internal_globals[index]->name;
        if(strncmp(filter, name, filter_len) == 0) {
            (*load_name_names)[*load_names_count] = name;
            (*load_names_count)++;
        }

        skip_global:
        index++;
    }

    Scenario *scenario = get_scenario_tag_data();
    if(scenario != NULL && devmode) {
        load_names_from_reflexive((const GenericReflexive *)(&scenario->globals), offsetof(ScenarioGlobal, name), sizeof(ScenarioGlobal));
    }
}
