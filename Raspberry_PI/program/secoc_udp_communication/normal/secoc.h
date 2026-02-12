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

uint32_t secoc_load_freshness();

void secoc_store_freshness(uint32_t freshness);

void secoc_init_freshness();

int secoc_protect(secoc_pdu_t* pdu);

void secoc_replay(secoc_pdu_t* pdu);
