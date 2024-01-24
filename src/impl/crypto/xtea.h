#ifndef DEMON__IMPL__CRYPTO__XTEA_H
#define DEMON__IMPL__CRYPTO__XTEA_H

#include <stdint.h>

/**
 * Decrypt data using XTEA
 * 
 * @param data_size size of data
 * @param data      data to decrypt
 * @param key       key to use for decryption
 */
void xtea_decrypt(size_t data_size, unsigned char *data, uint32_t *key);

#endif
