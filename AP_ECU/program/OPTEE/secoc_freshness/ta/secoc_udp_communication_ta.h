#include <inttypes.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <stdio.h>
#include "../include/common.h"

#define KEY_SIZE    16

typedef struct {
    uint8_t key[KEY_SIZE];
    uint32_t freshness;
} ta_ctx_t;

bool save_Freshness(const char* alias, const uint32_t freshness);

TEE_Result load_Freshness(const char* alias, uint32_t* freshness);

TEE_Result delete_Freshness(const char* alias);