#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int fd, newfd;
    struct sockaddr_in saddr, caddr;
    int addrlen = sizeof(saddr);
    char buff[1024], fname[1024];
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(fd < 0) {
        printf("SOCKET FAILURE\n");
        return 0;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7228);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(fd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("BIND ERROR\n");
        return 0;
    }

    listen(fd, 2);
    int len = sizeof(caddr);
    newfd = accept(fd, (struct sockaddr *) &caddr, &len);
    if(recv(newfd, fname, sizeof(fname), 0) <= 0) {
        printf("RECEIVED NO FILENAME\n");
        return 0;
    }
    FILE *f = fopen(fname, "rb");
    if(f == NULL) {
        printf("FILE NOT FOUND\n");
        return 0;
    }
    while(fgets(buff, sizeof(buff), f) != NULL) {
        if(send(newfd, buff, sizeof(buff, 0), 0) == -1) {
            printf("SEND FAILED\n");
            return 0;
        }
        memset(buff, 0, sizeof(buff));
    }
    printf("FILE TRANSFERRED\n");
    close(fd);
    close(newfd);
}