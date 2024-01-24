#include <d3d9.h>
#include "d3d9.h"
#include "../exception/exception.h"
#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

typedef IDirect3D9 *(__stdcall *Direct3DCreate9Ptr)(UINT SDKVersion);

static HMODULE *d3d9_handle = (HMODULE *)(0x735A84);
static Direct3DCreate9Ptr *Direct3DCreate9_addr = (Direct3DCreate9Ptr *)(0x735A94);
static IDirect3D9 **d3d9_interface = (IDirect3D9 **)(0x7116CC);
static IDirect3D9 **d3d9_interface2 = (IDirect3D9 **)(0x70C9A4);

static void set_buffering_disabled(bool disabled);
static void init_z_bias_values(void);

void load_d3d9(void) {
    *d3d9_handle = LoadLibraryA("d3d9.dll");
    *Direct3DCreate9_addr = (Direct3DCreate9Ptr)(GetProcAddress(*d3d9_handle,"Direct3DCreate9"));
    if(*d3d9_handle == NULL || *Direct3DCreate9_addr == NULL) {
        CRASHF_DEBUG("Can't load d3d9.dll");
    }
    *d3d9_interface = (*Direct3DCreate9_addr)(0x1F);
    if(*d3d9_interface == NULL) {
        CRASHF_DEBUG("Direct3DCreate9 failed");
    }

    set_buffering_disabled(true);
    init_z_bias_values();
}

static void init_z_bias_values(void) {
    *(float *)(0x7123A8) = -0.000055; // DecalZBiasValue
    *(float *)(0x7123AC) = -0.000005; // TransparentDecalZBiasValue
    *(float *)(0x7123B0) = -2.0; // DecalSlopeZBiasValue
    *(float *)(0x7123B4) = -2.0; // TransparentDecalSlopeZBiasValue
}

static void set_buffering_disabled(bool disabled) {
    *(uint32_t *)(0x712384) = disabled;
}

static IDirect3DDevice9 **d3d9_device = (IDirect3DDevice9 **)(0x70C9A0);

IDirect3DDevice9 **get_d3d9_device(void) {
    return d3d9_device;
}

static uint8_t *should_minimize_when_focus_is_lost = (uint8_t *)(0x70C99C);
uint32_t on_mouse_hover_unfocused(void) {
    // called whenever mousing over the window when it is not in focus. unknown what it does
    return *should_minimize_when_focus_is_lost && get_d3d9_device();
}

void assign_d3d_vertex_functions(void) {
    *(void **)(0x7AFCA4) = (void *)(0x51DA50);
    *(void **)(0x7AFCA8) = (void *)(0x51DD80);
    *(void **)(0x7AFCA0) = (void *)(0x51E7B0);
}
