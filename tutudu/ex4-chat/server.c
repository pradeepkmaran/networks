#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUF_SIZE 1024

void handle_client(int server_socket, struct sockaddr_in *client_addr, socklen_t client_len) {
    char buffer[BUF_SIZE];
    char user_input[BUF_SIZE];
    ssize_t n;

    n = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)client_addr, &client_len);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Received from client: %s\n", buffer);

        printf("Send reply: ");
        fgets(user_input, BUF_SIZE-1, stdin);
        user_input[strcspn(user_input, "\n")] = '\0';  

        sendto(server_socket, user_input, strlen(user_input), 0, (struct sockaddr*)client_addr, client_len);
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        handle_client(server_socket, &client_addr, client_len);
    }

    close(server_socket);
    return 0;
}
