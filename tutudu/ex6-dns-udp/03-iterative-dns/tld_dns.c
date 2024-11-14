#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define AUTH_PORT "8083"
#define TLD_DNS_PORT 8082
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

int main(){
    int serverfd, clientfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[BUFFER_SIZE];
    int n;

    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    servaddr.sin_port = htons(TLD_DNS_PORT);

    bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len);
    buffer[n] = '\0';

    printf("Received domain from Local DNS: %s\n", buffer);
    strcpy(buffer, AUTH_PORT);

    n = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, len);
    printf("Sent AUTH address to Local DNS\n");
    

    return 0;
}