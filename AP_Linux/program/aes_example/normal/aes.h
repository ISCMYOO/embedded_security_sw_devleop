#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <tee_client_api.h>
#include "../include/common.h"

typedef struct{
    uint8_t* cipher;
    uint32_t cipher_len;
    uint8_t iv[12];
    uint8_t tag[16];
    int has_value;
} EncryptData;

typedef struct{
    TEEC_Context ctx;
    TEEC_Session session;
} TeeAes;

int init(TeeAes* aes_obj);

TEEC_Result loadkey(TeeAes* aes_obj, const char* alias);

int generateKey(TeeAes* aes_obj, const char* alias);

int loadOrGenKey(TeeAes* aes_obj, const char* alias);

EncryptData encrypt(TeeAes* aes_obj, const uint8_t* plain, uint32_t plain_len, const uint8_t* aad, uint32_t aad_len);

uint8_t* decrypt(TeeAes* aes_obj,const uint8_t* cipher, uint32_t cipher_len, const uint8_t* iv, const uint8_t* aad, uint32_t aad_len, const uint8_t* tag);

void TeeAes_final(TeeAes* aes_obj);