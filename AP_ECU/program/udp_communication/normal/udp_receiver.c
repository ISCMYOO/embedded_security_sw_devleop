#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define LISTEN_PORT 5555
#define BUF_SIZE 512

int main(){
    int sock;
    struct sockaddr_in addr, src_addr;
    socklen_t src_len = sizeof(src_addr);
    char buf[BUF_SIZE];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LISTEN_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

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

        buf[len] = '\0';

        printf("[Receiver] From %s:%d | \"%s\"\n", inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port), buf);
    }

    close(sock);
    return 0;
}