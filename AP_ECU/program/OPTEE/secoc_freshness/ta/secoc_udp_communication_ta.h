#include <inttypes.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <stdio.h>
#include "../include/common.h"

#define KEY_SIZE    16

typedef struct __attribute__((packed)){
    uint8_t key[KEY_SIZE];
    uint32_t freshness;
} ta_ctx_t;

TEE_Result save_Freshness(const char* alias, const ta_ctx_t* ctx_obj);

TEE_Result load_Freshness(const char* alias, ta_ctx_t* ctx_obj);

TEE_Result delete_Freshness(const char* alias);