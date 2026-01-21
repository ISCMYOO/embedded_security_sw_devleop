#include <inttypes.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <stdio.h>
#include "../include/common.h"


void logHex(const uint8_t *hex, size_t size);

bool saveKey(const char* alias, unsigned char* key);

TEE_Result loadKey(const char* alias, unsigned char* key);

bool setOperation(TEE_OperationHandle* opHandle, unsigned char* key, TEE_OperationMode opMode);

bool encrypt(TEE_Param params[4], TEE_OperationHandle opHandle);

bool decrypt(TEE_Param params[4], TEE_OperationHandle opHandle);