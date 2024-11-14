#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DNS_PORT 5353
#define BUFFER_SIZE 1024

typedef struct {
    char domain[256];   
    char ip[16];
} Dns_Table;

Dns_Table dns_table[] = {
    {"www.google.com", "142.250.190.46"},
    {"www.example.com", "93.184.216.34"},
    {"www.facebook.com", "157.240.7.35"},
    {"www.github.com", "140.82.112.3"}
};

int find_ip_for_domain(const char *domain) {
    for (int i = 0; i < 4; i++) {
        if (strcmp(domain, dns_table[i].domain) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    int udp_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char domain[256];
    char ip[16];

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(DNS_PORT);

    bind(udp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        recvfrom(udp_sock, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&client_addr, &addr_len);
        sscanf(buffer, "%s", domain);

        int index = find_ip_for_domain(domain);
        if (index != -1) {
            strcpy(ip, dns_table[index].ip);
        } else {
            strcpy(ip, "0.0.0.0");
        }

        printf("Received request for domain: %s, responding with IP: %s\n", domain, ip);
        sendto(udp_sock, ip, strlen(ip) + 1, 0, (struct sockaddr*)&client_addr, addr_len);
    }

    close(udp_sock);
    return 0;
}
