#ifndef DEMON__IMPL__RASTERIZER__VERTEX_H
#define DEMON__IMPL__RASTERIZER__VERTEX_H

#include <stdint.h>
#include <stdbool.h>
#include <d3d9.h>

#define RASTERIZER_VERTEX_SHADER_COUNT 64

/**
 * @todo Add an enum for the vertex shader indices.
 */

typedef struct RasterizerVertexShader {
    IDirect3DVertexShader9 *shader;
    uint32_t unknown_1;
} RasterizerVertexShader;

#endif
