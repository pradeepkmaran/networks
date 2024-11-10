#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int fd, newfd;
    struct sockaddr_in saddr, caddr;
    int addrlen = sizeof(caddr);
    char buff[1024], fname[1024];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("SOCKET FAILURE\n");
        return 1;
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7228);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("BIND ERROR\n");
        close(fd);
        return 1;
    }

    if (listen(fd, 2) < 0) {
        printf("LISTEN ERROR\n");
        close(fd);
        return 1;
    }

    newfd = accept(fd, (struct sockaddr *)&caddr, (socklen_t *)&addrlen);
    if (newfd < 0) {
        printf("ACCEPT ERROR\n");
        close(fd);
        return 1;
    }

    if (recv(newfd, fname, sizeof(fname), 0) <= 0) {
        printf("RECEIVED NO FILENAME\n");
        close(newfd);
        close(fd);
        return 1;
    }

    FILE *f = fopen(fname, "rb");
    if (f == NULL) {
        printf("FILE NOT FOUND\n");
        close(newfd);
        close(fd);
        return 1;
    }

    while (fgets(buff, sizeof(buff), f) != NULL) {
        size_t bytes_read = strlen(buff);
        if (send(newfd, buff, bytes_read, 0) == -1) {
            printf("SEND FAILED\n");
            fclose(f);
            close(newfd);
            close(fd);
            return 1;
        }
    }

    printf("FILE TRANSFERRED\n");
    fclose(f);
    close(newfd);
    close(fd);
    return 0;
}
