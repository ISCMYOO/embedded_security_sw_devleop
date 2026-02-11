#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP <STM32MP157D-DK1 ip>
#define SERVER_PORT 5555

int main() {
    int sock;
    struct sockaddr_in server_addr;
    const char *msg = "Hello UDP";
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    sever_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
		printf("[Sender] Sent: \"%s\"\n", msg);
		
		close(sock);
		return 0;
}