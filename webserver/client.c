#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "http.h"

#define MAX_MSG_LENGTH 255

int client_treatment(int client_socket) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    char full_message[1200];
    int recv_line = 0;
    const char *error_400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length: 17\r\n\r\n400 Bad requet\r\n";

    /* Echo everything the client sends */
    while (fgets(client_message, MAX_MSG_LENGTH, client)) {
        if (recv_line == 0) {
            ++recv_line;
            if (method_parser(client_message) < 0) {
                fprintf(client, error_400);
                break;
            }
        }
        if (!strcmp(client_message, "\r\n") || !strcmp(client_message, "\n")) break;
    }

    printf("message =\n%s\n", full_message);
    fclose(client);

    exit(0);
}

int create_client_socket(int server_socket) {
    int client_socket;
    int pid;
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("accept");
        return -1;
    }

    if ((pid = fork()) == -1) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        sleep(1);
        if (client_treatment(client_socket) == -1) {
            printf("Could not start client treatment");
        }
    }
    close(client_socket);

    return 0;
}
