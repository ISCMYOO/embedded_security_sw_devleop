#include "secoc.h"

uint32_t freshness = 0;

int secoc_verify(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias){
    // check freshness
    if(pdu->freshness <= freshness){
        return -1; // Replay
    }

    // MAC 검증
    // uint8_t aes_cmac[CMAC_SIZE];
    // generate_aes_cmac(secoc_obj, alias, aes_cmac);
    // secoc_load_key(secoc_obj, alias);

    // 전역변수 freshness 갱신 
    freshness = pdu->freshness;

    // 마지막에 freshness 저장 조건 확인 후 저장하기, 10의 배수마다 저장
    if(freshness % 10 == 0 && store_freshness(secoc_obj, alias, freshness) == -1){
        printf("falied store freshness\n");
        return -1;
    }

    return 0;
}

int secoc_load_key(TeeSecOC* secoc_obj, const char* alias){
    return loadOrInitKey(secoc_obj, alias);
}

int secoc_store_key(TeeSecOC* secoc_obj, const char* alias, const uint8_t* key, uint32_t key_len){
    if(key_len != KEY_SIZE){
        printf("set key value is not match AES 128\n");
        return -1;
    }

    return store_key(secoc_obj, alias, key);
}

int secoc_load_freshness(TeeSecOC* secoc_obj, const char* alias){
    uint32_t tmp_freshness;
    // load freshness from RPMB(eMMC)
    if(loadOrGenFreshness(secoc_obj, alias, &tmp_freshness) == -1)
        return -1;
    
    freshness = tmp_freshness;

    return 0;
}

int secoc_read_freshness(TeeSecOC* secoc_obj, const char* alias){
    uint32_t ta_freshness;

    if(loadOrGenFreshness(secoc_obj, alias, &ta_freshness) == -1)
        return -1;
    
    printf("RAM Freshness : %d / TA Freshness : %d\n", freshness, ta_freshness);

    return 0;
}

int secoc_store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness_in){
    freshness = freshness_in;

    if(store_freshness(secoc_obj, alias, freshness_in) == -1)
        return -1;
    return 0;
}

int secoc_delete_object(TeeSecOC* secoc_obj, const char* alias){
    if(delete_object(secoc_obj, alias) == -1)
        return -1;
    
    freshness = 0;

    return 0;
}

int secoc_gen_aes_mac(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias){
    uint8_t aes_cmac[CMAC_SIZE] = {0};
    int result;
    TEEC_Result res;
    TEEC_SharedMemory in;

    in.flags = TEEC_MEM_INPUT;
    in.size = sizeof(MacParams);

    res = TEEC_AllocateSharedMemory(&(secoc_obj->ctx), &in);
    if(res != TEEC_SUCCESS){
        printf("TEEC_AllocatedSharedMemory failed 0x%08x\n", res);
        return -1;
    }

    memset(in.buffer, 0, sizeof(MacParams));

    MacParams* mac_params = (MacParams*)in.buffer;
    memcpy(mac_params->buffer, pdu->payload, pdu->payload_len);

    // payload || freshness
    mac_params->buffer_len = pdu->payload_len + sizeof(uint32_t);
    mac_params->buffer[pdu->payload_len] = (pdu->freshness >> 24) & 0xFF;
    mac_params->buffer[pdu->payload_len + 1] = (pdu->freshness >> 16) & 0xFF;
    mac_params->buffer[pdu->payload_len + 2] = (pdu->freshness >> 8) & 0xFF;
    mac_params->buffer[pdu->payload_len + 3] = (pdu->freshness) & 0xFF;
    
    result = gen_aes_mac(secoc_obj, &in, alias, aes_cmac);
    TEEC_ReleaseSharedMemory(&in);

    printf("MAC : ");
    logHex(aes_cmac, CMAC_SIZE);

    return result;
}