#include <windows.h>
#include <stdio.h>
#include "../crypto/md5.h"
#include "../crypto/xtea.h"
#include "../d3d9/d3d9.h"
#include "../d3d9/d3dx9.h"
#include "../exception/exception.h"
#include "shader.h"

RasterizerEffect *rasterizer_effects = (RasterizerEffect *)(0x694BC8);
ID3DXEffectPool **rasterizer_effect_pool = (ID3DXEffectPool **)(0x70CA80);
D3DXMACRO rasterizer_effects_defines[2];
uint32_t *pci_vendor_id = (uint32_t *)(0x71238C);
uint32_t *pci_device_id = (uint32_t *)(0x712390);
RasterizerShaderVersion *shader_current_version = (RasterizerShaderVersion *)(0x7B062C);
RasterizerVertexShader *rasterizer_vertex_shaders = (RasterizerVertexShader *)(0x695B08);
bool *shaders_disabled = (bool *)(0x7B061C); // this is set to true when -use00 is set
const RasterizerShaderVersion minimum_shader_version = {1, 1};

bool decrypt_shader_file(unsigned char *data, size_t data_size) {
    if(data_size < 0x22) {
        return false;
    }
    
    uint32_t key[4] = {0x3FFFFFDD, 0x00007FC3, 0x000000E5, 0x003FFFEF};
    xtea_decrypt(data_size, data, key);

    char hash_buffer[32];
    generate_md5_hash((char *)data, data_size - 33, hash_buffer);

    char *data_hash = (char *)data + data_size - 33;
    char *expected_hash = hash_buffer;
    for(size_t i = 0; i < 32; i++) {
        if(data_hash[i] != expected_hash[i]) {
            return false;
        }
    }
    return true;
}

bool read_shader_file(const char *filename, char **buffer, size_t *data_size) {
    *buffer = NULL;
    *data_size = 0;

    HANDLE file = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_SUPPORTS_EXTENDED_ATTRIBUTES, NULL);
    if(file == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    size_t file_size = GetFileSize(file, NULL);
    if(file_size == INVALID_FILE_SIZE) {
        CloseHandle(file);
        return false;
    }

    HGLOBAL buffer_handle = GlobalAlloc(GMEM_FIXED, file_size);
    if(buffer_handle != NULL) {
        size_t bytes_read = 0;
        bool success = ReadFile(file, buffer_handle, file_size, (LPDWORD)&bytes_read, NULL);
        if(success) {
            CloseHandle(file);
            bool decrypt_success = decrypt_shader_file((unsigned char *)buffer_handle, bytes_read);
            if(decrypt_success) {
                *buffer = buffer_handle;
                *data_size = file_size;
                return true;
            }
        }
        GlobalFree(buffer_handle);
    }

    CloseHandle(file);
    return false;
}

bool rasterizer_create_effect(int effect_index, char *shader_bytecode, size_t bytecode_size) {
    IDirect3DDevice9 *d3d9_device = *get_d3d9_device();
    ID3DXBuffer *compilation_errors = NULL;
    rasterizer_effects[effect_index].effect = NULL;

    HRESULT res = D3DXCreateEffect(d3d9_device, shader_bytecode, bytecode_size, rasterizer_effects_defines, NULL, 0, *rasterizer_effect_pool, &rasterizer_effects[effect_index].effect, &compilation_errors);

    if(compilation_errors != NULL) {
        ID3DXBuffer_Release(compilation_errors);
    }

    if(res != D3D_OK) {
        return false;
    }
    return true;
}

bool rasterizer_load_effect_texture_parameters(size_t effect_index) {
    RasterizerEffect *effect_entry = rasterizer_effects + effect_index;
    ID3DXEffect *effect = effect_entry->effect;
    char technique_name[40];
    bool technique_found = false;
    
    if(*pci_vendor_id == 0 && *pci_device_id != 0x270D) {
        int8_t major_version = shader_current_version->major;
        int8_t minor_version = shader_current_version->minor;
        bool technique_valid = false;
        D3DXHANDLE technique_handle;
        while(major_version >= 0 && !technique_found) {
            while(minor_version >= 0 && !technique_found) {
                sprintf(technique_name, "ps_%d_%d", major_version, minor_version);
                technique_handle = ID3DXEffect_GetTechniqueByName(effect, technique_name);
                
                technique_found = technique_valid;
                if(technique_handle != NULL) {
                    HRESULT res = ID3DXEffect_ValidateTechnique(effect, technique_handle);
                    technique_valid = res == D3D_OK;
                }
                minor_version--;
                technique_found = technique_valid;
            }
            major_version--;
            minor_version = 9;
        }
        if(technique_found) {
            HRESULT res = ID3DXEffect_SetTechnique(effect, technique_handle);
        }
        else {
            const char *format;
            if(shader_current_version->major < minimum_shader_version.major && shader_current_version->minor < minimum_shader_version.minor) {
                format = "TDefault_no_ps";
            }
            else {
                format = "TDefault_ps";
            }
            sprintf(technique_name, "%s", format);
            technique_handle = ID3DXEffect_GetTechniqueByName(effect, technique_name);
            if(technique_handle == NULL) {
                D3DXHANDLE next_technique_handle;
                HRESULT res = ID3DXEffect_FindNextValidTechnique(effect, technique_handle, &next_technique_handle);
                if(res != D3D_OK) {
                    return false;
                }
                technique_handle = next_technique_handle;
                res = ID3DXEffect_ValidateTechnique(effect, technique_handle);
                if(res != D3D_OK) {
                    return false;
                }
            }
        }
        technique_found = true;
    }
    else {
        sprintf(technique_name, "fallback");
        D3DXHANDLE technique_handle = ID3DXEffect_GetTechniqueByName(effect, technique_name);
        technique_found = technique_handle != NULL;
        if(!technique_found) {
            return false;
        }
    }

    effect_entry->textures[0] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture0");
    effect_entry->textures[1] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture1");
    effect_entry->textures[2] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture2");
    effect_entry->textures[3] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture3");

    return technique_found;
}

bool rasterizer_load_effects(void) {
    char *buffer;
    size_t buffer_size;
    bool success = read_shader_file("shaders/fx.bin", &buffer, &buffer_size);
    if(!success) {
        return false;
    }

    int i = 0;
    char *buffer_pointer = buffer;
    while(i < RASTERIZER_EFFECTS_COUNT) {
        char *current_shader_data = buffer_pointer + 4;
        uint32_t shader_size = *(uint32_t *)buffer_pointer;
        buffer_pointer = current_shader_data + shader_size;

        if(buffer_pointer >= buffer + buffer_size || !rasterizer_create_effect(i, current_shader_data, shader_size) || !rasterizer_load_effect_texture_parameters(i)) {
            for(size_t j = 0; j < RASTERIZER_EFFECTS_COUNT; j++) {
                ID3DXEffect **current_effect = &rasterizer_effects[j].effect;
                if(*current_effect != NULL) {
                    ID3DXEffect_Release(*current_effect);
                    *current_effect = NULL;
                }
            }
            break;
        }

        i++;
    }

    GlobalFree(buffer);
    
    if(i < RASTERIZER_EFFECTS_COUNT) {
        return false;
    }
    return true;
}

bool rasterizer_initialize_effects(void) {
    LCID previous_locale = GetThreadLocale();
    SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), SORT_DEFAULT));

    rasterizer_effects_defines[0].Name = "PS_2_0_TARGET";
    rasterizer_effects_defines[0].Definition = "ps_2_a";
    if(*pci_device_id == 0x270D) {
        rasterizer_effects_defines[0].Definition = "ps_2_0";
    }
    rasterizer_effects_defines[1].Name = NULL;
    rasterizer_effects_defines[1].Definition = NULL;

    HRESULT res = D3DXCreateEffectPool(rasterizer_effect_pool);
    if(res != D3D_OK) {
        CRASHF_DEBUG("D3DXCreateEffectPool failed with error code %08X", res);
        return false;
    }

    bool success = rasterizer_load_effects();
    if(!success) {
        CRASHF_DEBUG("Failed to load shaders/fx.bin");
        return false;
    }

    SetThreadLocale(previous_locale);

    RasterizerEffect *effect_entry;
    ID3DXEffect *effect;
    D3DXHANDLE *params;

    effect_entry = &rasterizer_effects[0];
    effect = effect_entry->effect;
    params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 6);
    params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_material_color");
    params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_plasma_animation");
    params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_primary_color");
    params[3] = ID3DXEffect_GetParameterByName(effect, NULL, "c_secondary_color");
    params[4] = ID3DXEffect_GetParameterByName(effect, NULL, "c_plasma_on_color");
    params[5] = ID3DXEffect_GetParameterByName(effect, NULL, "c_plasma_off_color");
    effect_entry->params = params;

    for(size_t i = 1; i <= 3; i++) {
        effect_entry = &rasterizer_effects[i];
        effect = effect_entry->effect;
        params = GlobalAlloc(0, sizeof(D3DXHANDLE));
        params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_material_color");
        effect_entry->params = params;
    }

    for(size_t i = 37; i <= 39; i++) {
        effect_entry = &rasterizer_effects[i];
        effect = effect_entry->effect;
        params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 3);
        params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_eye_forward");
        params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
        params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
        effect_entry->params = params;
    }

    for(size_t i = 32; i <= 34; i++) {
        effect_entry = &rasterizer_effects[i];
        effect = effect_entry->effect;
        params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 3);
        params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_eye_forward");
        params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
        params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
        effect_entry->params = params;
    }

    for(size_t i = 40; i <= 43; i++) {
        effect_entry = &rasterizer_effects[i];
        effect = effect_entry->effect;
        params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 4);
        params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_specular_brightness");
        params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
        params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
        params[3] = ID3DXEffect_GetParameterByName(effect, NULL, "c_multiplier");
        effect_entry->params = params;
    }

    effect_entry = &rasterizer_effects[106];
    params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 4);
    effect = effect_entry->effect;
    params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_eye_forward");
    params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
    params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
    params[3] = ID3DXEffect_GetParameterByName(effect, NULL, "c_group_intensity");
    effect_entry->params = params;

    effect_entry = &rasterizer_effects[107];
    effect = effect_entry->effect;
    params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 3);
    params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_eye_forward");
    params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
    params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
    effect_entry->params = params;

    effect_entry = &rasterizer_effects[108];
    effect = effect_entry->effect;
    params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 3);
    params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_eye_forward");
    params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
    params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
    effect_entry->params = params;

    effect_entry = &rasterizer_effects[114];
    effect = effect_entry->effect;
    params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 2);
    params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_desaturation_tint");
    params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_light_enhancement");
    effect_entry->params = params;

    for(size_t i = 116; i <= 121; i++) {
        effect_entry = &rasterizer_effects[i];
        effect = effect_entry->effect;
        params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 5);
        params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_primary_change_color");
        params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_fog_color_correction_0");
        params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_fog_color_correction_E");
        params[3] = ID3DXEffect_GetParameterByName(effect, NULL, "c_fog_color_correction_1");
        params[4] = ID3DXEffect_GetParameterByName(effect, NULL, "c_self_illumination_color");
        effect_entry->params = params;
    }

    return true;
}

RasterizerEffect *get_rasterizer_effect(size_t index) {
    if(index >= RASTERIZER_EFFECTS_COUNT) {
        return NULL;
    }
    return rasterizer_effects + index;
}

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

bool rasterizer_initialize_vertex_shaders(void) {
    size_t i = 0;
    while(i < RASTERIZER_VERTEX_SHADER_COUNT) {
        RasterizerVertexShader *shader = &rasterizer_vertex_shaders[i];
        if(shader->shader != NULL) {
            IDirect3DVertexShader9_Release(shader->shader);
            shader->shader = NULL;
        }
        i++;
    }

    bool success = rasterizer_load_vertex_shaders();
    if(!success) {
        CRASHF_DEBUG("Failed to load shaders/vsh.bin");
        return false;
    }

    return true;
}

void rasterizer_dispose_effects(void) {
    for(size_t i = 0; i < RASTERIZER_EFFECTS_COUNT; i++) {
        RasterizerEffect *current_entry = &rasterizer_effects[i];
        if(current_entry->effect != NULL) {
            ID3DXEffect_Release(current_entry->effect);
            current_entry->effect = NULL;
        }
        for(size_t j = 0; j < sizeof(current_entry->textures) / sizeof(current_entry->textures[0]); j++) {
            current_entry->textures[j] = NULL;
        }
        if(current_entry->params != NULL) {
            GlobalFree(current_entry->params);
            current_entry->params = NULL;
        }
    }
    if(*rasterizer_effect_pool != NULL) {
        ID3DXEffectPool_Release(*rasterizer_effect_pool);
        *rasterizer_effect_pool = NULL;
    }
}

D3DXHANDLE rasterizer_find_effect_technique_from_name(ID3DXEffect *effect, const char *name) {
    char technique_name[128];
    int8_t major_version = shader_current_version->major;
    int8_t minor_version = shader_current_version->minor;
    bool technique_found = false;
    bool technique_valid = false;
    D3DXHANDLE technique_handle;
    while(major_version >= 0 && !technique_found) {
        while(minor_version >= 0 && !technique_found) {
            sprintf(technique_name, "%s_ps_%d_%d", name, major_version, minor_version);
            technique_handle = ID3DXEffect_GetTechniqueByName(effect, technique_name);
            
            technique_found = technique_valid;
            if(technique_handle != NULL) {
                HRESULT res = ID3DXEffect_ValidateTechnique(effect, technique_handle);
                technique_valid = res == D3D_OK;
            }
            minor_version--;
            technique_found = technique_valid;
        }
        major_version--;
        minor_version = 9;
    }
    if(!technique_found) {
        return NULL;
    }
    return technique_handle;
}

RasterizerVertexShader *get_rasterizer_vertex_shader(size_t index) {
    if(index >= RASTERIZER_VERTEX_SHADER_COUNT) {
        return NULL;
    }
    return rasterizer_vertex_shaders + index;
}

RasterizerShaderVersion *get_current_shader_version(void) {
    return shader_current_version;
}

RasterizerShaderVersion get_minimum_shader_version(void) {
    return minimum_shader_version;
}

bool get_shaders_disabled(void) {
    return *shaders_disabled;
}
