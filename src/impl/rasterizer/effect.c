#include <windows.h>
#include <stdio.h>
#include "../d3d9/d3d9.h"
#include "../d3d9/d3dx9.h"
#include "../exception/exception.h"
#include "shader.h"
#include "effect.h"

RasterizerEffect *rasterizer_effects = (RasterizerEffect *)(0x694BC8);
ID3DXEffectPool **rasterizer_effect_pool = (ID3DXEffectPool **)(0x70CA80);
D3DXMACRO rasterizer_effects_defines[2];
uint32_t *pci_vendor_id = (uint32_t *)(0x71238C);
uint32_t *pci_device_id = (uint32_t *)(0x712390);
uint32_t *shader_max_version = (uint32_t *)(0x7B062C);

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
    char version[40];
    bool version_found = false;
    
    if(*pci_vendor_id == 0 && *pci_device_id == 0x270D) {
        uint8_t major_version = *shader_max_version >> 8 & 0xff;
        uint8_t minor_version = *shader_max_version & 0xff;
        bool technique_valid = false;
        D3DXHANDLE technique_handle;
        while(major_version >= 0 && !version_found) {
            while(minor_version >= 0 && !version_found) {
                sprintf(version, "ps_%d_%d", major_version, minor_version);
                technique_handle = ID3DXEffect_GetTechniqueByName(effect, version);
                
                version_found = technique_valid;
                if(technique_handle != NULL) {
                    HRESULT res = ID3DXEffect_ValidateTechnique(effect, technique_handle);
                    technique_valid = res == D3D_OK;
                }
                minor_version--;
                version_found = technique_valid;
            }
            major_version--;
            minor_version = 9;
        }
        if(version_found) {
            HRESULT res = ID3DXEffect_SetTechnique(effect, technique_handle);
        }
        else {
            const char *format;
            if(*shader_max_version < 0xFFFF0101) {
                format = "TDefault_no_ps";
            }
            else {
                format = "TDefault_ps";
            }
            sprintf(version, "%s", format);
            technique_handle = ID3DXEffect_GetTechniqueByName(effect, version);
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
        version_found = true;
    }
    else {
        sprintf(version, "fallback");
        D3DXHANDLE technique_handle = ID3DXEffect_GetTechniqueByName(effect, version);
        version_found = technique_handle != NULL;
        if(!version_found) {
            return false;
        }
    }

    effect_entry->textures[0] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture0");
    effect_entry->textures[1] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture1");
    effect_entry->textures[2] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture2");
    effect_entry->textures[3] = ID3DXEffect_GetParameterByName(effect, NULL, "Texture3");

    return version_found;
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

bool rasterizer_init_effects(void) {
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

    for(size_t i = 32; i <= 34; i++) {
        effect_entry = &rasterizer_effects[i];
        effect = effect_entry->effect;
        params = GlobalAlloc(0, sizeof(D3DXHANDLE) * 3);
        params[0] = ID3DXEffect_GetParameterByName(effect, NULL, "c_eye_forward");
        params[1] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_perpendicular_color");
        params[2] = ID3DXEffect_GetParameterByName(effect, NULL, "c_view_parallel_color");
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
