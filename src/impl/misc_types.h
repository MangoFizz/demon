#ifndef DEMON__IMPL_MISC_TYPES_H
#define DEMON__IMPL_MISC_TYPES_H

#include <uchar.h>

#include "id.h"

typedef uint32_t ColorARGBInt;

typedef union ScenarioScriptNodeValue {
    float f;
    TableID id;
    int8_t b;
    int16_t s;
    int32_t l;
} ScenarioScriptNodeValue;
_Static_assert(sizeof(ScenarioScriptNodeValue) == 0x4);

typedef struct ColorARGB {
    float a;
    float r;
    float g;
    float b;
} ColorARGB;
_Static_assert(sizeof(ColorARGB) == 0x10);

typedef struct ColorRGB {
    float r;
    float g;
    float b;
} ColorRGB;
_Static_assert(sizeof(ColorRGB) == 0xC);

typedef struct String32 {
    char string[32];
} String32;
_Static_assert(sizeof(String32) == 32);

typedef struct Data {
    uint32_t size;
    uint32_t flags;
    uint32_t file_offset; // only applies to sounds
    void *pointer;
    char padding[4];
} Data;
_Static_assert(sizeof(Data) == 0x14);

#endif
