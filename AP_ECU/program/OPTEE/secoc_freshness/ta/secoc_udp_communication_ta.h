#include <inttypes.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <stdio.h>
#include "../include/common.h"

typedef struct __attribute__((packed)){
    uint32_t freshness;
} ta_persist_t;

typedef struct __attribute__((packed)){
    ta_persist_t persist;
    bool data_loaded;
} ta_ctx_t;

TEE_Result create_Freshness(const char* alias, TEE_ObjectHandle* object);

TEE_Result save_Freshness(const char* alias, const ta_ctx_t* ctx_obj);

TEE_Result load_Freshness(const char* alias, ta_ctx_t* ctx_obj);

TEE_Result delete_Freshness(const char* alias);