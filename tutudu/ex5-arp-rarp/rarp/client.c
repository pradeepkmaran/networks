#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define UDP_PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int udp_sock;
    struct sockaddr_in server_addr;
    char client_mac[18] = "00:11:22:33:44:55"; 
    char server_ip[16];
    int addr_len = sizeof(server_addr);

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(udp_sock, client_mac, strlen(client_mac) + 1, MSG_CONFIRM, (struct sockaddr*)&server_addr, addr_len);

    recvfrom(udp_sock, server_ip, sizeof(server_ip), MSG_WAITALL, (struct sockaddr*)&server_addr, &addr_len);

    printf("Received IP: %s\n", server_ip);

    close(udp_sock);
    return 0;
}
