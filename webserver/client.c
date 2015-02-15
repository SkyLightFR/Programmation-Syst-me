#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "acquire.h"
#include "http.h"
#include "response.h"

void open_welcome_message(char *welcome_msg);

int client_treatment(int client_socket) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    http_request request;
    int parser;

    char welcome_msg[2000];

    /* Fetch everything the client sends until \r\n or \n */
    fgets_or_exit(client_message, MAX_MSG_LENGTH, client);
    parser = parse_http_request(client_message, &request);
    skip_headers(client);


    /* Send response according to request state */
    if (parser < 0)
        send_response(client, 400, "Bad Request", "Bad request\r\n");

    else if (request.method == HTTP_UNSUPPORTED)
        send_response(client, 405, "Method Not Allowed", "Method not allowed\r\n");

    else if (request.major_version == -1 || request.minor_version == -1)
        send_response(client, 505, "HTTP Version Not Supported", "HTTP Version Not Supported\r\n");

    else if (!strcmp(request.url, "/")) {
        open_welcome_message(welcome_msg);
        send_response(client, 200, "OK", welcome_msg);
    }

    else {
        send_response(client, 404, "Not Found", "Not found\r\n");
    }


    fclose(client);
    exit(0);
}

void open_welcome_message(char *welcome_msg) {
    struct stat welcome_stat;
    int welcome_fd;

    /* Fetch resource/welcome_message */
    welcome_fd = open("resource/welcome_message", O_RDONLY);
    if (welcome_fd == -1) {
        perror("read resource/welcome_message");
        strcpy(welcome_msg, "Could not read welcome file\n");

    } else {
        if (fstat(welcome_fd, &welcome_stat) == -1) {
            perror("fstat");
            strcpy(welcome_msg, "Could not read welcome file\n\0");

        /* Put content of resource/welcome_message in welcome_msg */
        } else if (read(welcome_fd, (void *)welcome_msg, welcome_stat.st_size) == -1) {
            perror("read welcome_message");
            strcpy(welcome_msg, "Could not read welcome file\n\0");
        }

        close(welcome_fd);
    }
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
