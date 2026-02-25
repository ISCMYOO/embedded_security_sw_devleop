#include "secoc.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <inttypes.h>

#define SERVER_IP       "192.168.100.1"
#define SERVER_PORT     5555

int get_udp_sock();

void set_udp_send_conf(struct sockaddr_in* receiver_addr);

void send_secoc_udp_message(secoc_ctx* secoc_obj, uint8_t service);