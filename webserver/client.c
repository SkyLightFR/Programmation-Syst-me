#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "http.h"

#define MAX_MSG_LENGTH 255

int client_treatment(int client_socket) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    int parser;
    int recv_line = 0;

    int welcome_fd;
    struct stat welcome_stat;
    char welcome_msg[2000];

    const char *response_bad_req = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length: 17\r\n\r\n400 Bad request\r\n";
    const char *response_rsc_not_found = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-length: 15\r\n\r\n404 Not found\r\n";
    const char *response_ok = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-length: %d\r\n\r\n%s\r\n";

    /* Fetch everything the client sends until \r\n or \n */
    while (fgets(client_message, MAX_MSG_LENGTH, client)) {
        if (recv_line == 0) {
            parser = method_parser(client_message);
            ++recv_line;
        }
        if (!strcmp(client_message, "\r\n") || !strcmp(client_message, "\n")) break;
    }

    /* Send response according to parser return value */
    if (parser < 0)
        fprintf(client, response_bad_req);
    else if (parser == RSC_NOT_FOUND)
        fprintf(client, response_rsc_not_found);
    else {
        /* Put content of resource/welcome_message in welcome_msg */
        welcome_fd = open("resource/welcome_message", O_RDONLY);
        if (welcome_fd == -1) {
            perror("read resource/welcome_message");
            strcpy(welcome_msg, "Could not read welcome file\n");
        } else {
            if (fstat(welcome_fd, &welcome_stat) == -1) {
                perror("fstat");
                strcpy(welcome_msg, "Could not read welcome file\n");
            } else if (read(welcome_fd, (void *)welcome_msg, welcome_stat.st_size) == -1) {
                perror("read welcome_message");
                strcpy(welcome_msg, "Could not read welcome file\n");
            } else {
                strcat(welcome_msg, "\0");
            }
            close(welcome_fd);
        }

        fprintf(client, response_ok, strlen(welcome_msg), welcome_msg);
    }

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
