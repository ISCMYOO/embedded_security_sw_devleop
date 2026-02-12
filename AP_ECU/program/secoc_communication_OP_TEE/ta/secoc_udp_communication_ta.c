#include "secoc_udp_communication_ta.h"

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

bool save_Freshness(const char* alias, const uint32_t freshness){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias), 
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_OVERWRITE,
                        TEE_HANDLE_NULL, NULL, 0, &object);
    
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    res = TEE_WriteObjectData(object, &freshness, 4);
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    IMSG("Save Freshness alias : %s", alias);
    TEE_CloseObject(object);
    return true;

EXIT_ERROR:
    EMSG("save Freshness failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return false;
}

TEE_Result load_Freshness(const char* alias, uint32_t* freshness){
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
    res = TEE_ReadObjectData(object, freshness, objectInfo.dataSize, &readBytes);
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;
    
    IMSG("Load Freshness alias : %s", alias);
    
    TEE_CloseObject(object);
    return res;

EXIT_ERROR:
    EMSG("load Freshness failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result delete_Freshness(const char* alias){
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

        bool success = save_Freshness(alias, freshness);
        TEE_Free(alias);

        if(success){
            ctx->freshness = freshness;
            return TEE_SUCCESS;
        }
        
        return TEE_ERROR_GENERIC;
    }else if(commandID == TA_LOAD_FRESHNESS){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        uint32_t freshness;
        TEE_Result res = load_Freshness(alias, &freshness);
        TEE_Free(alias);

        if(res == TEE_SUCCESS){
            ctx->freshness = freshness;
            params[1].value.a = freshness;
            params[1].value.b = 0;
        }

        return res;
    }else if(commandID == TA_DELETE_FRESHNESS){
        char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
        TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
        alias[params[0].memref.size] = '\0';

        TEE_Result res = delete_Freshness(alias);
        TEE_Free(alias);

        return res;
    }

    return TEE_ERROR_BAD_PARAMETERS;
}