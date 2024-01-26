#include <windows.h>
#include <stdio.h>
#include "../d3d9/d3d9.h"
#include "../exception/exception.h"
#include "shader.h"
#include "vertex.h"

RasterizerVertexShader *rasterizer_vertex_shaders = (RasterizerVertexShader *)(0x695B08);

bool rasterizer_load_vertex_shaders(void) {
    char *buffer;
    size_t buffer_size;
    bool success = read_shader_file("shaders/vsh.bin", &buffer, &buffer_size);
    if(!success) {
        return false;
    }

    IDirect3DDevice9 **d3d9_device = get_d3d9_device();

    char *buffer_pointer = buffer;
    size_t i = 0;
    while(i < RASTERIZER_VERTEX_SHADER_COUNT && buffer_pointer < buffer + buffer_size) {
        size_t shader_size = *(uint32_t *)(buffer_pointer);
        char *current_shader = buffer_pointer + sizeof(uint32_t);
        buffer_pointer = current_shader + shader_size;

        if(buffer_pointer > buffer + buffer_size) {
            break;
        }

        IDirect3DVertexShader9 *shader;
        HRESULT res = IDirect3DDevice9_CreateVertexShader(*d3d9_device, (DWORD *)current_shader, &shader);
        if(res != D3D_OK) {
            break;
        }

        rasterizer_vertex_shaders[i].shader = shader;
        i++;
    }

    GlobalFree(buffer);

    if(i < RASTERIZER_VERTEX_SHADER_COUNT) {
        for(size_t j = 0; j < i; j++) {
            if(rasterizer_vertex_shaders[j].shader != NULL) {
                IDirect3DVertexShader9_Release(rasterizer_vertex_shaders[j].shader);
            }
        }
        return false;
    }

    return true;
}
