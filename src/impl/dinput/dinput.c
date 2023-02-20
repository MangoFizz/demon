#include "dinput.h"
#include "../exception/exception.h"
#include <windows.h>

static HMODULE *dinput8_handle = (HMODULE *)(0x735A78);
static FARPROC *DirectInput8Create_addr = (FARPROC *)(0x735A88);

void load_dinput(void) {
    *dinput8_handle = LoadLibraryA("dinput8.dll");
    *DirectInput8Create_addr = GetProcAddress(*dinput8_handle,"DirectInput8Create");
    if(*dinput8_handle == NULL || *DirectInput8Create_addr == NULL) {
        CRASHF_DEBUG("Can't load dinput.dll");
    }
}
