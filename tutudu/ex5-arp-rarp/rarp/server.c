#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define UDP_PORT 8888
#define TCP_PORT 9999
#define BUFFER_SIZE 1024

typedef struct {
    char mac[18];   
    char ip[16];
} Mac_Ip_Map;

Mac_Ip_Map mappings[] = {
    {"00:11:22:33:44:55", "127.0.0.2"},
    {"11:22:33:44:55:66", "127.0.0.3"},
    {"22:33:44:55:66:77", "127.0.0.4"},
    {"33:44:55:66:77:88", "127.0.0.5"}
};

int find_ip_for_mac(const char mac[]) {
    for (int i = 0; i < 4; i++) {
        if (strcmp(mac, mappings[i].mac) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    int udp_sock, tcp_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_mac[18];
    char client_ip[16];

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(UDP_PORT);

    bind(udp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    recvfrom(udp_sock, client_mac, sizeof(client_mac), MSG_WAITALL, (struct sockaddr*)&client_addr, &addr_len);
    client_mac[strlen(client_mac)] = '\0';
    printf("Received MAC: %s\n", client_mac);

    int index = find_ip_for_mac(client_mac);
    if (index != -1) {
        strcpy(client_ip, mappings[index].ip);
    } else {
        strcpy(client_ip, "0.0.0.0");
    }

    sendto(udp_sock, client_ip, strlen(client_ip) + 1, 0, (struct sockaddr*)&client_addr, addr_len);
    close(udp_sock);

    return 0;
}
