#include "secoc.h"

int secoc_verify(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias){
    // check freshness
    if(!check_freshness(secoc_obj, alias, pdu->freshness)){
        return -1; // Replay
    }

    // MAC 검증

    return 0;
}

int secoc_load_key(TeeSecOC* secoc_obj, const char* alias){
    return load_key(secoc_obj, alias);
}

int secoc_store_key(TeeSecOC* secoc_obj, const char* alias, const uint8_t* key, uint32_t key_len){
    if(key_len != KEY_SIZE){
        printf("set key value is not match AES 128\n");
        return -1;
    }

    return store_key(secoc_obj, alias, key);
}

int secoc_read_key(TeeSecOC* secoc_obj, const char* alias){
    return read_key(secoc_obj, alias);
}

int secoc_load_freshness(TeeSecOC* secoc_obj, const char* alias){
    return load_freshness(secoc_obj, alias);
}

int secoc_read_freshness(TeeSecOC* secoc_obj, const char* alias){
    read_freshness(secoc_obj, alias);
    return 0;
}

int secoc_store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness_in){
    return store_freshness(secoc_obj, alias, freshness_in);
}

int secoc_delete_object(TeeSecOC* secoc_obj, const char* alias){
    return delete_object(secoc_obj, alias);
}

int secoc_gen_aes_mac(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias){
    uint8_t aes_cmac[SECOC_MAC_SIZE] = {0};
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
    mac_params->buffer[pdu->payload_len]     = (pdu->freshness >> 24) & 0xFF;
    mac_params->buffer[pdu->payload_len + 1] = (pdu->freshness >> 16) & 0xFF;
    mac_params->buffer[pdu->payload_len + 2] = (pdu->freshness >> 8)  & 0xFF;
    mac_params->buffer[pdu->payload_len + 3] = (pdu->freshness)       & 0xFF;
    
    result = gen_aes_mac(secoc_obj, &in, alias, aes_cmac);
    TEEC_ReleaseSharedMemory(&in);

    printf("MAC : ");
    logHex(aes_cmac, SECOC_MAC_SIZE);

    return result;
}