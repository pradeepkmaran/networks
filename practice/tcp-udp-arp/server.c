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
#define BCADDR "255.255.255.255"

int main() {
    // UDP Socket Setup
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in s_addr;
    socklen_t len = sizeof(s_addr);
    int broadcast_enable = 1;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(BCPORT);
    s_addr.sin_addr.s_addr = inet_addr(BCADDR);

    // Enable broadcast
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable));

    char req[] = "127.0.0.1|A1:56:C8:96:69:EE|127.0.0.3";
    printf("REQUEST SENT: %s\n", req);

    // Send UDP broadcast message
    sendto(sock, req, strlen(req), MSG_CONFIRM, (struct sockaddr *)&s_addr, len);

    // TCP Socket Setup
    int tcpsock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in tcp_s_addr;
    tcp_s_addr.sin_family = AF_INET;
    tcp_s_addr.sin_port = htons(PORT);
    tcp_s_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces

    // Bind TCP socket
    bind(tcpsock, (struct sockaddr *)&tcp_s_addr, sizeof(tcp_s_addr));

    // Listen for incoming TCP connections
    listen(tcpsock, 2);

    struct sockaddr_in tcp_c_addr;
    socklen_t tcp_c_len = sizeof(tcp_c_addr);
    int clisock = accept(tcpsock, (struct sockaddr *)&tcp_c_addr, &tcp_c_len);

    // Read TCP response
    char res[100];
    ssize_t n_bytes = read(clisock, res, sizeof(res) - 1);
    res[n_bytes] = '\0';  // Null-terminate the received string
    printf("RESPONSE RECEIVED: %s\n", res);

    // Cleanup
    close(clisock);
    close(tcpsock);
    close(sock);

    return 0;
}
