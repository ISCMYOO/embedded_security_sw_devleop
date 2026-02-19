#include "aes.h"

int init(TeeAes* aes_obj){
	TEEC_Result res;
	
	res = TEEC_InitializeContext(NULL, &(aes_obj->ctx));
	if(res != TEEC_SUCCESS){
		printf("result : 0x%x\n", res);
		return 0;
	}

	TEEC_UUID uuid = TA_AES_EXAMPLE_UUID;
	uint32_t origin;

	res = TEEC_OpenSession(&(aes_obj->ctx), &(aes_obj->session), &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
	if(res != TEEC_SUCCESS){
		printf("result : 0x%x / error : 0x%x\n", res, origin);
		return 0;
	}

	return 1;
}

TEEC_Result loadkey(TeeAes* aes_obj, const char* alias){
	TEEC_Operation op = {0};

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = (void*)alias;
	op.params[0].tmpref.size = strlen(alias);

	uint32_t origin;
	return TEEC_InvokeCommand(&(aes_obj->session), TA_CMD_LOAD_KEY, &op, &origin);
}

int generateKey(TeeAes* aes_obj, const char* alias){
	TEEC_Operation op = {0};

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = (void*)alias;
	op.params[0].tmpref.size = strlen(alias);

	uint32_t origin;
	TEEC_Result res = TEEC_InvokeCommand(&(aes_obj->session), TA_CMD_GENERATE_KEY, &op, &origin);
	if(res != TEEC_SUCCESS){
		printf("generateKey : TEEC_InvokeCommand failed 0x%x origin 0x%x", res, origin);
			return 0;
	}

	return 1;
}

int loadOrGenKey(TeeAes* aes_obj, const char* alias){
	TEEC_Result res = loadkey(aes_obj, alias);
	if(res == TEEC_ERROR_ITEM_NOT_FOUND){
		return generateKey(aes_obj, alias);
	}
	return (res == TEEC_SUCCESS);
}

EncryptData encrypt(TeeAes* aes_obj, const uint8_t* plain, uint32_t plain_len, const uint8_t* aad, uint32_t aad_len){
	EncryptData encData = {0};
	encData.has_value = 0;

	TEEC_SharedMemory in;
	in.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	in.size = sizeof(EncryptParams) + 4 * 2; // aadSize(4bytes), plainSize(4bytes)
	
	if(TEEC_AllocateSharedMemory(&(aes_obj->ctx), &in) != TEEC_SUCCESS){
		printf("TEEC_AllocatedSharedMemory failed\n");
		return encData;
	}
	memset(in.buffer, 0, in.size);

	EncryptParams* params = (EncryptParams*)in.buffer;
	params->aadSize = aad_len;
	params->plainSize = plain_len;
	
	memcpy(params->buffer, aad, aad_len);
	memcpy(params->buffer + aad_len, plain, plain_len);

	uint8_t* cipher = malloc(plain_len);
	uint8_t iv[12];
	uint8_t tag[16];

	TEEC_Operation op = {0, };
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT);
	op.params[0].memref.parent = &in;
	op.params[1].tmpref.buffer = cipher;
	op.params[1].tmpref.size = plain_len;
	op.params[2].tmpref.buffer = iv;
	op.params[2].tmpref.size = sizeof(iv);
	op.params[3].tmpref.buffer = tag;
	op.params[3].tmpref.size = sizeof(tag);

	uint32_t origin;
	if(TEEC_InvokeCommand(&(aes_obj->session), TA_CMD_AES_ENCRYPT, &op, &origin) != TEEC_SUCCESS){
		printf("TEEC_InvokeCommand failed\n");
		TEEC_ReleaseSharedMemory(&in);
		free(cipher);
		return encData;
	}

	TEEC_ReleaseSharedMemory(&in);

	encData.cipher = cipher;
	encData.cipher_len = plain_len;
	memcpy(encData.iv, iv, 12);
	memcpy(encData.tag, tag, 16);
	encData.has_value = 1;

	return encData;
}

uint8_t* decrypt(TeeAes* aes_obj,const uint8_t* cipher, uint32_t cipher_len, const uint8_t* iv, const uint8_t* aad, uint32_t aad_len, const uint8_t* tag){
	TEEC_SharedMemory in;
	
	in.size = sizeof(DecryptParams) + 4 * 4; // aadSize(4bytes), cipherSize(4bytes), ivSize(4bytes), tagSize(4bytes)
	in.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	if(TEEC_AllocateSharedMemory(&(aes_obj->ctx), &in) != TEEC_SUCCESS){
		printf("TEEC_AllocateSharedMemory failed\n");
		return NULL;
	}
	memset(in.buffer, 0, in.size);

	DecryptParams* params = (DecryptParams*)in.buffer;
	params->aadSize = aad_len;
	params->cipherSize = cipher_len;
	params->ivSize = 12;
	params->tagSize = 16;

	memcpy(params->buffer, aad, aad_len);
	memcpy(params->buffer + aad_len, cipher, cipher_len);
	memcpy(params->buffer + aad_len + cipher_len, iv, 12);
	memcpy(params->buffer + aad_len + cipher_len + 12, tag, 16);

	uint8_t* plain = (uint8_t*)malloc(cipher_len);

	TEEC_Operation op = {0};
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE, TEEC_NONE);

	op.params[0].memref.parent = &in;
	op.params[1].tmpref.buffer = plain;
	op.params[1].tmpref.size = cipher_len;

	uint32_t origin;
	TEEC_Result res = TEEC_InvokeCommand(&(aes_obj->session), TA_CMD_AES_DECRYPT, &op, &origin);
	TEEC_ReleaseSharedMemory(&in);

	if(res != TEEC_SUCCESS){
		printf("TEEC_InvokeCommand failed 0x%x origin 0x%x\n", res, origin);
		free(plain);
		return NULL;
	}
	
	return plain;
}

void TeeAes_final(TeeAes* aes_obj){
	TEEC_CloseSession(&(aes_obj->session));
	TEEC_FinalizeContext(&(aes_obj->ctx));
}