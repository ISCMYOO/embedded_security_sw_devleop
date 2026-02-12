#include "common.h"
#include "udpcom.h"

void print_menu(){
	printf("Select Menu\n\n");
	printf("1. Reset Freshness\n");
        printf("2. Send Message\n");
        printf("3. Replay Packet\n");
		printf("4. Read Freshness\n");
		printf("5. Set Freshness\n");
		printf("6. Exit\n");
        printf("> ");
}

int main(){
	uint8_t menu_num;

	while(1){
		print_menu();
		menu_num = get_number();
		printf("\n");

		if(menu_num == 1){ // freshness init
			secoc_store_freshness(0);
			printf("reset freshness 0\n");

			send_secoc_udp_message(menu_num);
		}else if(menu_num == 2){ // send message
			send_secoc_udp_message(menu_num);
			printf("\n\n");
		}else if(menu_num == 3){ // do replay attack
			send_secoc_udp_replay();
		}else if(menu_num == 4){ // read freshness
			int freshness = secoc_load_freshness();
			printf("now freshness : %d\n", freshness);
		}else if(menu_num == 5){ // set freshness
			printf("input set freshness : ");
			int set_freshness = get_number();
			secoc_store_freshness((uint32_t)set_freshness);

			send_secoc_udp_message(menu_num);
		}else if(menu_num == 6){ // Exit
			return 0;
		}else{
			printf("Your Select invalid\n");
			return 0;
		}
	}
	return 0;
}
