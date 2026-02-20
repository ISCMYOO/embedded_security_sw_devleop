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
	printf("9. Exit\n");
	printf("> ");
}

int main(){
	uint8_t menu_num;
	secoc_ctx secoc_obj;

	while(1){
		print_menu();
		menu_num = get_number();
		printf("\n");

		if(menu_num == 1){ // freshness init
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_obj.freshness = 0;
			secoc_store_freshness(&secoc_obj);
			printf("reset freshness 0\n");
		}else if(menu_num == 2){ // send message
			send_secoc_udp_message(&secoc_obj, menu_num);
			printf("\n\n");
		}else if(menu_num == 3){ // do replay attack
			send_secoc_udp_replay(&secoc_obj);
		}else if(menu_num == 4){ // read freshness
			send_secoc_udp_message(&secoc_obj, menu_num);
			int freshness = secoc_load_freshness(&secoc_obj);
			printf("now freshness : %d\n", freshness);
		}else if(menu_num == 5){ // set freshness
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 6){ // read Key
			secoc_load_key(&secoc_obj);
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 7){ // Sync Key
			secoc_load_key(&secoc_obj);
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 8){ // Generate and Sync key
			secoc_gen_key(&secoc_obj);
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == 9){ // Exit
			return 0;
		}else{
			printf("Your Select invalid\n");
		}
	}
	return 0;
}
