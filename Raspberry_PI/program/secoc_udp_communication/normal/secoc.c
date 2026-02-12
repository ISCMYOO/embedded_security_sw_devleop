#include "secoc.h"

uint32_t secoc_load_freshness(){
	uint32_t freshness;

	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/freshness.bin", "rb");
	if(!f){
		printf("create freshness file first\n");
		return 0;
	}

	fread(&freshness, sizeof(uint32_t), 1, f);
	fclose(f);
	
	return freshness;
}

void secoc_store_freshness(uint32_t freshness){
	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/freshness.bin", "wb");
	if(!f){
		printf("create freshness file first\n");
		return;
	}

	fwrite(&freshness, sizeof(uint32_t), 1, f);
	fclose(f);
}

uint32_t secoc_read_freshness(){
	uint32_t freshness;

	FILE *f = fopen("/home/iscmyoo/var/lib/secoc/freshness.bin", "rb");
	if(!f){
		printf("create freshness file first\n");
		return -1;
	}

	fread(&freshness, sizeof(uint32_t), 1, f);
	fclose(f);

	printf("now freshness : %d\n", freshness);

	return freshness;
}

int secoc_protect(secoc_pdu_t* pdu){
	uint32_t freshness;

	// get freshness and add 1
	freshness = secoc_load_freshness();
	freshness++;

	pdu->freshness = freshness;

	// calc mac (not now)
	memset(pdu->mac, 0xAA, SECOC_MAC_SIZE);

	secoc_store_freshness(freshness);

	return 0;
}

void secoc_replay(secoc_pdu_t* pdu){
	uint32_t freshness, replay_freshness;
	
	freshness = secoc_read_freshness();

	if(freshness == -1)	return;

	printf("Input replay freshness (0 ~ %d)\n", freshness - 1);
	printf("> ");
	replay_freshness = get_number();

	pdu->freshness = replay_freshness;

	// calc mac (not now)
	memset(pdu->mac, 0xAA, SECOC_MAC_SIZE);
}