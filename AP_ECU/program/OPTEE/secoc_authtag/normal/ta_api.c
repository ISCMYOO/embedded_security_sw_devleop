#include "ta_api.h"

int init(TeeSecOC* secoc_obj){
    TEEC_Result res;

    res = TEEC_InitializeContext(NULL, &(secoc_obj->ctx));
    if(res != TEEC_SUCCESS){
        printf("result : 0x%x\n", res);
        return -1;
    }

    TEEC_UUID uuid = TA_SECOC_EXAMPLE_UUID;
    uint32_t origin;

    res = TEEC_OpenSession(&(secoc_obj->ctx), &(secoc_obj->session), &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    if(res != TEEC_SUCCESS){
        printf("result : 0x%x / error : 0x%x\n", res, origin);
        return -1;
    }

    return 0;
}

int store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness){
    TEEC_Operation op = {0};
    TEEC_Result res;

    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = (void*)alias;
    op.params[0].tmpref.size = strlen(alias);
    op.params[1].value.a = freshness;
    op.params[1].value.b = 0;

    uint32_t origin;
    res = TEEC_InvokeCommand(&(secoc_obj->session), TA_STORE_FRESHNESS, &op, &origin);
    if(res != TEEC_SUCCESS){
        printf("store_freshness : TEEC_InvokeCommand failed 0x%x origin 0x%x\n", res, origin);
        return -1;
    }
    return 0;
}

TEEC_Result load_freshness(TeeSecOC* secoc_obj, const char* alias, uint32_t* freshness){
    TEEC_Result res;
    TEEC_Operation op = {0};

    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = (void*)alias;
    op.params[0].tmpref.size = strlen(alias);
    op.params[1].value.a = 0;
    op.params[1].value.b = 0;

    uint32_t origin;
    res = TEEC_InvokeCommand(&(secoc_obj->session), TA_LOAD_FRESHNESS, &op, &origin);
    if(res != TEEC_SUCCESS){
        printf("load_freshness : TEEC_InvokeCommand failed 0x%x origin 0x%x\n", res, origin);
        return res;
    }

    *freshness = op.params[1].value.a;
    return res;
}

int delete_object(TeeSecOC* secoc_obj, const char* alias){
    TEEC_Result res;
    TEEC_Operation op = {0};

    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = (void*)alias;
    op.params[0].tmpref.size = strlen(alias);

    uint32_t origin;
    res = TEEC_InvokeCommand(&(secoc_obj->session), TA_DELETE_OBJECT, &op, &origin);
    if(res != TEEC_SUCCESS){
        printf("delete_freshness : TEEC_InvokeCommand failed 0x%x origin 0x%x\n", res, origin);
        return -1;
    }

    return 0;
}

int loadOrGenFreshness(TeeSecOC* secoc_obj, const char* alias, uint32_t* freshness){
    TEEC_Result res = load_freshness(secoc_obj, alias, freshness);
    if(res == TEEC_ERROR_ITEM_NOT_FOUND){
        *freshness = 0;
        return store_freshness(secoc_obj, alias, *freshness);
    }

    if(res != TEEC_SUCCESS)
        return -1;
    return 0;
}

TEEC_Result load_key(TeeSecOC* secoc_obj, const char* alias){
    TEEC_Result res;
    TEEC_Operation op = {0};

    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = (void*)alias;
    op.params[0].tmpref.size = strlen(alias);

    uint32_t origin;
    res = TEEC_InvokeCommand(&(secoc_obj->session), TA_LOAD_AES_KEY, &op, &origin);
    if(res != TEEC_SUCCESS)
        printf("load_key : TEEC_InvokeCommand failed 0x%x origin 0x%x\n", res, origin);
    
    return res;
}

int store_key(TeeSecOC* secoc_obj, const char* alias, const uint8_t key[KEY_SIZE]){
    TEEC_Result res;
    TEEC_Operation op = {0};

    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = (void*)alias;
    op.params[0].tmpref.size = strlen(alias);
    op.params[1].tmpref.buffer = (void*)key;
    op.params[1].tmpref.size = KEY_SIZE;

    uint32_t origin;
    res = TEEC_InvokeCommand(&(secoc_obj->session), TA_STORE_AES_KEY, &op, &origin);
    if(res != TEEC_SUCCESS){
        printf("store_key : TEEC_InvokeCommand failed 0x%x origin 0x%x\n", res, origin);
        return -1;
    }
    
    return 0;
}

int loadOrInitKey(TeeSecOC* secoc_obj, const char* alias){
    TEEC_Result res = load_key(secoc_obj, alias);
    if(res == TEEC_ERROR_ITEM_NOT_FOUND){
        uint8_t init_key[KEY_SIZE] = {0};
        return store_key(secoc_obj, alias, init_key);
    }
    
    if(res != TEEC_SUCCESS)
        return -1;
    return 0;
}

int gen_aes_mac(TeeSecOC* secoc_obj, TEEC_SharedMemory* in, const char* alias, uint8_t aes_cmac[CMAC_SIZE]){
    TEEC_Result res;
    TEEC_Operation op = {0};

    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
    op.params[0].memref.parent = in;
    op.params[1].tmpref.buffer = (void*)alias;
    op.params[1].tmpref.size = strlen(alias);
    op.params[2].tmpref.buffer = aes_cmac;
    op.params[2].tmpref.size = CMAC_SIZE;

    uint32_t origin;

    res = TEEC_InvokeCommand(&(secoc_obj->session), TA_GENERATE_AES_CMAC, &op, &origin);

    if(res != TEEC_SUCCESS){
        printf("TEEC_InvokeCommand failed 0x%08x\n", res);
        return -1;
    }

    return 0;
}