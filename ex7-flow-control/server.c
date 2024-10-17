#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_PACKET_SIZE 4

typedef struct {
    char src_ip[16];        // Source IP address as a string
    char dest_ip[16];       // Destination IP address as a string
    int sequence_number;    // Sequence number
    char data[MAX_PACKET_SIZE + 1]; // Data + 1 for null terminator
    char fcs;               // Frame Check Sequence (dummy for now)
} Packet;

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    Packet packet;
    int expected_seq = 0, ack;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addr_size = sizeof(client_addr);

    // Receive packets
    while (1) {
        recvfrom(sockfd, &packet, sizeof(Packet), 0, (struct sockaddr *)&client_addr, &addr_size);
        printf("Received Packet: Seq %d, Data %s\n", packet.sequence_number, packet.data);

        if (packet.sequence_number == expected_seq) {
            printf("Packet %d is in sequence.\n", packet.sequence_number);
            expected_seq++;
        } else {
            printf("Packet %d is out of sequence, expecting %d.\n", packet.sequence_number, expected_seq);
        }

        // Send acknowledgment for the last correctly received packet
        ack = expected_seq - 1;
        sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *) &client_addr, addr_size);
        printf("Sent ACK %d\n", ack);
    }

    // Close socket
    close(sockfd);
    return 0;
}
