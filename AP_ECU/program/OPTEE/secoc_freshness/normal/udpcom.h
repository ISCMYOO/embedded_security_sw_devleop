#ifndef UDPCOM_H
#define UDPCOM_H

#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

#define LISTEN_PORT 5555
#define BUF_SIZE 1500 // Ethernet 환경에서의 SecOC 최대 크기

/*
UDP 소켓 생성 함수
*/
int get_udp_sock();

/*
UDP 수신자 세팅 함수
*/
void set_udp_receiver_config(struct sockaddr_in* addr);

#endif