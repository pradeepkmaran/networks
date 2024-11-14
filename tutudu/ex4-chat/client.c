#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    socklen_t server_len = sizeof(server_addr);
    ssize_t n;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  

        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, server_len);

        n = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &server_len);
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
