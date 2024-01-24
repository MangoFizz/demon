#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include "../crypto/md5.h"
#include "../crypto/xtea.h"
#include "../d3d9/d3d9.h"
#include "effect.h"

RasterizerEffect *rasterizer_effects = (RasterizerEffect *)(0x694BC8);
D3DXMACRO *rasterizer_effects_defines = (D3DXMACRO *)(0x7AFC80);
ID3DXEffectPool **rasterizer_effect_pool = (ID3DXEffectPool **)(0x70CA80);

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

bool rasterizer_load_effect(int effect_index, char *shader_bytecode, size_t bytecode_size) {
    IDirect3DDevice9 *d3d9_device = *get_d3d9_device();
    ID3DXBuffer *compilation_errors = NULL;
    rasterizer_effects[effect_index].effect = NULL;

    HRESULT res = D3DXCreateEffect(d3d9_device, shader_bytecode, bytecode_size, rasterizer_effects_defines, NULL, 0, *rasterizer_effect_pool, &rasterizer_effects[effect_index].effect, &compilation_errors);

    if(compilation_errors != NULL) {
        compilation_errors->lpVtbl->Release(compilation_errors);
    }

    if(res != D3D_OK) {
        return false;
    }
    return true;
}

bool rasterizer_init_effect_parameters(size_t effect_index) {
    RasterizerEffect *effect_entry = rasterizer_effects + effect_index;
    ID3DXEffect *effect = effect_entry->effect;
    char version[40];
    bool version_found = false;
    
    uint32_t pci_vendor_id = *(uint32_t *)(0x71238C);
    uint32_t pci_device_id = *(uint32_t *)(0x712390);
    if(pci_vendor_id == 0 && pci_device_id == 0x270D) {
        uint32_t shader_max_version = *(uint32_t *)(0x7B062C);
        uint8_t major_version = shader_max_version >> 8 & 0xff;
        uint8_t minor_version = shader_max_version & 0xff;
        bool technique_valid = false;
        D3DXHANDLE technique_handle;
        while(major_version >= 0 && !version_found) {
            while(minor_version >= 0 && !version_found) {
                sprintf(version, "ps_%d_%d", major_version, minor_version);
                technique_handle = effect->lpVtbl->GetTechniqueByName(effect, version);
                
                version_found = technique_valid;
                if(technique_handle != NULL) {
                    HRESULT res = effect->lpVtbl->ValidateTechnique(effect, technique_handle);
                    technique_valid = res == D3D_OK;
                }
                minor_version--;
                version_found = technique_valid;
            }
            major_version--;
            minor_version = 9;
        }
        if(version_found) {
            HRESULT res = effect->lpVtbl->SetTechnique(effect, technique_handle);
        }
        else {
            const char *format;
            if(shader_max_version < 0xFFFF0101) {
                format = "TDefault_no_ps";
            }
            else {
                format = "TDefault_ps";
            }
            sprintf(version, "%s", format);
            technique_handle = effect->lpVtbl->GetTechniqueByName(effect, version);
            if(technique_handle == NULL) {
                D3DXHANDLE next_technique_handle;
                HRESULT res = effect->lpVtbl->FindNextValidTechnique(effect, technique_handle, &next_technique_handle);
                if(res != D3D_OK) {
                    return false;
                }
                technique_handle = next_technique_handle;
                res = effect->lpVtbl->ValidateTechnique(effect, technique_handle);
                if(res != D3D_OK) {
                    return false;
                }
            }
        }
        version_found = true;
    }
    else {
        sprintf(version, "fallback");
        D3DXHANDLE technique_handle = effect->lpVtbl->GetTechniqueByName(effect, version);
        version_found = technique_handle != NULL;
        if(!version_found) {
            return false;
        }
    }

    D3DXHANDLE param_handle;
    param_handle = effect->lpVtbl->GetParameterByName(effect, NULL, "Texture0");
    effect_entry->param_1 = param_handle;
    param_handle = effect->lpVtbl->GetParameterByName(effect, NULL, "Texture1");
    effect_entry->param_2 = param_handle;
    param_handle = effect->lpVtbl->GetParameterByName(effect, NULL, "Texture2");
    effect_entry->param_3 = param_handle;
    param_handle = effect->lpVtbl->GetParameterByName(effect, NULL, "Texture3");
    effect_entry->param_4 = param_handle;

    return version_found;
}

bool rasterizer_init_effects() {
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

        if(buffer_pointer >= buffer + buffer_size || !rasterizer_load_effect(i, current_shader_data, shader_size) || !rasterizer_init_effect_parameters(i)) {
            for(size_t j = 0; j < RASTERIZER_EFFECTS_COUNT; j++) {
                ID3DXEffect **current_effect = &rasterizer_effects[j].effect;
                if(*current_effect != NULL) {
                    (*current_effect)->lpVtbl->Release(*current_effect);
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
