#ifndef DEMON__IMPL__RASTERIZER__EFFECT_H
#define DEMON__IMPL__RASTERIZER__EFFECT_H

#include <stdint.h>
#include <stdbool.h>
#include <d3dx9.h>

#define RASTERIZER_EFFECTS_COUNT 122

/**
 * @todo Add an enum for the effect indices.
 */

typedef struct RasterizerEffect {
    ID3DXEffect *effect;
    uint32_t unknown_1;
    D3DXHANDLE texture_0;
    D3DXHANDLE texture_1;
    D3DXHANDLE texture_2;
    D3DXHANDLE texture_3;
    D3DXHANDLE *params;
    uint32_t unknown_2;
} RasterizerEffect;

/**
 * Initializes effect shaders.
 * 
 * @return true if successful, crashes if not.
 */
bool rasterizer_init_effects(void);

/**
 * Gets an effect shader by index.
 * 
 * @param index     The index of the effect shader.
 * 
 * @return A pointer to the effect struct, or NULL if index is out of bounds.
 */
RasterizerEffect *get_rasterizer_effect(size_t index);

#endif
