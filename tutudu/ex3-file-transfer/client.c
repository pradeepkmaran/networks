#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFSIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char filename[256];
    char buffer[BUFSIZE];
    FILE *file;
    size_t bytes_read;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Enter filename: ");
    scanf("%s", filename);
    write(sockfd, filename, strlen(filename) + 1);

    strcat(filename, "_copy");
    file = fopen("received_file", "wb");
    while ((bytes_read = read(sockfd, buffer, BUFSIZE)) > 0) {
        fwrite(buffer, 1, bytes_read, file);
    }

    fclose(file);
    close(sockfd);
    return 0;
}
