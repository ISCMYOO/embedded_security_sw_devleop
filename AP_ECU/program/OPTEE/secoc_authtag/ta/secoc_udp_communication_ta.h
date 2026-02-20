#include <inttypes.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <stdio.h>
#include "../include/common.h"

typedef struct __attribute__((packed)){
    uint8_t key[KEY_SIZE];
    uint32_t freshness;
} ta_ctx_t;

void logHex(const uint8_t *hex, size_t size);

TEE_Result save_obj(const char* alias, const ta_ctx_t* ctx_obj);

TEE_Result load_obj(const char* alias, ta_ctx_t* ctx_obj);

TEE_Result delete_obj(const char* alias);