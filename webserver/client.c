#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_MSG_LENGTH 255


int create_client_socket(int server_socket) {
    int client_socket;
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
        }
        sleep(1);

    return client_socket;
}

int client_treatment(int client_socket) {
    int welcome_fd;
    struct stat welcome_stat;
    char *welcome_msg = malloc(2048);
    char *client_message = malloc(MAX_MSG_LENGTH);
    const char *welcomeerr = "Could not read welcome file\n";

    /* Fetch content of resource/welcome_message and print it */
    welcome_fd = open("resource/welcome_message", O_RDONLY);
    if (welcome_fd == -1) {
        perror("read resource/welcome_message");
        write(client_socket, welcomeerr, strlen(welcomeerr));
    } else {
        if (fstat(welcome_fd, &welcome_stat) == -1) {
            perror("fstat");
        }

        if (read(welcome_fd, (void *)welcome_msg, welcome_stat.st_size) == -1) {
            perror("read welcome_message");
        }

        close(welcome_fd);

        if (write(client_socket, welcome_msg, strlen(welcome_msg)) == -1) {
            perror("write welcome_message");
        }
        free(welcome_msg);
    }

    /* Echo everything the client sends */
    while(1) {
        int lg = recv(client_socket, client_message, MAX_MSG_LENGTH, 0);
        write(client_socket, client_message, lg);
    }
    free(client_message);

    return 0;
}
