#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define LISTEN_PORT 8888
#define BUFFER_SIZE 1024
#define PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in recv_addr, cliaddr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(recv_addr);
    char client_ip[16], client_mac[18];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_port = htons(PORT);

    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(LISTEN_PORT);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
    printf("Listening for broadcast messages on port %d...\n", LISTEN_PORT);

    printf("Enter the IP address: ");
    scanf("%s", client_ip);
    printf("Enter the MAC address: ");
    scanf("%s", client_mac);

    char src_ip[16], src_mac[18], dest_ip[16];

    while (1) {
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recv_addr, &addr_len);

        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            printf("\nReceived broadcast message: %s\n", buffer);

            sscanf(buffer, "%[^|]|%[^|]|%[^|]", src_ip, src_mac, dest_ip);

            if (strcmp(dest_ip, client_ip) == 0) {
                printf("IP address match\n");

                int len;
                int sockfd1, n, newfd;
                struct sockaddr_in servaddr;
                char str[1000];
                char buff[1024];
                char newbuff[1024];

                sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd1 < 0)
                    perror("\nCannot create socket\n");

                bzero(&servaddr, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                servaddr.sin_addr.s_addr = inet_addr(src_ip);
                servaddr.sin_port = htons(7228);

                connect(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr));

                snprintf(buffer, sizeof(buffer), "%s|%s|%s|%s|", src_ip, src_mac, dest_ip, client_mac);
                n = write(sockfd1, buffer, sizeof(buffer));
                printf("\nARP Reply Sent: %s\n", buffer);

                n = read(sockfd1, newbuff, sizeof(newbuff));
                printf("\nReceived packet is: %s \n", newbuff);

                close(sockfd1);
                close(newfd);
            } else {
                printf("IP address not matched\n");
            }
            break;
        } else {
            break;
        }
    }
    close(sockfd);

    return 0;
}
