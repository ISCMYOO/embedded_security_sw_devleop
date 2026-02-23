#include "secoc.h"

void secoc_init_obj(secoc_ctx* secoc_obj){
	secoc_obj->persist.freshness = 0;
	memset(secoc_obj->persist.key, 0, KEY_SIZE);
	secoc_obj->data_loaded = false;
}

int secoc_load_obj(secoc_ctx* secoc_obj){
	FILE *f = fopen("./var/lib/secoc/freshness.bin", "rb");
	if(!f){
		printf("create freshness file first\n");
		return -1;
	}

	fread(&(secoc_obj->persist.freshness), sizeof(uint32_t), 1, f);
	fclose(f);

	f = fopen("./var/lib/secoc/key.bin", "rb");
	if(!f){
		printf("crate key file first\n");
		return -1;
	}

	fread(secoc_obj->persist.key, sizeof(uint8_t), KEY_SIZE, f);
	fclose(f);

	secoc_obj->data_loaded = true;

	printf("[OBJECT LOAD]\n");
	secoc_read_obj(secoc_obj);

	return 0;
}

int secoc_store_obj(secoc_ctx* secoc_obj){
	FILE *f = fopen("./var/lib/secoc/freshness.bin", "wb");
	if(!f){
		printf("create freshness file first\n");
		return -1;
	}

	fwrite(&(secoc_obj->persist.freshness), sizeof(uint32_t), 1, f);
	fclose(f);

	f = fopen("./var/lib/secoc/key.bin", "wb");
	if(!f){
		printf("create key file first\n");
		return -1;
	}

	fwrite(secoc_obj->persist.key, sizeof(uint8_t), KEY_SIZE, f);
	fclose(f);

	return 0;
}

void secoc_read_obj(secoc_ctx* secoc_obj){
	if(!secoc_obj->data_loaded && secoc_load_obj(secoc_obj) == -1)
		return;

	printf("Freshness : %d\n", secoc_obj->persist.freshness);
	printf("Key : ");
	for(int i = 0; i < KEY_SIZE; i++)
		printf("%x ", secoc_obj->persist.key[i]);
	printf("\n\n");
}

int secoc_gen_key(secoc_ctx* secoc_obj){
	uint8_t key[KEY_SIZE];

	if(get_random(key, KEY_SIZE) == -1)
		return -1;
	
	memcpy(secoc_obj->persist.key, key, KEY_SIZE);

	if(secoc_store_obj(secoc_obj) == -1)
		return -1;
	
	secoc_obj->data_loaded = true;
		
	return 0;
}

int secoc_gen_mac(secoc_ctx* secoc_obj, const uint8_t payload[MAX_PAYLOAD], uint16_t payload_len){
	if(!secoc_obj->data_loaded && secoc_load_obj(secoc_obj) == -1)
		return -1;
	
	uint8_t mac[SECOC_MAC_SIZE];
	uint8_t buffer[MAX_PAYLOAD];
	uint32_t freshness = secoc_obj->persist.freshness;
	size_t mac_len;

	memcpy(buffer, payload, payload_len);
	buffer[payload_len] 	= (freshness >> 24) & 0xFF;
	buffer[payload_len + 1]	= (freshness >> 16) & 0xFF;
	buffer[payload_len + 2] = (freshness >> 8) 	& 0xFF;
	buffer[payload_len + 3] = (freshness)		& 0xFF; 

	CMAC_CTX *ctx = CMAC_CTX_new();

	CMAC_Init(ctx, secoc_obj->persist.key, KEY_SIZE, EVP_aes_128_cbc(), NULL);
	CMAC_Update(ctx, buffer, payload_len + 4);
	CMAC_Final(ctx, mac, &mac_len);

	CMAC_CTX_free(ctx);

	printf("MAC : ");
	for(int i = 0; i < (int)mac_len; i++){
		printf("%X ", mac[i]);
	}
	printf("\n\n");

	return 0;
}

void secoc_protect(secoc_ctx* secoc_obj, secoc_pdu_t* pdu){
	// get freshness and add 1
	if(!secoc_obj->data_loaded && secoc_load_obj(secoc_obj) == -1)
		return;
	(secoc_obj->persist.freshness)++;

	pdu->freshness = secoc_obj->persist.freshness;

	// calc mac (not now)
	memset(pdu->mac, 0xAA, SECOC_MAC_SIZE);

	secoc_store_obj(secoc_obj);
}

void secoc_replay(secoc_ctx* secoc_obj, secoc_pdu_t* pdu){
	uint32_t replay_freshness;
	
	if(!secoc_obj->data_loaded && secoc_load_obj(secoc_obj) == -1)
		return;

	printf("Input replay freshness (0 ~ %d)\n", secoc_obj->persist.freshness - 1);
	printf("> ");
	replay_freshness = get_number();

	pdu->freshness = replay_freshness;

	// calc mac (not now)
	memset(pdu->mac, 0xAA, SECOC_MAC_SIZE);
}