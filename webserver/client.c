#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "http.h"

#define MAX_MSG_LENGTH 255

int client_treatment(int client_socket) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    int welcome_fd;
    struct stat welcome_stat;
    char welcome_msg[1500];
    const char *welcomeerr = "Could not read welcome file\n";
    const char *server_message = "<TTS-SERVER> %s";

    /* Fetch content of resource/welcome_message and print it */
    welcome_fd = open("resource/welcome_message", O_RDONLY);
    if (welcome_fd == -1) {
        perror("read resource/welcome_message");
        fprintf(client, server_message, welcomeerr);
    } else {
        if (fstat(welcome_fd, &welcome_stat) == -1) {
            perror("fstat");
        }

        if (read(welcome_fd, (void *)welcome_msg, welcome_stat.st_size) == -1) {
            perror("read welcome_message");
        }

        close(welcome_fd);

        if (fprintf(client, welcome_msg) == -1) {
            perror("write welcome_message");
        }
    }

    /* Echo everything the client sends */
    while (fgets(client_message, MAX_MSG_LENGTH, client)) {
        printf("parser : %d\n", parser(client_message));
        printf("%s\n", client_message);
        if (fprintf(client, server_message, client_message) < 0) {
            perror("fprintf");
            return -1;
        }
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
