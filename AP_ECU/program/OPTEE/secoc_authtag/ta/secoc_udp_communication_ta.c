#include "secoc_udp_communication_ta.h"

void logHex(const uint8_t *hex, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", hex[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}


TEE_Result TA_CreateEntryPoint(void){
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void){}

TEE_Result TA_OpenSessionEntryPoint(uint32_t __unused paramTypes, TEE_Param __unused params[4], void __unused **sess_ctx){
    ta_ctx_t* ctx = TEE_Malloc(sizeof(ta_ctx_t), 0);
    if(!ctx)
        return TEE_ERROR_OUT_OF_MEMORY;
    
    TEE_MemFill(ctx, 0, sizeof(ta_ctx_t));

    *sess_ctx = ctx;
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ctx){
    ta_ctx_t* ctx = (ta_ctx_t*)sess_ctx;
    TEE_Free(ctx);
}

TEE_Result save_obj(const char* alias, const ta_ctx_t* ctx_obj){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias), 
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_OVERWRITE,
                        TEE_HANDLE_NULL, NULL, 0, &object);
    
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    res = TEE_WriteObjectData(object, ctx_obj, sizeof(ta_ctx_t));
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    IMSG("Save obj alias : %s", alias);
    TEE_CloseObject(object);
    return res;

EXIT_ERROR:
    EMSG("save obj failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result load_obj(const char* alias, ta_ctx_t* ctx_obj){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias), 
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_SHARE_READ,
                        &object);

    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;
    
    TEE_ObjectInfo objectInfo;
    res = TEE_GetObjectInfo1(object, &objectInfo);
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    uint32_t readBytes;
    res = TEE_ReadObjectData(object, ctx_obj, objectInfo.dataSize, &readBytes);
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;
    
    IMSG("Load obj alias : %s", alias);
    
    TEE_CloseObject(object);
    return res;

EXIT_ERROR:
    EMSG("load obj failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result delete_obj(const char* alias){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias),
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META,
                        &object);

    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;
    
    TEE_CloseAndDeletePersistentObject(object);

    IMSG("Delete alias : %s", alias);
    return res;

EXIT_ERROR:
    EMSG("delete Freshness failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t commandID, uint32_t __maybe_unused paramTypes, TEE_Param params[4]){
    ta_ctx_t* ctx = (ta_ctx_t*)sess_ctx;

    if(commandID == TA_STORE_FRESHNESS){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        uint32_t freshness = params[1].value.a;
        uint32_t freshness_tmp = ctx->freshness;

        ctx->freshness = freshness;

        TEE_Result res = save_obj(alias, ctx);
        TEE_Free(alias);

        if(res != TEE_SUCCESS)
            ctx->freshness = freshness_tmp;
        
        return res;
    }else if(commandID == TA_LOAD_FRESHNESS){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        TEE_Result res = load_obj(alias, ctx);
        TEE_Free(alias);

        if(res != TEE_SUCCESS)
            params[1].value.a = 0;
        else
            params[1].value.a = ctx->freshness;
        params[1].value.b = 0;
        
        return res;
    }else if(commandID == TA_DELETE_OBJECT){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        TEE_Result res = delete_obj(alias);
        TEE_Free(alias);

        return res;
    }else if(commandID == TA_LOAD_AES_KEY){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        TEE_Result res = load_obj(alias, ctx);
        TEE_Free(alias);

        if(res == TEE_SUCCESS){
            printf("Save Key Value : ");
            logHex(ctx->key, KEY_SIZE);
        }

        return res;
    }else if(commandID == TA_STORE_AES_KEY){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        uint8_t tmp_key[KEY_SIZE];
        TEE_MemMove(tmp_key, ctx->key, KEY_SIZE);
        
        TEE_MemMove(ctx->key, params[1].memref.buffer, KEY_SIZE);

        TEE_Result res = save_obj(alias, ctx);
        TEE_Free(alias);

        if(res != TEE_SUCCESS)
            TEE_MemMove(ctx->key, tmp_key, KEY_SIZE);
        else{
            printf("Save Key Value : ");
            logHex(ctx->key, KEY_SIZE);
        }

        return res;
    }
    
    // else if(commandID == TA_GENERATE_AES_CMAC){
    //     char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
    //     TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
    //     alias[params[0].memref.size] = '\0';

    //     TEE_Result res = load_key(alias, ctx);

    //     // TEE_Result res = generate_Aes_Mac(alias, params[1]);
    //     // TEE
    // }

    return TEE_ERROR_BAD_PARAMETERS;
}