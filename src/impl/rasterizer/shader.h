#ifndef DEMON__IMPL__RASTERIZER__SHADER_H
#define DEMON__IMPL__RASTERIZER__SHADER_H

#include <stdint.h>
#include <stdbool.h>

#define RASTERIZER_EFFECTS_COUNT 122
#define RASTERIZER_VERTEX_SHADER_COUNT 64

/**
 * Reads a shader file from disk and decrypts it.
 * 
 * @param filename      The filename of the shader file.
 * @param buffer        A pointer to a buffer that will be allocated and filled with the shader file data.
 * @param bytes_read    A pointer to a size_t that will be filled with the size of the shader file data.
 * 
 * @return true if successful, false if not.
 */
bool read_shader_file(const char *filename, char **buffer, size_t *bytes_read);

/**
 * @todo Add an enum for the effect indices.
 */

typedef struct RasterizerEffect {
    ID3DXEffect *effect;
    uint32_t unknown_1;
    D3DXHANDLE textures[4];
    D3DXHANDLE *params;
    uint32_t unknown_2;
} RasterizerEffect;

/**
 * Initializes effect shaders.
 * 
 * @return true if successful, crashes if not.
 */
bool rasterizer_effects_initialize(void);

/**
 * Gets an effect shader by index.
 * 
 * @param index     The index of the effect shader.
 * 
 * @return A pointer to the effect struct, or NULL if index is out of bounds.
 */
RasterizerEffect *get_rasterizer_effect(size_t index);

/**
 * @todo Add an enum for the vertex shader indices.
 */

typedef struct RasterizerVertexShader {
    IDirect3DVertexShader9 *shader;
    uint32_t unknown_1;
} RasterizerVertexShader;

/**
 * Initializes vertex shaders.
 * 
 * @return true if successful, crashes if not.
 */
bool rasterizer_initialize_vertex_shaders(void);

/**
 * Gets a vertex shader by index.
 * 
 * @param index     The index of the vertex shader.
 * 
 * @return A pointer to the vertex shader struct, or NULL if index is out of bounds.
 */
RasterizerVertexShader *get_rasterizer_vertex_shader(size_t index);

typedef struct RasterizerShaderVersion {
    uint8_t major;
    uint8_t minor;
} RasterizerShaderVersion;

/**
 * Gets the current shader version.
 * 
 * @return A pointer to the current shader version struct.
 */
RasterizerShaderVersion *get_current_shader_version(void);

/**
 * Gets the minimum shader version.
 * 
 * @return Minimum shader version struct.
 */
RasterizerShaderVersion get_minimum_shader_version(void);

/**
 * Gets whether shaders are disabled.
 * 
 * @return true if shaders are disabled, false if not.
 */
bool get_shaders_disabled(void);

#endif
