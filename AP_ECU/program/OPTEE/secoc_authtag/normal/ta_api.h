#ifndef TAAPI_H
#define TAAPI_H

#include <tee_client_api.h>
#include "../include/common.h"
#include "common.h"

typedef struct{
    TEEC_Context ctx;
    TEEC_Session session;
} TeeSecOC;

int init(TeeSecOC* secoc_obj);

int store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness);

int load_freshness(TeeSecOC* secoc_obj, const char* alias);

int delete_object(TeeSecOC* secoc_obj, const char* alias);

bool check_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness);

void read_freshness(TeeSecOC* secoc_obj, const char* alias);

int load_key(TeeSecOC* secoc_obj, const char* alias);

int read_key(TeeSecOC* secoc_obj, const char* alias);

int store_key(TeeSecOC* secoc_obj, const char* alias, const uint8_t key[KEY_SIZE]);

int gen_aes_mac(TeeSecOC* secoc_obj, TEEC_SharedMemory* in, const char* alias, uint8_t aes_cmac[SECOC_MAC_SIZE]);

#endif