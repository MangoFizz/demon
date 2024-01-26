#include <windows.h>
#include "../crypto/md5.h"
#include "../crypto/xtea.h"
#include "shader.h"

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
