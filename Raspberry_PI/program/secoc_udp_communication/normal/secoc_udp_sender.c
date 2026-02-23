#include "udpcom.h"

void print_menu(){
	printf("\nSelect Menu\n\n");
	printf("1. Reset Freshness\n");
	printf("2. Send Message\n");
	printf("3. Replay Packet\n");
	printf("4. Read Freshness\n");
	printf("5. Set Freshness\n");
	printf("6. Read Key\n");
	printf("7. Sync Key\n");
	printf("8. Gen & Sync Key\n");
	printf("9. Gen MAC\n");
	printf("10. Exit\n");
	printf("> ");
}

int main(){
	uint8_t menu_num;
	secoc_ctx secoc_obj;

	secoc_load_obj(&secoc_obj);

	while(1){
		print_menu();
		menu_num = get_number();
		printf("\n");

		if(menu_num == 1){ // freshness init
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_obj.persist.freshness = 0;
			if(secoc_store_obj(&secoc_obj) == 0)
				printf("reset freshness 0\n");
		}else if(menu_num == 2){ // send message
			send_secoc_udp_message(&secoc_obj, menu_num);
			printf("\n\n");
		}else if(menu_num == 3){ // do replay attack
			send_secoc_udp_replay(&secoc_obj);
		}else if(menu_num == 4){ // read freshness
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_read_obj(&secoc_obj);
		}else if(menu_num == 5){ // set freshness
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 6){ // read Key
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_read_obj(&secoc_obj);
		}else if(menu_num == 7){ // Sync Key
			if(!secoc_obj.data_loaded && secoc_load_obj(&secoc_obj) == -1)
				continue;
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 8){ // Generate and Sync key
			if(secoc_gen_key(&secoc_obj) == -1)
				continue;
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 9){
			if(!secoc_obj.data_loaded && secoc_load_obj(&secoc_obj) == -1)
				continue;
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 10){ // Exit
			return 0;
		}else{
			printf("Your Select invalid\n");
		}
	}
	return 0;
}
