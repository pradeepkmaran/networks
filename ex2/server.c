#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFF_SZ 1024

typedef struct {
    int client_socket;
    pthread_t thread_id;
} client_info_t;

void* handle_client(void *arg){
    client_info_t *info = (client_info_t *)arg;
    int client_socket = info->client_socket;
    char buff[BUFF_SZ];
    memset(buff, 0, BUFF_SZ);
    
    while(1) {
        int bytes_read = read(client_socket, buff, BUFF_SZ-1);
        if(bytes_read <= 0) {
            break;
        }
        buff[bytes_read]='\0';
        if(strcmp(buff, "bye") == 0) {
            break;
        }
        printf("Client [Thread %lu]: %s\n", info->thread_id, buff);

        printf("Your Message: ");
        memset(buff, 0, BUFF_SZ);
        fgets(buff, BUFF_SZ, stdin);
        buff[strcspn(buff, "\n")] = '\0';
        write(client_socket, buff, strlen(buff));
    }

    close(client_socket);
    free(info);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_sockaddr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        printf("SERVER SOCKET NOT CONNECTED\n");
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("BIND ERROR\n");
        return 0;
    }

    if(listen(server_socket, 5) < 0) {
        printf("LISTEN ERROR\n");
        return 0;
    }

    printf("SERVER LISTENING TO PORT %d\n", PORT);

    while(1) {
        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_sockaddr_len);
        if(client_socket < 0) {
            printf("ACCEPT FAILED\n");
            continue;
        } 

        client_info_t *info = malloc(sizeof(client_info_t));
        if(info == NULL) {
            printf("MEMORY ALLOC FAILED\n");
            close(client_socket);
            continue;
        }

        info->client_socket = client_socket;
        if(pthread_create(&info->thread_id, NULL, handle_client, (void *)info) != 0) {
            printf("FAILED TO CREATE THREAD\n");
            free(info);
            close(client_socket);
            continue;
        }
        pthread_detach(info->thread_id);
    }
    close(server_socket);
}