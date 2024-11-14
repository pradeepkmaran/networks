#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define AUTH_DNS_PORT 8083
#define BUFFER_SIZE 1024
#define GOOGLE "192.0.0.1"

int main(){
    int serverfd;

    //as server
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[BUFFER_SIZE];
    int n;

    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(AUTH_DNS_PORT);

    bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len);
    buffer[n] = '\0';
    printf("Received from TLD: %s\n", buffer);
    
    strcpy(buffer, GOOGLE);
    n = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, len);

    printf("Sent to TLD server: %s\n", buffer);

    close(serverfd);
    return 0;
}