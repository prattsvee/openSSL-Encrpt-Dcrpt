#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/blowfish.h>

#define BLOCKSIZE 8  // Block size for Blowfish

unsigned char init_vec[BLOCKSIZE] = {0};  // Zero-initialized IV for CBC mode

// Encryption function
void *fs_encrypt(void *plaintext, int bufsize, char *keystr, int *resultlen) {
    if (!plaintext || !keystr || bufsize <= 0) return NULL;

    // Set Blowfish key
    BF_KEY key;
    BF_set_key(&key, strlen(keystr), (const unsigned char *)keystr);

    // Calculate padding size
    int pad_size = BLOCKSIZE - (bufsize % BLOCKSIZE);
    int padded_len = bufsize + pad_size;

    // Allocate memory for result buffer
    unsigned char *result = (unsigned char *) malloc(padded_len);
    if (!result) return NULL;

    // Copy plaintext to result buffer and apply padding
    memcpy(result, plaintext, bufsize);
    memset(result + bufsize, pad_size, pad_size);

    unsigned char iv[BLOCKSIZE] = {0};  // Initialization vector for CBC mode
    unsigned char temp_block[BLOCKSIZE];

    // Encrypt the plaintext using CBC mode
    for (int i = 0; i < padded_len; i += BLOCKSIZE) {
        // XOR plaintext block with IV or previous ciphertext block
        for (int j = 0; j < BLOCKSIZE; j++) {
            temp_block[j] = result[i + j] ^ iv[j];
        }

        // Perform Blowfish encryption
        BF_ecb_encrypt(temp_block, result + i, &key, BF_ENCRYPT);

        // Update IV (next IV is the current ciphertext block)
        memcpy(iv, result + i, BLOCKSIZE);
    }

    *resultlen = padded_len;
    return result;
}

// Decryption function
void *fs_decrypt(void *ciphertext, int bufsize, char *keystr, int *resultlen) {
    if (!ciphertext || !keystr || bufsize <= 0 || bufsize % BLOCKSIZE != 0) return NULL;

    // Set Blowfish key
    BF_KEY key;
    BF_set_key(&key, strlen(keystr), (const unsigned char *)keystr);

    // Allocate memory for result buffer
    unsigned char *result = (unsigned char *) malloc(bufsize);
    if (!result) return NULL;

    unsigned char iv[BLOCKSIZE] = {0};  // Initialization vector for CBC mode
    unsigned char temp_block[BLOCKSIZE];
    unsigned char prev_block[BLOCKSIZE] = {0};

    // Decrypt the ciphertext using CBC mode
    for (int i = 0; i < bufsize; i += BLOCKSIZE) {
        // Save current block to temp_block before decryption
        memcpy(temp_block, (unsigned char *)ciphertext + i, BLOCKSIZE);

        // Perform Blowfish decryption
        BF_ecb_encrypt(temp_block, result + i, &key, BF_DECRYPT);

        // XOR decrypted block with IV or previous ciphertext block
        for (int j = 0; j < BLOCKSIZE; j++) {
            result[i + j] ^= iv[j];
        }

        // Update IV (next IV is the current ciphertext block)
        memcpy(iv, temp_block, BLOCKSIZE);
    }

    // Remove padding
    int pad_size = result[bufsize - 1];
    if (pad_size < 1 || pad_size > BLOCKSIZE) {
        free(result);
        return NULL;
    }

    *resultlen = bufsize - pad_size;
    return result;
}
