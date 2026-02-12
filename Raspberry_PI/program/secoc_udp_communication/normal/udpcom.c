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

void send_secoc_udp_message(uint8_t service){
        int sock;
        struct sockaddr_in receiver_addr;
        secoc_pdu_t pdu;
        char buf[MAX_PAYLOAD];

        memset(buf, 0, MAX_PAYLOAD);

        sock = get_udp_sock();
        if(sock == -1)  return;

        set_udp_send_conf(&receiver_addr);
        if(service != 1 && service != 5){
                printf("input value : ");
                fgets(buf, MAX_PAYLOAD, stdin);
        }

        memset(&pdu, 0, sizeof(pdu));
        pdu.payload_len = strlen(buf);
        pdu.service = service;
        memcpy(pdu.payload, buf, pdu.payload_len);

        secoc_protect(&pdu);

        sendto(sock, &pdu, sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint16_t) + pdu.payload_len + SECOC_MAC_SIZE, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        printf("[Sender] Sent: %s\n", buf);

        close(sock);
}

void send_secoc_udp_replay(){
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

        secoc_replay(&pdu);

        sendto(sock, &pdu, sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint16_t) + pdu.payload_len + SECOC_MAC_SIZE, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        printf("[Sender] Sent: %s\n", buf);

        close(sock);
}