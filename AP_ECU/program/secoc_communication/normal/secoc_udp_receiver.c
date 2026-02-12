#include "secoc.h"
#include "udpcom.h"

int main(){
    int sock;
    struct sockaddr_in addr, src_addr;
    socklen_t src_len = sizeof(src_addr);
    char buf[BUF_SIZE];
    secoc_pdu_t* pdu = (secoc_pdu_t*)buf;

    sock = get_udp_sock();
    if(sock == -1)  return 0;

    set_udp_receiver_config(&addr);

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind");
        return -1;
    }

    printf("[Receiver] UDP listening on port %d...\n", LISTEN_PORT);

    while(1){
        ssize_t len = recvfrom(sock, buf, BUF_SIZE - 1, 0, (struct sockaddr*)&src_addr, &src_len);
        if(len < 0){
            perror("recvfrom");
            continue;
        }

        if(secoc_verify(pdu) == 0){
            if(pdu->payload_len > MAX_PAYLOAD){
                printf("[DROP] invalid length\n");
            }
            printf("[SecOC OK]\n");
            printf("freshness : %d\n", pdu->freshness);
            printf("service : %d\n", pdu->service);
            printf("message : %.*s\n", pdu->payload_len, pdu->payload);
        }else{
            printf("[SecOC FAIL]\n");
            printf("sender freshness : %d\n", pdu->freshness);
            printf("receiver freshness : %d\n", freshness);
        }
    }

    close(sock);
    return 0;
}