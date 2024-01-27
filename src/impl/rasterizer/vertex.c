#include "../d3d9/d3d9.h"
#include "../d3d9/d3dx9.h"
#include "shader.h"
#include "vertex.h"

RasterizerVertexDeclaration *vertex_declarations = (RasterizerVertexDeclaration *)0x6D8D30;

bool rasterizer_initialize_vertex_declarations(void) {
    IDirect3DDevice9 **device = get_d3d9_device();

    D3DVERTEXELEMENT9 *vertex_declarators[] = {
        (D3DVERTEXELEMENT9 *)0x658108,
        (D3DVERTEXELEMENT9 *)0x6580A0,
        (D3DVERTEXELEMENT9 *)0x658020,
        (D3DVERTEXELEMENT9 *)0x657FD8,
        (D3DVERTEXELEMENT9 *)0x657FB8,
        (D3DVERTEXELEMENT9 *)0x657F68,
        (D3DVERTEXELEMENT9 *)0x657F80,
        (D3DVERTEXELEMENT9 *)0x657F98,
        (D3DVERTEXELEMENT9 *)0x658138,
        (D3DVERTEXELEMENT9 *)0x6580E0,
        (D3DVERTEXELEMENT9 *)0x658060,
        (D3DVERTEXELEMENT9 *)0x658080,
        (D3DVERTEXELEMENT9 *)0x657FF8,
        (D3DVERTEXELEMENT9 *)0x658180,
        (D3DVERTEXELEMENT9 *)0x6581A0,
        (D3DVERTEXELEMENT9 *)0x658158
    };

    for(size_t i = 0; i < RASTERIZER_VERTEX_DECLARATION_COUNT; i++) {
        vertex_declarations[i].declaration = NULL;
        vertex_declarations[i].fvf = 0;
        vertex_declarations[i].fvf_2 = 0;
    }

    bool success = true;

    #define CREATE_VERTEX_DECLARATION(vertex_data_index, vertex_declaration_index) { \
        HRESULT res = IDirect3DDevice9_CreateVertexDeclaration(*device, vertex_declarators[vertex_data_index], &vertex_declarations[vertex_declaration_index].declaration); \
        success = success && (res == D3D_OK); \
    }

    CREATE_VERTEX_DECLARATION(0, 0);
    CREATE_VERTEX_DECLARATION(0, 1);
    CREATE_VERTEX_DECLARATION(1, 2);
    CREATE_VERTEX_DECLARATION(1, 3);
    CREATE_VERTEX_DECLARATION(2, 4);
    CREATE_VERTEX_DECLARATION(2, 5);
    CREATE_VERTEX_DECLARATION(3, 6);
    CREATE_VERTEX_DECLARATION(3, 7);
    CREATE_VERTEX_DECLARATION(4, 8);
    CREATE_VERTEX_DECLARATION(5, 9);
    CREATE_VERTEX_DECLARATION(6, 10);
    CREATE_VERTEX_DECLARATION(7, 11);
    CREATE_VERTEX_DECLARATION(8, 12);
    CREATE_VERTEX_DECLARATION(9, 13);
    CREATE_VERTEX_DECLARATION(10, 14);
    CREATE_VERTEX_DECLARATION(11, 15);
    CREATE_VERTEX_DECLARATION(12, 16);
    CREATE_VERTEX_DECLARATION(13, 17);
    CREATE_VERTEX_DECLARATION(14, 18);

    RasterizerShaderVersion *current_shader_version = get_current_shader_version();
    RasterizerShaderVersion minimum_shader_version = get_minimum_shader_version();
    if(current_shader_version->major < minimum_shader_version.major && current_shader_version->minor < minimum_shader_version.minor) {
        vertex_declarations[0].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[1].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[2].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[3].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[4].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[5].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[6].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL | D3DFVF_TEX2;
        vertex_declarations[7].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL | D3DFVF_TEX2;
        vertex_declarations[8].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL | D3DFVF_TEX2;
        vertex_declarations[9].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL | D3DFVF_TEX2;
        vertex_declarations[10].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[11].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
        vertex_declarations[16].fvf_2 = D3DFVF_XYZB2 | D3DFVF_NORMAL;
    }
    else {
        vertex_declarations[0].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[1].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[2].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[3].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[4].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[5].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[6].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;
        vertex_declarations[7].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;
        vertex_declarations[8].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;
        vertex_declarations[9].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;
        vertex_declarations[10].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[11].fvf_2 = D3DFVF_XYZB2;
        vertex_declarations[16].fvf_2 = D3DFVF_XYZB2;
    }

    vertex_declarations[12].fvf_2 = D3DFVF_XYZB2;
    vertex_declarations[13].fvf_2 = D3DFVF_XYZB2;
    vertex_declarations[14].fvf_2 = D3DFVF_XYZB2;
    vertex_declarations[15].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;
    vertex_declarations[17].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;
    vertex_declarations[18].fvf_2 = D3DFVF_XYZB2 | D3DFVF_TEX2;

    D3DXFVFFromDeclarator(vertex_declarators[11], (DWORD *)&vertex_declarations[15].fvf);

    vertex_declarations[17].fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    vertex_declarations[18].fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;

    if(get_shaders_disabled()) {
        CREATE_VERTEX_DECLARATION(15, 19);
        vertex_declarations[19].fvf_2 = D3DFVF_XYZB2;
    }

    #undef CREATE_VERTEX_DECLARATION

    return success;
}
