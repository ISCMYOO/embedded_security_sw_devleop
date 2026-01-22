#include <stdio.h>
#include "aes.h"

void logHex(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
        if ((i+1) % 16 == 0) printf("\n");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void){
	TeeAes aes_enc_obj;
	TeeAes aes_dec_obj;

	if(init(&aes_enc_obj) == false){
		return 0;
	}

	char* seed = "alias0001";
	if(loadOrGenKey(&aes_enc_obj, seed) == false){
		return 0;
	}

	const char* str = "hello";
	size_t str_len = strlen(str);
	
	uint8_t* plain = malloc(str_len);
	memcpy(plain, str, str_len);

	const char* str2 = "world";
	size_t str2_len = strlen(str2);

	uint8_t* aad = malloc(str2_len);
	memcpy(aad, str2, str2_len);

	EncryptData encData  = encrypt(&aes_enc_obj, plain, str_len, aad, str2_len);
	if(encData.has_value == false){
		printf("encrypt failed\n");
		free(plain);
		free(aad);
		return 0;
	}
	
	printf("plain : ");
	logHex(plain, str_len);
	printf("cipher : ");
	logHex(encData.cipher, encData.cipher_len);
	printf("iv : ");
	logHex(encData.iv, 12);
	printf("tag : ");
	logHex(encData.tag, 16);
	TeeAes_final(&aes_enc_obj);


	if(init(&aes_dec_obj) == false){
		return 0;
	}

	if(loadOrGenKey(&aes_dec_obj, seed) == false){
		return 0;
	}

	uint8_t* decData = decrypt(&aes_dec_obj, encData.cipher, encData.cipher_len, encData.iv, aad, str2_len, encData.tag);
	if(decData == NULL){
		printf("decrypt failed\n");
		free(plain);
		free(aad);
		free(encData.cipher);
		TeeAes_final(&aes_dec_obj);
		return 0;
	}

	printf("plain : ");
	logHex((unsigned char*)decData, encData.cipher_len);

	free(plain);
	free(aad);
	free(decData);
	free(encData.cipher);
	TeeAes_final(&aes_dec_obj);

	return 0;
}