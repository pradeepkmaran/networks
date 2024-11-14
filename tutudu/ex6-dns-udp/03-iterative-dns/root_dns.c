#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define TLD_PORT "8082"
#define ROOT_DNS_PORT 8081
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

struct domain{
    char *name;
    char *ip;
};

int check(char *buffer){
    struct domain cache[] = {{"yahoo.com", "193.56.0.2$"},{"youtube.com", "193.56.0.3$"}};
    for(int i=0; i<2; i++){
        if(strcmp(buffer, cache[i].name) == 0){
            strcpy(buffer, cache[i].ip);
            return 1;
        }
    }
    return 0;
}


int main(){
    int serverfd, clientfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[BUFFER_SIZE];
    int n;

    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    servaddr.sin_port = htons(ROOT_DNS_PORT);

    bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len);
    buffer[n] = '\0';

    printf("Received domain from Local DNS: %s\n", buffer);
    if(check(buffer)){
        printf("Found in cache\n");
    }
    else{
        strcpy(buffer, TLD_PORT);
    }
    n = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, len);
    printf("Sent TLD address to Local DNS\n");
    

    return 0;
}