#pragma once

typedef struct __attribute__((packed))
{
    uint32_t aadSize;
    uint32_t plainSize;
	unsigned char buffer[]; // [AAD][Plain]
} EncryptParams;

typedef struct __attribute__((packed))
{
    uint32_t aadSize;
    uint32_t cipherSize;
    uint32_t ivSize;
    uint32_t tagSize;
	unsigned char buffer[]; // [AAD][Cipher][IV][TAG]
} DecryptParams;

#define TA_AES_EXAMPLE_UUID	{ 0x11111111, 0x1111, 0x1111, { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 } }

#define TA_CMD_GENERATE_KEY		0
#define TA_CMD_LOAD_KEY			1
#define TA_CMD_AES_ENCRYPT		2
#define TA_CMD_AES_DECRYPT		3