#ifndef DEMON__IMPL__RASTERIZER__SHADER_H
#define DEMON__IMPL__RASTERIZER__SHADER_H

#include <stdbool.h>
#include <stdint.h>
#include <d3d9.h>
#include <d3dx9.h>

#define RASTERIZER_EFFECTS_COUNT 122

typedef struct RasterizerEffect {
    ID3DXEffect *effect;
    uint32_t unknown_1;
    D3DXHANDLE param_1;
    D3DXHANDLE param_2;
    D3DXHANDLE param_3;
    D3DXHANDLE param_4;
    uint32_t unknown_2;
    uint32_t unknown_3;
} RasterizerEffect;

bool read_shader_file(const char *filename, char **buffer, size_t *bytes_read);

#endif
