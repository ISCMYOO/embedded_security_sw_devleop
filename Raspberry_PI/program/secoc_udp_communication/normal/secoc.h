#include <stdint.h> 
#include "common.h"

#define SECOC_MAC_SIZE 16

typedef struct {
    uint32_t freshness;
    uint8_t service;
    uint16_t payload_len;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t mac[SECOC_MAC_SIZE];
} secoc_pdu_t;

typedef struct{
	uint8_t key[KEY_SIZE];
	uint32_t freshness;
} secoc_ctx;

int secoc_load_freshness(secoc_ctx* secoc_obj);

int secoc_store_freshness(secoc_ctx* secoc_obj);

int secoc_load_key(secoc_ctx* secoc_obj);

int secoc_gen_key(secoc_ctx* secoc_obj);

void secoc_protect(secoc_ctx* secoc_obj, secoc_pdu_t* pdu);

void secoc_replay(secoc_ctx* secoc_obj, secoc_pdu_t* pdu);
