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

TEE_Result create_obj(const char* alias, TEE_ObjectHandle* object){
    TEE_Result res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias),
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_OVERWRITE,
                        TEE_HANDLE_NULL, NULL, 0, object);
    
    ta_persist_t init_persist;
    init_persist.freshness = 0;

    if(res != TEE_SUCCESS){
        IMSG("Create object failed alias : %s", alias);
        return res;
    }
    
    IMSG("Crate object alias : %s", alias);

    res = TEE_WriteObjectData(*object, &init_persist, sizeof(ta_persist_t));
    if(res != TEE_SUCCESS)
        IMSG("Init object failed alias : %s", alias);
    
    IMSG("Init object alias : %s", alias);

    return res;
}

TEE_Result save_obj(const char* alias, const ta_ctx_t* ctx_obj){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias),
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE, &object);

    if(res == TEE_ERROR_ITEM_NOT_FOUND){
        res = create_obj(alias, &object);
        if(res != TEE_SUCCESS)
            goto EXIT_ERROR;
    }else if(res != TEE_SUCCESS){
        goto EXIT_ERROR;
    }

    res = TEE_WriteObjectData(object, &(ctx_obj->persist), sizeof(ta_persist_t));
    if(res != TEE_SUCCESS)
        goto EXIT_ERROR;

    IMSG("Save object alias : %s", alias);
    TEE_CloseObject(object);
    return res;

EXIT_ERROR:
    EMSG("save object failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result load_obj(const char* alias, ta_ctx_t* ctx_obj){
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, alias, strlen(alias), 
                        TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_SHARE_READ,
                        &object);

    if(res == TEE_ERROR_ITEM_NOT_FOUND){
        res = create_obj(alias, &object);
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
    
    IMSG("Load object alias : %s", alias);
    
    TEE_CloseObject(object);
    return res;

EXIT_ERROR:
    EMSG("load object failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

TEE_Result delete_obj(const char* alias){
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
    EMSG("delete object failed 0x%08x", res);
    if(object != TEE_HANDLE_NULL)
        TEE_CloseObject(object);
    return res;
}

bool setOperation(TEE_OperationHandle* opHandle, TEE_OperationMode opMode, const uint8_t* key){
    TEE_Result res;
    bool result = false;
    TEE_ObjectHandle keyHandle = TEE_HANDLE_NULL;
    TEE_Attribute attr;

    res = TEE_AllocateOperation(opHandle, TEE_ALG_AES_CMAC, opMode, KEY_SIZE * 8);
    if(res != TEE_SUCCESS){
        EMSG("TEE_AllocateOperation failed 0x%08x", res);
        return result;
    }

    res = TEE_AllocateTransientObject(TEE_TYPE_AES, KEY_SIZE * 8, &keyHandle);
    if(res != TEE_SUCCESS){
        EMSG("TEE_AllocateTransientObject failed 0x%08x", res);
        goto EXIT_ERROR;
    }

    TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key, KEY_SIZE);

    res = TEE_PopulateTransientObject(keyHandle, &attr, 1);
    if(res != TEE_SUCCESS){
        EMSG("TEE_PopulateTransientObject failed 0x%08x", res);
        goto EXIT_ERROR;
    }

    res = TEE_SetOperationKey(*opHandle, keyHandle);
    if(res != TEE_SUCCESS){
        EMSG("TEE_SetOperationKey failed 0x%08x", res);
        goto EXIT_ERROR;
    }
    result = true;

EXIT_ERROR:
    if(keyHandle != TEE_HANDLE_NULL)
        TEE_FreeTransientObject(keyHandle);
    
    return result;
}

// 여기 수정 필요 params[0]는 항상 alias로 받는 걸로
bool gen_aes_mac(TEE_Param params[4], TEE_OperationHandle op){
    TEE_Result res;

    MacParams* tmp_params = (MacParams*)params[1].memref.buffer;
    
    TEE_MACInit(op, NULL, 0);
    TEE_MACUpdate(op, tmp_params->buffer, tmp_params->buffer_len);
    res = TEE_MACComputeFinal(op, NULL, 0, params[2].memref.buffer, &params[2].memref.size);

    if(res != TEE_SUCCESS){
        EMSG("TEE_MACComputeFinal failed 0x%08x", res);
        return false;
    }

    return true;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t commandID, uint32_t __maybe_unused paramTypes, TEE_Param params[4]){
    ta_ctx_t* ctx = (ta_ctx_t*)sess_ctx;

    TEE_Result res;

    char* alias = TEE_Malloc(params[0].memref.size + 1, 0);
    TEE_MemMove(alias, params[0].memref.buffer, params[0].memref.size);
    alias[params[0].memref.size] = '\0';

    if(commandID == TA_STORE_FRESHNESS){
        uint32_t freshness = params[1].value.a;
        uint32_t freshness_tmp = ctx->persist.freshness;

        ctx->persist.freshness = freshness;

        res = save_obj(alias, ctx);
        if(res != TEE_SUCCESS)  ctx->persist.freshness = freshness_tmp;
        ctx->data_loaded = true;

        goto EXIT;
    }else if(commandID == TA_LOAD_FRESHNESS){
        res = load_obj(alias, ctx);
        if(res == TEE_SUCCESS)
            ctx->data_loaded = true;
        
        goto EXIT;
    }else if(commandID == TA_DELETE_OBJECT){
        res = delete_obj(alias);
        if(res == TEE_SUCCESS){
            TEE_MemFill(ctx, 0, sizeof(ta_ctx_t));
            ctx->data_loaded =false;
        }

        goto EXIT;
    }else if(commandID == TA_CHECK_FRESHNESS){
        if(!ctx->data_loaded){
            res = load_obj(alias, ctx);
            if(res != TEE_SUCCESS)
                goto EXIT;
        }

        uint32_t pdu_freshness = params[1].value.a;

        if(pdu_freshness < ctx->persist.freshness){
            IMSG("SECOC verify failed : Replay Attack");
            IMSG("sender freshness : %d / receiver freshness : %d", pdu_freshness, ctx->persist.freshness);
            res = TEE_SUCCESS;
            goto EXIT;
        }

        params[2].value.a = 1;
        ctx->persist.freshness = pdu_freshness;

        if(pdu_freshness % 10 == 0){
            res = save_obj(alias, ctx);
            if(res != TEE_SUCCESS)
                goto EXIT;
        }

        res = TEE_SUCCESS;
        goto EXIT;
    }else if(commandID == TA_READ_FRESHNESS){
        if(!ctx->data_loaded){
            res = load_obj(alias, ctx);
            if(res != TEE_SUCCESS)
                goto EXIT;
        }
        IMSG("ctx freshness : %d", ctx->persist.freshness);
        
        res = TEE_SUCCESS;
        goto EXIT;
    }else if(commandID == TA_LOAD_AES_KEY){
        res = load_obj(alias, ctx);
        if(res == TEE_SUCCESS)
            ctx->data_loaded = true;
        
        goto EXIT;
    }else if(commandID == TA_STORE_AES_KEY){
        uint8_t tmp_key[KEY_SIZE];
        TEE_MemMove(tmp_key, ctx->persist.key, KEY_SIZE);
        TEE_MemMove(ctx->persist.key, params[1].memref.buffer, KEY_SIZE);

        res = save_obj(alias, ctx);
        if(res != TEE_SUCCESS)
            TEE_MemMove(ctx->persist.key, tmp_key, KEY_SIZE);

        goto EXIT;
    }else if(commandID == TA_READ_AES_KEY){
        if(!ctx->data_loaded){
            res = load_obj(alias, ctx);
            if(res != TEE_SUCCESS)
                goto EXIT;
        }

        printf("%s Key : ", alias);
        logHex(ctx->persist.key, KEY_SIZE);

        res = TEE_SUCCESS;
        goto EXIT;
    }else if(commandID == TA_GENERATE_AES_CMAC){
        TEE_OperationHandle opHandle = TEE_HANDLE_NULL;
        bool result;

        if(ctx->data_loaded){
            res = load_obj(alias, ctx);
            if(res != TEE_SUCCESS)
                goto EXIT;
        }

        result = setOperation(&opHandle, TEE_MODE_MAC, ctx->persist.key);
        if(!result){
            if(opHandle != TEE_HANDLE_NULL)
                TEE_FreeOperation(opHandle);
            res = TEE_ERROR_GENERIC;
            goto EXIT;
        }

        result = gen_aes_mac(params, opHandle);
        if(!result){
            if(opHandle != TEE_HANDLE_NULL)
                TEE_FreeOperation(opHandle);
            res = TEE_ERROR_GENERIC;
            goto EXIT;
        }

        TEE_FreeOperation(opHandle);
        res = TEE_SUCCESS;
        goto EXIT;
    }

    return TEE_ERROR_BAD_PARAMETERS;

EXIT:
    TEE_Free(alias);
    return res;
}