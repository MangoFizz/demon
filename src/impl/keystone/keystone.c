#include <windows.h>
#include <stdint.h>
#include <uchar.h>
#include <stdio.h>
#include "keystone.h"

struct KeystoneInstance;

static HMODULE *keystone_handle = (HMODULE *)(0x7116D0);
static struct KeystoneInstance *keystone_instance = (struct KeystoneInstance *)(0x7116D8);
static char16_t **current_directory = (char16_t **)(0x7116DC);
static FARPROC *keystone_KeystoneCreate = (FARPROC *)(0x7116D4);
static FARPROC *keystone_Call_KsRelease = (FARPROC *)(0x7116E0);
static FARPROC *keystone_Call_KsTranslateAccelerator = (FARPROC *)(0x7116E4);
static FARPROC *keystone_Call_KsCreateWindow = (FARPROC *)(0x7116E8);
static FARPROC *keystone_Call_KsGetWindow = (FARPROC *)(0x7116EC);
static FARPROC *keystone_Call_KsUpdate = (FARPROC *)(0x7116F0);
static FARPROC *keystone_Call_KsDispatchMessage = (FARPROC *)(0x7116F4);
static FARPROC *keystone_Call_KsSetFocusWindow = (FARPROC *)(0x7116F8);
static FARPROC *keystone_Call_KW_Release = (FARPROC *)(0x7116FC);
static FARPROC *keystone_Call_KW_GetControlByID = (FARPROC *)(0x711700);
static FARPROC *keystone_Call_KW_ReLayout = (FARPROC *)(0x711704);
static FARPROC *keystone_Call_KW_SetFocusControl = (FARPROC *)(0x711708);
static FARPROC *keystone_Call_KW_AddDirtyControl = (FARPROC *)(0x71170C);
static FARPROC *keystone_Call_KW_ShowWindow = (FARPROC *)(0x711710);
static FARPROC *keystone_Call_KC_GetAttribute = (FARPROC *)(0x711714);
static FARPROC *keystone_Call_KC_SetAttribute = (FARPROC *)(0x711718);
static FARPROC *keystone_Call_KC_SendMessage = (FARPROC *)(0x71171C);

static uint8_t *safemode = (uint8_t *)(0x709034);

void load_keystone(void) {
    if(*safemode) {
        return;
    }

    // Attempt to load Keystone's DLL.
    HMODULE keystone = (*keystone_handle = LoadLibrary("keystone.dll"));
    if(keystone == NULL) {
        fprintf(stderr, "keystone.dll could not be loaded!");
        *current_directory = NULL;
        return;
    }

    // Get the current directory as 16-bit
    size_t directory_length = GetCurrentDirectoryW(0, NULL) + 1; // include null terminator
    *current_directory = malloc(directory_length * sizeof(**current_directory));
    GetCurrentDirectoryW(directory_length, *current_directory);

    // Get these function addresses
    *keystone_KeystoneCreate = GetProcAddress(keystone, "KeystoneCreate");
    *keystone_Call_KsTranslateAccelerator = GetProcAddress(keystone, "Call_KsTranslateAccelerator");
    *keystone_Call_KsCreateWindow = GetProcAddress(keystone, "Call_KsCreateWindow");
    *keystone_Call_KsGetWindow = GetProcAddress(keystone, "Call_KsGetWindow");
    *keystone_Call_KsUpdate = GetProcAddress(keystone, "Call_KsUpdate");
    *keystone_Call_KsDispatchMessage = GetProcAddress(keystone, "Call_KsDispatchMessage");
    *keystone_Call_KW_Release = GetProcAddress(keystone, "Call_KW_Release");
    *keystone_Call_KsSetFocusWindow = GetProcAddress(keystone, "Call_KsSetFocusWindow");
    *keystone_Call_KW_GetControlByID = GetProcAddress(keystone, "Call_KW_GetControlByID");
    *keystone_Call_KC_GetAttribute = GetProcAddress(keystone, "Call_KC_GetAttribute");
    *keystone_Call_KC_SetAttribute = GetProcAddress(keystone, "Call_KC_SetAttribute");
    *keystone_Call_KC_SendMessage = GetProcAddress(keystone, "Call_KC_SendMessage");
    *keystone_Call_KW_ReLayout = GetProcAddress(keystone, "Call_KW_ReLayout");
    *keystone_Call_KW_SetFocusControl = GetProcAddress(keystone, "Call_KW_SetFocusControl");
    *keystone_Call_KW_AddDirtyControl = GetProcAddress(keystone, "Call_KW_AddDirtyControl");
    *keystone_Call_KsRelease = GetProcAddress(keystone, "Call_KsRelease");
    *keystone_Call_KW_ShowWindow = GetProcAddress(keystone, "Call_KW_ShowWindow");
}

void unload_keystone(void) {
    if(*keystone_handle != NULL) {
        FreeLibrary(*keystone_handle);
        *keystone_handle = NULL;
    }

    *keystone_KeystoneCreate = NULL;
    *keystone_Call_KsTranslateAccelerator = NULL;
    *keystone_Call_KsCreateWindow = NULL;
    *keystone_Call_KsGetWindow = NULL;
    *keystone_Call_KsUpdate = NULL;
    *keystone_Call_KsDispatchMessage = NULL;
    *keystone_Call_KW_Release = NULL;
    *keystone_Call_KsSetFocusWindow = NULL;
    *keystone_Call_KW_GetControlByID = NULL;
    *keystone_Call_KC_GetAttribute = NULL;
    *keystone_Call_KC_SetAttribute = NULL;
    *keystone_Call_KC_SendMessage = NULL;
    *keystone_Call_KW_ReLayout = NULL;
    *keystone_Call_KW_SetFocusControl = NULL;
    *keystone_Call_KW_AddDirtyControl = NULL;
    *keystone_Call_KsRelease = NULL;
    *keystone_Call_KW_ShowWindow = NULL;

    free(*current_directory);
    *current_directory = NULL;
}
