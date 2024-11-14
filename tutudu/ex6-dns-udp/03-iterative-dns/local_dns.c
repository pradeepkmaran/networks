#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

#define ROOT_DNS_PORT 8081
#define LOCAL_DNS_PORT 8080
#define BUFFER_SIZE 1024
#define IP_ADDRESS "127.0.0.1"

// Function to query a DNS server
int query_dns(char *domain, int port, char *resolved_ip) {
    int sockfd;
    struct sockaddr_in dns_addr;
    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(dns_addr);
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Setup DNS server address
    dns_addr.sin_family = AF_INET;
    dns_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    dns_addr.sin_port = htons(port);
    
    // Send query to DNS server
    if(sendto(sockfd, domain, strlen(domain), 0, (struct sockaddr *)&dns_addr, len) < 0){
        perror("Send Error");
    }
    
    // Receive response
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&dns_addr, &len);
    if(n<0){
        perror("Receive error");
    }
    buffer[n] = '\0';

    strcpy(resolved_ip, buffer);
    
    close(sockfd);
    return atoi(buffer);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[BUFFER_SIZE];
    char resolved_ip[BUFFER_SIZE];
    
    // Create Local DNS server socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(LOCAL_DNS_PORT);
    
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    // Receive domain query from client
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientaddr, &len);
    buffer[n] = '\0';
    printf("Received query for domain: %s\n", buffer);
    
    // Query ROOT DNS
    int tld_port = query_dns(buffer, ROOT_DNS_PORT, resolved_ip);
    printf("Root DNS returned TLD port: %d\n", tld_port);
    if(resolved_ip[strlen(resolved_ip) - 1] == '$'){
        sendto(sockfd, resolved_ip, strlen(resolved_ip), 0, (struct sockaddr *)&clientaddr, len);
        close(sockfd);
        return 0;
    }
    
    // Query TLD DNS
    int auth_port = query_dns(buffer, tld_port, resolved_ip);
    printf("TLD DNS returned Auth port: %d\n", auth_port);
    
    // Query Authoritative DNS
    query_dns(buffer, auth_port, resolved_ip);
    printf("Auth DNS returned IP: %s\n", resolved_ip);
    
    // Send resolved IP back to client
    sendto(sockfd, resolved_ip, strlen(resolved_ip), 0, (struct sockaddr *)&clientaddr, len);
    
    close(sockfd);
    return 0;
}
