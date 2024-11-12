#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 1212
#define BCPORT 8080
#define CLIENT_IP "127.0.0.3"
#define CLIENT_MAC "B1:26:B8:60:61:EA"

int main() {
    // UDP Socket Setup
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in s_addr, cli_addr;
    socklen_t len = sizeof(cli_addr);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(BCPORT);
    s_addr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces

    // Bind UDP socket
    bind(sock, (const struct sockaddr *)&s_addr, sizeof(s_addr));

    char req[100];
    ssize_t n_bytes = recvfrom(sock, req, sizeof(req) - 1, MSG_WAITALL, (struct sockaddr *)&cli_addr, &len);
    req[n_bytes] = '\0';  // Null-terminate the received string
    printf("REQUEST RECEIVED: %s\n", req);

    char ser_ip[20], ser_mac[20], cli_ip[20], cli_mac[20];
    sscanf(req, "%[^|]|%[^|]|%[^|]", ser_ip, ser_mac, cli_ip);

    if (strcmp(CLIENT_IP, cli_ip) != 0) {
        close(sock);
        return 0;  // Client IP doesn't match; ignore this request
    }

    // TCP Socket Setup
    int tcpsock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in tcp_s_addr;
    tcp_s_addr.sin_family = AF_INET;
    tcp_s_addr.sin_port = htons(PORT);
    tcp_s_addr.sin_addr.s_addr = inet_addr(ser_ip);

    // Connect to TCP server
    connect(tcpsock, (const struct sockaddr *)&tcp_s_addr, sizeof(tcp_s_addr));

    // Prepare the response and send it
    char res[100];
    snprintf(res, sizeof(res), "%s|%s", req, CLIENT_MAC);
    send(tcpsock, res, strlen(res), 0);

    printf("RESPONSE SENT: %s\n", res);

    // Cleanup
    close(tcpsock);
    close(sock);

    return 0;
}
