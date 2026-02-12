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

void set_udp_receiver_config(struct sockaddr_in* addr){
    memset(addr, 0, sizeof(addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(LISTEN_PORT);
    addr->sin_addr.s_addr = INADDR_ANY;
}

