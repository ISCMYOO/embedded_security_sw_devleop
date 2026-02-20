#include "secoc.h"

int secoc_load_freshness(secoc_ctx* secoc_obj){
	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/freshness.bin", "rb");
	if(!f){
		printf("create freshness file first\n");
		return -1;
	}

	fread(&(secoc_obj->freshness), sizeof(uint32_t), 1, f);
	fclose(f);

	return 0;
}

int secoc_store_freshness(secoc_ctx* secoc_obj){
	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/freshness.bin", "wb");
	if(!f){
		printf("create freshness file first\n");
		return -1;
	}

	fwrite(&(secoc_obj->freshness), sizeof(uint32_t), 1, f);
	fclose(f);

	return 0;
}

int secoc_load_key(secoc_ctx* secoc_obj){
	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/key.bin", "rb");
	if(!f){
		printf("create key file first\n");
		return -1;
	}

	fread(secoc_obj->key, sizeof(uint8_t), KEY_SIZE, f);
	fclose(f);

	printf("Key : ");
	for(int i = 0; i < KEY_SIZE; i++)
		printf("0x%X ", secoc_obj->key[i]);

	printf("\n\n");
	return 0;
}

int secoc_store_key(secoc_ctx* secoc_obj){
	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/key.bin", "wb");
	if(!f){
		printf("create key file first\n");
		return -1;
	}

	fwrite(secoc_obj->key, sizeof(uint8_t), KEY_SIZE, f);
	fclose(f);

	printf("Key : ");
	for(int i = 0; i < KEY_SIZE; i++)
		printf("0x%X ", secoc_obj->key[i]);
	
	printf("\n\n");
	return 0;
}

int secoc_gen_key(secoc_ctx* secoc_obj){
	uint8_t key[KEY_SIZE];

	if(get_random(key, KEY_SIZE) == -1)
		return -1;
	
	memcpy(secoc_obj->key, key, KEY_SIZE);

	if(secoc_store_key(secoc_obj) == -1)
		return -1;
	
	return 0;
}

void secoc_protect(secoc_ctx* secoc_obj, secoc_pdu_t* pdu){
	// get freshness and add 1
	secoc_load_freshness(secoc_obj);
	(secoc_obj->freshness)++;

	pdu->freshness = secoc_obj->freshness;

	// calc mac (not now)
	memset(pdu->mac, 0xAA, SECOC_MAC_SIZE);

	secoc_store_freshness(secoc_obj);
}

void secoc_replay(secoc_ctx* secoc_obj, secoc_pdu_t* pdu){
	uint32_t replay_freshness;
	
	if(secoc_load_freshness(secoc_obj) == -1)
		return;

	printf("Input replay freshness (0 ~ %d)\n", secoc_obj->freshness - 1);
	printf("> ");
	replay_freshness = get_number();

	pdu->freshness = replay_freshness;

	// calc mac (not now)
	memset(pdu->mac, 0xAA, SECOC_MAC_SIZE);
}