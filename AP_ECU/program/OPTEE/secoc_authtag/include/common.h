#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define TA_SECOC_EXAMPLE_UUID { 0x33333333, 0x3333, 0x3333, {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33} }

#define KEY_SIZE                16
#define MAX_PAYLOAD             256
#define SECOC_MAC_SIZE          16

typedef struct {
    uint32_t buffer_len;
    uint8_t buffer[MAX_PAYLOAD];
} MacParams;

#define TA_STORE_FRESHNESS      0
#define TA_LOAD_FRESHNESS       1
#define TA_DELETE_OBJECT        2
#define TA_CHECK_FRESHNESS      3
#define TA_READ_FRESHNESS       4
#define TA_GENERATE_AES_CMAC    5
#define TA_STORE_AES_KEY        6
#define TA_LOAD_AES_KEY         7
#define TA_READ_AES_KEY         8

void logHex(const uint8_t *hex, size_t size);