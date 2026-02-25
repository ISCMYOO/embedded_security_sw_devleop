#include "udpcom.h"

int get_udp_sock(){
	int sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock < 0){
                perror("socket");
                return -1;
        }
	return sock;
}

void set_udp_send_conf(struct sockaddr_in* receiver_addr){
	memset(receiver_addr, 0, sizeof(*receiver_addr));
        receiver_addr->sin_family = AF_INET;
        receiver_addr->sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_IP, &(*receiver_addr).sin_addr);
}

void send_secoc_udp_message(secoc_ctx* secoc_obj, uint8_t service){
        int sock;
        struct sockaddr_in receiver_addr;
        secoc_pdu_t pdu;
        char buf[MAX_PAYLOAD];
        uint32_t buf_len = 0;

        memset(buf, 0, MAX_PAYLOAD);

        sock = get_udp_sock();
        if(sock == -1)  return;

        set_udp_send_conf(&receiver_addr);
        if(service == CMD_SEND_MESSAGE || service == CMD_REPLAY_PACKET || service == CMD_SET_FRESHNESS || service == CMD_GEN_MAC){
                printf("input value : ");
                fgets(buf, MAX_PAYLOAD, stdin);
                if(strlen(buf) == 0)    buf_len = 0;
                else    buf_len = strlen(buf) - 1;
        }else if(service == CMD_SYNC_KEY || service == CMD_GEN_SYNC_KEY){
                memcpy(buf, secoc_obj->persist.key, KEY_SIZE);
                buf[KEY_SIZE] = '\0';
                buf_len = KEY_SIZE;
        }

        memset(&pdu, 0, sizeof(pdu));
        pdu.payload_len = buf_len;
        pdu.service = service;
        memcpy(pdu.payload, buf, pdu.payload_len);

        secoc_protect(secoc_obj, &pdu);

        sendto(sock, &pdu, sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint32_t) + pdu.payload_len + SECOC_MAC_SIZE, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        if(service == CMD_SEND_MESSAGE || service == CMD_REPLAY_PACKET){
                printf("[Sender] Sent: %s\n", buf);
        }else if(service == CMD_SET_FRESHNESS){
                parse_uint32(buf, &(secoc_obj->persist.freshness));
                secoc_store_obj(secoc_obj);
        }else if(service == CMD_GEN_MAC){
                secoc_gen_mac(secoc_obj, pdu.payload, pdu.payload_len);
        }

        close(sock);
}

void send_secoc_udp_replay(secoc_ctx* secoc_obj){
        int sock;
        struct sockaddr_in receiver_addr;
        secoc_pdu_t pdu;
        char buf[MAX_PAYLOAD];

        memset(buf, 0, MAX_PAYLOAD);

        sock = get_udp_sock();
        if(sock == -1)  return;

        set_udp_send_conf(&receiver_addr);
        
        printf("input value : ");
        fgets(buf, MAX_PAYLOAD, stdin);

        memset(&pdu, 0, sizeof(pdu));
        pdu.payload_len = strlen(buf);
        pdu.service = 2;
        memcpy(pdu.payload, buf, pdu.payload_len);

        secoc_replay(secoc_obj, &pdu);

        sendto(sock, &pdu, sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint16_t) + pdu.payload_len + SECOC_MAC_SIZE, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        printf("[Sender] Sent: %s\n", buf);

        close(sock);
}