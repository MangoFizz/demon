#include "dsound.h"
#include "../exception/exception.h"
#include <windows.h>

static HMODULE *dsound_handle = (HMODULE *)(0x735A7C);
static FARPROC *DirectSoundCreate8_addr = (FARPROC *)(0x735A90);

void load_dsound(void) {
    *dsound_handle = LoadLibraryA("dsound.dll");
    *DirectSoundCreate8_addr = GetProcAddress(*dsound_handle,"DirectSoundCreate8");
    if(*dsound_handle == NULL || *DirectSoundCreate8_addr == NULL) {
        CRASHF_DEBUG("Can't load dsound.dll");
    }
}
