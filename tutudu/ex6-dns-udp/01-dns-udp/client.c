#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DNS_PORT 5353
#define BUFFER_SIZE 1024

int main() {
    int udp_sock;
    struct sockaddr_in server_addr;
    char domain[256] = "www.google.com"; 
    char ip[16];
    socklen_t addr_len = sizeof(server_addr);

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(udp_sock, domain, strlen(domain) + 1, MSG_CONFIRM, (struct sockaddr*)&server_addr, addr_len);

    recvfrom(udp_sock, ip, sizeof(ip), MSG_WAITALL, (struct sockaddr*)&server_addr, &addr_len);

    printf("IP Address for %s: %s\n", domain, ip);

    close(udp_sock);
    return 0;
}
