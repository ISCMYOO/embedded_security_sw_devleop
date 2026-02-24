#include "secoc.h"
#include "udpcom.h"
#include "ta_api.h"

int main(){
    int sock;
    struct sockaddr_in addr, src_addr;
    socklen_t src_len = sizeof(src_addr);
    char buf[BUF_SIZE];
    secoc_pdu_t* pdu = (secoc_pdu_t*)buf;
    TeeSecOC secoc_obj;

    sock = get_udp_sock();
    if(sock == -1)  return 0;

    set_udp_receiver_config(&addr);

    if(init(&secoc_obj) == -1){
        printf("Failed create TA Object\n");
        return -1;
    }

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind");
        return -1;
    }

    printf("[Receiver] UDP listening on port %d...\n", LISTEN_PORT);

    // 통신 시작 전 RPMB에서 Freshness RAM으로 가져오기
    char* alias = "rpi0001";
    if(secoc_load_freshness(&secoc_obj, alias) == -1){
        printf("Failed load freshness from TA\n");
        return 0;
    }
    secoc_read_freshness(&secoc_obj, alias);

    while(1){
        ssize_t len = recvfrom(sock, buf, BUF_SIZE - 1, 0, (struct sockaddr*)&src_addr, &src_len);
        if(len < 0){
            perror("recvfrom");
            continue;
        }

        if(secoc_verify(&secoc_obj, pdu, alias) == 0){
            if(pdu->payload_len > MAX_PAYLOAD){
                printf("[DROP] invalid length\n");
            }else{
                printf("[SecOC OK]\n");
                if(pdu->service == 2){
                    printf("message : %.*s\n", pdu->payload_len, pdu->payload);
                }else if(pdu->service == 1){
                    uint32_t init_freshness = 0;
                    if(secoc_store_freshness(&secoc_obj, alias, init_freshness) == -1){
                        printf("Faileed init freshness from TA\n");
                        return -1;
                    }
                    secoc_read_freshness(&secoc_obj, alias);
                }else if(pdu->service == 5){
                    uint32_t set_freshness = str_to_u32(pdu->payload);
                    if(secoc_store_freshness(&secoc_obj, alias, set_freshness) == -1){
                        printf("Failed store freshness from TA\n");
                        return -1;
                    }
                    secoc_read_freshness(&secoc_obj, alias);
                }else if(pdu->service == 4){
                    secoc_read_freshness(&secoc_obj, alias);
                }else if(pdu->service == 6){
                    secoc_delete_freshness(&secoc_obj, alias);
                }else{
                    printf("Service Not Ready\n");
                }
            }
        }
    }

    close(sock);
    return 0;
}