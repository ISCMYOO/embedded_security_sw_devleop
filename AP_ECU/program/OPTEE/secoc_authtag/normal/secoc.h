#ifndef SECOC_H
#define SECOC_H

#include "ta_api.h"

extern uint32_t freshness;

typedef struct {
    uint32_t freshness; // 송신 카운터
    uint8_t service; // 제공되는 기능 번호
    uint32_t payload_len;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t mac[SECOC_MAC_SIZE];
} secoc_pdu_t;

int secoc_verify(TeeSecOC* secoc_obj, secoc_pdu_t *pdu, const char* alias);

int secoc_load_key(TeeSecOC* secoc_obj, const char* alias);

int secoc_store_key(TeeSecOC* secoc_obj, const char* alias, const uint8_t* key, uint32_t key_len);

int secoc_load_freshness(TeeSecOC* secoc_obj, const char* alias);

int secoc_read_freshness(TeeSecOC* secoc_obj, const char* alias);

int secoc_store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness_in);

int secoc_delete_freshness(TeeSecOC* secoc_obj, const char* alias);

int secoc_gen_aes_mac(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias);

#endif