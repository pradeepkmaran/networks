#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROADCAST_IP "255.255.255.255"
#define BROADCAST_PORT 8888
#define MESSAGE "This is a broadcast message!"

typedef struct {
    char src_ip[16];
    char src_mac[18];
    char dest_ip[16];
    char dest_mac[18];
    char data[17]; // 16-bit data
} Packet;

int main() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    int broadcast_enable = 1;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow broadcast
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable,
                   sizeof(broadcast_enable)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Define broadcast address
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    // Input packet details
    Packet packet;
    printf("Enter the details of packet received.\n");
    printf("Destination IP: ");
    scanf("%s", packet.dest_ip);
    printf("Source IP: ");
    scanf("%s", packet.src_ip);
    printf("Source MAC: ");
    scanf("%s", packet.src_mac);
    printf("16-bit data: ");
    scanf("%s", packet.data);

    char msg[1000];
    strcpy(msg, packet.src_ip);
    strcat(msg, "|");
    strcat(msg, packet.src_mac);
    strcat(msg, "|");
    strcat(msg, packet.dest_ip);
    strcat(msg, "|");

    // Send broadcast message
    if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&broadcast_addr,
               sizeof(broadcast_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Broadcast message sent successfully!\n");

    // Listen for reply
    int len;
    int sockfd1, newfd, n;
    struct sockaddr_in servaddr, cliaddr;
    char buff[1024];
    char str[1000];

    // socket
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0)
        perror("cannot create socket");

    // bind
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(7228);
    if (bind(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        perror("Bind error");

    // listen
    listen(sockfd1, 2);

    // accept
    len = sizeof(cliaddr);
    newfd = accept(sockfd1, (struct sockaddr *)&cliaddr, &len);

    // read
    n = read(newfd, buff, sizeof(buff));
    printf("\nMessage from Client: %s\n", buff);

    // writing echo msg
    char newstr[1000];
    strcpy(newstr, buff);
    strcat(newstr, packet.data);
    printf("\nMessage Sent: %s\n", newstr);
    n = write(newfd, newstr, sizeof(newstr));

    // close
    close(sockfd1);
    close(newfd);

    // Close socket
    close(sockfd);

    return 0;
}
