#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

// Function to calculate the Hamming code and return the encoded data
void calculateHammingCode(char *data, char *hammingCode) {
    int dataBits[4];
    int hammingBits[7];
    
    for (int i = 0; i < 4; i++) {
        dataBits[i] = data[i] - '0';
    }

    // Calculate parity bits
    hammingBits[2] = dataBits[0];
    hammingBits[4] = dataBits[1];
    hammingBits[5] = dataBits[2];
    hammingBits[6] = dataBits[3];

    hammingBits[0] = hammingBits[2] ^ hammingBits[4] ^ hammingBits[6];
    hammingBits[1] = hammingBits[2] ^ hammingBits[5] ^ hammingBits[6];
    hammingBits[3] = hammingBits[4] ^ hammingBits[5] ^ hammingBits[6];

    // Create the hamming code string
    for (int i = 0; i < 7; i++) {
        hammingCode[i] = hammingBits[i] + '0';
    }
    hammingCode[7] = '\0';
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[5], hammingCode[8];

    printf("Enter 4-bit data: ");
    scanf("%4s", data);



    calculateHammingCode(data, hammingCode);

    
    printf("Hamming code to send: %s\n", hammingCode);

    // causing an error manually
    hammingCode[1] = '0';

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    // Send the hamming code to the server
    send(sock, hammingCode, strlen(hammingCode), 0);
    printf("Hamming code sent\n");

    close(sock);
    return 0;
}
