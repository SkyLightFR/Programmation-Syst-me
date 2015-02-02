#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "http.h"

#define MAX_MSG_LENGTH 255

int client_treatment(int client_socket) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    const char *server_message = "<TTS-SERVER> %s";

    /* Echo everything the client sends */
    while (fgets(client_message, MAX_MSG_LENGTH, client)) {
        printf("parser = %d\n", method_parser(client_message));
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
