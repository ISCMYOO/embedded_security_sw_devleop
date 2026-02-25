#include <inttypes.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <stdio.h>
#include "../include/common.h"

typedef struct __attribute__((packed)){
    uint8_t key[KEY_SIZE];
    uint32_t freshness;
} ta_persist_t;

typedef struct __attribute__((packed)){
    ta_persist_t persist;
    bool data_loaded;
} ta_ctx_t;

TEE_Result create_obj(const char* alias, TEE_ObjectHandle* object);

TEE_Result save_obj(const char* alias, const ta_ctx_t* ctx_obj);

TEE_Result load_obj(const char* alias, ta_ctx_t* ctx_obj);

TEE_Result delete_obj(const char* alias);

TEE_Result check_freshness(const char* alias, ta_ctx_t* ctx_obj, TEE_Param params[4]);

bool setOperation(TEE_OperationHandle* opHandle, TEE_OperationMode opMode, const uint8_t* key);

bool gen_aes_mac(TEE_Param params[4], TEE_OperationHandle op);