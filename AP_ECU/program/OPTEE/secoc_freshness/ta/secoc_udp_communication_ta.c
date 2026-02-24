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
    ctx->data_loaded = false;

    *sess_ctx = ctx;
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ctx){
    ta_ctx_t* ctx = (ta_ctx_t*)sess_ctx;
    TEE_Free(ctx);
}

TEE_Result create_Freshness(const char* alias, TEE_ObjectHandle* object){
    TEE_Result res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias),
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_OVERWRITE,
                        TEE_HANDLE_NULL, NULL, 0, object);
    
    ta_persist_t init_persist;
    init_persist.freshness = 0;

    if(res != TEE_SUCCESS){
        IMSG("Create Freshness failed alias : %s", alias);
        return res;
    }
    
    IMSG("Crate Freshness alias : %s", alias);

    res = TEE_WriteObjectData(*object, &init_persist, sizeof(ta_persist_t));
    if(res != TEE_SUCCESS)
        IMSG("Init Freshness failed alias : %s", alias);
    
    IMSG("Init Freshness alias : %s", alias);

    return res;
}

TEE_Result save_Freshness(const char* alias, const ta_ctx_t* ctx_obj){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias),
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE, &object);

    if(res == TEE_ERROR_ITEM_NOT_FOUND){
        res = create_Freshness(alias, &object);
        if(res != TEE_SUCCESS)
            goto EXIT_ERROR;
    }else if(res != TEE_SUCCESS){
        goto EXIT_ERROR;
    }

    res = TEE_WriteObjectData(object, &(ctx_obj->persist), sizeof(ta_persist_t));
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    IMSG("Save Freshness alias : %s", alias);
    TEE_CloseObject(object);
    return res;

EXIT_ERROR:
    EMSG("save Freshness failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result load_Freshness(const char* alias, ta_ctx_t* ctx_obj){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias), 
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_SHARE_READ,
                        &object);

    if(res == TEE_ERROR_ITEM_NOT_FOUND){
        res = create_Freshness(alias, &object);
        if(res != TEE_SUCCESS)
            goto EXIT_ERROR;
    }else if(res != TEE_SUCCESS){
        goto EXIT_ERROR;
    }
    
    TEE_ObjectInfo objectInfo;
    res = TEE_GetObjectInfo1(object, &objectInfo);
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    uint32_t readBytes;
    res = TEE_ReadObjectData(object, &(ctx_obj->persist), objectInfo.dataSize, &readBytes);
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

    
    if(res == TEE_ERROR_ITEM_NOT_FOUND){
        IMSG("%s is not defined", alias);
        return TEE_SUCCESS;
    }else if(res != TEE_SUCCESS)
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

    char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
    TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
    alias[params[0].memref.size] = '\0';

    if(commandID == TA_STORE_FRESHNESS){
        uint32_t freshness = params[1].value.a;
        uint32_t freshness_tmp = ctx->persist.freshness;

        ctx->persist.freshness = freshness;

        TEE_Result res = save_Freshness(alias, ctx);
        TEE_Free(alias);

        if(res != TEE_SUCCESS)  ctx->persist.freshness = freshness_tmp;
        ctx->data_loaded = true;

        return res;
    }else if(commandID == TA_LOAD_FRESHNESS){
        TEE_Result res = load_Freshness(alias, ctx);
        TEE_Free(alias);

        if(res == TEE_SUCCESS)
            ctx->data_loaded = true;
        
        return res;
    }else if(commandID == TA_DELETE_FRESHNESS){
        TEE_Result res = delete_Freshness(alias);
        TEE_Free(alias);

        if(res == TEE_SUCCESS){
            TEE_MemFill(ctx, 0, sizeof(ta_ctx_t));
            ctx->data_loaded =false;
        }

        return res;
    }else if(commandID == TA_CHECK_FRESHNESS){
        TEE_Result res;
        if(!ctx->data_loaded){
            res = load_Freshness(alias, ctx);
            if(res != TEE_SUCCESS){
                TEE_Free(alias);
                return res; 
            }
        }

        uint32_t pdu_freshness = params[1].value.a;

        if(pdu_freshness < ctx->persist.freshness){
            TEE_Free(alias);
            IMSG("SECOC verify failed : Replay Attack");
            IMSG("sender freshness : %d / receiver freshness : %d", pdu_freshness, ctx->persist.freshness);
            return TEE_SUCCESS;
        }

        params[2].value.a = 1;
        ctx->persist.freshness = pdu_freshness;

        if(pdu_freshness % 10 == 0){
            res = save_Freshness(alias, ctx);
            if(res != TEE_SUCCESS){
                TEE_Free(alias);
                return res;
            }
        }

        TEE_Free(alias);
        return TEE_SUCCESS;
    }else if(commandID == TA_READ_FRESHNESS){
        if(!ctx->data_loaded){
            TEE_Result res = load_Freshness(alias, ctx);
            if(res != TEE_SUCCESS){
                TEE_Free(alias);
                return res;
            }
        }
        IMSG("ctx freshness : %d", ctx->persist.freshness);
        TEE_Free(alias);

        return TEE_SUCCESS;
    }

    return TEE_ERROR_BAD_PARAMETERS;
}