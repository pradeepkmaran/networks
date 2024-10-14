#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int client_socket;
    struct sockaddr_in server_addr;
    char buff[1024] = {0};
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    while(1) {
        printf("Your message: ");
        fgets(buff, 1024, stdin);
        buff[strcspn(buff, "\n")] = '\0';
        write(client_socket, buff, strlen(buff));
        if(strcmp(buff, "bye") == 0) {
            break;
        }
        memset(buff, 0, 1024);
        read(client_socket, buff, 1024);
        printf("SERVER: %s\n", buff);
    }
    close(client_socket);
}