#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define ROOT_DNS_PORT 8081
#define TLD_DNS_PORT 8082
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

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
    servaddr.sin_port = htons(ROOT_DNS_PORT);

    bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr));


    //as client
    struct sockaddr_in rootaddr;
    socklen_t rootlen = sizeof(rootaddr);

    rootaddr.sin_family = AF_INET;
    rootaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    rootaddr.sin_port = htons(TLD_DNS_PORT);

    n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len);
    buffer[n] = '\0';
    printf("Received from Local DNS: %s\n", buffer);
    n = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&rootaddr, rootlen);
    printf("Sent to TLD: %s\n", buffer);
    n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&rootaddr, &rootlen);
    buffer[n] = '\0';
    printf("Received from TLD: %s\n", buffer);
    n = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, len);
    printf("Sent to Local DNS: %s\n", buffer);

    close(serverfd);
    return 0;
}