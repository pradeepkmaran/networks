#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    int fd;
    struct sockaddr_in saddr;
    char buff[1024], fname[1024];

    fd = socket(AF_INET, SOCK_STREAM, 0);   
    if (fd < 0) {
        printf("SOCKET FAILURE\n");
        return 1;
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    saddr.sin_port = htons(7228);

    if (connect(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("CONNECT F   AILURE\n");
        close(fd);
        return 1;
    }

    printf("Enter filename to request: ");
    scanf("%s", fname);
    if (send(fd, fname, strlen(fname) + 1, 0) == -1) {
        printf("FILENAME SEND FAILED\n");
        close(fd);
        return 1;
    }

    printf("Save the file as: ");
    scanf("%s", fname);
    FILE *f = fopen(fname, "wb");
    if (f == NULL) {
        printf("FILE OPEN FAILURE\n");
        close(fd);
        return 1;
    }

    int n;
    while ((n = recv(fd, buff, sizeof(buff), 0)) > 0) {
        if (n == -1) {
            printf("FAILED TO RECEIVE\n");
            fclose(f);
            close(fd);
            return 1;
        }
        fwrite(buff, sizeof(char), n, f);
    }

    printf("FILE TRANSFERRED SUCCESSFULLY\n");
    fclose(f);
    close(fd);
    return 0;
}
