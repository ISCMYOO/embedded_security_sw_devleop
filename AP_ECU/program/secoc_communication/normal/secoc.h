#ifndef SECOC_H
#define SECOC_H

#include <stdint.h>

#define MAX_PAYLOAD 256
#define SECOC_MAC_SIZE 16

extern uint32_t freshness;

typedef struct {
    uint32_t freshness; // 송신 카운터
    uint8_t service; // 제공되는 기능 번호
    uint16_t payload_len;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t mac[SECOC_MAC_SIZE];
} secoc_pdu_t;

int secoc_verify(secoc_pdu_t *pdu);

#endif