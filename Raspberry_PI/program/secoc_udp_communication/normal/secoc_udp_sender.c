#include "udpcom.h"

void print_menu(){
	printf("\nSelect Menu\n\n");
	printf("1. Reset Freshness\n");
	printf("2. Send Message\n");
	printf("3. Replay Packet\n");
	printf("4. Read Freshness\n");
	printf("5. Set Freshness\n");
	printf("6. Load Freshness\n");
	printf("7. Delete Object\n");
	printf("8. Read Key\n");
	printf("9. Load Key\n");
	printf("10. Sync Key\n");
	printf("11. Gen & Sync Key\n");
	printf("12. Gen MAC\n");
	printf("13. Exit\n");
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

		if(menu_num == CMD_RESET_FRESHNESS){
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_obj.persist.freshness = 0;
			if(secoc_store_obj(&secoc_obj) == 0)
				printf("reset freshness 0\n");
		}else if(menu_num == CMD_SEND_MESSAGE){
			send_secoc_udp_message(&secoc_obj, menu_num);
			printf("\n\n");
		}else if(menu_num == CMD_REPLAY_PACKET){
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == CMD_READ_FRESHNESS){
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_read_obj(&secoc_obj);
		}else if(menu_num == CMD_SET_FRESHNESS){
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == CMD_LOAD_FRESHNESS){
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_load_obj(&secoc_obj);
		}else if(menu_num == CMD_DELETE_OBJECT){
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_delete_obj(&secoc_obj);
		}else if(menu_num == CMD_READ_KEY){
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_read_obj(&secoc_obj);
		}else if(menu_num == CMD_LOAD_KEY){
			send_secoc_udp_message(&secoc_obj, menu_num);
			secoc_load_obj(&secoc_obj);
		}else if(menu_num == CMD_SYNC_KEY){
			if(!secoc_obj.data_loaded && secoc_load_obj(&secoc_obj) == -1)
				continue;
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == CMD_GEN_SYNC_KEY){
			if(secoc_gen_key(&secoc_obj) == -1)
				continue;
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == CMD_GEN_MAC){
			if(!secoc_obj.data_loaded && secoc_load_obj(&secoc_obj) == -1)
				continue;
			send_secoc_udp_message(&secoc_obj, menu_num);
		}else if(menu_num == CMD_EXIT){ // Exit
			return 0;
		}else{
			printf("Your Select invalid\n");
		}
	}
	return 0;
}
