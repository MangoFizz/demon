#ifndef DEMON__IMPL__RASTERIZER__VERTEX_H
#define DEMON__IMPL__RASTERIZER__VERTEX_H

#include <stdint.h>
#include <stdbool.h>
#include <d3d9.h>

#define RASTERIZER_VERTEX_DECLARATION_COUNT 20

typedef struct RasterizerVertexDeclaration {
    IDirect3DVertexDeclaration9 *declaration;
    uint32_t fvf;
    uint32_t fvf_2; // ??? seems to be another D3DFVF
} RasterizerVertexDeclaration;

/**
 * Initializes vertex declarations.
 * 
 * @return true if successful, false if not.
 */
bool rasterizer_initialize_vertex_declarations(void);

/**
 * Disposes vertex declarations.
 */
void rasterizer_dispose_vertex_declarations(void);

#endif
