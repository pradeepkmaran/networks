#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

void checkAndCorrectHammingCode(char *receivedCode) {
    int hammingBits[7];
    for (int i = 0; i < 7; i++) {
        hammingBits[i] = receivedCode[i] - '0';
    }

    // Calculate the parity bits
    int p1 = hammingBits[0] ^ hammingBits[2] ^ hammingBits[4] ^ hammingBits[6];
    int p2 = hammingBits[1] ^ hammingBits[2] ^ hammingBits[5] ^ hammingBits[6];
    int p4 = hammingBits[3] ^ hammingBits[4] ^ hammingBits[5] ^ hammingBits[6];

    int errorPosition = p4 * 4 + p2 * 2 + p1 * 1;

    if (errorPosition == 0) {
        printf("No error detected in received data.\n");
    } else {
        printf("Error detected at position: %d\n", errorPosition);
        hammingBits[errorPosition - 1] ^= 1;  // Correct the error
        printf("Corrected code: ");
        for (int i = 0; i < 7; i++) {
            printf("%d", hammingBits[i]);
        }
        printf("\n");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[8] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    // Receive the hamming code
    read(new_socket, buffer, 7);
    printf("Received code: %s\n", buffer);

    // Check and correct errors
    checkAndCorrectHammingCode(buffer);

    close(new_socket);
    close(server_fd);
    return 0;
}
