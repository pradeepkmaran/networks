#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFSIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char filename[256];
    char buffer[BUFSIZE];
    FILE *file;
    size_t bytes_read;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    read(client_fd, filename, sizeof(filename));
    
    file = fopen(filename, "rb");
    if (file) {
        while ((bytes_read = fread(buffer, 1, BUFSIZE, file)) > 0) {
            write(client_fd, buffer, bytes_read);
        }
        fclose(file);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
