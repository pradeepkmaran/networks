#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8080
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

int main(){
    int serverfd;
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);
    char buffer[BUFFER_SIZE];
    int n;

    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    servaddr.sin_port = htons(PORT);

    printf("Enter DNS name to resolve: ");
    scanf("%s", buffer);

    n = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, len);

    n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';

    printf("Resolved IP Address: %s\n", buffer);

    close(serverfd);
    return 0;
}