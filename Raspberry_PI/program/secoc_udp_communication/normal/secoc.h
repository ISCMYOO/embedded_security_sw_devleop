#include "common.h"
#include <openssl/cmac.h>

#define SECOC_MAC_SIZE 16

typedef struct {
    uint32_t freshness;
    uint8_t service;
    uint32_t payload_len;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t mac[SECOC_MAC_SIZE];
} secoc_pdu_t;

typedef struct{
    uint8_t key[KEY_SIZE];
    uint32_t freshness;
} secoc_persist_t;

typedef struct{
    secoc_persist_t persist;
    bool data_loaded;
} secoc_ctx;

void secoc_init_obj(secoc_ctx* secoc_obj);

int secoc_load_obj(secoc_ctx* secoc_obj);

int secoc_store_obj(secoc_ctx* secoc_obj);

void secoc_read_obj(secoc_ctx* secoc_obj);

void secoc_delete_obj(secoc_ctx* secoc_obj);

int secoc_gen_key(secoc_ctx* secoc_obj);

int secoc_gen_mac(secoc_ctx* secoc_obj, const uint8_t payload[MAX_PAYLOAD], uint16_t paylod_len);

void secoc_protect(secoc_ctx* secoc_obj, secoc_pdu_t* pdu);

void secoc_replay(secoc_ctx* secoc_obj, secoc_pdu_t* pdu);
