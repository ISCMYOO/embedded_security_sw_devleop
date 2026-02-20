#pragma once

#define TA_SECOC_EXAMPLE_UUID { 0x22222222, 0x2222, 0x2222, {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22} }

#define CMAC_SIZE               16
#define KEY_SIZE                16

#define TA_STORE_FRESHNESS      0
#define TA_LOAD_FRESHNESS       1
#define TA_DELETE_OBJECT        2
#define TA_GENERATE_AES_CMAC    3
#define TA_LOAD_AES_KEY         4
#define TA_STORE_AES_KEY        5
