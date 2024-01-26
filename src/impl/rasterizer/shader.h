#ifndef DEMON__IMPL__RASTERIZER__SHADER_H
#define DEMON__IMPL__RASTERIZER__SHADER_H

#include <stdint.h>
#include <stdbool.h>

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

#endif
