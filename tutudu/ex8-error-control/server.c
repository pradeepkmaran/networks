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

    int p1 = hammingBits[0] ^ hammingBits[2] ^ hammingBits[4] ^ hammingBits[6];
    int p2 = hammingBits[1] ^ hammingBits[2] ^ hammingBits[5] ^ hammingBits[6];
    int p4 = hammingBits[3] ^ hammingBits[4] ^ hammingBits[5] ^ hammingBits[6];

    int errorPosition = p4 * 4 + p2 * 2 + p1 * 1;

    if (errorPosition == 0) {
        printf("No error detected in received data.\n");
    } else {
        printf("Error detected at position: %d\n", errorPosition);
        hammingBits[errorPosition - 1] ^= 1; 
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

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    read(new_socket, buffer, 7);
    printf("Received code: %s\n", buffer);

    checkAndCorrectHammingCode(buffer);

    close(new_socket);
    close(server_fd);
    return 0;
}
