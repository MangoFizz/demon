#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "init.h"
#include "window.h"
#include "../d3d9/d3d9.h"
#include "../dinput/dinput.h"
#include "../dsound/dsound.h"
#include "../keystone/keystone.h"
#include "../memory/memory.h"
#include "../exception/exception.h"

static char *command_line_str = NULL;
static const char ***parameters = (const char ***)(0x7116C0);
static uint32_t *parameter_count = (uint32_t *)(0x7116C4);

extern bool (*init_d3d9)(void);
extern void (*game_loop)(void);

extern void (*init_gamespy)(const char *game, const uint8_t *bytes, const char *ip, uint16_t port);

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

    *(float *)(0x7123A8) = -0.000055; // DecalZBiasValue
    *(float *)(0x7123AC) = -0.000005; // TransparentDecalZBiasValue
    *(float *)(0x7123B0) = -2.0; // DecalSlopeZBiasValue
    *(float *)(0x7123B4) = -2.0; // TransparentDecalSlopeZBiasValue

    return NULL; // do not show a message dialog
}

static size_t generate_arg_list(const char *cmd_line, bool no_op) {
    char last_char = ' ';
    size_t arg_count = 0;
    for(char *c = command_line_str; *c != 0; c++) {
        char this_char = *c;
        bool is_arg = this_char != last_char && last_char == ' ';
        if(!no_op) {
            if(this_char == ' ') {
                *c = 0; // replace whitespace with null terminator
            }
            else if(is_arg) {
                (*parameters)[arg_count] = c;
            }
        }
        if(is_arg) {
            arg_count++;
        }
        last_char = this_char;
    }
    if(!no_op) {
        *parameter_count = arg_count;
    }
}

static void check_args(const char *args) {
    size_t l = strlen(args);
    command_line_str = malloc(l + 1);
    command_line_str[l] = 0;
    memcpy(command_line_str, args, l);
    size_t arg_count = generate_arg_list(command_line_str, true);
    *parameters = malloc(arg_count * sizeof(**parameters));
    generate_arg_list(command_line_str, false);

    *(uint32_t *)(0x70C9D8) = get_exe_argument_value("-window", NULL);
    *(uint32_t *)(0x709028) = get_exe_argument_value("-connect", NULL);
    *(uint32_t *)(0x709038) = get_exe_argument_value("-nowinkey", NULL);

    *(uint32_t *)(0x709020) = 0; // -screenshot
    *(uint32_t *)(0x709024) = 0; // -nosound
    *(uint32_t *)(0x70902C) = 0; // -nonetwork
    *(uint32_t *)(0x709030) = 0; // -width640
    *(uint32_t *)(0x70C9D0) = 0; // -checkfpu
    *(uint32_t *)(0x709034) = 0; // -safemode
}

static void setup_network(void) {
    uint32_t *cport_value = (uint32_t *)(0x68FECC);

    const char *port = NULL;
    if(get_exe_argument_value("-port", &port) && (port != NULL)) {
        *(uint32_t *)(0x68FEC8) = atol(port);
        *(uint32_t *)(0x70BC18) = 1;
    }

    const char *cport = NULL;
    if(get_exe_argument_value("-cport", &cport) && (cport != NULL)) {
        *cport_value = atol(cport);
        *(uint32_t *)(0x70BC18) = 1;
    }

    const char *ip = NULL;
    if(get_exe_argument_value("-ip", &ip) && (ip != NULL)) {
        *(uint32_t *)(0x67E9A4) = inet_addr(ip);
        if(*(uint32_t *)(0x67E9A4) != 0xFFFFFFFF) {
            *(uint32_t *)(0x67E998) = ntohl(*(uint32_t *)(0x67E9A4));
        }
    }

    init_gamespy("halod", "yG3d9w", ip, *cport_value);
}

static char asdf[32768];

void game_main(HMODULE module_handle, uint32_t zero, const char *args, uint32_t one) {
    *(char **)(0x711738) = asdf;
    memset(asdf, 0xEE, sizeof(asdf));

    *(uint32_t *)(0x6E7688) = GetTickCount();
    *(HMODULE *)(0x7123E0) = GetModuleHandleA("strings.dll");
    *(uint32_t *)(0x6976A8) = 0x409;

    if(!*(HMODULE *)(0x7123E0)) {
        CRASHF_DEBUG("oh no");
    }

    query_system_specs_stub();
    read_config_stub();
    SetLastError(0);

    // Unknown what these do
    *(uint8_t *)(0x7123F8) = 0;
    *(uint32_t *)(0x7359C8) = 0x64F0BC;
    *(const char **)(0x6DA860) = args;
    *(uint32_t *)(0x7359EC) = one;
    *(uint32_t *)(0x7359E8) = 0;
    *(uint8_t *)(0x735A74) = 0;
    *(uint32_t *)(0x709018) = 0;
    *(uint8_t *)(0x735A75) = 0;
    set_game_window_handle(NULL);
    *(HCURSOR *)(0x6DA85C) = LoadCursorA(NULL, (LPCSTR)(0x7F00));
    *(uint16_t *)(0x67E9B8) = 0x30;
    *(HMODULE *)(0x7359E0) = module_handle;
    check_args(args);
    allocate_heaps();
    load_d3d9();
    load_dsound();
    load_dinput();

    *(HMODULE *)(0x735A80) = LoadLibraryA("shfolder.dll");
    *(FARPROC *)(0x735A8C) = GetProcAddress(*(HMODULE *)(0x735A80),"SHGetFolderPathA");
    if(*(HMODULE *)(0x735A80) == NULL || *(FARPROC *)(0x735A8C) == NULL) {
        CRASHF_DEBUG("Can't load shfolder.dll");
    }

    load_keystone();

    if(!init_d3d9()) {
        CRASHF_DEBUG("Can't init d3d9.dll");
    }

    setup_network();
    game_loop();
    unload_keystone();
}

void game_main_entry(void) {
    game_main(GetModuleHandleA(NULL), 0, GetCommandLineA(), 1);
}
