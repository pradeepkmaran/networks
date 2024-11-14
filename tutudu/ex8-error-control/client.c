#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

void calculateHammingCode(char *data, char *hammingCode) {
    int dataBits[4];
    int hammingBits[7];
    
    for (int i = 0; i < 4; i++) {
        dataBits[i] = data[i] - '0';
    }

    hammingBits[2] = dataBits[0];
    hammingBits[4] = dataBits[1];
    hammingBits[5] = dataBits[2];
    hammingBits[6] = dataBits[3];

    hammingBits[0] = hammingBits[2] ^ hammingBits[4] ^ hammingBits[6];
    hammingBits[1] = hammingBits[2] ^ hammingBits[5] ^ hammingBits[6];
    hammingBits[3] = hammingBits[4] ^ hammingBits[5] ^ hammingBits[6];

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

    hammingCode[1] = '0';

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, hammingCode, strlen(hammingCode), 0);
    printf("Hamming code sent\n");

    close(sock);
    return 0;
}
